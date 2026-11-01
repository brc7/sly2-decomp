typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; int (*pfn)(void *); };
struct SO { char pad[0xC]; struct VTE *vt; char pad1[0x240 - 0x10]; qword pos; char pad2[0x11AC - 0x250]; int x11AC; };
struct L { int x0; struct L *pNext; int x8; struct SO *pso; };
struct C { char pad[0x64]; struct L *pl; };
struct T { char pad[0xE4]; struct C *pc; void *xE8; void *xEC; int xF0; int xF4; int xF8; };
struct W { char pad[0xEC]; void *xEC; char pad1[0x130 - 0xF0]; struct W *pNext; };
struct BUF { char a[0x180]; };
extern char D_003DD490[];
extern struct W *D_003E1480;
void *PpoHero(void *p);
int func_001A61A0(struct SO *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);
int func_0020F1B8(void *p);
qword func_001A62E8(struct SO *p);
qword func_00153E60(struct T *p);
qword ConvertAloPos(int z, struct W *pw, qword v);
int func_00118668(void *p, int mask, qword a, qword b, int n, struct BUF *pbuf);
int FCheckVolPoint(void *p, qword v);
int func_00154770(struct T *p, struct SO *pso);
int func_001545F0(struct T *p, struct SO *pso);
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }

/** @sly2-annot addr=0x1541D8
 * Purpose: Computes the new detection state (0 none, 1 seen, 2 lost) of one target: rejects
 *   hidden/disguised/invisible targets, BSP line-of-sight through all world occluders, inner/outer volumes
 *   (0x227C90) and optional line test (0x1545F0)
 * Info: proposed name KCalculateDetectorTarget (T1); TU detector
 * Notes: names_v2 C proposed UpdateStepguardEffect__FP9STEPGUARD (0.438)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
int KCalculateDetectorTarget(struct T *p, struct SO *pso, int k)
{
    struct BUF buf;
    qword v;
    struct W *pw;
    struct SO *pme = PpoHero(D_003DD490);
    if (!pso) { k = 0; goto done; }
    if (!func_001A61A0(pso)) { k = 0; goto done; }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x32) && pso->vt[77].pfn((char *)pso + pso->vt[77].d) == 2) { k = 0; goto done; }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x34) && pme && FIsBasicDerivedFrom__FP5BASIC3CID(pme, 0x46)) { k = 0; goto done; }
    if (func_0020F1B8(D_003DD490)) { k = 0; goto done; }
    if (p->xF4) { k = 0; goto done; }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x34) && pso->x11AC) { k = 0; goto done; }
    if (p->xF0) { k = 1; goto done; }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x33))
        v = Add(func_001A62E8(pso), pso->pos);
    else
        v = func_001A62E8(pso);
    for (pw = D_003E1480; pw; pw = pw->pNext) {
        qword a = ConvertAloPos(0, pw, func_00153E60(p));
        qword b = ConvertAloPos(0, pw, v);
        if (func_00118668(pw->xEC, 0x7FFFFFFF, a, b, 4, &buf) > 0) { k = 0; goto done; }
    }
    switch (k) {
    case 0:
        if (p->pc) {
            struct L *pl;
            for (pl = p->pc->pl; pl; pl = pl->pNext) {
                if (pl->pso == pso) {
                    if (pl->x0) { k = 1; goto done; }
                    break;
                }
            }
        }
        if (!FCheckVolPoint(p->xE8, v)) { k = 0; goto done; }
        if (p->xEC && !FCheckVolPoint(p->xEC, v)) { k = 0; goto done; }
        if (!(p->xF8 & 1) && func_00154770(p, pso)) { k = 0; goto done; }
        if (p->xF8 & 2) { k = 1; goto done; }
        { k = !func_001545F0(p, pso); goto done; }
    case 1:
        if (p->xEC && !FCheckVolPoint(p->xEC, v)) { k = 0; goto done; }
        if (!FCheckVolPoint(p->xE8, v)) { k = 2; goto done; }
        break;
    case 2:
        if (p->xEC && !FCheckVolPoint(p->xEC, v)) { k = 0; goto done; }
        if (FCheckVolPoint(p->xE8, v)) {
            if (p->xF8 & 2) { k = 1; goto done; }
            if (func_001545F0(p, pso)) return k;
            k = 1;
            goto done;
        }
        break;
    default:
        goto done;
    }
    if ((p->xF8 & 8) && func_001545F0(p, pso)) k = 0;
done:
    return k;
}
