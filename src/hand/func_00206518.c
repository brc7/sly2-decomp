typedef int qword __attribute__((mode(TI)));
struct VE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VE *vt; };
struct P2F0 { char pad[8]; qword *pq; float f; };
struct F64 { char pad[0xE0]; void *pE0; void *pE4; };
struct S {
    char pad0[0xEC]; void *pEC; char pad1[0x26C - 0xF0]; char a26C[0x2F0 - 0x26C]; struct P2F0 *p2F0;
    char pad2[0x300 - 0x2F4]; qword q300; float f310; char pad3[0x338 - 0x314]; int n338; int n33C; int n340; int n344;
    char pad4[0x494 - 0x348]; int n494; void *p498; void *p49C; char pad5[0xE78 - 0x4A0]; int nE78; char pad6[0xE84 - 0xE7C];
    int nE84; int nE88; char pad7[0xF64 - 0xE8C]; struct F64 *pF64; char pad8[0xFD0 - 0xF68]; char aFD0[0xFE8 - 0xFD0];
    int flFE8; struct O *pFEC;
};
extern int D_002E1E40, D_002F7900, D_002DD5BC, D_002DD648;
void func_001CB518(struct S *p);
void func_001EECD0(void *p);
void func_00209778(void *p);
void func_00209870(struct S *p, int n);
void func_0020CE40(void *p);
void func_0020D1C8(void *p);
void func_0020D1A0(void *p);
qword func_001A62E8(void *p);
float func_00204740(struct S *p);
void func_00204660(struct S *p, float f);
unsigned long func_00207A20(int n);
void *func_00209AD0(struct S *p, unsigned long m);
void func_00153EF8(struct F64 *pf, struct S *p, int n);
void func_001F1A40(struct S *p);
void func_001F1CF8(struct S *p);
struct O *func_00205438(struct S *p, int n);
void func_001A58D0(struct F64 *pf, struct S *p, int n);

/** @sly2-annot addr=0x206518
 * Purpose: STEPGUARD vt26: proxy/spawn reset: copy max values to current (HP etc.), clear BHVs, mover and
 *   flags, re-cache home pos/heading (+0x300/+0x310), capability bits +0xFE8 from anim bindings for flags
 *   9/8/0xF/0x11, add the 4 player characters as detector targets, roll loot, push root behaviour BHV156 (cid
 *   0x9C)
 * Info: proposed name ResetStepguardToHome (T1); class STEPGUARD slot26; TU stepguard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00206518(struct S *p)
{
    struct O *o;
    qword *pq;
    func_001CB518(p);
    pq = &p->q300;
    p->n33C = p->n338;
    p->nE88 = p->nE84;
    p->n344 = p->n340;
    func_001EECD0(p->a26C);
    func_00209778(p->aFD0);
    func_00209870(p, 0);
    func_0020CE40(p->p498);
    func_0020D1C8(p->p49C);
    func_0020D1A0(p->p498);
    p->q300 = func_001A62E8(p);
    p->f310 = func_00204740(p);
    p->p2F0->pq = pq;
    p->p2F0->f = p->f310;
    func_00204660(p, p->f310);
    p->flFE8 = 0;
    if (func_00209AD0(p, func_00207A20(9)))
        p->flFE8 |= 2;
    else
        p->nE78 = 2;
    if (func_00209AD0(p, func_00207A20(8)))
        p->flFE8 |= 4;
    if (func_00209AD0(p, func_00207A20(0xF)))
        p->flFE8 |= 1;
    if (func_00209AD0(p, func_00207A20(0x11)))
        p->flFE8 |= 8;
    func_00153EF8(p->pF64, p, D_002E1E40);
    func_00153EF8(p->pF64, p, D_002F7900);
    func_00153EF8(p->pF64, p, D_002DD5BC);
    func_00153EF8(p->pF64, p, D_002DD648);
    if (p->n494) {
        func_001F1A40(p);
        func_001F1CF8(p);
    }
    if (p->pFEC)
        p->pFEC->vt[9].pf((char *)p->pFEC + p->pFEC->vt[9].d);
    o = func_00205438(p, 0x9C);
    o->vt[8].pf((char *)o + o->vt[8].d);
    func_001A58D0(p->pF64, p, 0xF);
    p->pF64->pE0 = p;
    p->pF64->pE4 = p->pEC;
}
