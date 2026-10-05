#!/usr/bin/env python3
"""
Previa da compressao DXT -- veja no PC como as suas texturas ficariam no console.

Para cada PNG, comprime com o mesmo codificador do PACK_TEXTURES.py --dxt e
descomprime com uma copia exata do decodificador do console (gfx_pc.c). Grava
uma imagem lado a lado (ORIGINAL | DXT) e mostra a qualidade (PSNR em dB:
acima de ~40 a diferenca e dificil de ver; abaixo de ~32 costuma aparecer).

Uso (dentro de mk64-master):
    py .\\DXT_PREVIEW.py extracted_textures\\karts\\mario\\frames
    py .\\DXT_PREVIEW.py caminho\\de\\uma\\textura.png --zoom 2
    py .\\DXT_PREVIEW.py PASTA --compacto        (sprites em DXT1, menor)

As previas vao para a pasta dxt_preview\\ (nada e alterado nas texturas).
"""
from pathlib import Path
import argparse, importlib.util, sys

try:
    import numpy as np
    from PIL import Image
except ImportError:
    sys.exit("Precisa de: pip install pillow numpy")


def carregar_pack():
    p = Path(__file__).resolve().parent / "PACK_TEXTURES.py"
    spec = importlib.util.spec_from_file_location("pack_textures", p)
    m = importlib.util.module_from_spec(spec)
    argv = sys.argv; sys.argv = [argv[0]]
    try:
        spec.loader.exec_module(m)
    finally:
        sys.argv = argv
    return m


def _rgb565(c):
    r = (c >> 11) & 31; g = (c >> 5) & 63; b = c & 31
    return np.stack([(r << 3) | (r >> 2), (g << 2) | (g >> 4), (b << 3) | (b >> 2)], -1).astype(np.int32)


def dxt_decode(dados, w, h, fmt):
    """Copia exata de x360_dxt_decode (gfx_pc.c), vetorizada."""
    passo = 16 if fmt == 2 else 8
    b = np.frombuffer(dados, np.uint8).reshape(-1, passo).astype(np.int64)
    n = b.shape[0]
    if fmt == 2:
        a0, a1 = b[:, 0], b[:, 1]
        pal = np.zeros((n, 8), np.int64)
        pal[:, 0] = a0; pal[:, 1] = a1
        oito = a0 > a1
        for k in range(1, 7):
            pal[:, k + 1] = np.where(oito, ((7 - k) * a0 + k * a1) // 7, pal[:, k + 1])
        for k in range(1, 5):
            pal[:, k + 1] = np.where(~oito, ((5 - k) * a0 + k * a1) // 5, pal[:, k + 1])
        pal[:, 6] = np.where(~oito, 0, pal[:, 6]); pal[:, 7] = np.where(~oito, 255, pal[:, 7])
        bits = np.zeros(n, np.uint64)
        for k in range(5, -1, -1):
            bits = (bits << np.uint64(8)) | b[:, 2 + k].astype(np.uint64)
        aidx = ((bits[:, None] >> (np.arange(16, dtype=np.uint64) * np.uint64(3))) & np.uint64(7)).astype(np.int64)
        alfa = np.take_along_axis(pal, aidx, 1)
        b = b[:, 8:]
    c0 = b[:, 0] | (b[:, 1] << 8); c1 = b[:, 2] | (b[:, 3] << 8)
    idx32 = b[:, 4] | (b[:, 5] << 8) | (b[:, 6] << 16) | (b[:, 7] << 24)
    e0 = _rgb565(c0); e1 = _rgb565(c1)
    quatro = (fmt == 2) | (c0 > c1)
    col = np.zeros((n, 4, 4), np.int64)
    col[:, 0, :3] = e0; col[:, 1, :3] = e1
    col[:, 2, :3] = np.where(quatro[:, None], (2 * e0 + e1) // 3, (e0 + e1) // 2)
    col[:, 3, :3] = np.where(quatro[:, None], (e0 + 2 * e1) // 3, 0)
    col[:, :3, 3] = 255
    col[:, 3, 3] = np.where(quatro, 255, 0)
    idx = (idx32[:, None] >> (np.arange(16) * 2)) & 3
    px = np.take_along_axis(col, idx[:, :, None].repeat(4, 2), 1)
    if fmt == 2:
        px[:, :, 3] = alfa
    return px.reshape(h // 4, w // 4, 4, 4, 4).transpose(0, 2, 1, 3, 4).reshape(h, w, 4).astype(np.uint8)


def xadrez(w, h, t=8):
    yy, xx = np.mgrid[0:h, 0:w]
    v = np.where(((xx // t) + (yy // t)) % 2 == 0, 200, 150).astype(np.uint8)
    return np.dstack([v, v, v])


def sobre_xadrez(rgba):
    a = rgba[..., 3:4].astype(np.float32) / 255
    fundo = xadrez(rgba.shape[1], rgba.shape[0]).astype(np.float32)
    return (rgba[..., :3] * a + fundo * (1 - a)).astype(np.uint8)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("alvo", help="um PNG ou uma pasta")
    ap.add_argument("--out", default="dxt_preview")
    ap.add_argument("--zoom", type=int, default=1, help="ampliar a previa (ex.: 2)")
    ap.add_argument("--compacto", action="store_true", help="sprites em DXT1 (como --dxt-compacto)")
    ap.add_argument("--max", type=int, default=200, help="no maximo N imagens (padrao 200)")
    a = ap.parse_args()

    pack = carregar_pack()
    import inspect
    if "compacto" not in inspect.signature(pack.dxt_encode).parameters:
        sys.exit("O PACK_TEXTURES.py desta pasta e de uma versao anterior. Substitua-o pela "
                 "versao enviada junto com o DXT_PREVIEW.py e rode de novo.")
    alvo = Path(a.alvo)
    pngs = [alvo] if alvo.is_file() else sorted(alvo.rglob("*.png"))
    pngs = [p for p in pngs if not p.name.endswith(".orig.png")][:a.max]
    if not pngs:
        sys.exit("nenhum PNG encontrado")
    out = Path(a.out); out.mkdir(exist_ok=True)
    notas = []
    for p in pngs:
        im = np.asarray(Image.open(p).convert("RGBA"))
        h, w = im.shape[:2]
        if w % 4 or h % 4:
            print(f"  {p.name}: {w}x{h} nao e multiplo de 4 -- fica sem compressao no pak")
            continue
        fmt, dados = pack.dxt_encode(im.tobytes(), w, h, compacto=a.compacto)
        dec = dxt_decode(dados, w, h, fmt)
        m = im[..., 3] >= 128
        d = ((im[..., :3].astype(float) - dec[..., :3]) ** 2)[m]
        psnr = 99.0 if d.size == 0 or d.mean() == 0 else 10 * np.log10(255 ** 2 / d.mean())
        lado = np.concatenate([sobre_xadrez(im), np.full((h, 4, 3), 255, np.uint8), sobre_xadrez(dec)], 1)
        img = Image.fromarray(lado)
        if a.zoom > 1:
            img = img.resize((img.width * a.zoom, img.height * a.zoom), Image.NEAREST)
        nome = "__".join(p.relative_to(alvo).parts) if alvo.is_dir() else p.name
        img.save(out / nome)
        notas.append((psnr, nome, fmt, len(dados), w * h * 4))
        print(f"  {psnr:5.1f} dB  DXT{'1' if fmt == 1 else '5'}  {w*h*4//1024:>5} KB -> {len(dados)//1024:>4} KB  {nome}")
    if notas:
        notas.sort()
        print(f"\n{len(notas)} previa(s) em {out}\\  (ORIGINAL a esquerda | DXT a direita)")
        print("piores (comece olhando estas):")
        for ps, nome, *_ in notas[:5]:
            print(f"  {ps:5.1f} dB  {nome}")


if __name__ == "__main__":
    main()
