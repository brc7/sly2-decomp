typedef struct S { char pad[0x1C]; int c; int pad2[2]; unsigned short i; short pad3; int fDirty; int x30; int x34; } S;
int FIsSlEmpty(void *p);
static inline int Clamp(int n, int lo, int hi)
{
    if (n < lo) return lo;
    if (n > hi) return hi;
    return n;
}
/** @sly2-annot addr=0x1EDA68
 * Purpose: Set the SAI frame index (clamped to [0,c-1]) and mark dirty, unless locked
 * Info: proposed name SetSaiIframe (T1); TU shdanim
 * Notes: names_v2 v1 proposed InitLoop__FP4LOOPP4SAAF (0.638)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void func_001EDA68(S *p, int n)
{
    unsigned short i;
    if (FIsSlEmpty(&p->x34)) return;
    i = Clamp(n, 0, p->c - 1);
    if (p->i != i) {
        p->i = i;
        p->fDirty = 1;
    }
}
