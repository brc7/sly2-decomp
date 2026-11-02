/* RunBtnAsegs (Claude agent, 2026-10-17): matched. The p/pso s0/s1 swap went away when the po selection reads p->po in each branch (`if (p->po->oid == oid) po = p->po; else if (oid != -1) po = f(..., p->po); else po = 0;`) instead of a po = p->po copy first. */
struct OID { char pad[0x18]; int oid; };
struct X14 { struct OID *po; };
struct SO { char pad[0x14]; struct X14 *px14; };
struct ENT { struct SO *pso; int x; };
struct E { int c; struct ENT a[8]; };
struct P {
    struct OID *po; struct E ae[2]; char pad[0x8C - 0x8C]; float g8C; char pad2[0xA0 - 0x90]; int xA0; char pad3[4];
    int xA8; void *pAC; int xB0; char mB4[0x134 - 0xB4]; int x134; char m138[4];
};
extern char D_003DD490[];
extern char D_002DEB30[];
struct OID *PloFindSwObject__FP2SWi3OIDP2LO(void *g, int k, int oid, struct OID *po);
void func_00130598(struct SO *pso, struct OID *po, int n, int x, float g, float h);
void TriggerRsmg(void *g, int a, void *b, struct OID *po, int f);
void func_001DFD58(void *p, int a);
void ApplyTaskStates(void *g, int a, void *b, struct OID *po);
/** @sly2-annot addr=0x1488D8
 * Purpose: Apply/seek every aseg in ASH list i on its object
 * Info: proposed name RunBtnAsegs (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void RunBtnAsegs(struct P *p, int i)
{
    int j;
    struct SO *pso;
    for (j = 0; j < p->ae[i].c; j++) {
        struct OID *po;
        int oid, x;
        pso = p->ae[i].a[j].pso;
        if (!pso) continue;
        oid = pso->px14->po->oid;
        x = p->ae[i].a[j].x;
        if (p->po->oid == oid) po = p->po;
        else if (oid != -1) po = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, oid, p->po);
        else po = 0;
        if (p->xA0) {
            int n = 0;
            if (p->g8C < 0.0f) n = 2;
            func_00130598(pso, po, n, x, 0.0f, p->g8C);
        } else {
            func_00130598(pso, po, 1, x, 0.0f, p->g8C);
        }
    }
    TriggerRsmg(D_003DD490, p->xB0, p->mB4, p->po, i == 0);
    if (i == 0) {
        if (p->pAC) func_001DFD58(p->pAC, p->xA8);
        ApplyTaskStates(D_002DEB30, p->x134, p->m138, p->po);
    }
}
