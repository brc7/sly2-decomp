typedef int qword __attribute__((mode(TI)));
struct L { char pad[0x28]; unsigned long fl28; };
struct T { char pad[0x50]; int k; };
struct F64 { char pad[0xE8]; L *pl; };
struct O;
struct S2 { char pad[0xEC]; void *pEC; char pad2[0xE68 - 0xF0]; int nE68; char pad3[0xF64 - 0xE6C]; F64 *pF64; };
struct O { char pad[0x14]; T *pt; char pad1[0x20 - 0x18]; qword pos; char pad2[0x40 - 0x30]; S2 *p40; void *p44; char pad3[0x54 - 0x48]; int k54; };
extern "C" {
int func_00208D70(S2 *p);
void *func_00154180(F64 *p, void *x);
qword func_001A62E8(void *p);
int func_00156918(O *o, qword q);
int func_00227C90(L *pl, qword q);
void func_0020FAA0(void *g, qword a, qword b, void *p1, void *p2, int *pi, int *pj);
int func_0014CC58(int z, qword a, qword b, int i, int j, void *phit);
void func_0020FA10();
extern char D_003DD490[];
int func_00208BE0(S2 *p, O *o);
}
class FRAME { public: FRAME(); ~FRAME(); };

/** @sly2-annot addr=0x208BE0
 * Purpose: STEPGUARD vt89: can this guard perceive stimulus o (noise TU objects): senses enabled, level >=
 *   +0xE68, not own; kind 0 radius test (0x156918), kind 1 inner volume of detector + clear line
 *   (PsoHitTestLineObjects)
 * Info: proposed name FStepguardSenseStimulus (T1); class STEPGUARD slot89; TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#tu_1cbeb0-0x1cbeb0-0x1cc158,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00208BE0(S2 *p, O *o)
{
    int k;
    T *pt;
    qword pos, tpos;
    if (!func_00208D70(p))
        return 0;
    if (o->k54 < p->nE68)
        return 0;
    if (o->p40 && o->p40 == p)
        return 0;
    if (o->k54 == 4) {
        void *x = o->p44;
        void *e = func_00154180(p->pF64, x);
        if (x != p && !e)
            return 0;
    }
    pt = o->pt;
    pos = func_001A62E8(p);
    tpos = o->pos;
    switch (pt->k) {
    case 0:
        return func_00156918(o, func_001A62E8(p));
    case 1: {
        L *pl = p->pF64->pl;
        if (!pl || ((pl->fl28 >> 60) & 1) == 0)
            return 0;
        if (!func_00227C90(pl, tpos))
            return 0;
        {
            FRAME frame;
            int i, j;
            func_0020FAA0(D_003DD490, pos, tpos, (void *)func_0020FA10, p->pEC, &i, &j);
            if (!func_0014CC58(0, pos, tpos, i, j, 0))
                return 1;
            return 0;
        }
    }
    }
    return 0;
}
