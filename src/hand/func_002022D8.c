struct O { char pad[0x28]; unsigned long fl; char pad2[0xD0]; struct O *p100; };
struct S { char pad[0x30]; int c; struct O *a[1]; char pad2[0x80]; int nB8; };
#define BIT60(o) ((int)((o)->fl >> 60) & 1)
/** @sly2-annot addr=0x2022D8
 * Purpose: in-volume flag or any visible target
 * Info: proposed name FReconSelAvailable (T2); TU tu_201D78
 * Notes: names_v2 C proposed LoadBrkFromBrx__FP3BRKP18CBinaryInputStream (0.167)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
int func_002022D8(struct S *p)
{
    int i;
    if (p->nB8) return 1;
    for (i = 0; i < p->c; i++) {
        struct O *o = p->a[i];
        if (BIT60(o) && (!o->p100 || !BIT60(o->p100))) return 1;
    }
    return 0;
}
