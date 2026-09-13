from pathlib import Path
root=Path(__file__).resolve().parents[1]
sources=[]
skip_parts={"/src/os/","/asm/","/tools/"}
for p in root.rglob("*"):
    if p.suffix.lower() in {".c",".cpp"}:
        s="/"+str(p.relative_to(root)).replace("\\","/")
        if not any(x in s for x in skip_parts):
            sources.append(str(p.relative_to(root)))
out=root/"xbox360"/"xdk_source_manifest.txt"
out.write_text("\n".join(sorted(sources)))
print("Wrote",out,"with",len(sources),"source files")
