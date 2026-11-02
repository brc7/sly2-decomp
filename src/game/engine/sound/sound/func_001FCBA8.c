struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[8]; };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x40 - 0x10]; int f40; char pad3[0x50 - 0x44]; unsigned short fl; short s52; };
void ResumeSnd(struct X *);
void SetSndFlags(struct X *, int, int);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
/** @sly2-annot addr=0x1FCBA8
 * Purpose: SUKR start callback: store stream handle or stop on failure; Decref
 * Info: proposed name OnStreamsndStarted (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FCBA8(int f, long l)
{
    struct X *px = (struct X *)l;
    px->f40 = f;
    if (f) SetSndFlags(px, 0x1000, 0x1000);
    else {
        ResumeSnd(px);
        px->s52 = 0;
        if (!(px->fl & 8)) VF(px, 7);
    }
    VF(px, 2);
}
