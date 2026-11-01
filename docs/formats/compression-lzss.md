# Compression: whole-file LZSS

World files and sprite/texture files are compressed as a whole with a simple LZSS; FMVs and the memory-card icon are raw.
`CBinaryInputStream::Decompress` 0x1179F0 (`src/P2/bis.cpp`) decodes it into an 8 KB window; `SetDecompress` 0x1179D8 enables it before
`LoadSwFromBrx` `[C:static]`. The algorithm is byte-for-byte the one in SlyTools `sly_decompress.hpp` `[C:verified]` (a fast decoder and a literal
transcription agree on 30 random streams; both sample worlds decode to fully parseable data: 9,969,830 -> 17,420,982 and 1,949,759 -> 4,084,599 bytes).

## Format

- No header and no stored decompressed length: decode exactly `size` bytes from the FILEREC (the tail may decode a few junk bytes from sector padding).
- A flag byte, consumed LSB first; bit = 1 means a literal byte, bit = 0 means a back-reference.
- A back-reference is a little-endian u16 `v`: **absolute** ring index `v & 0x1FFF` (not a distance) and length `(v >> 13) + 3` (3..10),
  copied byte by byte (overlap allowed).
- Ring: 0x2000 bytes; write cursor = output position mod 0x2000. On PS2 the ring is the scratchpad (0x70000000), which is **not cleared**; tools
  zero-fill it. Both samples parse with zero-fill, but nothing proves that no stream references an unwritten slot `[H]`.

```python
def sly_lzss(src: bytes) -> bytes:
    ring = bytearray(0x2000); out = bytearray(); pos = 0; i = 0; n = len(src)
    while i < n:
        flags = src[i]; i += 1
        for _ in range(8):
            if i >= n: break
            if flags & 1:                                  # literal
                b = src[i]; i += 1
                out.append(b); ring[pos] = b; pos = (pos + 1) & 0x1FFF
            else:                                          # reference
                if i + 1 >= n: break
                v = src[i] | (src[i + 1] << 8); i += 2
                off = v & 0x1FFF
                for _ in range((v >> 13) + 3):
                    b = ring[off]; off = (off + 1) & 0x1FFF
                    out.append(b); ring[pos] = b; pos = (pos + 1) & 0x1FFF
            flags >>= 1
    return bytes(out)
```

## Interaction with Align

`Align(n)` in the stream aligns the **absolute offset in the decompressed stream** (the window starts aligned and pending bytes move to the
window start on each refill) `[C:static]`, confirmed at the head/object boundary of both samples `[C:verified]`.
