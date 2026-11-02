typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x1C]; void *p1C; qword v20; char pad2[0x40 - 0x30]; int n40; char pad3[0x4E - 0x44]; short n4E; unsigned short f50; unsigned short n52; };
extern qword D_002A2DD0;
int func_0011D4D0(float, float, float);
void SetSndFlags(struct P *, int, int);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return func_0011D4D0(X(a), X(b), 0.0001f) && func_0011D4D0(Y(a), Y(b), 0.0001f) && func_0011D4D0(Z(a), Z(b), 0.0001f);
}
/** @sly2-annot addr=0x1FB0C0
 * Purpose: vt7: if not already playing/starting, request start + all dirty bits (0x1F1, +pause if paused,
 *   +0x200 if positional)
 * Info: proposed name StartSnd (T1); class SND; TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void StartSnd(struct P *p)
{
    int f = 0;
    int k;
    if (!p->n40) {
        f = p->n52 || (p->f50 & 1);
        f ^= 1;
    }
    if (!f) return;
    { int fA = p->n4E > 0; k = 0x1F1; if (fA) k = 0x1F3; }
    if (p->p1C || !FNear(D_002A2DD0, p->v20))
        k |= 0x200;
    SetSndFlags(p, k, k);
}
