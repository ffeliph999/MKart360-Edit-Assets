# MKart360 — Edit Textures Fork

Fork of [sirdankz/MKart360](https://github.com/sirdankz/MKart360) (Xbox 360
port of Mario Kart 64, based on the [n64decomp/mk64](https://github.com/n64decomp/mk64)
decompilation). This fork works **only with the US ROM** and adds a
**runtime HD texture replacement system**, without modifying the ROM or the
compiled game assets.

---

## What changed compared to the original project

### Modified code

- **`mk64-master/include/xbox360/gfx_pc.c`**
  HD texture hook inside `import_texture()`: the game computes an FNV-1a
  hash of each original texture's content, and if an HD version with that
  hash exists, it's used instead. Includes:
  - reading `tex.pak` through a hash-indexed table;
  - fallback to standalone `.tex` files in subfolders (to work around the
    FATX per-folder file limit);
  - RAM cache for HD textures (avoids re-reading from disk);
  - enlarged texture cache (from 512 to 1024 entries);
  - diagnostic trace disabled by default (`X360_HDTEX_TRACE 0`).
  - `x360_try_draw_hd_menu_quad` already exists in the code but is
    **inactive** (not called yet) — reserved for a future replacement of
    the large menu textures.

- **`mk64-master/src/xbox360/xbox360_renderer.cpp`**
  Now reuses the Direct3D texture when its size doesn't change, avoiding
  `CreateTexture`/`Release` on every frame submitted — reduces overhead
  when textures are swapped frequently.

- **`mk64-master/PUBLIC_SOURCE_GOLD_HASHES.json`**
  Renderer verification hash updated to reflect the change above.

- **`mk64-master/.gitignore`**
  Now ignores extracted textures, `tex.pak`, trace logs, and
  `src/xbox360/generated_banks/` (all derived from the user's own ROM, and
  shouldn't go into version control).

### Known limitations

- Large menu images (**TKMK00** format — title screen, character names)
  are **not** replaced yet.
- Lakitu's TMEM windows (56×72) follow the same rule measured for karts,
  without their own dedicated measurement — may not be fully accurate.

---

## New tools

All scripts below live in `mk64-master/` and are run from inside that
folder.

| File | Purpose |
|---|---|
| `EXTRACT_MK64_TEXTURES.py` | Extracts textures from the ROM into editable PNGs |
| `EXTRACT_LAKITU.py` | Extracts all Lakitu frames specifically (starting light, checkered flag, fishing, final lap, wrong-way, etc.) |
| `PACK_TEXTURES.py` | Packs edited PNGs back into the format the game reads (`tex.pak`) |
| `HALVE_PNGS.py` | Halves PNG resolution, to fit the console's memory |
| `SCAN_HALVES.py` | Diagnostic tool: finds where the "bottom halves" of kart sprites live when their hash doesn't match |
| `CROSS_CHECK.py` | Cross-references the game's trace log with the manifests to find textures that weren't found |

One-time install requirement:

```powershell
pip install pillow
```

### Full usage workflow

**1. Extract textures from the ROM**

```powershell
py .\EXTRACT_MK64_TEXTURES.py --rom .\baserom.us.z64
```

Generates the `extracted_textures\` folder with:

- **root** — common textures, named `<hash>__name.png`;
- **`generated\`** — generated banks (menus, HUD), named by symbol;
- **`karts\<character>\frames\`** — driver+kart sprites (321 per
  character);
- **`*_manifest.json`** — metadata linking each PNG to its hash. **Do not
  delete or move these files.**

By default the extractor doesn't overwrite existing PNGs (safe to re-run —
it only fills in what's missing). Use `--force` to regenerate everything
from scratch (discards your edits). Other options: `--no-karts`,
`--no-generated`, `--out FOLDER`.

To extract only Lakitu's frames:

```powershell
py .\EXTRACT_LAKITU.py --rom .\baserom.us.z64
py .\EXTRACT_LAKITU.py --rom .\baserom.us.z64 --force   # overwrites PNGs
```

**2. Edit**

Open the PNGs and redraw/upscale them in HD. You can freely change the
resolution (e.g. 256×256 instead of a 64×64 original). **Don't rename the
files** — the name (or the path recorded in the manifest) is what links the
edited texture back to the original.

**3. Pack**

```powershell
py .\PACK_TEXTURES.py --only karts --pak
```

Generates `tex\tex.pak`, a single file with everything bundled inside.

| Option | Effect |
|---|---|
| `--pak` | Produces a single file (**recommended**) |
| `--only PREFIX` | Limits to a subset, e.g. `--only karts\bowser` |
| `--out FOLDER` | Output folder (default `tex`) |
| `--max N` | Skips images larger than N pixels (default 2048) |

Without `--pak`, thousands of loose `.tex` files are generated — it works,
but loads more slowly and is more fragile to transfer; use it only for
debugging. Only pack what you actually edited: unedited PNGs produce
textures identical to the originals, with no visual gain but still costing
space and memory.

**4. Install on the console**

Copy `tex.pak` to the root of the game folder, next to the executable:

```
MK64.xex
baserom.us.z64
tex.pak          <- here, NOT inside a tex\ folder
```

Swapping textures doesn't require recompiling — `tex.pak` is read at
runtime. Recompiling is only needed when you change C code.

### Memory limits

The Xbox 360 has 512 MB shared between system and video. The full
character roster adds up to 2568 sprites (2 files each):

| Resolution | Estimated total | Notes |
|---|---|---|
| 256×256 | ~640 MB | doesn't fit |
| 128×128 | ~160 MB | recommended |
| 96×96 | ~90 MB | more headroom |

To shrink already-edited PNGs:

```powershell
py .\HALVE_PNGS.py --recursive              # preview, doesn't change anything
py .\HALVE_PNGS.py --apply --recursive      # actually applies it
```

Use `--backup` to keep the originals as `*.orig.png`.

### Diagnostics (when a texture doesn't show up in HD)

In `include\xbox360\gfx_pc.c`, change:

```c
#define X360_HDTEX_TRACE 0   →   1
```

Recompile, play for a few seconds, then grab `game:\hdtex-trace.log`. Each
line shows the hash computed at runtime and `found=1` or `found=0`. To
cross-reference that log with the manifests:

```powershell
py .\CROSS_CHECK.py --log .\hdtex-trace.log --kart bowser
```

If the missing texture is a kart sprite's "bottom half" that never matches
(`found=0` even though it exists in the manifest), run:

```powershell
py .\SCAN_HALVES.py --rom .\baserom.us.z64 --log .\hdtex-trace.log --kart bowser
```

This scans the decompressed ROM block looking for which 2048-byte window
produces the missing hash, revealing the half's real offset — useful for
fixing the extractor.

Leave the trace disabled during normal use: it writes to disk during
gameplay and affects performance.

### Common issues

- **Textures don't show up** → check that `tex.pak` is at the root, next
  to `MK64.xex` (not inside `tex\`). If that's correct, force a full
  rebuild (`/t:Rebuild`) — `gfx_pc.c` is included by another file, and
  incremental builds sometimes miss the change.
- **Only half the sprite is HD** → CI8 sprites are loaded in two windows
  because of the 4 KB TMEM limit, each with its own hash. The extractor
  already handles this; if it happens, regenerate the manifests with the
  updated extractor and repack.
- **Transfer to the console fails at the end** → the Xbox 360's file
  system (FATX) allows a maximum of 4096 files per folder. That's exactly
  what the `--pak` option solves, by producing a single file.
- **Stutters in-game** → lower the resolution (128×128) or the number of
  replaced characters. A small hitch when a new opponent appears on screen
  is expected: their sprites are loaded at that moment.

### How it works under the hood

1. The game loads a texture and computes an FNV-1a hash of the original
   content.
2. `gfx_pc.c` looks up that hash in `tex.pak`.
3. If found, the HD version is sent to the GPU instead of the original.
4. If not found, it falls back to the normal path — nothing breaks.

Original dimensions are preserved internally for coordinate mapping, so
the HD texture can be any resolution. Loaded data is cached in RAM to
avoid re-reading from disk.

---

## How to build the project

### Requirements

- Windows
- Python 3
- Xbox 360 SDK / Visual Studio Xbox 360 build tools
- Your own US Mario Kart 64 ROM
- An Xbox 360 capable of running homebrew XEX files

### 1. Prepare the assets

Place your ROM at:

```
mk64-master\baserom.us.z64
```

Then, inside `mk64-master`:

```powershell
py ".\PUBLIC_PREPARE_MK64_ASSETS.py"
```

The script verifies the ROM and generates the ROM-derived files that are
intentionally left out of the public source release.

### 2. Build

From the folder containing `MK64.sln`, run:

```powershell
& "$env:WINDIR\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe" ".\MK64.sln" /t:Build "/p:Configuration=Release" "/p:Platform=Xbox 360"
```

Or, more simply:

```powershell
powershell -ExecutionPolicy Bypass -File ".\PUBLIC_BUILD_XBOX360.ps1"
```

The compiled `.xex` will be placed in the Xbox 360 project's Release
output folder.

### 3. Apply HD textures (optional)

After building, generate and copy `tex.pak` as described in the
["New tools"](#new-tools) section above. Swapping textures afterward
doesn't require rebuilding.

> If something doesn't take effect after editing `gfx_pc.c`, force
> `/t:Rebuild` — incremental builds sometimes don't detect changes to that
> file.

---

## Credits

- **[n64decomp/mk64](https://github.com/n64decomp/mk64)** — the original
  Mario Kart 64 decompilation, the foundation of the whole project.
- **[sirdankz/MKart360](https://github.com/sirdankz/MKart360)** — the
  Xbox 360 port (native build, console-to-console multiplayer, rendering
  fixes), of which this repository is a fork.

Mario Kart 64 and related properties belong to Nintendo. This is an
unofficial homebrew port, not affiliated with or endorsed by Nintendo.
