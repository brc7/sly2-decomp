#!/usr/bin/env python3
"""Extract members from a PCSX2 save state (.p2s = zip, members usually Zstandard-compressed, zip method 93).

  python3 savestate.py list  STATE.p2s
  python3 savestate.py extract STATE.p2s OUTDIR [member ...]   (default members: eeMemory.bin Screenshot.png)

For the snapshot pipeline: extract eeMemory.bin into WORK/ram/ (default build/snapshots/ram/, git-ignored).
Stdlib + one Zstandard decoder for method 93 (see zstd_decompress).
eeMemory.bin is the PS2's 32 MB main RAM: EE address A (KSEG0/physical, < 0x02000000) is at file offset A.
"""
import os, struct, subprocess, sys, zipfile

def _raw(zf, info):
    with open(zf.filename, 'rb') as f:
        f.seek(info.header_offset)
        h = f.read(30)
        n, m = struct.unpack('<HH', h[26:30])
        f.seek(info.header_offset + 30 + n + m)
        return f.read(info.compress_size)

def zstd_decompress(raw, size):
    """First available of: `zstandard`, `backports.zstd`, `compression.zstd` (Python 3.14+), the `zstd` CLI."""
    try:
        import zstandard
        return zstandard.ZstdDecompressor().decompress(raw, max_output_size=size)
    except ImportError:
        pass
    for mod in ('backports.zstd', 'compression.zstd'):
        try:
            return __import__(mod, fromlist=['decompress']).decompress(raw)
        except ImportError:
            pass
    return subprocess.run(['zstd', '-dc'], input=raw, capture_output=True, check=True).stdout

def read_member(path, name):
    zf = zipfile.ZipFile(path)
    info = zf.getinfo(name)
    if info.compress_type in (0, 8):
        return zf.read(name)
    if info.compress_type == 93:
        raw = _raw(zf, info)
        return zstd_decompress(raw, info.file_size)
    raise SystemExit(f'{name}: unsupported compression {info.compress_type}')

def main():
    if len(sys.argv) < 3 or sys.argv[1] in ('-h', '--help'):
        print(__doc__); sys.exit(0 if len(sys.argv) > 1 else 2)
    cmd, path = sys.argv[1], sys.argv[2]
    if cmd == 'list':
        for i in zipfile.ZipFile(path).infolist():
            print(f'{i.filename:32s} {i.file_size:>10d}  method {i.compress_type}')
    elif cmd == 'extract':
        out = sys.argv[3]; os.makedirs(out, exist_ok=True)
        stem = os.path.splitext(os.path.basename(path))[0]
        for name in (sys.argv[4:] or ['eeMemory.bin', 'Screenshot.png']):
            d = read_member(path, name)
            dst = os.path.join(out, f'{stem}.{name}')
            open(dst, 'wb').write(d); print(dst, len(d))
    else:
        raise SystemExit(__doc__)

if __name__ == '__main__':
    main()
