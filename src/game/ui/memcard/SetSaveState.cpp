struct CRef;
extern "C" void func_00129EC8(CRef *);
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef(const CRef &) __asm__("func_001291A8");
    ~CRef() { func_00129EC8(this); }
};
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct SLOT { char pad[0x5408]; char x5408[0x5C]; };
struct THR;
struct MC {
    int x0; int x4; int state; int xC; float x10; char pad[0x24 - 0x14];
    SLOT aslot[3];
    int xFD50; SLOT *pslotCur; int xFD58; THR *thr[1];
};
struct G { char pad[0x3B1C]; int x3B1C; };
struct CLOCK { char pad[0x3C]; float t3c; };
struct ST { char pad[0x44]; int x44; };
extern "C" {
extern G D_003DD490;
extern CLOCK D_002DDED0;
extern ST D_002E6550;
extern char D_002E65A0[];
void func_001ACC38(ST *, int);
int func_001ADEA8(void *);
void SetMcDialogState(ST *, int);
void HideMcDialogWidget(void *);
int func_001AB398(char *, char *);
void func_001D8D80(SLOT *, int);
void SetSaveState(MC *p, int state);
}
CRef Eval(int, SLOT *) __asm__("func_002581C0");
static inline float GetF(int k, SLOT *p) { return Eval(k, p).u.g; }
struct X { int A() __asm__("func_001AB0A0"); int B() __asm__("func_001AB0F0"); };
void StartThread(void *, int (X::*)()) __asm__("CreateMemcardThread");

/** @sly2-annot addr=0x1AB418
 * Purpose: SAVE state change (x8) with per-state actions on the 3 slots (Splice CRef evals), UI state
 *   updates, ResetGs on load
 * Info: proposed name SetSaveState (T1); TU memcard
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void SetSaveState(MC *p, int state)
{
    int i;
    if (state == p->state)
        return;
    if (p->state == 6) {
        p->pslotCur = 0;
        if (p->xFD50) {
            ST *pst = &D_002E6550;
            if (pst->x44 == 1) {
                func_001ACC38(pst, 3);
                if (func_001ADEA8(D_002E65A0)) {
                    SetMcDialogState(pst, 0x23);
                    HideMcDialogWidget(D_002E65A0);
                } else {
                    SetMcDialogState(pst, 0x24);
                }
            }
        }
    }
    switch (state) {
    case 0:
    case 1:
        StartThread(&p->thr, &X::A);
        break;
    case 3:
        StartThread(&p->thr, &X::B);
        break;
    case 6:
        p->pslotCur = &p->aslot[0];
        for (i = 0; i < 3; i++) {
            SLOT *pslot = &p->aslot[i];
            float g = GetF(D_003DD490.x3B1C, pslot);
            if (g != 0.0f) {
                if (func_001AB398(p->pslotCur->x5408, pslot->x5408) < 0)
                    p->pslotCur = pslot;
            } else {
                func_001D8D80(pslot, 0);
            }
        }
        break;
    }
    p->state = state;
    p->x10 = D_002DDED0.t3c;
}
