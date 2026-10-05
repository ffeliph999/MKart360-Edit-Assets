#!/usr/bin/env python3
"""
Reduz a resolucao de PNGs, in-place (ou noutra pasta).

Sem opcao de tamanho, reduz a METADE (como antes). Formas de escolher o tamanho
(use so uma):
    --fator N        divide por N  (2 = metade, 4 = 1/4, 1.5 = 2/3 ...)
    --escala P       P por cento do tamanho atual (75 = 3/4)
    --max N          limita o lado maior a N px, mantendo a proporcao
                     (imagens ja menores ficam como estao)
    --tamanho LxA    tamanho exato (ex.: 256x128)
    --so-dxt         NAO reduz: so ajusta as imagens cujas dimensoes nao sao
                     multiplos de 4 (exigencia do DXT) para o multiplo de 4 mais
                     proximo, SE isso nao distorcer mais que --tolerancia (1%);
                     as demais ficam intocadas. Pastas "originais" sao ignoradas
                     (texturas de referencia do N64). --forcar ajusta mesmo
                     distorcendo -- cuidado com texturas desenhadas em faixas
                     (titulos, botoes e fontes do menu): podem desalinhar.
    --restaurar      devolve todos os *.orig.png (copias do --backup) ao lugar

As dimensoes sao arredondadas para multiplos de --multiplo (padrao 4: o DXT do
tex.pak exige multiplos de 4; texturas fora disso ficam sem compressao).
--multiplo 1 desliga o arredondamento. Se arredondar mudar a proporcao da
imagem em mais de --tolerancia por cento (padrao 1), ela NAO e arredondada:
fica com o tamanho exato (sem DXT no tex.pak) e o script avisa.

Memoria de video (por textura, no Xbox 360): RGBA = 4 bytes/pixel;
DXT1 (opacas) = 0,5; DXT5 (com transparencia) = 1. Com o tex.pak gerado com
--dxt, um sprite 256x256 em DXT5 ocupa o mesmo que um 128x128 em RGBA.

Por padrao sobrescreve os PNGs no lugar. Use --backup para guardar os
originais antes (<nome>.orig.png), ou --out PASTA para escrever noutra pasta.

Uso:
    py .\\HALVE_PNGS.py                                  # previa (metade)
    py .\\HALVE_PNGS.py --apply                          # aplica (metade)
    py .\\HALVE_PNGS.py --fator 4 --apply --backup
    py .\\HALVE_PNGS.py --max 256 --recursive --apply
    py .\\HALVE_PNGS.py --pasta extracted_textures\\karts --escala 75 --recursive
    py .\\HALVE_PNGS.py --pasta extracted_textures --so-dxt --recursive --apply --backup
"""
from pathlib import Path
import argparse, re, sys

try:
    from PIL import Image
except ImportError:
    sys.exit("Precisa do Pillow: pip install pillow")


def arredonda(v, mult):
    if mult <= 1:
        return max(int(round(v)), 1)
    return max(int(round(v / mult)) * mult, mult)


def novo_tamanho(w, h, a):
    """Devolve (nw, nh, sem_arredondar) ou None se a imagem deve ficar como esta.
    sem_arredondar = True quando o arredondamento distorceria demais a imagem."""
    if a.so_dxt:
        if w % 4 == 0 and h % 4 == 0:
            return None                          # ja compativel: nao mexe
        return arredonda(w, 4), arredonda(h, 4), False
    if a.tamanho:
        nw, nh = a.tamanho                       # tamanho exato pedido: sem arredondar
        return None if (nw, nh) == (w, h) else (nw, nh, False)
    if a.max:
        lado = max(w, h)
        if lado <= a.max:
            return None
        f = lado / a.max
    else:
        f = 100.0 / a.escala if a.escala else a.fator
    ew, eh = w / f, h / f                        # tamanho exato (fracionario)
    nw, nh = arredonda(ew, a.multiplo), arredonda(eh, a.multiplo)
    sem = False
    if a.multiplo > 1:
        distorcao = abs((nw / nh) / (w / h) - 1.0) * 100.0
        if distorcao > a.tolerancia:
            nw, nh, sem = arredonda(ew, 1), arredonda(eh, 1), True
    if (nw, nh) == (w, h):
        return None
    return nw, nh, sem


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    modo = ap.add_mutually_exclusive_group()
    modo.add_argument("--fator", type=float, default=2.0, help="divide por N (padrao 2 = metade)")
    modo.add_argument("--escala", type=float, help="porcentagem do tamanho atual (ex.: 75)")
    modo.add_argument("--max", type=int, help="limita o lado maior a N px")
    modo.add_argument("--tamanho", help="tamanho exato LxA (ex.: 256x128)")
    modo.add_argument("--so-dxt", action="store_true",
                      help="nao reduz: so ajusta para multiplos de 4 as imagens que nao sao (DXT)")
    modo.add_argument("--restaurar", action="store_true",
                      help="devolve os *.orig.png (copias do --backup) ao nome original")
    ap.add_argument("--forcar", action="store_true",
                    help="com --so-dxt: ajusta mesmo as que distorcem mais que --tolerancia")
    ap.add_argument("--multiplo", type=int, default=4,
                    help="arredonda as dimensoes para multiplos de N (padrao 4, exigido pelo DXT; 1 desliga)")
    ap.add_argument("--tolerancia", type=float, default=1.0,
                    help="distorcao maxima (%%) aceita ao arredondar; acima disso usa o tamanho exato (padrao 1)")
    ap.add_argument("--min", type=int, default=8, help="nao reduz abaixo desta dimensao (padrao 8)")
    ap.add_argument("--pasta", help="pasta com os PNGs (padrao: a pasta deste script)")
    ap.add_argument("--apply", action="store_true", help="aplica de fato (sem isto, so mostra a previa)")
    ap.add_argument("--recursive", action="store_true", help="processa tambem as subpastas")
    ap.add_argument("--backup", action="store_true", help="guarda o original como <nome>.orig.png")
    ap.add_argument("--out", help="escreve nesta pasta em vez de sobrescrever")
    ap.add_argument("--ampliar", action="store_true",
                    help="permite AUMENTAR (fator < 1, escala > 100 ou --tamanho maior)")
    a = ap.parse_args()

    if a.tamanho:
        m = re.fullmatch(r"\s*(\d+)\s*[xX]\s*(\d+)\s*", a.tamanho)
        if not m:
            sys.exit("--tamanho deve ser LxA, ex.: 256x128")
        a.tamanho = (int(m.group(1)), int(m.group(2)))
    if a.fator is not None and a.fator <= 0:
        sys.exit("--fator deve ser maior que 0")
    if a.escala is not None and a.escala <= 0:
        sys.exit("--escala deve ser maior que 0")
    if a.max is not None and a.max < 1:
        sys.exit("--max deve ser pelo menos 1")

    here = Path(a.pasta).resolve() if a.pasta else Path(__file__).resolve().parent
    if not here.is_dir():
        sys.exit(f"pasta nao encontrada: {here}")
    if a.restaurar:
        bkps = sorted(here.rglob("*.orig.png") if a.recursive else here.glob("*.orig.png"))
        if not bkps:
            print(f"nenhum *.orig.png em {here}" + ("" if a.recursive else " (use --recursive para subpastas)"))
            return
        for b in bkps:
            dest = b.with_name(b.name[:-len(".orig.png")] + ".png")
            print(f"  {b.relative_to(here)} -> {dest.name}")
            if a.apply:
                b.replace(dest)
        print(f"\n{len(bkps)} copia(s) de seguranca" + (" restaurada(s)" if a.apply else " -- PREVIA, use --apply"))
        return

    files = sorted(here.rglob("*.png") if a.recursive else here.glob("*.png"))
    files = [f for f in files if not f.name.endswith(".orig.png")]
    if a.so_dxt:
        ignorados = [f for f in files if "originais" in (p.lower() for p in f.relative_to(here).parts[:-1])]
        if ignorados:
            print(f"{len(ignorados)} PNG(s) em pastas 'originais' ignorado(s) (referencias do N64)")
        files = [f for f in files if f not in set(ignorados)]
    if not files:
        print(f"nenhum PNG encontrado em {here}" + ("" if a.recursive else " (use --recursive para subpastas)"))
        return

    outdir = Path(a.out).resolve() if a.out else None
    if outdir:
        outdir.mkdir(parents=True, exist_ok=True)

    print(f"{len(files)} PNG(s) em {here}")
    if not a.apply:
        print("PREVIA -- nada sera gravado. Use --apply para valer.\n")

    feitos = pulados = iguais = sem_dxt = 0
    px_antes = px_depois = 0

    for f in files:
        rel = f.relative_to(here)
        try:
            img = Image.open(f)
            w, h = img.size
        except Exception as exc:
            print(f"  ! {rel}: nao consegui abrir ({exc})")
            pulados += 1
            continue

        alvo = novo_tamanho(w, h, a)
        if alvo is None:
            iguais += 1
            continue
        nw, nh, sem = alvo
        if a.so_dxt:
            distorcao = abs((nw / nh) / (w / h) - 1.0) * 100.0
            if distorcao > a.tolerancia and not a.forcar:
                print(f"  - {rel}: {w}x{h} -> {nw}x{nh} distorceria {distorcao:.1f}%; mantida (fica sem DXT)")
                pulados += 1
                continue
            aviso = f"  (proporcao muda {distorcao:.1f}%)" if distorcao > a.tolerancia else ""
            print(f"  {rel}: {w}x{h} -> {nw}x{nh}{aviso}")
        if (nw > w or nh > h) and not a.ampliar and not a.so_dxt:
            print(f"  - {rel}: {w}x{h} -> {nw}x{nh} AUMENTARIA; pulando (use --ampliar se quiser)")
            pulados += 1
            continue
        if (nw < a.min or nh < a.min) and not a.so_dxt:
            print(f"  - {rel}: {w}x{h} -> ficaria {nw}x{nh}, abaixo de --min, pulando")
            pulados += 1
            continue
        if a.tamanho and abs((nw / nh) - (w / h)) > 0.01:
            print(f"  ! {rel}: {w}x{h} -> {nw}x{nh} muda a proporcao (a imagem fica esticada)")

        px_antes += w * h
        px_depois += nw * nh
        if sem:
            sem_dxt += 1
            print(f"  ! {rel}: arredondar para multiplo de {a.multiplo} distorceria mais de "
                  f"{a.tolerancia:g}%; fica {nw}x{nh} exato (SEM DXT no tex.pak)")
        elif (nw % 4 or nh % 4):
            sem_dxt += 1

        if not a.apply:
            if not a.so_dxt:
                print(f"  {rel}: {w}x{h} -> {nw}x{nh}")
            feitos += 1
            continue

        try:
            destino = (outdir / rel) if outdir else f
            if outdir:
                destino.parent.mkdir(parents=True, exist_ok=True)
            elif a.backup:
                bkp = f.with_suffix(".orig.png")
                if not bkp.exists():
                    img.close()
                    f.replace(bkp)
                    img = Image.open(bkp)
            img = img.convert("RGBA")
            # LANCZOS preserva melhor o detalhe ao reduzir
            img.resize((nw, nh), Image.LANCZOS).save(destino, "PNG", optimize=True)
            feitos += 1
        except Exception as exc:
            print(f"  ! {rel}: falhou ({exc})")
            pulados += 1

    if a.so_dxt:
        print(f"\n{feitos} imagem(ns) ajustada(s) para multiplos de 4, "
              f"{iguais} ja compativel(is) (intocadas), {pulados} mantida(s)/pulada(s)")
    else:
        print(f"\n{feitos} processado(s), {iguais} ja no tamanho, {pulados} pulado(s)")
    if sem_dxt:
        print(f"{sem_dxt} imagem(ns) com dimensoes que nao sao multiplos de 4: ficam sem DXT no tex.pak")
    if a.so_dxt:
        if feitos:
            print("depois de aplicar, todas essas imagens podem ir em DXT no tex.pak (--dxt)")
    elif px_antes:
        def tam(px, bpp):
            b = px * bpp
            return f"{b/1024/1024:.1f} MB" if b >= 1024 * 1024 else f"{b/1024:.0f} KB"
        print("memoria de video estimada:")
        for nome, bpp in (("RGBA", 4), ("DXT5", 1), ("DXT1", 0.5)):
            print(f"  {nome}: {tam(px_antes, bpp)} -> {tam(px_depois, bpp)}")
    if not a.apply:
        print("\nNada foi alterado. Rode de novo com --apply.")
    elif outdir:
        print(f"gravado em {outdir}")
    elif a.backup:
        print("originais guardados como *.orig.png")


if __name__ == "__main__":
    main()
