extern "C" {
void *func_001AE478(void *p);
void func_001AE4A0(void *p, int n);
void func_00129EC8(void *p);
void func_001291A8(void *p, const void *q);
void *func_001C7B18(int n);
int func_001C7D08(void);
void func_001C7C30(int n, int f);
}
class FRAME { int a[4]; public: FRAME() { func_001AE478(this); } ~FRAME() { func_001AE4A0(this, 2); } };
struct CRef {
    int m_tagk; int m_tag;
    CRef() {}
    CRef(int f) { m_tagk = 0x17; m_tag = f; }
    CRef(const CRef &r) { func_001291A8(this, &r); }
    ~CRef() { func_00129EC8(this); }
};
extern char D_003D4A60[];
extern int D_003E1BF4;
extern "C" CRef func_00258200(void *pv, const CRef &ref, void *p);
/* Copy the reference into the setter's value argument. */
static inline void SetPv(void *pv, const CRef &r, void *)
{
    CRef arg(r);
    const CRef &ret = func_00258200(pv, arg, D_003D4A60);
}
/** @sly2-annot addr=0x1C7C30
 * Purpose: set clue bit / count
 * Info: proposed name SetClueCollected (T2); TU prize
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C7C30(int n, int f)
{
    FRAME frame;
    void *pv = func_001C7B18(n);
    if (!pv) return;
    {
        CRef r(f != 0);
        SetPv(pv, r, D_003D4A60);
    }
    D_003E1BF4 = func_001C7D08();
}
