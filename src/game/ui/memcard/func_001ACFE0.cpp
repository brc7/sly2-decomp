// Game-state transition handler: switch on the new state id (x40) and run the menu/HUD hooks for it.
// State 34 sends an option message (CRef true) to the global option object D_003E0FE4.
//
// Match notes: the three-argument call to func_00258200 uses the same idiom as func_0013D328 / ResetRumbleSaveVar:
// SetPv receives the context at the call site (evaluated before the CRef copy) but its body names
// D_003D4A60 again after the copy. That is what puts `lui a3` after the copy ctor yet keeps a3's low half
// ahead of the a2 setup (the scheduler boosts insns that set a register exactly once in the function).
class CRef;
extern "C" void func_00129EC8(CRef *pref);                       // ~CRef
extern "C" CRef *func_001291A8(CRef *, const CRef *);            // CRef copy ctor, returns this
class CRef {
public:
    int m_tagk;
    int m_tag;
    CRef(bool f) { m_tagk = 0x17; m_tag = f; }
    CRef(const CRef &ref) { func_001291A8(this, &ref); }
    ~CRef() { func_00129EC8(this); }
};
class O;
struct S { char pad[0x40]; int x40; int x44; float x48; int x4C; };   // x40 = state id, x4C = counter
extern "C" {
extern char D_003DA530[], D_003D9ED0[], D_003D4A60[], D_002E65E0[];
extern O *D_003E0FE4;
int FScreenShown(void *p);
int func_001B0160(void *p);
void ClearMcDialogState(S *s);
void func_001ACC38(S *s, int n);
void func_001AF660(void *p);
void StartNewGame(void *p);
int func_001B0108(void *p);
void func_001AF608(void *p);
void SetMcDialogState(S *s, int n);
void func_001ABD40(void *p);
void func_001ACFE0(S *s);
CRef func_00258200(O *, const CRef &, void *);                   // O::Set(const CRef &, context), returns a CRef
}
static inline void SetPv(O *po, const CRef &r, void *) { CRef arg(r); const CRef &ret = func_00258200(po, arg, D_003D4A60); }
/** @sly2-annot addr=0x1ACFE0
 * Purpose: Switch on dialog state x40 and run follow-up (UI hooks, SAVE jobs, 0x1ABD40)
 * Info: proposed name OnMcDialogChoice (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ACFE0(S *s)
{
    switch (s->x40) {
    case 17: case 18: case 21: case 24:
        {
            void *g = D_003DA530;
            if (FScreenShown(g) && func_001B0160(D_003D9ED0)) {
                ClearMcDialogState(s);
                func_001ACC38(s, 2);
                func_001AF660(D_003D9ED0);
                StartNewGame(g);
                break;
            }
        }
        /* fallthrough */
    case 20: case 23: case 29: case 35: case 36: case 46:
        ClearMcDialogState(s);
        func_001ACC38(s, 2);
        if (func_001B0108(D_003D9ED0))
            func_001AF608(D_003D9ED0);
        break;
    case 27: case 28: case 32:
        ClearMcDialogState(s);
        break;
    case 34:
        {
            O *po = D_003E0FE4;
            CRef ref(true);
            SetPv(po, ref, D_003D4A60);
        }
        if (s->x4C < 4) {
            if (s->x4C > 0) {
                SetMcDialogState(s, 0x1A);
                func_001ABD40(D_002E65E0);
            }
        }
        break;
    }
}
