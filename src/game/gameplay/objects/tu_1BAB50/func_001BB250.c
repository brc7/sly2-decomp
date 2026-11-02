typedef struct VTE { short d; short pad; void (*pfn)(); } VTE;
typedef struct E { int a; int b; } E;
typedef struct O {
    char pad0[0xC];
    VTE *vt;
    char pad10[0xEC - 0x10];
    struct X *pxEC;
    char padF0[0x148 - 0xF0];
    unsigned long x148;
    int c150;
    E *a154;
} O;
typedef struct L { int x0; struct L *next; int *px8; O *po; } L;
typedef struct X { char pad[0x64]; L *pl; } X;
extern char D_003DD490[];
void func_001F4F10(O *p);
int func_001BBE48(O *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(O *p, int k);
void func_001BAED8(O *p, O *q, int z);
void func_001A7E10(void *g, int k);

/** @sly2-annot addr=0x1BB250
 * Purpose: CID 98 vt58: SO base, then for contacts with allowed trigger classes schedule a reaction; chain to
 *   touching CID-98 objects
 * Info: proposed name ScheduleCid98TouchEvents (T2); class CID98 slot58; TU tu_1BAB50
 * Notes: verifier PLAUSIBLE: Placeholder class (CID 0x62 vtable slot 58). ScheduleSoTouchEvents, then for
 *   each pending contact whose class matches an enabled filter entry: ScheduleCid98Reaction (both ways when
 *   the other is also C | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest+verdicts_followups
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BB250(O *p)
{
    L *pl;
    int i, j;
    func_001F4F10(p);
    if (func_001BBE48(p)) return;
    for (pl = p->pxEC->pl; pl; pl = pl->next) {
        if (*pl->px8 == 0) continue;
        for (i = 0; i < p->c150; i++) {
            E *pe = &p->a154[i];
            if (!pe->b) continue;
            if (!FIsBasicDerivedFrom__FP5BASIC3CID(pl->po, pe->a)) continue;
            if (!(p->x148 & 0x3000000)) func_001BAED8(p, pl->po, 0);
            if (FIsBasicDerivedFrom__FP5BASIC3CID(pl->po, 0x62)) {
                O *q = pl->po;
                for (j = 0; j < q->c150; j++) {
                    E *pe2 = &q->a154[j];
                    if (!pe2->b) continue;
                    if (!FIsBasicDerivedFrom__FP5BASIC3CID(p, pe2->a)) continue;
                    func_001BAED8(q, p, 0);
                    break;
                }
            }
            ((void (*)(void *, O *))p->vt[54].pfn)((char *)p + p->vt[54].d, pl->po);
            ((void (*)(void *, O *))pl->po->vt[54].pfn)((char *)pl->po + pl->po->vt[54].d, p);
            func_001A7E10(D_003DD490, pl->x0);
            *pl->px8 = 0;
            pl->x0 = 0;
            break;
        }
    }
}
