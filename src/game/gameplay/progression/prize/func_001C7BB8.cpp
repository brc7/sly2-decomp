extern "C" {
void func_001AE478(void *p);
void func_001AE4A0(void *p, int n);
void func_00129EC8(void *p);
void *func_001C7B18(void *p);
}
class FRAME { int a[4]; public: FRAME() { func_001AE478(this); } ~FRAME() { func_001AE4A0(this, 2); } };
struct CRef { int m_tagk; int m_tag; };
extern char D_003D4A60[];
extern "C" void func_002581C0(CRef *pref, void *pv, void *p);
static inline int NFromPv(void *pv) { CRef ref; int n; func_002581C0(&ref, pv, D_003D4A60); n = ref.m_tag; func_00129EC8(&ref); return n; }
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
    void *pv = func_001C7B18(p);
    if (!pv) return 0;
    return NFromPv(pv);
}
