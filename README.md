# Mario Kart 64 - Xbox 360 Port

An Xbox 360 port of Mario Kart 64 based on the [n64decomp/mk64](https://github.com/n64decomp/mk64) decompilation project.

This version includes Xbox 360 build support, 2-4 player console-to-console multiplayer, Xbox LIVE Party-assisted host discovery, and per-console fullscreen player views.

## Features

- Native Xbox 360 project/source files
- 2, 3, and 4 player console-to-console multiplayer
- LAN and direct Internet host/join
- Xbox LIVE Party-assisted host discovery and join flow
- Per-console fullscreen views for P1/P2/P3/P4
- 16:9 and centered 4:3 display modes
- Controller rebinding/tuning options
- Independent Game / Netplay / Party logging controls
- Split-screen aspect-ratio correction
- Sky/background rendering fixes
- Public asset-generation scripts

Each network player uses a separate Xbox 360.

## September 2026 update

The boot menu now includes an **Options** screen with display, controller, and logging settings. Display mode can be switched between **16:9** and centered **4:3** before starting Offline, Host, or Join.

Netplay protocol 3 uses adaptive deterministic input buffering and early input/relay paths to reduce unnecessary host-relay latency for 2-4 player sessions while preserving synchronized simulation. All consoles in the same session must run the same build.

Xbox LIVE Party integration is used as a rendezvous path only. The host can open the normal Party/Friends UI, and a guest who joins the same Party can use the Join screen to discover the MK360 host automatically. Actual gameplay still uses MK360's normal UDP transport on port 6464. Manual IP joining remains available.

Logging is split into three independent options:

- **Game Logging** - normal MK64 diagnostics
- **Netplay Logging** - writes `mk64-netplay.log`
- **Party Logging** - writes `mk64-party-host.log`

Only one file logger can be enabled at a time to avoid overlapping synchronous log writes during gameplay. Netplay and Party logging default to off.

The Windows regression suite is in `mk64-master/tests` and covers online slots/aspects, packet validation, desync detection, history wrap, and multi-player loss/jitter/outage simulation. Live Xbox 360 testing has also been used during development of the Party and multiplayer paths.

## What is not included

This repository does **not** include:

- A Mario Kart 64 ROM
- ROM-derived generated assets
- A prebuilt XEX
- An Xbox 360 ISO

You must provide your own US Mario Kart 64 ROM.

Place it here:

```text
mk64-master\baserom.us.z64
```

## Requirements

- Windows
- Python 3
- Xbox 360 SDK / Visual Studio Xbox 360 build tools
- Your own US Mario Kart 64 ROM
- An Xbox 360 capable of running homebrew XEX files

## Preparing the assets

Open PowerShell in the `mk64-master` folder and run:

```powershell
py ".\PUBLIC_PREPARE_MK64_ASSETS.py"
```

The script verifies the ROM and creates the generated files that are intentionally left out of the public source release.

## Building

From the folder containing `MK64.sln`, run:

```powershell
& "$env:WINDIR\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe" ".\MK64.sln" /t:Build "/p:Configuration=Release" "/p:Platform=Xbox 360"
```

You can also use:

```powershell
powershell -ExecutionPolicy Bypass -File ".\PUBLIC_BUILD_XBOX360.ps1"
```

The compiled XEX will be placed in the Xbox 360 project's Release output folder.

## Multiplayer

Make sure `baserom.us.z64` is beside `MK64.xex` on the console.

### Xbox LIVE Party-assisted join

1. Host chooses **Host 2-4 Player Game**.
2. Host presses **X** to open/start the Xbox LIVE Party flow and invites friends to the Party.
3. Guest joins the host's Xbox LIVE Party, launches MK360, chooses **Join**, then presses **X** to find the host from Party data.
4. The host starts the MK360 session normally after the players are connected.

### Manual LAN / Internet join

Guests can still enter the host's LAN IPv4 address or public IPv4 address manually.

The multiplayer code uses UDP port:

```text
6464
```

For Internet play, UDP 6464 must be able to reach the host console.

Player assignment is:

```text
P1 = Host
P2 = Guest 1
P3 = Guest 2
P4 = Guest 3
```

## Notes

This is a source release. ROM-derived files are intentionally generated locally from the user's own copy of the game.

If you are building from a fresh clone or extracted release, run the asset-preparation script before compiling.

## Credits

This port is based on the work of the **[n64decomp/mk64](https://github.com/n64decomp/mk64) contributors**, who created and maintain the Mario Kart 64 decompilation project.

Mario Kart 64 and related Nintendo properties belong to their respective rights holders. This is an unofficial homebrew port and is not affiliated with or endorsed by Nintendo.
