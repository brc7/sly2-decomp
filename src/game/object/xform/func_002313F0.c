extern char D_003DD490[];
void func_00147E88(void *, int);
typedef struct S {
    char pad[0x10];
    int x10;
    int x14;
    char pad2[0x10];
    int x28;
    signed char ips;
    char x2D;
    long k : 4;
    unsigned long b : 8;
    unsigned int f : 1;
    char pad3[0x24];
    int x54;
} S;

/** @sly2-annot addr=0x2313F0
 * Purpose: marks world stamp stale and flags
 * Info: proposed name InvalidateXfm (T2); TU xform
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/object-model.md#xform-0x231100-0x2319e8
 * @end-sly2-annot */
void func_002313F0(S *p)
{
    p->x54 = -1;
    if (p->x10 == 0 && p->f)
        func_00147E88(D_003DD490, p->x14);
}
