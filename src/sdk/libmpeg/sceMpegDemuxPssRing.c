// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* (Claude agent, T1589, 2026-10-24) libmpeg demux of a PSS ring buffer: parse pack headers and PES packets and hand each
   packet to the matching sceMpegAddStrCallback entry (id == pes.id & mask), else to the 0xBDFF000000 catch-all entry.
   Built on the sony8 v11 draft (build/hand_wip/sceMpegDemuxPssRing.v11.c). Keys, all from gcc 2.9's reorg/sched/global:
   - the empty asm at the head of the PES loop: the first `== 0x1BA` bne must stay unfilled in reorg pass 1 (an asm insn
     ends reorg's search of the taken thread); the final `goto again` beq then steals `move a0,s1` and its label is
     deleted, so pass 2 fills the bne from the fall-through (`daddu a0,s1`) like the original.
   - the triple do{}while(0) around the final test: flow weights each use by loop depth, so `limit` gets 9 refs and
     outranks mp (s6 limit / s7 mp / fp func). One or two levels give 16/11 diffs, real outer loops 49-87.
   - cbdata = 0 / cont = 1 first and cbgp = 0 after pss: sched1 pairs one store with one ALU set per cycle in source
     order, which gives the original's sw cbdata / sd pos and sd buf / sw cbgp spill-store order.
   - each data pointer in its own block (a0/a1 in the table callback). */
typedef struct {
    unsigned long buf, pos;
    unsigned char *base;
    unsigned char *cur;
    int bits;
    unsigned char *start, *end;
    int size;
} BS;
typedef struct { unsigned int extension, low, high, hasSystemHeader; } HEADER;
typedef struct {            /* as in src/sdk/libmpeg/_PES_packet.c */
    unsigned long id;
    int len;
    int scrambling;
    unsigned long pts;
    unsigned long dts;
    int data;               /* bit position of the payload */
    int datalen;            /* payload length in bytes */
    int top;                /* bit position of the packet start */
} PES;
typedef struct { HEADER hdr; int pad[2]; PES pes; } PSS;
typedef struct {
    int type;
    unsigned char *header;  /* packet start in the ring */
    unsigned char *data;    /* payload start in the ring */
    int len;
    unsigned long pts;
    unsigned long dts;
} CBDATA;
typedef int (*CBFUNC)(void *mp, CBDATA *cb, void *data);
typedef struct {
    unsigned long id;
    unsigned long mask;
    CBFUNC func;
    void *data;
    void *gp;
    int pad;
} CBENT;
typedef struct { char p0[0x60]; CBENT *tbl; int n; } MI;
typedef struct { char p0[0x40]; MI *mi; } MPEG;
extern int _pack_header(BS *p, HEADER *out);
extern int _PES_packet(MI *mi, BS *p, PES *pes);

static inline void *SetGP(void *gp)
{
    void *old;
    __asm__ volatile("move %0, $gp\n\tmove $gp, %1" : "=&r"(old) : "r"(gp) : "memory");
    return old;
}

static inline void Fill(BS *p)
{
    do {
        p->buf |= (unsigned long)*p->cur++ << (56 - p->bits);
        if (p->cur >= p->end) p->cur = p->start;
        p->bits += 8;
    } while (p->bits < 57);
    p->pos = 0;
}

#define CALLBACK(F, D, G) do { \
    { \
        unsigned char *p = bs->base + (pss->pes.top >> 3); \
        cb.type = 6; \
        if (p >= bs->end) p -= bs->size; \
        cb.header = p; \
    } \
    { \
        unsigned char *p = bs->base + (pss->pes.data >> 3); \
        if (p >= bs->end) p -= bs->size; \
        cb.pts = pss->pes.pts; \
        cb.data = p; \
    } \
    cb.len = pss->pes.datalen; \
    cb.dts = pss->pes.dts; \
    old = SetGP(G); \
    cont = (F)(mp, &cb, (D)); \
    SetGP(old); \
} while (0)

int sceMpegDemuxPssRing(MPEG *mp, unsigned char *addr, int size, unsigned char *buftop, int bufsize)
{
    BS bsbuf;
    PSS pssbuf;
    CBDATA cb;
    void *cbdata;
    CBENT *tbl;
    int result;
    void *cbgp;
    BS *bs;
    PSS *pss;
    MI *mi = mp->mi;
    int limit = size << 3;
    CBFUNC func = 0;
    int cont;
    int i;
    void *old;

    cbdata = 0;
    cont = 1;
    tbl = mi->tbl;
    bsbuf.size = bufsize;
    bsbuf.base = addr;
    bsbuf.cur = addr;
    bsbuf.pos = 0;
    bsbuf.start = buftop;
    result = 0;
    bsbuf.end = buftop + bufsize;
    bsbuf.buf = 0;
    bsbuf.bits = 0;
    pss = &pssbuf;
    cbgp = 0;
    Fill(&bsbuf);
    bs = &bsbuf;

    i = 0;
    if (mi->n > 0) do {
        CBENT *e = (CBENT *)((i << 5) + (int)tbl);
        if (e->id == 0xBDFF000000) {
            func = e->func;
            cbdata = e->data;
            cbgp = e->gp;
        }
        if (func) break;
        i++;
    } while (i < mi->n);
    if ((int)(bs->buf >> 32) == 0x1BA) {
again:
        _pack_header(bs, &pss->hdr);
    }
    __asm__ volatile("");  /* reorg barrier: see the note at the top */
    while ((bs->buf >> 40) == 1 && (int)(bs->buf >> 32) != 0x1BA && (int)(bs->buf >> 32) != 0x1B9) {
        if (!((int)bs->pos < limit) || !cont) goto out;
        _PES_packet(mi, bs, &pss->pes);
        if (limit < (int)bs->pos) continue;
        for (i = 0; i < mi->n; i++) {
            CBENT *e = (CBENT *)((i << 5) + (int)tbl);
            if (e->id == (pss->pes.id & e->mask)) {
                CALLBACK(e->func, e->data, e->gp);
                break;
            }
        }
        if (i == mi->n && func)
            CALLBACK(func, cbdata, cbgp);
        if (cont)
            result = (int)(bs->pos >> 3);
    }
out:
    /* depth-3 wrapper = register priority of limit, see the note at the top */
    do { do { do { if (!(limit < (int)bs->pos) && (int)(bs->buf >> 32) == 0x1BA) goto again; } while (0); } while (0); } while (0);
    return result;
}
