struct CRef;
extern "C" void func_00129EC8(CRef *);
struct CRef { int m_tagk; float m_tag; CRef(const CRef &) __asm__("func_001291A8"); ~CRef() { func_00129EC8(this); } };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B a[3]; char pad2[0xFD50 - 0x24 - 3 * 0x5464]; struct B *xFD50; };
struct GD { char pad[0x3B1C]; int x3B1C; };
struct E { char pad[0x14]; int x14; int pad2[1]; };
struct H { char pad[0x24]; int x24; };
extern "C" {
extern struct G D_002E65E0;
extern struct B D_002E6604[];
extern char D_002E6550[];
extern struct GD D_003DD490;
extern struct H D_003D9ED0;
extern struct E D_003D9EDC[];
void func_001ACC38(void *p, int n);
int IdPauseTopPage(void *g);
}
CRef Eval(int a, void *b) __asm__("RefGetGlobalSaveVar");
static inline float F(int x, void *pb) { return Eval(x, pb).m_tag; }
/** @sly2-annot addr=0x1AD538
 * Purpose: Menu hook: eval 3 slots' Splice values and update entries
 * Info: proposed name UpdateSaveMenuSlots (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" void func_001AD538(void)
{
    float a[3];
    struct E *pe;
    int i;
    func_001ACC38(D_002E6550, 1);
    if (D_003D9ED0.x24 <= 0)
        return;
    pe = &D_003D9EDC[D_003D9ED0.x24];
    {
        struct B *pb = D_002E6604;
        for (i = 0; i < 3; i++, pb++) {
            a[i] = F(D_003DD490.x3B1C, pb);
        }
    }
    switch (IdPauseTopPage(&D_003D9ED0)) {
    case 0x1E:
        {
            int j;
            for (j = 0; j < 3; j++) {
                if (a[j] == 0.0f) {
                    pe->x14 = j;
                    break;
                }
            }
        }
        break;
    case 0x1F:
        {
            int j;
            for (j = 0; j < 3; j++) {
                if (a[j] != 0.0f) {
                    pe->x14 = j;
                    break;
                }
            }
        }
        break;
    case 0x19:
        if (D_002E65E0.xFD50)
            pe->x14 = D_002E65E0.xFD50 - D_002E65E0.a;
        break;
    }
}
