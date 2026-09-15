"""Compile the exact HUD entry points without unrelated platform dependencies."""
from pathlib import Path
root = Path(__file__).resolve().parents[1]
functions = [
    ("src/actors/blue_and_red_shells/update.inc.c", "static s16 red_shell_target_ahead("),
    ("src/code_80057C60.c", "void render_hud("),
    ("src/render_objects.c", "void x360_render_online_place("),
    ("src/racing/skybox_and_splitscreen.c", "void func_802A3730("),
    ("src/cpu_vehicles_camera_path.c", "void set_places("),
    ("src/racing/race_logic.c", "void update_race_position_data("),
]
parts = []
for path, signature in functions:
    text = (root / path).read_text(encoding="utf-8")
    start = text.index(signature)
    brace = text.index("{", start)
    depth = 1
    end = brace + 1
    while depth:
        if text[end] == "{": depth += 1
        if text[end] == "}": depth -= 1
        end += 1
    parts.append(text[start:end])
out = root / "tests/build/hud_production.inc"
out.parent.mkdir(exist_ok=True)
out.write_text("\n\n".join(parts), encoding="utf-8")

# Keep the rank/lap portion of native initialization verbatim. The host harness
# mocks the remaining path/AI setup, while verifying the online spawn calls it.
text = (root / "src/cpu_vehicles_camera_path.c").read_text(encoding="utf-8")
start = text.index("        gLapCountByPlayerId[i] = -1;", text.index("void init_players("))
end = text.index("        gWrongDirectionCounter[i]", start)
with out.open("a", encoding="utf-8") as f:
    f.write("\nstatic void initialize_rank_fixture(void) { s32 i, temp_v0_3;\n")
    f.write("for (i=0;i<NUM_PLAYERS;++i) { Player *player=&gPlayers[i];\n")
    f.write(text[start:end])
    f.write("} }\n")
text = (root / "src/actors/blue_and_red_shells/update.inc.c").read_text(encoding="utf-8")
start = text.index("                    shell->targetPlayer = red_shell_target_ahead(shell->playerId);")
end = text.index("\n                }\n            }", start)
with out.open("a", encoding="utf-8") as f:
    f.write("\nstatic void acquire_red_shell_target(struct ShellActor *shell) {\n")
    f.write("Player *player=&gPlayers[shell->playerId]; s32 temp_v0;\n")
    f.write(text[start:end])
    f.write("\n}\n")
