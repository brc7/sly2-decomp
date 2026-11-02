typedef int qword __attribute__((mode(TI)));
struct SOURCE;
struct P { char pad[0x1C]; struct SOURCE *source; qword v; };
extern qword D_002A2DD0;
int func_0011D4D0(float, float, float);
void SetSndFlags(struct P *, unsigned short, unsigned short);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return func_0011D4D0(X(a), X(b), 0.0001f) && func_0011D4D0(Y(a), Y(b), 0.0001f) && func_0011D4D0(Z(a), Z(b), 0.0001f);
}
/** @sly2-annot addr=0x1FB2F8
 * Purpose: Attach sound to an ALO (positional), flag 0x200
 * Info: proposed name SetSndSourceLo (T1); TU sound
 * Notes: verifier CORRECT: SetSndSourceAlo -> SetSndSourceLo
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/animation.md#7-key-functions, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/engine/sound.md#2-sound-instances
 * @end-sly2-annot */
void SetSndSourceLo(struct P *p, struct SOURCE *source)
{
    p->source = source;
    if (source) SetSndFlags(p, 0x200, 0x200);
    else if (FNear(D_002A2DD0, p->v)) SetSndFlags(p, 0x200, 0);
}
