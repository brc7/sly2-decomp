struct ALLOCREQ { int cb; int nKind; };
class CBinaryInputStream {
public:
    int S8Read();
    int S16Read();
};
struct E;
struct LOX { char pad[0xA8]; E *pe; };
struct E { LOX *p0; LOX *p4; int n8; int nC; };
extern LOX D_002C44F0;
class LOBase {
public:
    int a, b, c;
#define V(n) virtual void V##n();
    V(1) V(2) V(3) V(4) V(5) V(6) V(7) V(8) V(9) V(10)
    V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20)
    V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30)
    V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40)
    V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50)
#undef V
    virtual LOX *PloFind(int n);
};
extern "C" void *func_001AE2A0(ALLOCREQ *preq);
extern "C" void func_002303A0(LOBase *p, LOX *plo, int n);
/** @sly2-annot addr=0x1603B8
 * Purpose: Reads an s8 count of transform-constraint records {source LO, dest LO, fPosition, fRotation} from
 *   the BRX stream, links each dest to its record and switches the dest transform mode (0x2303A0(...,0xF))
 * Info: proposed name LoadXfmConstraintsFromBrx (T1); TU tu_1603B8
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8+parse-verified
 * See: docs/engine/hud-ui.md#4-source-files, docs/engine/object-model.md#7-xfm-and-transform-helpers,
 *   docs/engine/object-model.md#8-source-files
 * @end-sly2-annot */
extern "C" void func_001603B8(LOBase *p, CBinaryInputStream *pbis)
{
    int c = pbis->S8Read();
    for (int i = 0; i < c; i++) {
        ALLOCREQ req;
        req.cb = 0x10;
        req.nKind = 0x10;
        E *pe = (E *)func_001AE2A0(&req);
        pe->p0 = p->PloFind(pbis->S16Read());
        if (!pe->p0)
            pe->p0 = &D_002C44F0;
        pe->p4 = p->PloFind(pbis->S16Read());
        pe->n8 = pbis->S8Read();
        pe->nC = pbis->S8Read();
        pe->p4->pe = pe;
        func_002303A0(p, pe->p4, 0xF);
    }
}
