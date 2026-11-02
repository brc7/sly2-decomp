extern "C" {
void STACKSCOPE_ctor(void *p);
void STACKSCOPE_ctor_1AE4A0(void *p, int n);
void Decref__4CRef(void *p);
void *FClueCollected(void *p);
}
class FRAME { int a[4]; public: FRAME() { STACKSCOPE_ctor(this); } ~FRAME() { STACKSCOPE_ctor_1AE4A0(this, 2); } };
struct CRef { int m_tagk; int m_tag; };
extern char D_003D4A60[];
extern "C" void RefGetGlobalSaveVar(CRef *pref, void *pv, void *p);
static inline int NFromPv(void *pv) { CRef ref; int n; RefGetGlobalSaveVar(&ref, pv, D_003D4A60); n = ref.m_tag; Decref__4CRef(&ref); return n; }
extern "C" int func_001C7BB8(void *p);
/** @sly2-annot addr=0x1C7BB8
 * Purpose: wrapper of FClueCollected
 * Info: proposed name FClueCollectedSelf (T2); TU prize
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
int func_001C7BB8(void *p)
{
    FRAME frame;
    void *pv = FClueCollected(p);
    if (!pv) return 0;
    return NFromPv(pv);
}
