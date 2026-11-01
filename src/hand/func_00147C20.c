extern char D_003DD490[];
struct P { char pad[0x160]; char asega[4]; int n164, n168, n16C, n170; };
void *PloFindSwObject__FP2SWi3OIDP2LO(void *p, int k, int oid, void *q);
void ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(void *aseg, void *alo, float t, float s, int n, void *pp);
void func_00147410(struct P *p, int n);
/** @sly2-annot addr=0x147C20
 * Purpose: set params and find the aseg object to play
 * Info: proposed name StartDancer (T2); class DANCER; TU tu_1471C0
 * Notes: class identity from whole_game WE names/object strings: StartCid68 -> StartDancer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+whole_game(class identity)
 * See: docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20
 * @end-sly2-annot */
void func_00147C20(struct P *p, int oid, int n)
{
    char *aseg;
    p->n170 = n;
    p->n16C = 1;
    p->n168 = 1;
    p->n164 = 1;
    aseg = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 1, oid, p);
    if (aseg) {
        unsigned int v = *(unsigned int *)(aseg + 0x40);
        if ((v ^ 1) & 1) *(int *)v |= 0x100;
        else *(unsigned int *)(aseg + 0x40) = v | 0x100;
        ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(aseg, p, 0.0f, 1.0f, 4, p->asega);
        func_00147410(p, 0);
    }
}
