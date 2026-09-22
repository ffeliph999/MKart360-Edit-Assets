#!/usr/bin/env python3
"""
Extrai TODOS os quadros do Lakitu da ROM.

Os sprites estao mapeados em assets/lakitu/*.json (offset, dimensoes, tipo e
paleta de cada quadro), separados por situacao: semaforo, bandeirada, pesca,
volta final, contramao, etc.

Alem de exportar os PNGs, o script:
  - lista quantos quadros ha em cada categoria;
  - avisa sobre quadros que aparecem nos JSONs mas nao puderam ser exportados,
    dizendo o motivo (paleta ausente, dados curtos, formato desconhecido);
  - procura sprites de Lakitu que estejam APENAS nos bancos gerados do port,
    fora dos JSONs, para nao passarem despercebidos.

Uso (dentro de mk64-master):
    py .\\EXTRACT_LAKITU.py --rom .\\baserom.us.z64
    py .\\EXTRACT_LAKITU.py --rom .\\baserom.us.z64 --force   # sobrescreve PNGs
"""
from pathlib import Path
import argparse, json, sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import EXTRACT_MK64_TEXTURES as EX
from EXTRACT_MK64_TEXTURES import (
    source_bytes, fnv1a32, int_value, decode, decode_palette,
    write_png, BPP,
)

# Definida aqui em vez de importada, para o script funcionar tambem com
# versoes mais antigas do EXTRACT_MK64_TEXTURES.py.
def tmem_halves_for(pixels, w):
    """Janelas de TMEM de uma textura CI8 que nao cabe numa carga so.

    CI8 deixa 2048 bytes para texels (a TLUT ocupa a outra metade dos 4KB).
    MEDIDO em runtime para os karts (largura 64): a segunda janela comeca em
    2048-w, ou seja UMA LINHA antes do meio, e as duas se sobrepoem nessa
    linha. A mesma regra e aplicada a outras larguras, com recorte quando a
    janela passaria do fim dos dados.
    """
    if not w or len(pixels) <= 2048:
        return []
    off = 2048 - w
    end = min(off + 2048, len(pixels))
    return [
        {"y0": 0, "y1": 2048 // w,
         "hash": f"{fnv1a32(pixels[0:2048]):08x}"},
        {"y0": off // w, "y1": end // w,
         "hash": f"{fnv1a32(pixels[off:end]):08x}"},
    ]


def extrair(root, rom, outdir, cache, force):
    src = root / "assets" / "lakitu"
    if not src.is_dir():
        sys.exit(f"nao encontrei {src}")

    jsons = sorted(src.glob("*.json"))
    if not jsons:
        sys.exit(f"nenhum .json em {src}")

    manifest, pulados = [], []
    por_categoria = {}

    for jp in jsons:
        entries = json.loads(jp.read_text(encoding="utf-8"))
        for symbol, info in entries.items():
            fmt = str(info.get("type", "")).lower()
            if fmt not in BPP:
                continue

            # paletas sao entradas auxiliares, nao quadros
            if symbol.startswith("common_tlut") or "tlut" in symbol.lower():
                continue

            tlut_ref = info.get("tlut")
            if fmt.startswith("ci") and not tlut_ref:
                pulados.append((symbol, "sem TLUT declarada"))
                continue

            w, h = int(info["width"]), int(info["height"])
            needed = (w * h * BPP[fmt] + 7) // 8
            boff = int_value(info.get("block_offset", 0))
            raw = source_bytes(rom, int_value(info["rom_offset"]), cache, boff + needed)
            pixels = raw[boff:boff + needed]
            if len(pixels) < needed:
                pulados.append((symbol, f"dados curtos ({len(pixels)} < {needed})"))
                continue

            palette = None
            if fmt.startswith("ci"):
                refs = [tlut_ref] if isinstance(tlut_ref, str) else list(tlut_ref)
                palette = []
                falhou = None
                for pal_symbol in refs:
                    pal = entries.get(pal_symbol)
                    if pal is None:
                        falhou = f"TLUT ausente: {pal_symbol}"
                        break
                    pw, ph = int(pal["width"]), int(pal["height"])
                    pboff = int_value(pal.get("block_offset", 0))
                    need_pal = pw * ph * 2
                    praw = source_bytes(rom, int_value(pal["rom_offset"]), cache, pboff + need_pal)
                    palette.extend(decode_palette(praw[pboff:pboff + need_pal]))
                if falhou:
                    pulados.append((symbol, falhou))
                    continue

            try:
                rgba = decode(fmt, pixels, w, h, palette)
            except Exception as exc:
                pulados.append((symbol, str(exc)))
                continue

            categoria = info.get("output_dir") or jp.stem
            rel = Path("lakitu") / categoria / f"{symbol}.png"
            destino = outdir / rel
            if destino.exists() and not force:
                por_categoria.setdefault(categoria, 0)
                por_categoria[categoria] += 1
                manifest.append(_entrada(rel, symbol, info, w, h, fmt, pixels))
                continue

            write_png(destino, w, h, rgba)
            por_categoria.setdefault(categoria, 0)
            por_categoria[categoria] += 1
            manifest.append(_entrada(rel, symbol, info, w, h, fmt, pixels))

    return manifest, pulados, por_categoria


def _entrada(rel, symbol, info, w, h, fmt, pixels):
    return {
        "png": str(rel).replace("\\", "/"),
        "symbol": symbol,
        "rom_offset": f"0x{int_value(info['rom_offset']):X}",
        "width": w, "height": h, "format": fmt,
        "decoded_hash_fnv1a32": f"{fnv1a32(pixels):08x}",
        "tmem_halves": tmem_halves_for(pixels, w) if fmt == "ci8" else [],
    }


def procurar_fora_dos_jsons(root, manifest):
    """Sprites com 'lakitu' no nome que existem nos bancos gerados do port mas
    nao aparecem nos JSONs -- para nenhum quadro passar despercebido."""
    ja = {e["symbol"].lower() for e in manifest}
    achados = set()
    banks = root / "src" / "xbox360" / "generated_banks"
    if banks.is_dir():
        for c in banks.glob("*.c"):
            try:
                txt = c.read_text(encoding="utf-8", errors="ignore")
            except OSError:
                continue
            for linha in txt.splitlines():
                if "lakitu" not in linha.lower() or "[]" not in linha:
                    continue
                for tok in linha.replace("[", " ").replace("]", " ").split():
                    if "lakitu" in tok.lower() and tok.lower() not in ja:
                        achados.add((tok, c.name))
    return sorted(achados)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--rom", default="baserom.us.z64")
    ap.add_argument("--out", default="extracted_textures")
    ap.add_argument("--force", action="store_true",
                    help="sobrescreve PNGs existentes (por padrao sao preservados)")
    a = ap.parse_args()

    root = Path(__file__).resolve().parent
    rom = Path(a.rom).read_bytes()
    outdir = Path(a.out)
    cache = {}

    manifest, pulados, por_cat = extrair(root, rom, outdir, cache, a.force)

    print(f"\n{len(manifest)} quadros do Lakitu -> {outdir / 'lakitu'}\n")
    for cat in sorted(por_cat):
        print(f"  {cat:<20} {por_cat[cat]:>3} quadros")

    if pulados:
        print(f"\n{len(pulados)} nao exportados:")
        for sym, motivo in pulados[:20]:
            print(f"  - {sym}: {motivo}")
        if len(pulados) > 20:
            print(f"  ... e mais {len(pulados) - 20}")

    fora = procurar_fora_dos_jsons(root, manifest)
    if fora:
        print(f"\n{len(fora)} simbolo(s) com 'lakitu' fora dos JSONs "
              f"(sem dimensoes declaradas, nao exportaveis automaticamente):")
        for tok, arq in fora[:15]:
            print(f"  - {tok}  ({arq})")

    if manifest:
        saida = outdir / "lakitu_sprite_manifest.json"
        saida.write_text(json.dumps(manifest, indent=1), encoding="utf-8")
        print(f"\nmanifest: {saida}")
        print("Empacote com:  py .\\PACK_TEXTURES.py --only lakitu --pak")


if __name__ == "__main__":
    main()
