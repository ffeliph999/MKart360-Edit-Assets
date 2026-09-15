from pathlib import Path
root=Path(__file__).resolve().parents[1]
parts=[]
for file,signatures in [
 ('src/xbox360/xbox360_diagnostics.cpp',['extern "C" int x360_logging_enabled(','extern "C" void x360_set_logging(','extern "C" void x360_log_start(','extern "C" void x360_log(']),
 ('src/xbox360/xbox360_video.cpp',['extern "C" void x360_present_and_pace('])]:
 text=(root/file).read_text(encoding='utf-8')
 for signature in signatures:
  start=text.index(signature);brace=text.index('{',start)
  # x360_log also has a forward declaration at file start.
  if ';' in text[start:brace]:
   start=text.index(signature,start+len(signature));brace=text.index('{',start)
  end=brace+1;depth=1
  while depth:
   depth+=(text[end]=='{')-(text[end]=='}');end+=1
  parts.append(text[start:end])
out=root/'tests/build/performance_production.inc';out.parent.mkdir(exist_ok=True)
out.write_text('\n\n'.join(parts),encoding='utf-8')

# Exercise the diagnostic expressions taken from the graphics interpreter.
gfx=(root/'include/xbox360/gfx_pc.c').read_text(encoding='utf-8')
assignments=[]
for begin in ['rdp.palette_load_capture_hash =','rdp.loaded_texture[load_bank].load_capture_hash =','Gfx *const preflight_bad =']:
 start=gfx.index(begin);end=gfx.index(';',start)+1;assignments.append(gfx[start:end])
(root/'tests/build/performance_graphics.inc').write_text(
 'void diagnostics(){unsigned load_bank=0,size_bytes=4096;Gfx *dl_root=0;\n'+
 '\n'.join(assignments)+'\n}\n',encoding='utf-8')
