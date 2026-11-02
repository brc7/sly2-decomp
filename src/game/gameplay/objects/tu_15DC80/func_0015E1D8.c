struct VTE { short d; short pad; void (*pfn)(); };
typedef struct CLOCK { float t; float dt; } CLOCK;
extern CLOCK D_002DDED0;
struct O { int a[3]; struct VTE *vt; int pad[6]; unsigned long fl; int pad2[14]; float t68; };
/** @sly2-annot addr=0x15E1D8
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Info: TU tu_15DC80
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * @end-sly2-annot */
void func_0015E1D8(struct O *o)
{
    if (!((int)(o->fl >> 60) & 1)) {
        struct VTE *e = &o->vt[8];
        o->t68 = D_002DDED0.t;
        e->pfn((char *)o + e->d);
    }
}
