// func_001DE5D8: (re)build the endpoint state of every entry of p and its bounding box. Each entry has two endpoints;
// an endpoint names an object: a path (shape test 10) becomes a moving endpoint with a 1/2/6-segment motion profile
// (mode 8 / 0 / 16, the last a trapezoidal accelerate-cruise-decelerate profile), shape 3/0x30 objects and missing
// objects become fixed points. All points (or the whole transformed path) are accumulated into a min/max box that
// becomes p's single bound (func_00166270 copies it to world space). Fresh rewrite (wave-4 lane 9).
// What mattered:
//  - Min/Max take VU_VECTOR (const qword & conversion) parameters as in func_001DF0A0: the extra RTL copies keep the
//    endpoint loop large enough that loop.c does not hoist 1.0f and 16 out of it;
//  - the min/max initializers are non-const globals (const loads may be scheduled above the spill stores);
//  - every segment is addressed as seg = &endpoint->track.motion.segments[k] (the store to the first field folds to
//    an endpoint-relative address), fields stored in struct order;
//  - the search loop reuses `seg` (set many times, so sched1 does not boost its sets), which lets `next` take $v0;
//  - the bound points are written through a block-local pointer so the address is loaded before each call.
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
typedef unsigned int uint;

struct VTE { short delta, pad; void *fn; };
struct M4 { qword rows[4]; };
struct PATH {
    char pad0[0xC];
    VTE *vt;
    char pad10[0xE8 - 0x10];
    int count;
    int padEC;
    void *padF0;
    qword *points;
};
struct PARAMS { float fraction, speed, duration; int mode; };
struct SEGMENT { float start, speed, accel, duration, elapsed; };
struct MOTION { int count; SEGMENT segments[6]; int current; };
struct TRACK { PATH *path; float cursor; union { PARAMS params; MOTION motion; }; };
struct ENDPOINT { int id, kind; int pad8[2]; union { qword point; TRACK track; }; };
struct ENTRY { ENDPOINT endpoints[2]; char pad140[0x10]; };
union U { qword q; float f[4]; };
struct GEO { int count; U *points; int planeCount; void *planes; int edgeCount; void *edges; };
struct P;
struct BOUND {
    void *object;
    P *owner;
    float radius;
    int tag;
    ulong flags;
    int id, pinned;
    M4 matrix;
    char pad60[8];
    int tree[4];
    GEO local, world;
    int state, stamp;
};
struct P {
    char pad0[0xC];
    VTE *vt;
    char pad10[8];
    int id;
    char pad1C[0xC];
    ulong flags28;
    char pad30[0x10];
    uint flags;
    char pad44[0x1C];
    void *space;
    char pad64[0xE0 - 0x64];
    int boundCount;
    BOUND *bounds;
    char padE8[0x198 - 0xE8];
    int count;
    ENTRY *entries;
};
struct MSG { int bytes, alignment; };
struct G;
extern G D_003DD490;
extern qword D_002A2DF0, D_002A2E00;
extern float D_002C3610;
extern "C" {
void RemoveSoParts(P *);
void *PloFindSwObject__FP2SWi3OIDP2LO(G *, int, int, P *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
float SMaxShape(PATH *);
void EvaluateShapeS(PATH *, qword *, qword *, float);
void *func_002333B4(void *, int, int);
qword func_001A62E8(void *);
qword func_0022FC48(void *, void *, qword);
void *func_001AE2A0(MSG *);
void func_00166270(GEO *, const M4 *, GEO *);
void AddSoParts(P *);
}

struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
static inline qword Min(VU_VECTOR a, VU_VECTOR b) { qword r; __asm__("vmini.xyz %0, %1, %2" : "=j"(r) : "j"(a.q), "j"(b.q)); return r; }
static inline qword Max(VU_VECTOR a, VU_VECTOR b) { qword r; __asm__("vmax.xyz %0, %1, %2" : "=j"(r) : "j"(a.q), "j"(b.q)); return r; }
static inline qword Transform(const M4 *m, qword p)
{
    qword r;
    __asm__("vmulax.xyz ACC,%1,%5x\n\tvmadday.xyz ACC,%2,%5y\n\tvmaddaz.xyz ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w"
            : "=j"(r) : "j"(m->rows[0]), "j"(m->rows[1]), "j"(m->rows[2]), "j"(m->rows[3]), "j"(p));
    return r;
}
static inline float Clamp(float f, float low, float high) { if (f < low) return low; else if (high < f) return high; else return f; }
static inline M4 *GetMatrix(PATH *path) { VTE *v = &path->vt[31]; return ((M4 * (*)(void *))v->fn)((char *)path + v->delta); }
static inline void SetDirty(P *p) { uint f = p->flags; if ((f ^ 1) & 1) *(uint *)f |= 2; else p->flags = f | 2; }

extern "C" void func_001DE5D8(P *p)
{
    int i;
    qword minimum, maximum;
    RemoveSoParts(p);
    minimum = D_002A2DF0;
    maximum = D_002A2E00;
    for (i = 0; i < p->count; i++) {
        ENTRY *entry = &p->entries[i];
        uint j;
        for (j = 0; j < 2; j++) {
            ENDPOINT *endpoint = &entry->endpoints[j];
            endpoint->kind = -1;
            if (endpoint->id != -1) {
                void *object = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 1, endpoint->id, p);
                if (object) {
                    if (FIsBasicDerivedFrom__FP5BASIC3CID(object, 10)) {
                        PATH *path = (PATH *)object;
                        PARAMS params = endpoint->track.params;
                        int reverse = 0;
                        params.fraction = Clamp(params.fraction, 0.0f, 1.0f);
                        if (params.speed < 0.0f) { reverse = 1; params.speed = -params.speed; }
                        params.duration = __builtin_fabsf(params.duration);
                        if (params.mode != 8 && params.mode != 16 && params.mode != 0) params.mode = 8;
                        float length = SMaxShape(path);
                        if (0.0f < params.duration) params.speed = length / params.duration;
                        else if (0.0f < params.speed) params.duration = length / params.speed;
                        float start = params.fraction * length;
                        if (params.speed == 0.0f || params.duration == 0.0f) {
                            endpoint->kind = 0;
                            EvaluateShapeS(path, &endpoint->point, 0, start);
                        } else {
                            endpoint->track.path = path;
                            endpoint->kind = 1;
                            endpoint->track.cursor = start;
                            func_002333B4(endpoint->track.motion.segments, 0, sizeof(endpoint->track.motion.segments));
                            SEGMENT *seg;
                            switch (params.mode) {
                            case 8:
                                endpoint->track.motion.count = 1;
                                seg = endpoint->track.motion.segments;
                                if (reverse) {
                                    seg->start = length;
                                    seg->speed = -params.speed;
                                    seg->elapsed = (1.0f - params.fraction) * params.duration;
                                } else {
                                    seg->speed = params.speed;
                                    seg->elapsed = params.fraction * params.duration;
                                }
                                seg->duration = params.duration;
                                endpoint->track.motion.current = 0;
                                break;
                            case 0:
                                endpoint->track.motion.count = 2;
                                seg = endpoint->track.motion.segments;
                                if (reverse) {
                                    seg->start = length;
                                    seg->speed = -params.speed;
                                    seg->elapsed = (1.0f - params.fraction) * params.duration;
                                } else {
                                    seg->speed = params.speed;
                                    seg->elapsed = params.fraction * params.duration;
                                }
                                seg->duration = params.duration;
                                seg = &endpoint->track.motion.segments[1];
                                if (reverse) seg->start = 0.0f;
                                else seg->start = length;
                                seg->speed = 0.0f;
                                seg->elapsed = 0.0f;
                                endpoint->track.motion.current = 0;
                                break;
                            case 16: {
                                struct { float in, out; } ramp;
                                if (3.0f * D_002C3610 <= length) ramp.in = D_002C3610;
                                else ramp.in = length * 0.2f;
                                float twice = ramp.in + ramp.in;
                                ramp.out = length - ramp.in;
                                float rampTime = twice * params.duration / (length + twice);
                                float middleTime = params.duration - (rampTime + rampTime);
                                params.speed = (length - twice) / middleTime;
                                float accel = params.speed * params.speed / twice;
                                endpoint->track.motion.count = 6;
                                seg = endpoint->track.motion.segments;
                                seg->accel = accel;
                                seg->duration = rampTime;
                                seg = &endpoint->track.motion.segments[1];
                                seg->start = ramp.in;
                                seg->speed = params.speed;
                                seg->duration = middleTime;
                                seg = &endpoint->track.motion.segments[2];
                                seg->start = ramp.out;
                                seg->speed = params.speed;
                                seg->accel = -accel;
                                seg->duration = rampTime;
                                seg = &endpoint->track.motion.segments[3];
                                seg->start = length;
                                seg->accel = -accel;
                                seg->duration = rampTime;
                                seg = &endpoint->track.motion.segments[4];
                                seg->start = ramp.out;
                                seg->speed = -params.speed;
                                seg->duration = middleTime;
                                seg = &endpoint->track.motion.segments[5];
                                seg->start = ramp.in;
                                seg->speed = -params.speed;
                                seg->accel = accel;
                                seg->duration = rampTime;
                                // find the segment the start position falls in and the time already spent in it
                                for (endpoint->track.motion.current = 0; endpoint->track.motion.current < 3; endpoint->track.motion.current++) {
                                    seg = &endpoint->track.motion.segments[endpoint->track.motion.current];
                                    SEGMENT *next = seg + 1;
                                    if (start < next->start) {
                                        float t = ((start - seg->start) + (start - seg->start)) / (seg->speed + next->speed);
                                        if (reverse) {
                                            int k = 5 - endpoint->track.motion.current;
                                            endpoint->track.motion.current = k;
                                            seg = &endpoint->track.motion.segments[k];
                                            seg->elapsed = seg->duration - t;
                                        } else {
                                            seg->elapsed = t;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            }
                            SetDirty(p);
                        }
                    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(object, 3) || FIsBasicDerivedFrom__FP5BASIC3CID(object, 0x30)) {
                        endpoint->kind = 0;
                        endpoint->point = func_001A62E8(object);
                    }
                }
            }
            if (endpoint->kind == -1) {
                endpoint->kind = 0;
                endpoint->point = func_001A62E8(p);
            }
            switch (endpoint->kind) {
            case 0:
                minimum = Min(minimum, endpoint->point);
                maximum = Max(maximum, endpoint->point);
                endpoint->point = func_0022FC48(0, p->space, endpoint->point);
                break;
            case 1: {
                // the matrix callback may change the endpoint's path, so it is re-read
                M4 matrix = *GetMatrix(endpoint->track.path);
                int k;
                for (k = 0; k < endpoint->track.path->count; k++) {
                    qword point = Transform(&matrix, endpoint->track.path->points[k]);
                    minimum = Min(minimum, point);
                    maximum = Max(maximum, point);
                }
                break;
            }
            }
        }
    }
    if (p->count) {
        MSG request;
        p->boundCount = 1;
        request.bytes = sizeof(BOUND);
        request.alignment = 16;
        BOUND *bound = (BOUND *)func_001AE2A0(&request);
        p->bounds = bound;
        bound->object = p->space;
        bound->owner = p;
        bound->tag = p->id;
        bound->flags |= 1UL << 48;
        bound->local.count = 2;
        request.alignment = 16;
        request.bytes = 2 * sizeof(qword);
        bound->local.points = (U *)func_001AE2A0(&request);
        func_00166270(&bound->local, 0, &bound->world);
        bound->world.points[0].q = minimum;
        bound->world.points[1].q = maximum;
        { U *out = bound->local.points; out[0].q = func_0022FC48(0, p->space, minimum); }
        { U *out = bound->local.points; out[1].q = func_0022FC48(0, p->space, maximum); }
    }
    if ((int)(p->flags28 >> 60) & 1) AddSoParts(p);
}
