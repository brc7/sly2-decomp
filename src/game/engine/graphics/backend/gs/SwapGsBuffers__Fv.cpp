struct CSR { unsigned int SIGNAL:1, FINISH:1, HSINT:1, VSINT:1, EDWINT:1, ZERO1:3, FLUSH:1, RESET:1, ZERO2:2, NFIELD:1, FIELD:1, FIFO:2, REV:8, ID:8; };
struct CRITSECT;
extern CRITSECT D_002DFB40;
extern int D_002DFB5C;
extern int D_002DEDB8;
extern int D_002DEDB4;
extern int D_002DEDB0;
extern int D_002DEDBC;
extern int D_002BE594;
extern char D_002DEBE0[];
extern char D_002DECD0[];
extern char D_002DEB80[];
void SyncVBlank();
void EnterCritSect(CRITSECT *pcs);
void LeaveCritSect(CRITSECT *pcs);
void BlendDisplayOnBufferMismatch();
extern "C" void sceGsSetHalfOffset(void *pdraw, short x, short y, short half);
extern "C" void func_00234CA0(int n);
extern "C" void func_00240A40(void *pdb, int i);
extern "C" void sceGsSyncPath(int a, int b);
static inline int FField() { return ((CSR *)0x12001000)->FIELD; }
/** @sly2-annot addr=0x16F590
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void SwapGsBuffers()
{
    D_002DFB5C = 1;
    SyncVBlank();
    D_002DFB5C = 0;
    D_002DEDB8 = D_002DEDB4;
    if (D_002BE594 > 0) {
        D_002DEDB4 = 0;
        D_002BE594--;
    } else {
        D_002DEDB4 = FField() == 0;
    }
    EnterCritSect(&D_002DFB40);
    sceGsSetHalfOffset((D_002DEDB0 & 1) ? D_002DEBE0 : D_002DECD0, 0x800, 0x800, D_002DEDB4);
    func_00234CA0(0);
    D_002DEDB0++;
    func_00240A40(D_002DEB80, D_002DEDB0);
    if (D_002DEDBC == 1 || D_002DEDBC == 2)
        BlendDisplayOnBufferMismatch();
    sceGsSyncPath(0, 0);
    LeaveCritSect(&D_002DFB40);
}
