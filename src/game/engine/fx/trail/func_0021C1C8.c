typedef int qword __attribute__((mode(TI)));
struct M { qword r[4]; };
struct T { float t; float g; float length; int pad; struct M m; };
struct VT { char pad[0xF8]; short d; short pd; struct M *(*matrix)(void *); };
struct S {
    char pad[0xC]; struct VT *vt; char pad10[0x11C]; float g;
    char pad130[0x24]; int state; int count; struct T *array; int index; int idle;
    char pad168[0x18]; void *follow; float rate; float distance;
    char pad18c[4]; qword previous;
};
extern struct CLOCK { float t; float dt; } D_002DDED0;
void func_001F3980(struct S *);
qword func_001A62E8(void *);
void func_0021C5F8(struct S *, qword);
static inline qword VSub(qword a, qword b)
{
    qword d;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(d) : "j"(a), "j"(b));
    return d;
}
static inline qword Dot(qword d) {
    qword one;
    __asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(d), "=j"(one));
    return d;
}
static inline float Sqrt(qword d) {
    float f;
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "+j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(d));
    return f;
}
/** @sly2-annot addr=0x21C1C8
 * Purpose: after SO::Update (0x1F3980) pushes {t, uAlpha, length, PmatWorld} into ring; optional follow
 *   target moved by rate/distance (0x21C5F8)
 * Info: proposed name Cid105_slot17 (T2); class CID105 slot17; TU trail
 * Notes: agent qualifier: (per-frame SO update) - record history sample
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820
 * @end-sly2-annot */
void func_0021C1C8(struct S *p)
{
    func_001F3980(p);
    if (!p->state) {
        struct T *e = &p->array[p->index];
        p->index = (p->index + 1) % p->count;
        e->t = D_002DDED0.t;
        e->g = p->g;
        e->m = *p->vt->matrix((char *)p + p->vt->d);
        if (p->g <= 0.0f)
            p->idle++;
        else
            p->idle = 0;
        if (p->follow) {
            qword pos;
            float length;
            if (p->g == 0.0f)
                p->distance = -4.0f;
            pos = func_001A62E8(p->follow);
            {
                float rate = p->rate;
                qword d = VSub(p->previous, pos);
                float dist = p->distance;
                d = Dot(d);
                p->previous = pos;
                length = Sqrt(d);
                p->distance = dist + length / rate;
            }
            e->length = p->distance;
        }
        func_0021C5F8(p, func_001A62E8(p));
    }
}
