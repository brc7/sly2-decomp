// func_001C5148 (w2lane4, wave 2): per-frame update of an animated object (state 6 only forwards to func_001F3980).
// Step 1: damped velocity/position integration of the object's own coordinate (VU: velocity = velocity * damping + acceleration * dt,
//         coordinate += velocity * dt) which is pushed into the position channel.
// Step 2: if the object's holder has an animation, find the position/orientation motions of this object in the animation's action
//         (entries keyed by id + channel via virtual slot 51), sample them (virtual slot 3) and apply them in the frame that
//         BuildOrthonormalMatrixZ builds from (camera position - channel position, z = 0).
// Matching notes:
//  * "rV" on the dt word keeps the load as an asm memory operand (qmtc2 source gets its own reload register, which decides
//    v0/v1 and the lqc2/qmtc2 order of the first VU block).
//  * The locals sit in the innermost block so that the VU_FLOAT temporaries share the first stack slot (sp+0) and the
//    positions/frame follow it; BuildOrthonormalMatrixZ/LoadRotateMatrixEuler return M3 by value (hidden pointer) and are assigned to locals.
//  * operator* is the usual inline matrix product with a named return value plus a dead local copy `t`; each row is stored to
//    `t` and `mat` right after its asm (the order of those stores decides the vf numbering of the loaded rows).
//  * `spin` is a second variable for p->orientation so that `current` and the later reload get different registers.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int words[4]; };
union M3 { qword rows[3]; float values[12]; };
struct VTE { short delta, pad; void *fn; };
struct CHANNEL { M3 matrix; qword position; char pad40[0x64]; int stamp; char padA8[8]; qword velocity; };
struct MOTION { VTE *vt; };
struct ENTRY { unsigned int key; MOTION *motion; };
struct ACTION { char pad5C[0x5C]; int count; ENTRY *entries; };
struct ANIM { char pad14[0x14]; ACTION *action; int word18; float time; char pad20[0x20]; float duration; };
struct HOLDER { char pad30[0x30]; ANIM *animation; };
struct P {
    char padC[0xC]; VTE *vt; char pad10[8]; int id; char pad1C[0x3C];
    CHANNEL *position, *orientation;
    char pad60[0xF0]; int state; char pad154[0x14]; HOLDER *holder; char pad16C[4];
    qword coordinate, velocity, acceleration;
};
union FLOATWORD { float value; int bits; };
struct CLOCK { float time; FLOATWORD dt; };
struct CAMERA { char pad90[0x90]; qword position; };
struct SW { char pad3D58[0x3D58]; int stamp; };
extern "C" {
extern CLOCK D_002DDED0;
extern CAMERA D_002DDF40;
extern SW D_003DD490;
extern float D_002C0D4C;
extern qword D_002A2DC0;
void func_001F3980(P *);
qword *func_00256D18(qword *, qword);
M3 BuildOrthonormalMatrixZ(qword, qword);
M3 LoadRotateMatrixEuler(qword);
}

struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
    VU_FLOAT(int bits) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "rV"(bits)); }
};
struct VU_VECTOR {
    qword q;
    VU_VECTOR(const qword &v) { q = v; }
};
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
static inline qword Advance(qword value, qword velocity, VU_FLOAT time)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(value), "j"(velocity), "j"(time.q));
    return r;
}
static inline qword Difference(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Add(qword a, qword b)
{
    qword r;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword RotateVec(const M3 &m, qword value)
{
    __asm__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z"
            : "+j"(value) : "j"(m.rows[0]), "j"(m.rows[1]), "j"(m.rows[2]));
    return value;
}
static inline M3 operator*(const M3 &matL, const M3 &matR) return mat
{
    M3 t;
    qword qwL0 = matL.rows[0], qwL1 = matL.rows[1], qwL2 = matL.rows[2];
    qword v0, v1, v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v0) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(matR.rows[0]));
    t.rows[0] = v0; mat.rows[0] = v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v1) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(matR.rows[1]));
    t.rows[1] = v1; mat.rows[1] = v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v2) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(matR.rows[2]));
    t.rows[2] = v2; mat.rows[2] = v2;
}
static inline CHANNEL *GetChannel(P *p, int channel)
{
    VTE &e = p->vt[51];
    return ((CHANNEL *(*)(P *, int))e.fn)((P *)((char *)p + e.delta), channel);
}
static inline void Sample(MOTION *motion, const ANIM *animation, qword *value, qword *velocity)
{
    VTE &e = motion->vt[3];
    ((void (*)(MOTION *, float, float, qword *, qword *))e.fn)((MOTION *)((char *)motion + e.delta), animation->time, animation->duration, value, velocity);
}

extern "C" void func_001C5148(P *p)
{
    if (p->state == 6) { func_001F3980(p); return; }
    float factor;
    if (p->state == 0 || p->state == 5) factor = 1.0f + D_002C0D4C * D_002DDED0.dt.value;
    else factor = 1.0f;
    p->velocity = VecCombo(VU_FLOAT(factor), VU_VECTOR(p->velocity), VU_FLOAT(D_002DDED0.dt.bits), VU_VECTOR(p->acceleration));
    qword newPosition = Advance(p->coordinate, p->velocity, VU_FLOAT(D_002DDED0.dt.bits));
    CHANNEL *channel = p->position;
    p->coordinate = newPosition;
    func_00256D18(&channel->position, newPosition);
    *(float *)((char *)channel + 0x3C) = 1.0f;
    p->position->velocity = p->velocity;
    { int stamp = ++D_003DD490.stamp; p->position->stamp = stamp; }
    if (p->holder && p->holder->animation) {
        ANIM *animation = p->holder->animation;
        ACTION *action = animation->action;
        MOTION *positionMotion = 0, *orientationMotion = 0;
        int i = 0;
        for (; i < action->count; i++) {
            ENTRY *entry = &action->entries[i];
            unsigned int key = entry->key;
            if (!(key & 0x40000000U)) continue;
            if (((int)(key << 12) >> 12) != p->id) continue;
            {
                CHANNEL *selected = GetChannel(p, (int)(key << 2) >> 22);
                if (selected == p->position) positionMotion = entry->motion;
                else if (selected == p->orientation) orientationMotion = entry->motion;
            }
            if (positionMotion && orientationMotion) break;
        }
        if (positionMotion || orientationMotion) {
            U dir, position, velocity, angles, angularVelocity;
            M3 frame, rot;
            dir.q = Difference(D_002DDF40.position, p->position->position);
            dir.words[2] = 0;
            frame = BuildOrthonormalMatrixZ(dir.q, D_002A2DC0);
            if (positionMotion) {
                Sample(positionMotion, animation, &position.q, &velocity.q);
                channel = p->position;
                func_00256D18(&channel->position, Add(channel->position, RotateVec(frame, position.q)));
                *(float *)((char *)channel + 0x3C) = 1.0f;
                CHANNEL *current = p->position;
                current->velocity = Add(current->velocity, RotateVec(frame, velocity.q));
                { int stamp = ++D_003DD490.stamp; p->position->stamp = stamp; }
            }
            if (orientationMotion) {
                Sample(orientationMotion, animation, &angles.q, &angularVelocity.q);
                rot = LoadRotateMatrixEuler(angles.q);
                CHANNEL *current = p->orientation;
                current->matrix = frame * rot;
                current->matrix.values[3] = 0.0f; current->matrix.values[7] = 0.0f; current->matrix.values[11] = 0.0f;
                CHANNEL *spin = p->orientation;
                spin->velocity = RotateVec(frame, angularVelocity.q);
                { int stamp = ++D_003DD490.stamp; p->orientation->stamp = stamp; }
            }
        }
    }
    func_001F3980(p);
}
