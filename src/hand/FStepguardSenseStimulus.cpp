typedef int qword __attribute__((mode(TI)));
struct L { char pad[0x28]; unsigned long fl28; };
struct T { char pad[0x50]; int k; };
struct F64 { char pad[0xE8]; L *pl; };
struct O;
struct S2 { char pad[0xEC]; void *pEC; char pad2[0xE68 - 0xF0]; int nE68; char pad3[0xF64 - 0xE6C]; F64 *pF64; };
struct O { char pad[0x14]; T *pt; char pad1[0x20 - 0x18]; qword pos; char pad2[0x40 - 0x30]; S2 *p40; void *p44; char pad3[0x54 - 0x48]; int k54; };
extern "C" {
int FStepguardSensesEnabled(S2 *p);
void *PdteFindDetectorTarget(F64 *p, void *x);
qword func_001A62E8(void *p);
int func_00156918(O *o, qword q);
int FCheckVolPoint(L *pl, qword q);
void IntersectSwBoundingBox(void *g, qword a, qword b, void *p1, void *p2, int *pi, int *pj);
int func_0014CC58(int z, qword a, qword b, int i, int j, void *phit);
void func_0020FA10();
extern char D_003DD490[];
int FStepguardSenseStimulus(S2 *p, O *o);
}
class FRAME { public: FRAME(); ~FRAME(); };

int FStepguardSenseStimulus(S2 *p, O *o)
{
    int k;
    T *pt;
    qword pos, tpos;
    if (!FStepguardSensesEnabled(p))
        return 0;
    if (o->k54 < p->nE68)
        return 0;
    if (o->p40 && o->p40 == p)
        return 0;
    if (o->k54 == 4) {
        void *x = o->p44;
        void *e = PdteFindDetectorTarget(p->pF64, x);
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
        if (!FCheckVolPoint(pl, tpos))
            return 0;
        {
            FRAME frame;
            int i, j;
            IntersectSwBoundingBox(D_003DD490, pos, tpos, (void *)func_0020FA10, p->pEC, &i, &j);
            if (!func_0014CC58(0, pos, tpos, i, j, 0))
                return 1;
            return 0;
        }
    }
    }
    return 0;
}
