struct CRef;
extern "C" void Decref__4CRef(CRef *);
struct CRef {
    int tagk; int tag;
    CRef(int k, int n) { tagk = k; tag = n; }
    ~CRef() { Decref__4CRef(this); }
};
struct VTE { short d; short pad; void *(*pfn)(void *); };
struct E { int id; int a; float g; };
struct P { char pad[0xC]; VTE *vt; char pad2[0x1180 - 0x10]; E ae[3]; };
struct CLOCK { float t; float dt; };
extern "C" {
extern CLOCK D_002DDED0;
extern char D_002A4D50[];
extern char D_002A4D58[];
void SetGadgetSlotState(P *p, E *pe, int n);
int sprintf(char *pch, const char *fmt, ...);
}
CRef RefSetSaveVarV(const CRef &ref, void *o, const char *fmt, ...);
/** @sly2-annot addr=0x141FC8
 * Purpose: Assign gadget id to slot i (<3): stop old, clear duplicates in other slots, write all to save var
 *   apukCur[%d], then vt105
 * Info: proposed name SetStepplayerGadget (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
extern "C" void SetStepplayerGadget(P *p, unsigned int i, int id)
{
    if (i < 3) {
        E *pe = &p->ae[i];
        SetGadgetSlotState(p, pe, -1);
        pe->id = id;
        pe->a = 0;
        pe->g = D_002DDED0.t;
        for (int j = 0; j < 3; j++) {
            char ach[16];
            E *pe2 = &p->ae[j];
            if (j != i && pe2->id == id) {
                SetGadgetSlotState(p, pe2, -1);
                pe2->id = -1;
            }
            sprintf(ach, D_002A4D50, j);
            VTE *e = &p->vt[103];
            RefSetSaveVarV(CRef(0x15, p->ae[j].id), e->pfn((char *)p + e->d), D_002A4D58, ach, 0);
        }
        VTE *e = &p->vt[105];
        ((void (*)(void *))e->pfn)((char *)p + e->d);
    }
}
