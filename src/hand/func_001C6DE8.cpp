// func_001C6DE8: find a landing point on the objects below/above `point` (used three times by func_001C6FD8, which sits
// right after it in the same file and shares its FRAME / VU_FLOAT / VV / AddScaled idioms). Casts a line from point up to
// point + axis * gMax and, failing that, down to point + axis * gMin, against `apcand` (fetched with IntersectSwBoundingBox when the
// caller passed none); returns the hit position, or point itself when gMin >= 0 and nothing above was found.
// Matching notes (wave 4, lane 1):
//  * FRAME is a plain RAII local with out-of-line ctor/dtor (as in func_001C6FD8); g++ emits the three `dtor(this, 2)` calls
//    and cross-jumping rebuilds the target's shared tails. Parameters are used directly (their homes are 0x80/0x84).
//  * The game loads IntersectSwBoundingBox's third argument (posMax) into $a2 BEFORE the `apcand == 0` test: that load is the last insn
//    of block 0, so reorg puts it in the bnez delay slot, and its write of $a2 gives the `sw a2,0x84` home store one more
//    dependent, which is why that store precedes the $ra/$f20 saves in the prologue. cse merges any plain copy made before the
//    test, so the argument is held in a variable pinned to $a2 (the one allowed pin).
typedef int qword __attribute__((mode(TI)));
union VECTOR4 { qword q; float f[4]; };
struct LSG {
    VECTOR4 posA;           // 0x00: hit point (lower cast)
    VECTOR4 posB;           // 0x10: hit point (upper cast)
    VECTOR4 normalA;        // 0x20
    VECTOR4 normalB;        // 0x30
    float t;                // 0x40
    float u;                // 0x44
    char pad48[0x18];
};
struct FRAME { int n; FRAME(); ~FRAME(); };
struct CANDIDATE;
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
};
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
extern "C" {
int func_001C5570(void *, CANDIDATE *);
void IntersectSwBoundingBox(void *, qword, qword, int (*)(void *, CANDIDATE *), void *, int *, CANDIDATE ***);
CANDIDATE *PsoHitTestLineObjects(int, qword, qword, int, CANDIDATE **, LSG *)
    __asm__("PsoHitTestLineObjects__FiP6VECTORT1iPP2SOP3LSG");
extern char D_003DD490[];
extern qword D_002A2DC0;
}
// base + v * g
static inline qword AddScaled(VV base, VU_FLOAT g, qword v)
{
    qword r;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(base.q), "j"(v), "j"(g.q));
    return r;
}

extern "C" int func_001C6DE8(float gMin, float gMax, float gThreshold, qword point, int ccand, CANDIDATE **apcand,
                             qword *ppos)
{
    LSG lsg;
    FRAME frame;
    qword posMax = AddScaled(point, VU_FLOAT(gMax), D_002A2DC0);
    qword posMin = AddScaled(point, VU_FLOAT(gMin), D_002A2DC0);
    register qword posMaxArg __asm__("$6") = posMax;   // pin: the game loads this $a2 argument before the test (see top)
    if (!apcand)
        IntersectSwBoundingBox(D_003DD490, point, posMaxArg, func_001C5570, 0, &ccand, &apcand);
    if (gMax > 0) {
        if (PsoHitTestLineObjects(0, point, posMax, ccand, apcand, &lsg) && lsg.t == 0) {
            if (lsg.u == 1 || lsg.normalB.f[2] < gThreshold)
                return 0;
            *ppos = lsg.posB.q;
            return 1;
        }
    }
    if (gMin < 0) {
        if (PsoHitTestLineObjects(0, point, posMin, ccand, apcand, &lsg)) {
            if (lsg.t == 0 || lsg.normalA.f[2] < gThreshold)
                return 0;
            *ppos = lsg.posA.q;
            return 1;
        }
        return 0;
    }
    *ppos = point;
    return 1;
}
