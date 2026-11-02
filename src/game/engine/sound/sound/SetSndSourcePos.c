typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x1C]; int n1C; qword v; };
extern qword D_002A2DD0;
int FFloatsNear__Ffff(float, float, float);
void SetSndFlags(struct P *, int, int);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return FFloatsNear__Ffff(X(a), X(b), 0.0001f) && FFloatsNear__Ffff(Y(a), Y(b), 0.0001f) && FFloatsNear__Ffff(Z(a), Z(b), 0.0001f);
}
/** @sly2-annot addr=0x1FB3F0
 * Purpose: Set world position (positional unless zero vector), flag 0x200
 * Info: proposed name SetSndSourcePos (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void SetSndSourcePos(struct P *p, qword v)
{
    p->v = v;
    if (!FNear(D_002A2DD0, v)) SetSndFlags(p, 0x200, 0x200);
    else if (!p->n1C) SetSndFlags(p, 0x200, 0);
}
