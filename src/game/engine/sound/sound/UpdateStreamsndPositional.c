typedef int qword __attribute__((mode(TI)));
struct LO {char pad28[0x28];unsigned long flags;};
struct RANGE {float low,high;};
struct E {struct LO *object;struct RANGE limits;float gain;};
struct P {char pad1C[0x1C];struct LO *object;qword position;char pad30[0x70];int counts[6];struct E entries[6][8];};
extern qword D_002A2DD0;
int func_0011D4D0(float,float,float);
void UpdateSndPositional(struct P *);
qword func_001A62E8(struct LO *);
void CalculateSndVolPan(struct P *,qword,const struct RANGE *,float *,int *);
void func_001FC380(struct P *,int,int);
void SetStreamsndGroupVolume(struct P *,int,float);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return func_0011D4D0(X(a), X(b), 0.0001f) && func_0011D4D0(Y(a), Y(b), 0.0001f) && func_0011D4D0(Z(a), Z(b), 0.0001f);
}
/** @sly2-annot addr=0x1FC9B0
 * Purpose: vt16: base positional update, then per sub-group: loudest visible source sets group vol/pan
 * Info: proposed name UpdateStreamsndPositional (T1); class STREAMSND; TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void UpdateStreamsndPositional(struct P *p)
{
    int group;
    if (p->object || !FNear(D_002A2DD0, p->position))
        UpdateSndPositional(p);
    for (group = 0; group < 6; group++) {
        int count = p->counts[group];
        if (count) {
            float maximum = 0.0f;
            int flags = 0, active = 0, i;
            int *pflags = &flags;
            float fade;
            for (i = 0; i < count; i++) {
                struct E *entry = &p->entries[group][i];
                struct LO *object = entry->object;
                if (object && ((object->flags >> 60) & 1) == 1) {
                    active++;
                    CalculateSndVolPan(p, func_001A62E8(object), &entry->limits, &fade, pflags);
                    pflags = 0;
                    { float gain = fade * entry->gain; maximum = maximum > gain ? maximum : gain; }
                }
            }
            func_001FC380(p, group, active == 1 ? flags : 0);
            SetStreamsndGroupVolume(p, group, maximum);
        }
    }
}
