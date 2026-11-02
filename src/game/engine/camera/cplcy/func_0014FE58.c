typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct X { void *po; };
extern qword D_002A2DC0;
qword func_001A62E8(void *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);
/** @sly2-annot addr=0x14FE58
 * Purpose: Look anchor: focus object position (0x1A62E8), +150 along up if it is a STEPPLAYER
 * Info: proposed name PosCplookAnchor (T1); TU cplcy
 * Notes: names_v2 C proposed FCheckTbspPoint__FP4TBSPP6VECTOR (0.649)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
qword func_0014FE58(struct X *p)
{
    U s;
    U pos;
    pos.q = func_001A62E8(p->po);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->po, 0x34)) {
        register qword rs __asm__("vf2");
        register qword rp __asm__("vf3");
        register qword rd __asm__("vf1");
        rp = pos.q;
        rd = D_002A2DC0;
        __asm__("qmtc2.ni %1, %0" : "=j"(rs) : "r"(150.0f));
        s.q = rs;
        __asm__("vadda.xyzw $ACC, %0, $vf0\n\tvmaddx.xyzw %0, %2, %1x" : "+j"(rp) : "j"(rs), "j"(rd));
        pos.q = rp;
    }
    return pos.q;
}
