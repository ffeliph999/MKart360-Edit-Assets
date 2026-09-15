"""Exercise production crop predicates and marker handling without the Xbox GPU."""
from pathlib import Path
root = Path(__file__).resolve().parents[1]
gfx = (root / 'include/xbox360/gfx_pc.c').read_text()
renderer = (root / 'src/xbox360/xbox360_renderer.cpp').read_text()
def predicate(source):
    start = source.index('    const bool local=')
    end = source.index(';', start) + 1
    return source[start:end] + '\n    return local;'
start = gfx.index('            case G_NOOP:')
end = gfx.index('            // RSP commands:', start)
handler = gfx[start:end]
output = root / 'tests/build/hud_bridge_production.inc'
output.parent.mkdir(exist_ok=True)
output.write_text('bool rectangle_crop(){mkview::Rect source;\n' + predicate(gfx) +
    '\n}\nbool triangle_crop(){mkview::Rect crop;int online_players=count,local_slot=slot;\n' +
    predicate(renderer) + '\n}\nvoid marker(unsigned tag){\n' +
    'struct Command {struct {unsigned w1;} words;} command={{tag}};Command *cmd=&command;\n' +
    'switch(G_NOOP){\n' + handler + '}\n}\n')
