from pathlib import Path
import re
root=Path(__file__).resolve().parents[1]
functions=[
 ('src/code_8006E9C0.c','void init_cloud_object('),
 ('src/update_objects.c','void func_800788F8('),
 ('src/update_objects.c','void update_clouds('),
 ('src/render_objects.c','void func_800518F8('),
 ('src/render_objects.c','void func_80051ABC('),
 ('src/render_objects.c','void func_80051EBC('),
 ('src/code_80057C60.c','static s32 x360_draw_online_clouds('),
 ('src/code_80057C60.c','void func_80057FC4(')]
parts=[]
for path,signature in functions:
 text=(root/path).read_text(encoding='utf-8');start=text.index(signature)
 end=text.index('{',start)+1;depth=1
 while depth:
  depth+=(text[end]=='{')-(text[end]=='}');end+=1
 parts.append(text[start:end])
data=(root/'src/data/some_data.c').read_text(encoding='utf-8')
parts+=re.findall(r'CloudData g\w+Clouds\[\] = \{.*?\n\};',data,re.S)
assert len(parts)==14, len(parts)
out=root/'tests/build/cloud_production.inc';out.parent.mkdir(exist_ok=True)
out.write_text('\n\n'.join(parts),encoding='utf-8')
