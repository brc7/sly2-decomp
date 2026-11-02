// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* (Claude agent) libmpeg demux: MPEG-2 PES packet header parser. BS helpers as in src/sdk/libmpeg/_pack_header.c.
   Keys: tests on pes->id (no local copy); PTS/DTS as lo/hi like _pack_header; `flags == 3` test outside the
   `flags & 2` block; block-scoped `len` in the no-header branch. tbl = bit sizes of ES_rate/DSM/copy/CRC fields. */
typedef struct {
    unsigned long buf, pos;
    unsigned char *base;
    unsigned char *cur;
    int bits;
    unsigned char *start, *end;
    int size;
} BS;
typedef struct {
    unsigned long id;
    int len;            /* PES_packet_length */
    int scrambling;
    unsigned long pts;
    unsigned long dts;
    int data;           /* bit position of the payload */
    int datalen;        /* payload length in bytes */
    int top;            /* bit position of the packet start */
} PES;
typedef struct MI MI;
void _sceMpegError(MI *mi, const char *msg);

static __inline__ void Skip(BS *p, int n)
{
    p->buf <<= n;
    p->bits -= n;
    if (p->bits < 57) {
        do {
            p->buf |= (unsigned long)*p->cur++ << (56 - p->bits);
            if (p->cur >= p->end) p->cur = p->start;
            p->bits += 8;
        } while (p->bits < 57);
    }
    p->pos += n;
}
static __inline__ unsigned int Get(BS *p, int n)
{
    unsigned int value = p->buf >> (64 - n);
    Skip(p, n);
    return value;
}
static __inline__ void SkipBytes(BS *p, int n)
{
    if (n) {
        p->buf = 0;
        p->bits = 0;
        p->pos += n * 8;
        p->cur = p->base + (int)(p->pos >> 3);
        if (p->cur >= p->end) p->cur -= p->size;
        Skip(p, 0);
    }
}

int _PES_packet(MI *mi, BS *p, PES *pes)
{
    unsigned char tbl[16] = {0, 16, 8, 24, 8, 24, 16, 32, 24, 40, 32, 48, 32, 48, 40, 56};
    unsigned int hi, lo;
    unsigned int flags, escr, misc, ext, hdrlen, a, b, c;
    unsigned int priv, packf, seqc, pstd, ext2, extlen, i;
    int top, n;

    pes->top = p->pos;
    Skip(p, 24);
    pes->id = (unsigned long)Get(p, 8) << 32;
    pes->len = Get(p, 16);
    pes->dts = -1;
    pes->pts = -1;
    if (pes->id != 0xBC00000000 && pes->id != 0xBE00000000 && pes->id != 0xBF00000000 && pes->id != 0xF000000000
        && pes->id != 0xF100000000 && pes->id != 0xFF00000000 && pes->id != 0xF200000000 && pes->id != 0xF800000000) {
        Skip(p, 2);
        pes->scrambling = Get(p, 2);
        Skip(p, 4);
        flags = Get(p, 2);
        escr = Get(p, 1);
        misc = Get(p, 4);
        ext = Get(p, 1);
        hdrlen = Get(p, 8);
        top = p->pos;
        if (flags & 2) {
            Skip(p, 4);
            a = Get(p, 3);
            Skip(p, 1);
            b = Get(p, 15);
            Skip(p, 1);
            c = Get(p, 15);
            Skip(p, 1);
            lo = (a << 30) | (b << 15) | c;
            hi = a >> 2;
            pes->pts = ((unsigned long)hi << 32) | lo;
        }
        if (flags == 3) {
            Skip(p, 4);
            a = Get(p, 3);
            Skip(p, 1);
            b = Get(p, 15);
            Skip(p, 1);
            c = Get(p, 15);
            Skip(p, 1);
            lo = (a << 30) | (b << 15) | c;
            hi = a >> 2;
            pes->dts = ((unsigned long)hi << 32) | lo;
        }
        if (escr == 1) Skip(p, 48);
        if (misc) Skip(p, tbl[misc]);
        if (ext == 1) {
            priv = Get(p, 1);
            packf = Get(p, 1);
            seqc = Get(p, 1);
            pstd = Get(p, 1);
            Skip(p, 3);
            ext2 = Get(p, 1);
            if (priv == 1) {
                Skip(p, 48);
                Skip(p, 48);
                Skip(p, 32);
            }
            if (packf == 1) {
                _sceMpegError(mi, "pack_header_field_flag needs to be '0' in PS\n");
                return 0;
            }
            if (seqc == 1) Skip(p, 16);
            if (pstd == 1) Skip(p, 16);
            if (ext2 == 1) {
                Skip(p, 1);
                extlen = Get(p, 7);
                for (i = 0; i < extlen; i++) Skip(p, 8);
            }
        }
        SkipBytes(p, hdrlen - (int)((p->pos - top) >> 3));
        n = pes->len - hdrlen - 3;
        pes->datalen = n;
        pes->data = p->pos;
        if (pes->id == 0xBD00000000) {
            pes->id |= Get(p, 32);
            n -= 4;
        }
        SkipBytes(p, n);
    } else if (pes->id == 0xBC00000000 || pes->id == 0xBF00000000 || pes->id == 0xF000000000 || pes->id == 0xF100000000
               || pes->id == 0xFF00000000 || pes->id == 0xF200000000 || pes->id == 0xF800000000) {
        int len = pes->len;
        if (pes->id == 0xBF00000000) {
            pes->id |= Get(p, 32);
            len -= 4;
        }
        SkipBytes(p, len);
    } else if (pes->id == 0xBE00000000) {
        SkipBytes(p, pes->len);
    }
    return 1;
}
