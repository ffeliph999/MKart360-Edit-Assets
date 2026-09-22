#!/usr/bin/env python3
"""
Extrai as texturas do MK64 para PNG usando PUBLIC_ASSET_RECIPES.json + a ROM,
com dimensoes e paleta lidas de yamls\\us\\*.yml (fonte de verdade -- sem chute).

Uso (dentro de mk64-master):
    py .\\EXTRACT_MK64_TEXTURES.py --rom .\\baserom.us.z64

Saida: extracted_textures\\<hash>__<nome>.png  +  texture_dims.json
"""
from pathlib import Path
import argparse, json, re, struct, sys, zlib
from collections import defaultdict

# ---------------------------------------------------------------- MIO0

def mio0_decode(src):
    if len(src) < 16 or src[:4] != b"MIO0":
        raise ValueError("not MIO0")
    out_size = int.from_bytes(src[4:8], "big")
    comp_pos = int.from_bytes(src[8:12], "big")
    raw_pos = int.from_bytes(src[12:16], "big")
    ctrl_pos, mask, ctrl = 16, 0, 0
    out = bytearray()
    while len(out) < out_size:
        if mask == 0:
            ctrl = src[ctrl_pos]; ctrl_pos += 1; mask = 0x80
        if ctrl & mask:
            out.append(src[raw_pos]); raw_pos += 1
        else:
            w = int.from_bytes(src[comp_pos:comp_pos + 2], "big"); comp_pos += 2
            length = (w >> 12) + 3
            dist = (w & 0xFFF) + 1
            start = len(out) - dist
            for i in range(length):
                out.append(out[start + i])
        mask >>= 1
    return bytes(out[:out_size])


def source_bytes(rom, offset, cache, raw_size=None):
    """Retorna um bloco MIO0 descomprimido ou bytes crus da ROM.

    Para dados crus, ``rom[offset:]`` e a ROM inteira restante. Armazenar isso
    no cache uma vez por paleta acabava consumindo varios GB ao exportar todos
    os karts. ``raw_size`` limita o bloco guardado ao tamanho realmente usado.
    """
    cache_key = (offset, raw_size) if raw_size is not None else (offset, None)
    if cache_key in cache:
        return cache[cache_key]
    if rom[offset:offset + 4] == b"MIO0":
        data = mio0_decode(rom[offset:])
    else:
        data = rom[offset:offset + raw_size] if raw_size is not None else rom[offset:]
    cache[cache_key] = data
    return data

# ---------------------------------------------------------------- PNG (puro, sem PIL)

SKIP_EXISTING_PNGS = True  # controlado por --force; protege edicoes ja feitas pelo usuario


def write_png(path, w, h, rgba):
    if SKIP_EXISTING_PNGS and path.is_file():
        return False  # nao sobrescreve um PNG que o usuario ja pode ter editado
    def chunk(tag, data):
        c = tag + data
        return struct.pack(">I", len(data)) + c + struct.pack(">I", zlib.crc32(c) & 0xFFFFFFFF)
    raw = bytearray()
    stride = w * 4
    for y in range(h):
        raw.append(0)
        raw += rgba[y * stride:(y + 1) * stride]
    png = (b"\x89PNG\r\n\x1a\n"
           + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0))
           + chunk(b"IDAT", zlib.compress(bytes(raw), 9))
           + chunk(b"IEND", b""))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(png)
    return True

# ---------------------------------------------------------------- fnv1a (mesmo hash do patch C)

def fnv1a32(data):
    h = 2166136261
    for b in data:
        h = ((h ^ b) * 16777619) & 0xFFFFFFFF
    return h

# ---------------------------------------------------------------- parser YAML minimo

def parse_yaml_symbols(text):
    """Parser bem simples para o formato usado pelos yamls/us/*.yml:
       symbol_name:
         key: value
         key2: value2
       Retorna dict symbol_name -> {key: value(str)}.
       Ignora blocos sem indentacao de 2 espacos (listas, etc.)."""
    out = {}
    current = None
    for raw_line in text.splitlines():
        if not raw_line.strip() or raw_line.strip().startswith("#"):
            continue
        if raw_line[0] not in (" ", "\t") and raw_line.rstrip().endswith(":"):
            current = raw_line.strip()[:-1]
            out[current] = {}
            continue
        m = re.match(r"^\s+([A-Za-z_][A-Za-z0-9_]*):\s*(.*)$", raw_line)
        if m and current is not None:
            key, val = m.group(1), m.group(2).strip().strip('"').strip("'")
            out[current][key] = val
    return out


def load_all_yaml_symbols(root):
    symbols = {}
    yaml_dir = root / "yamls" / "us"
    if not yaml_dir.is_dir():
        return symbols
    for p in yaml_dir.glob("*.yml"):
        try:
            text = p.read_text(encoding="utf-8", errors="ignore")
        except OSError:
            continue
        symbols.update(parse_yaml_symbols(text))
    return symbols


def load_asset_json_symbols(root):
    """Carrega os metadados de assets/courses/*.json e demais asset JSONs.

    Eles descrevem varias texturas que nao tem sufixo de formato no arquivo
    gerado (por exemplo, CI8 com TLUT e RGBA16 comprimido em MIO0). Ignora JSONs
    que nao sejam o mapa simples ``simbolo -> propriedades`` de assets.
    """
    symbols = {}
    assets_dir = root / "assets"
    if not assets_dir.is_dir():
        return symbols
    for path in assets_dir.rglob("*.json"):
        try:
            entries = json.loads(path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError):
            continue
        if not isinstance(entries, dict):
            continue
        for symbol, info in entries.items():
            if isinstance(info, dict) and "width" in info and "height" in info:
                symbols[symbol] = info
    return symbols


# ---------------------------------------------------------------- bancos gerados do port 360

MENU_TEXTURE_RE = re.compile(
    r"\{\s*-?\d+\s*,\s*([A-Za-z_][A-Za-z0-9_]*)\s*,\s*(\d+)\s*,\s*(\d+)\s*,"
)


def load_menu_texture_dims(root):
    """Lê largura/altura da tabela MenuTexture do próprio jogo."""
    table = root / "src" / "data" / "textures.c"
    if not table.is_file():
        return {}
    text = table.read_text(encoding="utf-8", errors="ignore")
    return {symbol: (int(w), int(h))
            for symbol, w, h in MENU_TEXTURE_RE.findall(text)}


# ---------------------------------------------------------------- dimensoes reais via source scan

# gDPLoadTextureBlock(pkt, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt)
# gDPLoadTextureBlock_4b(pkt, timg, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt)
# gsDPLoadTextureBlock(timg, fmt, siz, width, height, ...) -- variante "gs" sem o arg pkt
# gDPLoadTextureTile / gsDPLoadTextureTile(pkt?, timg, fmt, siz, width, height, uls, ult, lrs, lrt, ...)
#   -- aqui width/height sao as dimensoes DECLARADAS da imagem completa (para calculo de stride),
#      nao o tamanho do recorte uls/ult/lrs/lrt -- servem igual para nosso proposito.
_LOAD_MACRO_RE = re.compile(
    r'\b(gs?DPLoadTextureBlock(?:_4b)?|gs?DPLoadTextureTile)\s*\(((?:[^()]|\([^()]*\))*)\)',
    re.DOTALL,
)
_INT_RE = re.compile(r'^(0x[0-9A-Fa-f]+|\d+)$')
_FMT_RE_TOKEN = re.compile(r'^G_IM_FMT_')
_SIZ_RE_TOKEN = re.compile(r'^G_IM_SIZ_')


def _split_top_level_args(s):
    """Divide os argumentos de uma chamada de macro por virgula, ignorando
    virgulas dentro de parenteses aninhados (ex: chamadas dentro de args)."""
    args, depth, cur = [], 0, []
    for ch in s:
        if ch == '(':
            depth += 1; cur.append(ch)
        elif ch == ')':
            depth -= 1; cur.append(ch)
        elif ch == ',' and depth == 0:
            args.append(''.join(cur).strip()); cur = []
        else:
            cur.append(ch)
    if cur:
        args.append(''.join(cur).strip())
    return args


def _dims_from_macro_args(macro_name, args):
    """Dado o nome da macro e a lista de argumentos (como texto), tenta
    localizar width,height de forma robusta: ancora no token de formato
    (G_IM_FMT_*) e/ou tamanho (G_IM_SIZ_*) em vez de depender so de posicao,
    ja que a presenca do argumento 'pkt' e do 'siz' varia por macro/variante."""
    is_4b = macro_name.endswith('_4b')
    fmt_idx = next((i for i, a in enumerate(args) if _FMT_RE_TOKEN.match(a)), None)
    if fmt_idx is None:
        return None
    if is_4b:
        wh_start = fmt_idx + 1
    else:
        siz_idx = next((i for i in range(fmt_idx + 1, len(args)) if _SIZ_RE_TOKEN.match(args[i])), None)
        if siz_idx is None:
            return None
        wh_start = siz_idx + 1
    if wh_start + 1 >= len(args):
        return None
    w_tok, h_tok = args[wh_start], args[wh_start + 1]
    if not (_INT_RE.match(w_tok) and _INT_RE.match(h_tok)):
        return None
    w = int(w_tok, 0)
    h = int(h_tok, 0)
    if w <= 0 or h <= 0 or w > 4096 or h > 4096:
        return None
    return w, h


_INCLUDE_DECL_RE = re.compile(
    r'\b(?:u8|u16|s8|s16)\s+([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*\]\s*=\s*\{\s*'
    r'#include\s*"[^"]*/([A-Za-z0-9_]+)\.[A-Za-z0-9_.]*inc\.c"',
)


def build_symbol_aliases(root):
    """Varre 'u8 NOME[] = { #include ".../<stem>.<fmt>.inc.c" };' para mapear
    o nome de arquivo (usado como simbolo nas recipes/bancos) para o NOME de
    variavel C real usado nas chamadas de display list -- em cursos
    ('course_textures.linkonly.c') esses dois nomes costumam ser diferentes
    (ex: arquivo 'sign_welcome_0' -> variavel 'gBBTextureSignWelcome0').
    Retorna {stem_do_arquivo: nome_da_variavel_C}.
    """
    aliases = {}
    for path in root.rglob("*.c"):
        try:
            text = path.read_text(encoding="utf-8", errors="ignore")
        except OSError:
            continue
        if "#include" not in text:
            continue
        for varname, stem in _INCLUDE_DECL_RE.findall(text):
            aliases.setdefault(stem, varname)
    return aliases


def scan_source_for_dims(root):
    """Varre todo o codigo .c/.inc.c procurando chamadas gDPLoadTextureBlock/
    gDPLoadTextureTile (e variantes _4b/gs) que referenciam cada simbolo de
    textura, extraindo a largura/altura REAL declarada ali -- em vez de
    chutar a partir do tamanho em bytes. Cobre a maioria dos casos comuns;
    helpers customizados por ator continuam exigindo checagem manual.
    Retorna {symbol: (w, h)}. Em caso de chamadas conflitantes para o mesmo
    simbolo (raro), mantem a primeira encontrada.
    """
    dims = {}
    for path in root.rglob("*.c"):
        try:
            text = path.read_text(encoding="utf-8", errors="ignore")
        except OSError:
            continue
        if "DPLoadTexture" not in text:
            continue
        for m in _LOAD_MACRO_RE.finditer(text):
            macro_name, argstr = m.group(1), m.group(2)
            args = _split_top_level_args(argstr)
            if len(args) < 3:
                continue
            # o ponteiro da textura (timg) e o 1o arg (macros "gs...") ou o
            # 2o (macros com "pkt" na frente); aceitamos qualquer identificador
            # de argumento que bata com um simbolo de textura conhecido.
            wh = None
            for a in args[:3]:
                sym = a.strip()
                if not re.match(r'^[A-Za-z_][A-Za-z0-9_]*$', sym):
                    continue
                if sym.startswith('G_') or sym.startswith('gDisplayList') or sym in ('pkt', 'displayListHead'):
                    continue  # macros/RDP-enums/ponteiro de display list, nao textura
                if sym in dims:
                    continue
                if wh is None:
                    wh = _dims_from_macro_args(macro_name, args)
                    if wh is None:
                        break
                dims[sym] = wh
    return dims


def generated_format(data_len, w, h):
    """Infere somente formatos não ambíguos a partir do tamanho descomprimido.

    RGBA16/32 são determinados pelo tamanho. Para 4bpp, os bancos de fontes
    usam intensidade; exportamos I4, que preserva seus valores sem inventar uma
    paleta. CI4/CI8 sem TLUT conhecida nunca são exportados em escala de cinza.
    """
    pixels = w * h
    if data_len == pixels * 4:
        return "rgba32"
    if data_len == pixels * 2:
        return "rgba16"
    if data_len == (pixels + 1) // 2:
        return "i4"
    return None


def generated_palette(symbol, asset_info, generated_by_symbol, recipes_by_symbol,
                      rom, cache):
    """Busca uma TLUT declarada por metadata, sem tentativa por nome parecido."""
    tlut = asset_info.get("tlut") if asset_info else None
    if not isinstance(tlut, str):
        return None
    record = generated_by_symbol.get(tlut) or recipes_by_symbol.get(tlut)
    if record is None:
        return None
    off = int(record["rom_offset"])
    block_off = int(record.get("block_offset", 0))
    size = int(record.get("size", 512))
    raw = source_bytes(rom, off, cache, block_off + size)[block_off:block_off + size]
    return decode_palette(raw)


def extract_generated_textures(root, rom, outdir, cache, asset_json_symbols, recipes_by_symbol,
                                source_dims, symbol_aliases):
    """Exporta recursos visuais dos bancos gerados que podem ser provados corretos.

    O relatório inclui todo item não exportado e o motivo. Isso é deliberado:
    dados TKMK00, microcódigo e texturas indexadas sem TLUT não devem receber
    uma paleta inventada, pois isso produziria PNGs com cores falsas.
    """
    map_path = root / "PUBLIC_GENERATED_BANK_MAP.json"
    if not map_path.is_file():
        return 0, 0
    entries = json.loads(map_path.read_text(encoding="utf-8"))
    generated_by_symbol = {entry["symbol"]: entry for entry in entries}
    menu_dims = load_menu_texture_dims(root)
    exported, skipped = [], []

    for entry in entries:
        bank, symbol = entry["bank"], entry["symbol"]
        if bank.endswith("_kart.c"):
            continue  # ja exportados por extract_kart_sprites
        if bank == "rsp.c":
            skipped.append({"bank": bank, "symbol": symbol, "reason": "microcodigo RSP; nao e imagem"})
            continue

        info = asset_json_symbols.get(symbol)
        dims_source = None
        if info:
            dims = (int(info["width"]), int(info["height"]))
            dims_source = "assets json"
        elif symbol in menu_dims:
            dims = menu_dims[symbol]
            dims_source = "size descomprimido + MenuTexture"
        elif symbol in source_dims:
            dims = source_dims[symbol]
            dims_source = "gDPLoadTextureBlock (source scan)"
        elif symbol_aliases.get(symbol) in source_dims:
            dims = source_dims[symbol_aliases[symbol]]
            dims_source = "gDPLoadTextureBlock (source scan)"
        else:
            dims = None
        if not dims:
            skipped.append({"bank": bank, "symbol": symbol, "reason": "sem dimensoes declaradas no codigo"})
            continue
        w, h = dims
        source = source_bytes(rom, int(entry["rom_offset"]), cache, int(entry["size"]))
        if source[:4] == b"TKMK":
            skipped.append({"bank": bank, "symbol": symbol,
                            "reason": "TKMK00; requer decodificador dedicado"})
            continue

        block_off = int_value(info.get("block_offset", 0)) if info else 0
        data = source[block_off:]
        fmt = (info.get("type", "").lower() if info else "")
        palette = None
        if fmt in ("ci4", "ci8"):
            palette = generated_palette(symbol, info, generated_by_symbol, recipes_by_symbol, rom, cache)
            if palette is None:
                skipped.append({"bank": bank, "symbol": symbol,
                                "reason": "textura indexada sem TLUT declarada"})
                continue
        elif fmt not in BPP:
            fmt = generated_format(len(data), w, h)
            if fmt is None:
                skipped.append({"bank": bank, "symbol": symbol,
                                "reason": f"tamanho {len(data)} nao corresponde a {w}x{h} em formato conhecido"})
                continue

        needed = (w * h * BPP[fmt] + 7) // 8
        if len(data) < needed:
            skipped.append({"bank": bank, "symbol": symbol,
                            "reason": f"dados insuficientes ({len(data)} < {needed})"})
            continue
        try:
            rgba = decode(fmt, data[:needed], w, h, palette)
        except Exception as exc:
            skipped.append({"bank": bank, "symbol": symbol, "reason": str(exc)})
            continue

        rel = Path("generated") / Path(bank).stem / f"{symbol}.png"
        write_png(outdir / rel, w, h, rgba)
        format_source = dims_source
        entry_out = {"png": str(rel).replace("\\", "/"), "bank": bank,
                     "symbol": symbol, "width": w, "height": h,
                     "format": fmt,
                     "decoded_hash_fnv1a32": f"{fnv1a32(data[:needed]):08x}",
                     "format_source": format_source}

        # Texturas resolvidas via a tabela MenuTexture sao desenhadas por
        # render_menu_textures()/func_80095E10, que particiona em TMEM se
        # nao couber numa carga so (RGBA16/IA16 grandes, ex: retratos de
        # selecao de personagem). Cada particao precisa do proprio hash.
        if format_source == "size descomprimido + MenuTexture" and fmt in ("rgba16", "ia16"):
            bpp_bytes = 2
            row_bytes = w * bpp_bytes
            tiles = compute_menu_tmem_tiles(w, h)
            if len(tiles) > 1:
                entry_out["tmem_tiles"] = [
                    {"y0": y0, "y1": y1,
                     "hash": f"{fnv1a32(data[y0*row_bytes:y1*row_bytes]):08x}"}
                    for (y0, y1) in tiles
                ]

        exported.append(entry_out)

    (outdir / "generated_texture_manifest.json").write_text(
        json.dumps(exported, indent=1), encoding="utf-8")
    (outdir / "generated_texture_skipped.json").write_text(
        json.dumps(skipped, indent=1), encoding="utf-8")
    return len(exported), len(skipped)

# ---------------------------------------------------------------- formatos N64

def _x5to8(v):
    return (v << 3) | (v >> 2)


def decode_rgba16_texel(col16):
    r = (col16 >> 11) & 0x1F
    g = (col16 >> 6) & 0x1F
    b = (col16 >> 1) & 0x1F
    a = 255 if (col16 & 1) else 0
    return (_x5to8(r), _x5to8(g), _x5to8(b), a)


def decode(fmt, data, w, h, palette=None):
    out = bytearray(w * h * 4)
    n = w * h
    if fmt == "rgba16":
        for i in range(n):
            col16 = (data[i * 2] << 8) | data[i * 2 + 1]
            out[i*4:i*4+4] = bytes(decode_rgba16_texel(col16))
    elif fmt == "rgba32":
        out[:] = data[:n * 4]
    elif fmt == "ia16":
        for i in range(n):
            g, a = data[i*2], data[i*2+1]
            out[i*4:i*4+4] = bytes((g, g, g, a))
    elif fmt == "ia8":
        for i in range(n):
            b = data[i]
            g = ((b >> 4) & 0xF) * 17
            a = (b & 0xF) * 17
            out[i*4:i*4+4] = bytes((g, g, g, a))
    elif fmt == "i8":
        for i in range(n):
            g = data[i]
            out[i*4:i*4+4] = bytes((g, g, g, 255))
    elif fmt == "ia4":
        for i in range(n):
            b = (data[i >> 1] >> (0 if i & 1 else 4)) & 0xF
            g = ((b >> 1) & 0x7) * 36
            a = 255 if (b & 1) else 0
            out[i*4:i*4+4] = bytes((g, g, g, a))
    elif fmt == "i4":
        for i in range(n):
            g = ((data[i >> 1] >> (0 if i & 1 else 4)) & 0xF) * 17
            out[i*4:i*4+4] = bytes((g, g, g, 255))
    elif fmt in ("ci8", "ci4"):
        if palette is None:
            for i in range(n):
                idx = data[i] if fmt == "ci8" else ((data[i >> 1] >> (0 if i & 1 else 4)) & 0xF)
                v = idx * (17 if fmt == "ci4" else 1)
                out[i*4:i*4+4] = bytes((v, v, v, 255))
        else:
            for i in range(n):
                idx = data[i] if fmt == "ci8" else ((data[i >> 1] >> (0 if i & 1 else 4)) & 0xF)
                if idx < len(palette):
                    out[i*4:i*4+4] = bytes(palette[idx])
    else:
        raise ValueError("formato desconhecido: " + fmt)
    return bytes(out)


def decode_palette(raw):
    """raw = bytes de uma TLUT rgba16 (2 bytes por entrada, big-endian)."""
    n = len(raw) // 2
    pal = []
    for i in range(n):
        col16 = (raw[i*2] << 8) | raw[i*2+1]
        pal.append(decode_rgba16_texel(col16))
    return pal

# ---------------------------------------------------------------- particionamento de TMEM (render_menu_textures)

TMEM_TILE_BUDGET = 0x400  # constante observada em func_80095E10 (sempre com siz=G_IM_SIZ_16b)


def compute_menu_tmem_tiles(width, height):
    """Replica o particionamento de TMEM que func_80095E10 faz (chamada por
    render_menu_textures em menu_items.c, SEMPRE com siz=G_IM_SIZ_16b).

    A textura e desenhada em faixas horizontais de 'temp_lo' linhas cada,
    porque uma imagem RGBA16/IA16 maior que a TMEM (4KB) nao cabe numa unica
    carga. import_texture() roda uma vez POR FAIXA, cada vez com um ponteiro
    de origem e tamanho DIFERENTES (so aquela faixa) -- entao o hash tem que
    ser calculado por faixa, nao da imagem inteira, para bater com o hash
    calculado em runtime.

    Para imagens que cabem numa carga so, isso naturalmente devolve uma unica
    faixa cobrindo a imagem inteira -- compativel com o caso simples.
    """
    var_t0 = 1
    while var_t0 < width:
        var_t0 *= 2
    temp_lo = TMEM_TILE_BUDGET // max(var_t0, 1)
    while (temp_lo // 2) > height:
        temp_lo //= 2
    if temp_lo < 1:
        temp_lo = 1
    tiles = []
    y = 0
    while y < height:
        y1 = min(y + temp_lo, height)
        tiles.append((y, y1))
        y = y1
    return tiles

# ---------------------------------------------------------------- dimensoes (fallback quando nao ha YAML)

BPP = {"rgba32": 32, "rgba16": 16, "ia16": 16, "ia8": 8, "i8": 8,
       "ci8": 8, "ia4": 4, "i4": 4, "ci4": 4}


def guess_dims(npix):
    cands = []
    w = 1
    while w <= npix:
        if npix % w == 0:
            h = npix // w
            if (h & (h - 1)) == 0:
                cands.append((w, h))
        w <<= 1
    if not cands:
        return None
    cands.sort(key=lambda p: (abs(p[0] / p[1] - 1), -p[0]))
    return cands[0]


FMT_RE = re.compile(r"\.(rgba32|rgba16|ia16|ia8|ia4|i8|i4|ci8|ci4)\.inc\.c$", re.I)
EXCLUDE_RE = re.compile(r"staff_ghost", re.I)


def symbol_of(rel):
    """'assets/.../common_texture_traffic_light_01.ci8.inc.c' -> 'common_texture_traffic_light_01'"""
    base = Path(rel).name
    return base.split(".", 1)[0]


# ---------------------------------------------------------------- sprites dos pilotos/karts

def int_value(value):
    """Aceita tanto offsets JSON em hexadecimal quanto inteiros."""
    return int(value, 0) if isinstance(value, str) else int(value)


def dxt_oddline_swap(data, row_bytes):
    """Reproduz a troca de palavras de 32 bits nas linhas impares que o
    G_LOADBLOCK aplica ao carregar para a TMEM (ver comentario
    B17G7E DXT-TMEM-LAYOUT-VERIFY em gfx_pc.c). Em cada linha impar, os dois
    words de 32 bits dentro de cada grupo de 64 bits sao trocados."""
    out = bytearray(data)
    nrows = len(data) // row_bytes if row_bytes else 0
    for r in range(nrows):
        if r % 2 == 0:
            continue
        base = r * row_bytes
        for off in range(0, row_bytes - 7, 8):
            i = base + off
            out[i:i+4], out[i+4:i+8] = out[i+4:i+8], out[i:i+4]
    return bytes(out)


def tmem_halves_for(pixels, w):
    """Janelas de TMEM para uma textura CI8 que nao cabe numa carga so.

    CI8 deixa 2048 bytes para texels (a TLUT ocupa a outra metade dos 4KB).
    MEDIDO em runtime para os karts (64 de largura): a segunda janela comeca
    em 2048-w, ou seja UMA LINHA antes do meio, e as duas se sobrepoem nessa
    linha. Aplicamos a mesma regra para outras larguras, com recorte quando
    a janela passaria do fim dos dados.
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


def extract_asset_sprites(root, rom, outdir, cache, dirname):
    """Exporta sprites descritos por assets/<dirname>/*.json.

    Mesma ideia dos karts, mas generica: o JSON traz rom_offset, dimensoes,
    tipo e a TLUT de cada quadro. Diferencas tratadas aqui:
      - "tlut" pode ser uma string (uma paleta so) ou uma lista;
      - entradas de paleta podem ter "block_offset", um deslocamento DENTRO
        do bloco descomprimido (o caso das TLUTs em common_data).
    Usado para assets/lakitu, que o extrator antes ignorava por completo.
    """
    src_dir = root / "assets" / dirname
    if not src_dir.is_dir():
        return 0, []

    manifest = []
    count = 0
    for json_path in sorted(src_dir.glob("*.json")):
        entries = json.loads(json_path.read_text(encoding="utf-8"))
        for symbol, info in entries.items():
            fmt = str(info.get("type", "")).lower()
            if fmt not in BPP:
                continue
            tlut_ref = info.get("tlut")
            if fmt.startswith("ci") and not tlut_ref:
                continue  # paleta desconhecida: nao inventamos cores

            w, h = int(info["width"]), int(info["height"])
            needed = (w * h * BPP[fmt] + 7) // 8
            block_off = int_value(info.get("block_offset", 0))
            raw = source_bytes(rom, int_value(info["rom_offset"]), cache, block_off + needed)
            pixels = raw[block_off:block_off + needed]
            if len(pixels) < needed:
                print(f"  ! {symbol}: dados curtos ({len(pixels)} < {needed})")
                continue

            palette = None
            if fmt.startswith("ci"):
                refs = [tlut_ref] if isinstance(tlut_ref, str) else list(tlut_ref)
                palette = []
                for pal_symbol in refs:
                    pal = entries.get(pal_symbol)
                    if pal is None:
                        print(f"  ! {symbol}: TLUT ausente ({pal_symbol})")
                        palette = None
                        break
                    pw, ph = int(pal["width"]), int(pal["height"])
                    pboff = int_value(pal.get("block_offset", 0))
                    praw = source_bytes(rom, int_value(pal["rom_offset"]), cache, pboff + pw * ph * 2)
                    palette.extend(decode_palette(praw[pboff:pboff + pw * ph * 2]))
                if palette is None:
                    continue

            try:
                rgba = decode(fmt, pixels, w, h, palette)
            except Exception as exc:
                print(f"  ! {symbol}: {exc}")
                continue

            out_sub = info.get("output_dir") or json_path.stem
            rel = Path(dirname) / out_sub / f"{symbol}.png"
            write_png(outdir / rel, w, h, rgba)
            manifest.append({
                "png": str(rel).replace("\\", "/"),
                "symbol": symbol,
                "rom_offset": f"0x{int_value(info['rom_offset']):X}",
                "width": w, "height": h, "format": fmt,
                "decoded_hash_fnv1a32": f"{fnv1a32(pixels):08x}",
                "tmem_halves": tmem_halves_for(pixels, w) if fmt == "ci8" else [],
            })
            count += 1

    return count, manifest


def extract_kart_sprites(root, rom, outdir, cache):
    """Exporta os 321 quadros CI8 de cada piloto+kart.

    Os JSONs em assets/karts sao a fonte de verdade: cada quadro MIO0 e 64x64,
    e sua TLUT de 256 cores e formada pela paleta do piloto (192 cores) seguida
    da paleta das rodas daquele quadro (64 cores).  Essa segunda metade e o que
    faltava para os sprites nao aparecerem em escala de cinza.
    """
    kart_dir = root / "assets" / "karts"
    manifest = []
    count = 0
    if not kart_dir.is_dir():
        print("Karts : assets\\karts nao encontrado; ignorando sprites de piloto.")
        return count

    for json_path in sorted(kart_dir.glob("*_kart.json")):
        entries = json.loads(json_path.read_text(encoding="utf-8"))
        for symbol, info in entries.items():
            # Os demais itens sao pedaços de paleta; somente os frames sao PNGs.
            if not symbol.endswith("_frame") and "_frame" not in symbol:
                continue
            if info.get("type", "").lower() != "ci8" or not info.get("tlut"):
                continue

            w, h = int(info["width"]), int(info["height"])
            expected = w * h
            compressed = source_bytes(rom, int_value(info["rom_offset"]), cache, expected)
            if len(compressed) < expected:
                print(f"  ! quadro curto: {symbol} ({len(compressed)} < {expected})")
                continue
            pixels = compressed[:expected]

            palette = []
            for palette_symbol in info["tlut"]:
                palette_info = entries.get(palette_symbol)
                if palette_info is None:
                    raise KeyError(f"TLUT ausente para {symbol}: {palette_symbol}")
                pw, ph = int(palette_info["width"]), int(palette_info["height"])
                raw_palette = source_bytes(
                    rom, int_value(palette_info["rom_offset"]), cache, pw * ph * 2)
                palette.extend(decode_palette(raw_palette))
            if len(palette) < 256:
                raise ValueError(f"TLUT incompleta para {symbol}: {len(palette)} cores")

            rgba = decode("ci8", pixels, w, h, palette)
            relative_output = Path(info["output_dir"]) / f"{symbol}.png"
            write_png(outdir / "karts" / relative_output, w, h, rgba)
            manifest.append({
                "png": str(Path("karts") / relative_output).replace("\\", "/"),
                "symbol": symbol,
                "rom_offset": f"0x{int_value(info['rom_offset']):X}",
                "width": w,
                "height": h,
                "format": "ci8",
                "decoded_hash_fnv1a32": f"{fnv1a32(pixels):08x}",
                # CI8 exige a TLUT ocupando metade da TMEM (4KB), sobrando
                # apenas 2048 bytes para texels. Um sprite CI8 64x64 (4096 B)
                # portanto NAO cabe numa carga so: o jogo o carrega em duas
                # metades de 64x32, e hasheia cada metade separadamente.
                # E o hash de cada metade que precisa casar em runtime.
                # CI8 nao cabe inteiro na TMEM (a TLUT ocupa metade dos 4KB),
                # entao o sprite e carregado em duas janelas de 2048 bytes.
                # MEDIDO em runtime (SCAN_HALVES.py): a segunda janela comeca
                # em 1984, nao em 2048 -- ou seja, UMA LINHA ANTES do meio.
                # As duas metades se sobrepoem em uma linha e a ultima linha
                # da imagem nao e usada.
                "tmem_halves": ([
                    {"y0": 0, "y1": 2048 // w,
                     "hash": f"{fnv1a32(pixels[0:2048]):08x}"},
                    {"y0": (2048 - w) // w, "y1": (2048 - w + 2048) // w,
                     "hash": f"{fnv1a32(pixels[2048 - w:2048 - w + 2048]):08x}"},
                ] if len(pixels) > 2048 and w else []),
                "tlut": info["tlut"],
            })
            count += 1

    (outdir / "kart_sprite_manifest.json").write_text(
        json.dumps(manifest, indent=1), encoding="utf-8")
    return count


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root")
    ap.add_argument("--rom")
    ap.add_argument("--out", default="extracted_textures")
    ap.add_argument("--dims", help="json opcional { 'path/rel.inc.c': [w,h] } para forcar dimensoes")
    ap.add_argument("--force", action="store_true",
                    help="sobrescreve PNGs ja existentes (por padrao eles sao preservados, "
                         "para nao perder edicoes ja feitas)")
    ap.add_argument("--no-karts", action="store_true",
                    help="nao exporta os sprites de piloto+kart de assets/karts")
    ap.add_argument("--no-generated", action="store_true",
                    help="nao exporta texturas verificadas dos bancos gerados do port 360")
    a = ap.parse_args()

    global SKIP_EXISTING_PNGS
    SKIP_EXISTING_PNGS = not a.force

    root = Path(a.root).resolve() if a.root else Path(__file__).resolve().parent
    rom_path = Path(a.rom).resolve() if a.rom else root / "baserom.us.z64"
    if not rom_path.is_file():
        sys.exit("ROM nao encontrada: " + str(rom_path))
    rom = rom_path.read_bytes()

    recipes = json.loads((root / "PUBLIC_ASSET_RECIPES.json").read_text(encoding="utf-8"))
    overrides = json.loads(Path(a.dims).read_text(encoding="utf-8")) if a.dims else {}
    yaml_symbols = load_all_yaml_symbols(root)
    asset_json_symbols = load_asset_json_symbols(root)
    print(f"YAML: {len(yaml_symbols)} simbolos carregados de yamls\\us\\*.yml")
    print(f"JSON: {len(asset_json_symbols)} simbolos carregados de assets\\**\\*.json")
    print("Varrendo o codigo-fonte por chamadas gDPLoadTextureBlock/Tile (pode levar um instante)...")
    symbol_aliases = build_symbol_aliases(root)
    source_dims = scan_source_for_dims(root)
    print(f"SRC : {len(source_dims)} simbolos com dimensao real encontrada no codigo-fonte "
          f"({len(symbol_aliases)} aliases arquivo->variavel-C mapeados)")

    recipes_by_symbol = {symbol_of(r["path"]): r for r in recipes}

    # Para arquivos sem sufixo de formato (texturas de curso empacotadas em
    # arrays [][N], ex: gTextureMole1.inc.c): se a pasta tiver exatamente
    # UMA recipe com "tlut" no caminho, usamos ela como paleta CI8 por default.
    tlut_by_dir = defaultdict(list)
    for r in recipes:
        rp = str(r["path"]).replace("\\", "/")
        if "tlut" in rp.lower():
            tlut_by_dir[str(Path(rp).parent)].append(r)

    outdir = root / a.out
    cache = {}
    dims_report = {}
    ok = skipped = guessed = from_metadata = 0
    skip_list = []

    for r in recipes:
        rel = str(r["path"]).replace("\\", "/")
        if "tlut" in rel.lower():
            continue  # paletas nao sao texturas, sao lidas sob demanda abaixo
        if EXCLUDE_RE.search(rel):
            continue  # nao e textura (ex: staff_ghost = dados de replay)
        m = FMT_RE.search(rel)
        no_suffix = not m
        fmt = m.group(1).lower() if m else "ci8"  # sem sufixo -> assume CI8 (padrao para sprites de curso)
        size = int(r["size"])
        off = int(r.get("block_offset", 0))
        data = source_bytes(rom, int(r["rom_offset"]), cache, off + size)[off:off + size]

        sym = symbol_of(rel)
        # Os JSONs de assets sao mais completos para texturas de curso; os YAMLs
        # continuam cobrindo os assets que nao possuem entrada JSON.
        yinfo = asset_json_symbols.get(sym) or yaml_symbols.get(sym)
        palette = None
        used_yaml = False

        if yinfo and "width" in yinfo and "height" in yinfo:
            yw, yh = int(yinfo["width"]), int(yinfo["height"])
            yfmt = yinfo.get("type", yinfo.get("format", fmt)).lower()
            eff_fmt = yfmt if yfmt in BPP else fmt
            need_bytes = (yw * yh * BPP[eff_fmt] + 7) // 8
            if need_bytes <= len(data):
                w, h, fmt, used_yaml = yw, yh, eff_fmt, True
                from_metadata += 1
                tlut_sym = yinfo.get("tlut", yinfo.get("tlut_symbol"))
                if isinstance(tlut_sym, list):
                    # Listas sao usadas pelos sprites de kart, tratados abaixo.
                    tlut_sym = None
                if fmt in ("ci4", "ci8") and tlut_sym and tlut_sym in recipes_by_symbol:
                    tr = recipes_by_symbol[tlut_sym]
                    toff = int(tr.get("block_offset", 0)); tsize = int(tr["size"])
                    traw = source_bytes(rom, int(tr["rom_offset"]), cache, toff + tsize)[toff:toff + tsize]
                    palette = decode_palette(traw)
            else:
                print(f"  ! YAML dims nao cabem nos bytes ({sym}: {yw}x{yh} precisa {need_bytes}B, "
                      f"recipe tem {len(data)}B) -- usando chute")

        used_source_scan = False
        real_sym = symbol_aliases.get(sym, sym)
        if not used_yaml and (sym in source_dims or real_sym in source_dims):
            sw, sh = source_dims.get(sym) or source_dims[real_sym]
            need_bytes = (sw * sh * BPP.get(fmt, 8) + 7) // 8
            if need_bytes <= len(data):
                w, h, used_source_scan = sw, sh, True
                from_metadata += 1

        if not used_yaml and not used_source_scan:
            npix = size * 8 // BPP.get(fmt, 8)
            wh = overrides.get(rel) or guess_dims(npix)
            if wh is None:
                skip_list.append(f"{rel}  ({npix}px, fmt={fmt})")
                skipped += 1
                continue
            w, h = wh
            guessed += 1
            if no_suffix:
                dir_tluts = tlut_by_dir.get(str(Path(rel).parent), [])
                if len(dir_tluts) == 1:
                    tr = dir_tluts[0]
                    toff = int(tr.get("block_offset", 0)); tsize = int(tr["size"])
                    traw = source_bytes(rom, int(tr["rom_offset"]), cache, toff + tsize)[toff:toff + tsize]
                    try:
                        palette = decode_palette(traw)
                    except Exception:
                        palette = None

        try:
            rgba = decode(fmt, data, w, h, palette)
        except Exception as e:
            print("  ! falha:", rel, e)
            skipped += 1
            continue

        h32 = fnv1a32(data)
        write_png(outdir / f"{h32:08x}__{Path(rel).stem}.png", w, h, rgba)
        dims_report[rel] = {"w": w, "h": h, "fmt": fmt, "size": size, "hash": f"{h32:08x}",
                             "source": "metadata" if yinfo else "guess"}
        ok += 1

    (root / "texture_dims.json").write_text(
        json.dumps(dims_report, indent=1, sort_keys=True), encoding="utf-8")
    if skip_list:
        (root / "texture_skip_list.txt").write_text("\n".join(skip_list), encoding="utf-8")

    cache.clear()
    kart_count = 0 if a.no_karts else extract_kart_sprites(root, rom, outdir, cache)

    # Outras pastas de asset com JSON proprio (lakitu, etc). Antes eram
    # ignoradas: os sprites existiam na ROM e estavam mapeados, mas nenhum
    # caminho do extrator os lia.
    assets_root = root / "assets"
    other_count = 0
    other_manifest = []
    if assets_root.is_dir():
        for sub in sorted(p for p in assets_root.iterdir() if p.is_dir()):
            if sub.name in ("karts", "code", "include"):
                continue
            if not any(sub.glob("*.json")):
                continue
            n, man = extract_asset_sprites(root, rom, outdir, cache, sub.name)
            if n:
                print(f"Assets: {n} sprites de '{sub.name}' -> {outdir / sub.name}")
                other_count += n
                other_manifest.extend(man)
    if other_manifest:
        (outdir / "asset_sprite_manifest.json").write_text(
            json.dumps(other_manifest, indent=1), encoding="utf-8")
    # Os três grupos não compartilham blocos. Liberar MIO0 já processados evita
    # acumular dezenas de blocos grandes antes da varredura dos bancos gerados.
    cache.clear()
    generated_count, generated_skipped = (
        (0, 0) if a.no_generated else
        extract_generated_textures(root, rom, outdir, cache, asset_json_symbols, recipes_by_symbol,
                                    source_dims, symbol_aliases)
    )

    print(f"\nOK    : {ok} texturas comuns -> {outdir}")
    print(f"Karts : {kart_count} sprites piloto+kart -> {outdir / 'karts'}")
    print(f"Bancos: {generated_count} texturas verificadas -> {outdir / 'generated'}")
    print(f"  nao exportados: {generated_skipped} (motivos em generated_texture_skipped.json)")
    print(f"  de metadados (dimensao real): {from_metadata}")
    print(f"  adivinhadas:             {guessed}")
    print(f"SKIP  : {skipped}  (lista completa em texture_skip_list.txt)")
    print("Dims gravadas em texture_dims.json")


if __name__ == "__main__":
    main()
