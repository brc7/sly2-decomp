struct MSG { int cb; int nKind; };
struct FRAME {
    int a[4];
    FRAME();
    ~FRAME();
};
struct ENT { char a[0x50]; };
struct OBJ {
    char pad[0x50];
    int c;
    ENT *ae;
    int cMax;
    char pad5c[0x7C - 0x5C];
    int f7C;
};
void *func_001AE318(MSG *);
void *func_001AE1D0(MSG *);
void CopyAb__FPvT0Ui(void *, void *, int);
void AddRelayBlockBegin(OBJ *, int);
void LoadLoFromBrx(OBJ *, void *);
static inline int Max(int a, int b) { return a < b ? b : a; }
/** @sly2-annot addr=0x1DFB10
 * Purpose: RELAY::LoadFromBrx (vt14): build commands into a 0xA000-byte temp buffer (0x200 x 0x50), start
 *   with BlockBegin(-1), fAppendEnd=1, LoadLoFromBrx (the object's Splice init adds commands), then copy into
 *   an exact-size world-heap array
 * Info: proposed name LoadRelayFromBrx (T1); class RELAY slot14; TU relay
 * Notes: superseded r6_1D5150 proposal: LoadSggFromBrx? (0.35) | level_parser CONFIRMED-parse: payload layout
 *   followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r6b_rest+parse-verified
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
extern "C" void LoadRelayFromBrx(OBJ *p, void *x)
{
    FRAME frame;
    MSG msg;
    MSG *pmsg = &msg;
    pmsg->cb = 0xA000;
    pmsg->nKind = 0x10;
    p->ae = (ENT *)func_001AE318(pmsg);
    AddRelayBlockBegin(p, -1);
    p->f7C = 1;
    LoadLoFromBrx(p, x);
    {
        ENT *aeOld = p->ae;
        p->cMax = Max(p->c, p->cMax) + 1;
        pmsg->cb = p->cMax * sizeof(ENT);
        pmsg->nKind = 0x10;
        p->ae = (ENT *)func_001AE1D0(pmsg);
        CopyAb__FPvT0Ui(p->ae, aeOld, p->c * sizeof(ENT));
    }
}
