// func_001DF0A0: for every segment entry of p, find where the segment's two endpoints hit the request (first
// func_0014C6A0, else the shape test FIsBasicDerivedFrom + func_00205670) and add a kind-10 contact there with PxpAdd
// (func_001A7C18), then post the OBJ::Notify (ZapLasenBeams) message through EnsureSwCallback.
// What mattered (w3lane2):
//  - the same 6-argument virtual signature as func_001823C8 / AddBombCustomXps, with every argument passed through to
//    the empty base stub func_001F5990 (no code, but the call's argument uses fix the prologue schedule);
//  - VU helpers taking VU_VECTOR parameters (const qword & conversion): the extra RTL copies keep the loop above
//    loop.c's ~95-insn limit, so i * sizeof(ENTRY) stays a mult inside the loop as in the target;
//  - Len(v) = Sqrt(Dot(v, v)) with a two-input Dot, one asm per VU step (target VU order and vf registers);
//  - the distance is a float in a GPR (qmfc2) passed bit-for-bit as PxpAdd's int argument (v0 scratch, not v1).
typedef int qword __attribute__((mode(TI)));
struct CONTACT;
struct ENDPOINT { int id, kind; char unknown8[8]; qword value; char unknown20[0x80]; };
struct ENTRY { ENDPOINT endpoint[2]; char unknown140[8]; CONTACT *head; int unknown14C; };
struct OBJ {
    char pad0[0xE4];
    void *shape;
    char padE8[0x198 - 0xE8];
    int entryCount;
    ENTRY *entries;
    void Notify() __asm__("ZapLasenBeams");
};
struct SUB { char pad[0x14]; void **x14; };
struct REQUEST { int x0; SUB *x4; };
struct HIT { qword pos; char pad[0x50]; };
union VECTOR { qword q; float f[4]; };
struct G;
extern G D_003DD490;
typedef void (OBJ::*PMF)();
extern "C" {
void func_001F5990(OBJ *, REQUEST *, int, int, int, CONTACT **);
void GetLasenBeamEnds(OBJ *, ENDPOINT *, qword *, float *);
int func_0014C6A0(REQUEST *, qword, qword, int, HIT *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
int func_00205670(void *, qword, qword, qword *);
void func_001A7C18(G *, void *, REQUEST *, qword, qword, int, int, int, CONTACT **, float);   // PxpAdd
void EnsureSwCallback(G *, OBJ *, PMF, int);
}

struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
static inline qword Sub(VU_VECTOR a, VU_VECTOR b)
{
    qword d;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a.q), "j"(b.q));
    return d;
}
static inline qword Dot(VU_VECTOR a, VU_VECTOR b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a.q), "j"(b.q));
    return d;
}
static inline qword Sqrt(VU_VECTOR d)
{
    qword l;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d.q));
    return l;
}
static inline qword Len(VU_VECTOR v) { return Sqrt(Dot(v, v)); }
static inline qword Div(VU_VECTOR v, VU_VECTOR l)
{
    qword r;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l.q), "j"(v.q));
    return r;
}
static inline float Scalar(VU_VECTOR v)
{
    float f;
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(v.q));
    return f;
}

/** @sly2-annot addr=0x1DF0A0
 * Purpose: LASEN vt60: raycast each beam (0x14C6A0) and record what it hits (contact list per beam), then zap
 * Info: proposed name UpdateLasenContacts (T2); class LASEN slot60; TU sensor
 * Notes: names_v2 C proposed SetLasenSensors__FP5LASEN7SENSORS (0.438) | agent qualifier: (vt60)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
extern "C" void func_001DF0A0(OBJ *p, REQUEST *q, int a2, int a3, int a4, CONTACT **pout)
{
    int i;
    func_001F5990(p, q, a2, a3, a4, pout);
    for (i = 0; i < p->entryCount; i++) {
        ENTRY *e = &p->entries[i];
        qword seg[2];
        VECTOR pos;
        HIT hit;
        int found = 0;
        GetLasenBeamEnds(p, e->endpoint, seg, 0);
        if (!func_0014C6A0(q, seg[0], seg[1], 1, &hit)) {
            void *so = *q->x4->x14;
            if (FIsBasicDerivedFrom__FP5BASIC3CID(so, 0x33))
                found = func_00205670(so, seg[0], seg[1], &pos.q);
        } else {
            found = 1;
            pos.q = hit.pos;
        }
        if (found) {
            float dist = Scalar(Len(Sub(seg[0], pos.q)));
            qword dir = Sub(seg[1], seg[0]);
            dir = Div(dir, Len(dir));
            func_001A7C18(&D_003DD490, p->shape, q, pos.q, dir, 10, *(int *)&dist, 0, &e->head, 0.0f);
            EnsureSwCallback(&D_003DD490, p, &OBJ::Notify, 0);
        }
    }
}
