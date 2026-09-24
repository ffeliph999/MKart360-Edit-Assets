#!/usr/bin/env python3
r"""
Empacota os PNGs HD editados em extracted_textures\ para o formato .tex
que o patch de gfx_pc.c le em runtime (game:\tex\<hash>.tex).

Reconhece 3 origens de PNG:
  1. extracted_textures\<hash>__nome.png              (texturas common_data)
  2. extracted_textures\generated\<banco>\<symbol>.png (via generated_texture_manifest.json)
  3. extracted_textures\karts\...\<symbol>.png         (via kart_sprite_manifest.json)

Para entradas com "tmem_tiles" no manifest (imagens grandes de menu, ex:
retratos de selecao de personagem, "press start"): o jogo ORIGINAL as
particiona em varias faixas de TMEM, cada faixa com hash proprio. Com o
patch x360_try_draw_hd_menu_quad aplicado (gfx_pc.c + menu_items.c), essas
imagens passam a ser desenhadas INTEIRAS de uma vez, usando o hash da
imagem completa -- que e o comportamento PADRAO deste script.
Use --tiles apenas se aquele patch NAO estiver aplicado (modo antigo, que
gera um .tex por faixa; nunca conseguimos casar o particionamento real,
por isso o patch existe).

Requer Pillow:  pip install pillow

Uso (dentro de mk64-master, apos rodar o extrator):
    py .\PACK_TEXTURES.py --in extracted_textures --out tex

Para empacotar so um subconjunto, use --only com um prefixo de caminho:
    py .\PACK_TEXTURES.py --only generated\course_player_selection
"""
from pathlib import Path
import argparse, json, re, struct, sys

try:
    from PIL import Image
except ImportError:
    sys.exit("Precisa do Pillow: pip install pillow")

PAK_MODE = False
PAK_MAGIC = 0x4844504B  # "HDPK"
MAGIC = 0x54584448  # 'HDXT', mesmo valor de X360_HDTEX_MAGIC no patch C
NAME_RE = re.compile(r"^([0-9a-fA-F]{8})__")


def load_manifest(path):
    if not path.is_file():
        return []
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return []


PAK_ENTRIES = []   # usado no modo --pak: (hash_hex, w, h, bytes)


def write_tex(outdir, hash_hex, img):
    w, h = img.size
    if PAK_MODE:
        PAK_ENTRIES.append((hash_hex, w, h, img.tobytes()))
        return None
    # Subpasta pelos 2 primeiros digitos do hash: o FATX do Xbox 360 nao aceita
    # mais de 4096 entradas por pasta, e o elenco completo de karts passa disso
    # (a copia falha silenciosamente nos ultimos arquivos). Com 256 subpastas
    # sobram ~20 arquivos em cada.
    sub = outdir / hash_hex[:2]
    sub.mkdir(parents=True, exist_ok=True)
    out_path = sub / f"{hash_hex}.tex"
    with open(out_path, "wb") as f:
        # Xbox 360 e big-endian (PowerPC); o C le os bytes crus sem conversao,
        # entao o header precisa estar em big-endian tambem, senao magic/w/h
        # chegam com os bytes invertidos e sao rejeitados (mesmo com o hash
        # do nome do arquivo batendo certinho com o hash calculado em runtime).
        f.write(struct.pack(">III", MAGIC, w, h))
        f.write(img.tobytes())
    return out_path


_ARRAY_RE = re.compile(
    r"unsigned\s+char\s+([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*\]\s*=\s*\{(.*?)\};", re.S)
_HEX_RE = re.compile(r"0x([0-9A-Fa-f]{1,2})")


def _fnv1a32(data):
    h = 0x811C9DC5
    for b in data:
        h = ((h ^ b) * 0x01000193) & 0xFFFFFFFF
    return h


def aplicar_geometria(geo_path, generated):
    """Para cada imagem da geometria, le os bytes do simbolo no banco gerado
    (descomprimindo MIO0), calcula o hash de cada pedaco do jeito que o port
    calcula (trecho continuo a partir do canto x0,y0) e injeta como
    tmem_halves na entrada do manifest."""
    try:
        doc = json.loads(geo_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return 0
    banco = Path(__file__).resolve().parent / "src" / "xbox360" / "generated_banks" / doc.get("bank", "")
    if not banco.is_file():
        print(f"  ! {banco} nao existe -- rode o prepare/build antes")
        return 0
    try:
        from EXTRACT_MK64_TEXTURES import mio0_decode
    except ImportError:
        mio0_decode = None
    arrays = {nome: corpo for nome, corpo in _ARRAY_RE.findall(
        banco.read_text(encoding="utf-8", errors="ignore"))}
    por_simbolo = {e.get("symbol"): k for k, e in generated.items()}
    feitos = 0
    for simbolo, info in doc.get("images", {}).items():
        chave = por_simbolo.get(simbolo)
        corpo = arrays.get(simbolo)
        if chave is None or corpo is None:
            continue
        dados = bytes(int(h, 16) for h in _HEX_RE.findall(corpo))
        if dados[:4] == b"MIO0" and mio0_decode:
            dados = mio0_decode(dados)
        W = int(info["width"])
        tiles = []
        for t in info["tiles"]:
            pw = t["x1"] - t["x0"]
            ph = t["y1"] - t["y0"]
            off = (t["y0"] * W + t["x0"]) * 2
            L = pw * ph * 2
            if off + L > len(dados):
                continue
            tiles.append(dict(t, hash=f"{_fnv1a32(dados[off:off + L]):08x}"))
        if tiles:
            generated[chave] = dict(generated[chave])
            generated[chave]["tmem_halves"] = tiles
            feitos += 1
    return feitos


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--in", dest="indir", default="extracted_textures")
    ap.add_argument("--out", dest="outdir", default="tex")
    ap.add_argument("--max", type=int, default=2048)
    ap.add_argument("--only", help="so empacota PNGs cujo caminho relativo comeca com este prefixo")
    ap.add_argument("--pak", action="store_true",
                    help="gera UM arquivo tex.pak em vez de milhares de .tex soltos. "
                         "Evita milhares de aberturas de arquivo em runtime (causa dos "
                         "engasgos de cache frio) e contorna o limite do FATX.")
    ap.add_argument("--tiles", action="store_true",
                    help="modo ANTIGO: fatia imagens grandes de menu em varios .tex (um por faixa "
                         "de TMEM). So necessario se o patch de menu_items.c (x360_try_draw_hd_menu_quad) "
                         "NAO estiver aplicado. Com o patch, o padrao (imagem inteira) e o correto.")
    a = ap.parse_args()

    indir = Path(a.indir)
    outdir = Path(a.outdir)
    outdir.mkdir(parents=True, exist_ok=True)
    global PAK_MODE
    PAK_MODE = a.pak
    only_prefix = a.only.replace("\\", "/").lower() if a.only else None

    # --- indices dos manifests, por caminho de PNG relativo -------------
    generated = {e["png"].replace("\\", "/").lower(): e
                 for e in load_manifest(indir / "generated_texture_manifest.json")}
    karts = {e["png"].replace("\\", "/").lower(): e
             for e in load_manifest(indir / "kart_sprite_manifest.json")}
    # Geometria PORTATIL dos pedacos de menu (menu_tiles_geometry.json, versionado
    # no repositorio). Tem so coordenadas; os hashes sao calculados aqui a
    # partir dos bancos gerados da ROM de quem esta usando -- assim funciona
    # com qualquer versao de ROM e sem precisar ligar o trace.
    geo_path = Path(__file__).resolve().parent / "menu_tiles_geometry.json"
    if geo_path.is_file():
        n_geo = aplicar_geometria(geo_path, generated)
        print(f"Geometria de menu: {n_geo} imagem(ns) (menu_tiles_geometry.json)")

    # Faixas MEDIDAS em runtime para as imagens grandes de menu (menu_tiles_measured.json, opcional).
    # O jogo fatia essas imagens em varias faixas, cada uma com hash proprio;
    # usamos os cortes medidos, reaproveitando o caminho de "tmem_halves".
    medido_path = indir / "menu_tiles_measured.json"
    if medido_path.is_file():
        try:
            medido = json.loads(medido_path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError):
            medido = {}
        aplicadas = 0
        for png_rel, info in medido.items():
            chave = png_rel.replace("\\", "/").lower()
            if chave in generated and info.get("tiles"):
                generated[chave] = dict(generated[chave])
                generated[chave]["tmem_halves"] = info["tiles"]
                aplicadas += 1
        print(f"Faixas medidas de menu: {aplicadas} imagem(ns) (menu_tiles_measured.json)")

    # sprites de outras pastas de asset (lakitu, etc)
    # asset_sprite_manifest.json: EXTRACT_MK64_TEXTURES.py (assets/<pasta>/*.json)
    # lakitu_sprite_manifest.json: EXTRACT_LAKITU.py
    for extra in ("asset_sprite_manifest.json", "lakitu_sprite_manifest.json"):
        karts.update({e["png"].replace("\\", "/").lower(): e
                      for e in load_manifest(indir / extra)})
    print(f"Manifests: {len(generated)} generated + {len(karts)} karts")

    n_files = n_tiles = 0
    missed = []

    for png in indir.rglob("*.png"):
        rel = png.relative_to(indir).as_posix()
        if only_prefix and not rel.lower().startswith(only_prefix):
            continue

        entry = generated.get(rel.lower()) or karts.get(rel.lower())
        m = NAME_RE.match(png.name)

        if entry is None and m is None:
            missed.append(rel)
            continue

        img = Image.open(png).convert("RGBA")
        w, hgt = img.size

        # Sprites CI8 (karts) sao carregados em metades de 64x32 por causa do
        # limite de TMEM (a TLUT ocupa metade dos 4KB). Cada metade tem hash
        # proprio em runtime -- sempre honrado, nao depende de --tiles.
        if entry and entry.get("tmem_halves"):
            halves = entry["tmem_halves"]
            orig_h = int(entry["height"])
            orig_w = int(entry["width"])
            # Blocos que passam da borda (ex: 33 linhas a partir de y=32 numa
            # imagem de 64): estende a arte HD repetindo a ultima linha/coluna,
            # para o recorte manter a proporcao exata do bloco original em vez
            # de esticar (o que criaria emenda visivel).
            max_y1 = max(int(t["y1"]) for t in halves)
            max_x1 = max(int(t.get("x1", orig_w)) for t in halves)
            need_h = max(hgt, round(max_y1 / orig_h * hgt))
            need_w = max(w, round(max_x1 / orig_w * w))
            if need_h > hgt or need_w > w:
                ext = Image.new("RGBA", (need_w, need_h))
                ext.paste(img, (0, 0))
                if need_h > hgt:
                    ultima = img.crop((0, hgt - 1, w, hgt))
                    for yy in range(hgt, need_h):
                        ext.paste(ultima, (0, yy))
                if need_w > w:
                    col = ext.crop((w - 1, 0, w, need_h))
                    for xx in range(w, need_w):
                        ext.paste(col, (xx, 0))
                img = ext
            # Usa as LINHAS reais de cada janela (y0/y1 do manifest), nao uma
            # divisao ao meio: as duas janelas se sobrepoem em uma linha
            # (a segunda comeca em 1984 bytes, nao 2048).
            for i, t in enumerate(halves):
                y0e = round(int(t["y0"]) / orig_h * hgt)
                y1e = round(int(t["y1"]) / orig_h * hgt)
                if y1e <= y0e:
                    y1e = y0e + 1
                if y1e > img.size[1]:
                    y1e = img.size[1]
                # Pedacos 2D (menus): x0/x1 opcionais; sem eles, largura inteira.
                x0e = round(int(t.get("x0", 0)) / orig_w * w)
                x1e = round(int(t.get("x1", orig_w)) / orig_w * w)
                if x1e <= x0e:
                    x1e = x0e + 1
                if x1e > img.size[0]:
                    x1e = img.size[0]
                crop = img.crop((x0e, y0e, x1e, y1e))
                cw, ch = crop.size
                if cw > a.max or ch > a.max:
                    print(f"  ! {rel} metade {i}: {cw}x{ch} excede --max={a.max}, pulando")
                    continue
                write_tex(outdir, t["hash"], crop)
                n_tiles += 1
                # Grava tambem sob os hashes alternativos (variantes de layout
                # de TMEM). Sao poucos arquivos a mais e garante que a metade
                # case independentemente de qual variante o jogo usa.
                for alt in t.get("alt_hashes", []):
                    if alt and alt != t["hash"]:
                        write_tex(outdir, alt, crop)
                        n_tiles += 1
            n_files += 1
            continue

        if a.tiles and entry and entry.get("tmem_tiles"):
            orig_h = int(entry["height"])
            tiles = entry["tmem_tiles"]
            # Fronteiras proporcionais calculadas uma unica vez, para as
            # fatias ficarem contiguas (sem sobreposicao/gap) na imagem editada.
            bounds = [round(t["y0"] / orig_h * hgt) for t in tiles] + [hgt]
            for i, t in enumerate(tiles):
                y0e, y1e = bounds[i], bounds[i + 1]
                if y1e <= y0e:
                    y1e = y0e + 1
                crop = img.crop((0, y0e, w, y1e))
                cw, ch = crop.size
                if cw > a.max or ch > a.max:
                    print(f"  ! {rel} faixa {i}: {cw}x{ch} excede --max={a.max}, pulando")
                    continue
                write_tex(outdir, t["hash"], crop)
                n_tiles += 1
            n_files += 1
            continue

        h = m.group(1).lower() if m else entry.get("decoded_hash_fnv1a32")
        if h is None:
            missed.append(rel)
            continue
        if w > a.max or hgt > a.max:
            print(f"  ! {rel}: {w}x{hgt} excede --max={a.max}, pulando")
            continue
        write_tex(outdir, h, img)
        n_files += 1

    if PAK_MODE:
        # tex.pak: cabecalho + indice + dados. Um unico arquivo, aberto uma vez
        # pelo jogo; cada textura vira um seek+read em vez de abrir arquivo.
        pak = outdir / "tex.pak"
        header = struct.pack(">III", PAK_MAGIC, 1, len(PAK_ENTRIES))
        index_size = len(PAK_ENTRIES) * 20
        data_off = len(header) + index_size
        index, blob, cur = b"", [], data_off
        for hh, w, h, raw in PAK_ENTRIES:
            index += struct.pack(">IIIII", int(hh, 16), cur, len(raw), w, h)
            blob.append(raw)
            cur += len(raw)
        with open(pak, "wb") as f:
            f.write(header); f.write(index)
            for b in blob:
                f.write(b)
        print(f"\n{n_files} imagens processadas ({n_tiles} em faixas de TMEM)")
        print(f"tex.pak: {len(PAK_ENTRIES)} texturas, "
              f"{pak.stat().st_size/1024/1024:.1f} MB em UM arquivo")
        print("")
        print(f"  ATENCAO: copie o arquivo {pak.name} para a RAIZ da pasta do jogo")
        print("  no console, AO LADO do MK64.xex -- NAO dentro de uma pasta tex\\.")
        print("  Estrutura correta no console:")
        print("      MK64.xex")
        print("      baserom.us.z64")
        print("      tex.pak")
        return

    print(f"\n{n_files} imagens processadas ({n_tiles} delas expandidas em faixas de TMEM)")
    total = list(outdir.rglob("*.tex"))
    total_mb = sum(p.stat().st_size for p in total) / 1024 / 1024
    print(f"{len(total)} arquivos .tex em {outdir}\\  ({total_mb:.1f} MB)")
    if missed:
        print(f"{len(missed)} PNGs sem hash conhecido (nao empacotados):")
        for r in missed[:15]:
            print("  -", r)
        if len(missed) > 15:
            print(f"  ... e mais {len(missed) - 15}")
    print("Copie a pasta para o Xbox 360 como game:\\tex\\")


if __name__ == "__main__":
    main()
