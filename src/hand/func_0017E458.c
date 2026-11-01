struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct D { char pad[0x10]; int n10; };
struct C { char pad[0xDC]; struct D *pDC; };
struct O { char pad[0xE8]; struct C *pE8; };
struct T { char pad[0x120]; int n120; int pad2; int n128; };
struct S { char pad[0x10]; struct O *p10; int pad2; void *p18; char pad3[0x14]; struct T *p30; int pad4; int n38; int pad5; float g40; };
void func_001EEF98(struct S *p);
int func_0017E570(struct S *p);
int func_0017EAE8(void *p);
int func_00174FB8(void *p);
int func_0017E5F0(struct S *p);
void func_0017E580(struct S *p);
void func_0017E6B0(struct S *p, int n);
int func_00178D00(void *p);
/** @sly2-annot addr=0x17E458
 * Purpose: BHVPATHLINK Update: per segment: kind 0 walk (timeout), kind 1 leap; advance when at the segment
 *   end; final relocation step
 * Info: proposed name UpdateBhvpathlink (T1); class BHVPATHLINK slot10; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATHLINK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017E458(struct S *p) {
    func_001EEF98(p);
    if (func_0017E570(p)) return;
    switch (p->n38) {
    case 1:
        if (func_0017EAE8(p->p18)) func_0017E580(p);
        break;
    case 0: {
        struct T *t = p->p30;
        switch (t->n120) {
        case 1:
            if (func_00174FB8(p->p18)) {
                if (func_0017E5F0(p)) func_0017E580(p);
                else func_0017E6B0(p, 1);
            }
            break;
        case 0:
            if (p->g40 < D_002DDED0.t) {
                func_0017E6B0(p, 1);
            } else {
                struct C *c = p->p10->pE8;
                struct D *d = c ? c->pDC : 0;
                int n = d ? d->n10 : 0;
                if (n == t->n128 && func_00178D00(p->p18)) func_0017E580(p);
            }
            break;
        }
        break;
    }
    }
}
