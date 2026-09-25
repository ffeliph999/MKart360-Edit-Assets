#!/usr/bin/env python3
"""
EXPERIMENTO BETA -- 60 FPS offline (1-2 jogadores, sem netplay)
=================================================================

Isto NAO e uma correcao oficial do port, e um teste local. Ele muda o ritmo
de simulacao e apresentacao do jogo durante corridas offline de 1 ou 2
jogadores (Time Trial, Grand Prix, VS local): em vez de calcular 2 passos de
fisica por quadro e mostrar so o resultado do segundo (30 quadros por
segundo na tela), passa a calcular 1 passo por quadro e mostrar todos (60
quadros por segundo). O netplay, o modo 3-4 jogadores e os menus continuam
exatamente como sempre, a 30 FPS.

O resultado pode variar. Elementos que sao atualizados por QUADRO exibido em
vez de por PASSO de simulacao (o brilho das rodas, o Lakitu, animacoes de
objetos, efeitos, HUD) podem ficar rodando no dobro da velocidade. Se notar
qualquer bug, travamento ou comportamento estranho -- rode este script com
"revert" para desfazer. Nada aqui e permanente: os arquivos originais ficam
guardados em backup e a compilacao so muda quando voce recompila.

Uso (dentro de mk64-master, ANTES de compilar):
    py .\\EXPERIMENTO_60FPS.py status     -- mostra o estado atual
    py .\\EXPERIMENTO_60FPS.py apply      -- aplica o experimento (faz backup)
    py .\\EXPERIMENTO_60FPS.py revert     -- desfaz e restaura o backup

Depois de apply ou revert, recompile com /t:Rebuild -- os arquivos tocados
(main.c e xbox360_video.cpp) precisam ser recompilados por inteiro.
"""
from pathlib import Path
import argparse, re, sys, shutil, datetime

MARKER = "X360_EXPERIMENTO_60FPS"
MAIN_REL = Path("src/main.c")
VIDEO_REL = Path("src/xbox360/xbox360_video.cpp")


def die(msg):
    print(f"\nERRO: {msg}")
    sys.exit(1)


def find_root():
    """Sobe diretorios a partir daqui ate achar src/main.c."""
    here = Path(__file__).resolve().parent
    for base in (here, *here.parents):
        if (base / MAIN_REL).is_file():
            return base
    die("nao encontrei src/main.c a partir daqui -- rode este script de "
        "dentro de mk64-master")


def backup_path(p):
    return p.with_suffix(p.suffix + ".exp60bak")


def is_patched(text):
    return MARKER in text


def patch_main_c(text):
    if is_patched(text):
        return text, False

    anchor1 = "s32 gGamestate = 0xFFFF;\n"
    if text.count(anchor1) != 1:
        die(f"main.c: ancora de gGamestate nao encontrada como esperado "
            f"(achei {text.count(anchor1)} ocorrencia(s)). O arquivo pode "
            f"ja ter sido modificado de outra forma, ou o projeto mudou de "
            f"versao. Nada foi alterado.")
    injection = f'''#ifdef XBOX360_PORT
/* ===== EXPERIMENTO BETA: 60 FPS offline (aplicado por EXPERIMENTO_60FPS.py) =====
   Corridas offline de 1-2 jogadores a 60 FPS (um passo de simulacao por
   quadro exibido, em vez de dois). Netplay, 3-4 jogadores e menus continuam
   a 30 FPS. Para desfazer: py .\\EXPERIMENTO_60FPS.py revert */
#define {MARKER} 1
int x360_exp60_active = 0;
int x360_exp60_now(void) {{ return x360_exp60_active && gGamestate == RACING; }}
#endif
'''
    text = text.replace(anchor1, anchor1 + injection, 1)

    anchor2 = (
        "    if (sNumVBlanks < 0) {\n"
        "        sNumVBlanks = 1;\n"
        "    }\n"
        "    func_802A4EF4();\n"
    )
    if text.count(anchor2) != 1:
        die("main.c: ancora do inicio de race_logic_loop nao encontrada "
            "como esperado. Nada foi alterado.")
    decide = f'''#if defined(XBOX360_PORT) && {MARKER}
    x360_exp60_active = !x360_net_active() && !x360_net8_active() &&
        (gActiveScreenMode == SCREEN_MODE_1P ||
         gActiveScreenMode == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL ||
         gActiveScreenMode == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL);
#endif
'''
    text = text.replace(anchor2, anchor2 + decide, 1)

    tick_override = f'''#if defined(XBOX360_PORT) && {MARKER}
            if (x360_exp60_active) gTickSpeed = 1;  /* EXPERIMENTO 60 FPS */
#endif
'''
    anchor3 = "        case SCREEN_MODE_1P:\n            gTickSpeed = 2;\n"
    if text.count(anchor3) != 1:
        die("main.c: ancora do caso SCREEN_MODE_1P nao encontrada como "
            "esperado. Nada foi alterado.")
    text = text.replace(anchor3, anchor3 + tick_override, 1)

    pat = re.compile(
        r"(case SCREEN_MODE_2P_SPLITSCREEN_(?:VERTICAL|HORIZONTAL):.*?"
        r"gTickSpeed = 2;\n            \}\n#endif\n)", re.S)
    n = 0

    def rep(m):
        nonlocal n
        n += 1
        return m.group(1) + tick_override

    text = pat.sub(rep, text)
    if n != 2:
        die(f"main.c: esperava 2 blocos de 2 jogadores, encontrei {n}. "
            f"O arquivo pode ter mudado de versao. Nada foi alterado.")

    return text, True


def unpatch_main_c(text):
    # so usado se nao houver backup (caso raro); remove por marcadores
    if not is_patched(text):
        return text, False
    text = re.sub(
        r"#ifdef XBOX360_PORT\n/\* ===== EXPERIMENTO BETA.*?#endif\n\n?",
        "", text, count=1, flags=re.S)
    text = re.sub(
        rf"#if defined\(XBOX360_PORT\) && {MARKER}\n"
        rf"    x360_exp60_active = .*?\n#endif\n\n?",
        "", text, flags=re.S)
    text = re.sub(
        rf"#if defined\(XBOX360_PORT\) && {MARKER}\n"
        rf"            if \(x360_exp60_active\) gTickSpeed = 1;.*?\n#endif\n",
        "", text)
    return text, True


def patch_video_cpp(text):
    if is_patched(text):
        return text, False
    anchor1 = 'extern "C" void x360_present_and_pace(void) {'
    if text.count(anchor1) != 1:
        die("xbox360_video.cpp: ancora de x360_present_and_pace nao "
            "encontrada. Nada foi alterado.")
    text = text.replace(
        anchor1,
        f'extern "C" int x360_exp60_now(void);  /* {MARKER} (main.c) */\n'
        + anchor1, 1)

    anchor2 = "    LONGLONG frame=g_clock_frequency.QuadPart/30;"
    if text.count(anchor2) != 1:
        die("xbox360_video.cpp: ancora do limitador de 30 FPS nao "
            "encontrada. Nada foi alterado.")
    text = text.replace(
        anchor2,
        "    LONGLONG frame=g_clock_frequency.QuadPart/"
        "(x360_exp60_now()?60:30);", 1)
    return text, True


def cmd_status(root):
    main_p, video_p = root / MAIN_REL, root / VIDEO_REL
    for label, p in (("main.c", main_p), ("xbox360_video.cpp", video_p)):
        if not p.is_file():
            print(f"  {label}: NAO ENCONTRADO ({p})")
            continue
        patched = is_patched(p.read_text(encoding="utf-8", errors="ignore"))
        bkp = backup_path(p)
        print(f"  {label}: {'APLICADO' if patched else 'original'}"
              f"{'  (backup presente)' if bkp.is_file() else ''}")


def cmd_apply(root):
    print(__doc__)
    resp = input("\nDigite APLICAR para confirmar (qualquer outra tecla "
                  "cancela): ").strip()
    if resp != "APLICAR":
        print("cancelado, nada foi alterado.")
        return

    for rel, patch_fn in ((MAIN_REL, patch_main_c), (VIDEO_REL, patch_video_cpp)):
        p = root / rel
        if not p.is_file():
            die(f"{rel} nao encontrado em {root}")
        text = p.read_text(encoding="utf-8", errors="ignore")
        if is_patched(text):
            print(f"  {rel}: ja estava aplicado, nada a fazer")
            continue
        new_text, changed = patch_fn(text)
        if not changed:
            continue
        bkp = backup_path(p)
        if not bkp.is_file():
            shutil.copy2(p, bkp)
            print(f"  backup salvo: {bkp.name}")
        else:
            print(f"  backup ja existia, mantendo: {bkp.name}")
        p.write_text(new_text, encoding="utf-8")
        print(f"  {rel}: experimento aplicado")

    print(f"\nPronto -- data: {datetime.date.today()}")
    print("Agora recompile com /t:Rebuild para o experimento valer no .xex.")
    print("Se notar qualquer bug em jogo, rode:  py .\\EXPERIMENTO_60FPS.py revert")


def cmd_revert(root):
    algo_feito = False
    for rel in (MAIN_REL, VIDEO_REL):
        p = root / rel
        bkp = backup_path(p)
        if bkp.is_file():
            shutil.copy2(bkp, p)
            print(f"  {rel}: restaurado a partir do backup")
            algo_feito = True
            continue
        if p.is_file():
            text = p.read_text(encoding="utf-8", errors="ignore")
            if is_patched(text):
                new_text, changed = unpatch_main_c(text) if rel == MAIN_REL \
                    else patch_video_cpp_revert(text)
                if changed:
                    p.write_text(new_text, encoding="utf-8")
                    print(f"  {rel}: sem backup -- removido por marcadores "
                          f"(confira o resultado antes de compilar)")
                    algo_feito = True
            else:
                print(f"  {rel}: ja estava no original, nada a fazer")

    if not algo_feito:
        print("  nada para reverter -- os arquivos ja estao no original.")
        return
    print("\nRecompile com /t:Rebuild para o jogo voltar ao original de 30 FPS.")


def patch_video_cpp_revert(text):
    text = re.sub(
        rf'extern "C" int x360_exp60_now\(void\);  /\* {MARKER} \(main\.c\) \*/\n',
        "", text)
    text = text.replace(
        "    LONGLONG frame=g_clock_frequency.QuadPart/"
        "(x360_exp60_now()?60:30);",
        "    LONGLONG frame=g_clock_frequency.QuadPart/30;", 1)
    return text, True


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                  formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("action", choices=["status", "apply", "revert"])
    a = ap.parse_args()

    root = find_root()
    print(f"projeto: {root}\n")

    if a.action == "status":
        cmd_status(root)
    elif a.action == "apply":
        cmd_apply(root)
    else:
        cmd_revert(root)


if __name__ == "__main__":
    main()
