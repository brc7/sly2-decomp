// func_001A4478: per-frame "shader basis" packets for a query point.
//  1. point = *p->input (optionally transformed by M4 *transform); func_001A3F78 collects the 4 best-ranked objects.
//  2. If p->strength != 0: four 48-byte VIF packets (tag 0x6C03C000 | (i*3+0x15) + 12 words), from each result
//     object's payload (+0x120), the rest from the default block D_002BFC60 (same code as func_001A4AD8), padded to a qword.
//  3. If p->limit != 0: weighted centre of the enabled results (kind 1: virtual slot 31 returns a matrix, its row 3;
//     kind 0: point - direction * D_002BFC90), clamped to D_002BFC94 * limit from the point; basis from GetNormalVectors,
//     row 2 = dir * extent; packet (M4 rows + kind + fraction) unpacked with tag 0x6C05C00E after aligning to 12 mod 16.
#include "vifs.h"
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; uint u[4]; };
struct M3 { U row[3]; };
struct M4 { U row[4]; };
struct BLOCK { uint words[12]; };
struct VTE { short delta; short pad; M4 *(*pfn)(void *); };
struct OBJ {
    char pad0[0xC]; VTE *vt; char pad10[0xA0 - 0x10];
    qword direction;              // 0xA0
    char padB0[0xE0 - 0xB0];
    int kind;                     // 0xE0
    char padE4[0x120 - 0xE4];
    BLOCK payload;                // 0x120
    char pad150[0x158 - 0x150];
    int enabled;                  // 0x158
};
struct RESULT { float score; OBJ *object; };
struct SPHERE { qword pos; float radius; int pad[3]; };
struct PACKET { U basis[3]; U position; int kind; float fraction; int pad[2]; };
struct CAMERA { char pad[0x48]; float radius; char pad4C[0x90 - 0x4C]; qword position; };
struct P {
    int pad0; qword *input; int pad8[2];
    float strength;               // 0x10
    int cqwPacket;                // 0x14
    uchar *pbPacket;              // 0x18
    float limit;                  // 0x1C
    float extra;                  // 0x20
    uchar *pbPayload;             // 0x24
    int cqwBasis;                 // 0x28
    uchar *pbBasis;               // 0x2C
};
struct WORLD;
extern VIFS D_002DEA28;
extern BLOCK D_002BFC60;
extern float D_002BFC90, D_002BFC94;
extern qword D_002A2DA0, D_002A2DB0, D_002A2DC0;
extern CAMERA D_002DDF40;
extern "C" {
void func_001A3F78(WORLD *world, qword pos, int capacity, RESULT *results);
void GetNormalVectors(qword a, qword b, qword c, M3 *pm);
qword *func_00256D18(qword *p, qword v);
int func_001A4258(qword pos, float radius, int csphere, SPHERE *asphere);
}
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); } };
static inline qword Zero() { qword r; __asm__("vsub.xyzw %0,$vf0,$vf0" : "=j"(r)); return r; }
static inline qword Transform(const M4 &m, qword v)
{
    __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddaz.xyz $ACC,%3,%0z\n\tvmaddw.xyz %0,%4,$vf0w"
            : "+j"(v) : "j"(m.row[0].q), "j"(m.row[1].q), "j"(m.row[2].q), "j"(m.row[3].q));
    return v;
}
/* base + v * g */
static inline qword AddScaled(qword base, const qword &v, VU_FLOAT g)
{
    qword r;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%3,%2x" : "=j"(r) : "j"(base), "j"(g.q), "j"(v));
    return r;
}
static inline qword Scale(qword v, VU_FLOAT g) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(v), "j"(g.q)); return r; }
static inline qword Add(qword a, qword b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Divide(qword v, VU_FLOAT g) { qword r; __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(r) : "j"(g.q), "j"(v)); return r; }
static inline float Length(qword v)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(d) : "j"(d));
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(d));
    return f;
}
static inline uint *Write(VIFS *g, uint x) { uchar *d; *(uint *)g->m_pb = x; d = g->m_pb + 4; g->m_pb = d; return (uint *)d; }
#define Copy(D,S,W) uint*d_=(D);const uint*s_=(S);do{int k;for(k=0;k<(W);k+=4){uint a=s_[0],b=s_[1],c=s_[2],e=s_[3];d_[0]=a;d_[1]=b;d_[2]=c;d_[3]=e;s_+=4;d_+=4;}}while(0);

static inline void Clear(P *p) { p->cqwBasis = 0; p->pbBasis = 0; p->pbPayload = 0; }
static inline void Align(uint align)
{
    VIFS *dma = &D_002DEA28;
    uchar *pb = dma->m_pb;
    while (((uint)pb & 15) != align) {
        *(uint *)pb = 0;
        dma->m_pb += 4;
        pb = dma->m_pb;
    }
}
extern "C" void func_001A4478(WORLD *world, M4 *transform, P *p)
{
    RESULT results[4];
    qword point = *p->input;
    if (transform)
        point = Transform(*transform, point);
    func_001A3F78(world, point, 4, results);
    if (p->strength == 0.0f) {
        p->cqwPacket = 0;
        p->pbPacket = 0;
    } else {
        int i = 0;
        uint slot;
        p->pbPacket = D_002DEA28.m_pb;
        for (slot = 0; slot < 4; slot++) {
            OBJ *o = results[slot].object;
            if (!o)
                break;
            {
                uint *d = Write(&D_002DEA28, 0x6C03C000 | (i * 3 + 0x15));
                Copy(d, o->payload.words, 12);
                D_002DEA28.m_pb += 48;
            }
            i++;
        }
        for (; i < 4; i++) {
            VIFS *g = &D_002DEA28;
            const uint *src = D_002BFC60.words;
            uint *d = Write(g, 0x6C03C000 | (i * 3 + 0x15));
            Copy(d, src, 12);
            g->m_pb += 48;
        }
        Align(0);
        p->cqwPacket = (D_002DEA28.m_pb - p->pbPacket) >> 4;
    }
    if (p->limit == 0.0f) {
        Clear(p);
    } else {
        float total = 0.0f;
        qword sum = Zero();
        uint slot;
        for (slot = 0; slot < 4; slot++) {
            OBJ *o = results[slot].object;
            if (!o)
                break;
            if (o->enabled) {
                switch (o->kind) {
                case 1: {
                    float w = results[slot].score;
                    VU_FLOAT vw(w);
                    M4 *m;
                    total += w;
                    m = o->vt[31].pfn((char *)o + o->vt[31].delta);
                    sum = AddScaled(sum, m->row[3].q, vw);
                    break;
                }
                case 0: {
                    float w = results[slot].score;
                    total += w;
                    sum = AddScaled(sum, AddScaled(point, o->direction, VU_FLOAT(-D_002BFC90)), VU_FLOAT(w));
                    break;
                }
                }
            }
        }
        if (total == 0.0f) {
            Clear(p);
        } else {
            qword center = Divide(sum, VU_FLOAT(total));
            qword dir = Sub(point, center);
            float dist = Length(dir);
            float limit = D_002BFC94 * p->limit;
            float extent;
            if (dist < limit) {
                if (dist < 1.0f)
                    dir = D_002A2DC0;
                else
                    dir = Divide(dir, VU_FLOAT(dist));
                center = AddScaled(point, dir, VU_FLOAT(-limit));
                dist = limit;
            } else {
                dir = Divide(dir, VU_FLOAT(dist));
            }
            extent = dist + p->extra;
            M3 basis;
            GetNormalVectors(dir, D_002A2DB0, D_002A2DA0, &basis);
            basis.row[2].q = Scale(dir, VU_FLOAT(extent));
            PACKET packet;
            U *row = packet.basis;
            func_00256D18(&packet.position.q, center);
            row[0] = basis.row[0];
            row[1] = basis.row[1];
            row[2] = basis.row[2];
            packet.basis[2].u[3] = 0;
            packet.basis[1].u[3] = 0;
            packet.basis[0].u[3] = 0;
            row[3].f[3] = 1.0f;
            packet.fraction = dist / extent;
            SPHERE spheres[3];
            spheres[0].pos = point;
            spheres[0].radius = p->limit;
            spheres[1].pos = Add(center, basis.row[2].q);
            spheres[1].radius = p->limit * extent / dist;
            spheres[2].pos = AddScaled(center, basis.row[2].q, VU_FLOAT(1.5f));
            *(int *)&spheres[2].radius = 0;
            packet.kind = func_001A4258(D_002DDF40.position, D_002DDF40.radius, 3, spheres);
            {
                uchar *pb = D_002DEA28.m_pb;
                p->pbBasis = pb;
                // pin: sched1 puts the Align(12) constant first (birthing-insn boost), so the %hi(D_002DEA28)
                // pseudo dies right where this one is born and local-alloc's fake-lifetime check keeps the
                // hi out of v1; a hard-reg v1 here has no fake lifetime, giving the target's v1 for both.
                register uchar *payload asm("$3") = pb + 16;
                p->pbPayload = payload;
            }
            Align(12);
            {
                VIFS *g = &D_002DEA28;
                uint *d = Write(g, 0x6C05C00E);
                do {
                    int k;
                    for (k = 0; k < 20; k += 4) {
                        uint a = row->u[0], b = row->u[1], c = row->u[2], e = row->u[3];
                        d[0] = a; d[1] = b; d[2] = c; d[3] = e;
                        row++;
                        d += 4;
                    }
                } while (0);
                g->m_pb += 80;
            }
            p->cqwBasis = (D_002DEA28.m_pb - p->pbBasis) >> 4;
        }
    }
}
