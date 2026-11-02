typedef int qword __attribute__((mode(TI)));
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
struct T { char pad[0x14]; int x14; };
struct P { char pad[0x10DC]; void *x10DC; char pad1[0x17FC - 0x10E0]; int x17FC; int x1800; int x1804; char pad2[0x190C - 0x1808]; struct T *x190C; };
void SetMurrayState(struct P *p, int n);
qword func_001A62E8(void *p);
qword ConvertAloVec(void *a, void *b, qword q);
void SetMurrayAttack(struct P *p, int k);
static __inline__ qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
/** @sly2-annot addr=0x1B8508
 * Purpose: Square attack: SetMurrayState(11) and SetMurrayAttack kind 0/1 (alternating hand, chosen by side
 *   of target)
 * Info: proposed name StartMurrayPunch (T1); TU murray
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void StartMurrayPunch(struct P *p)
{
    int k;
    SetMurrayState(p, 0xB);
    if (p->x190C) {
        int v = p->x190C->x14;
        if (v == p->x17FC || v == p->x1804) k = 1;
        else k = 0;
    } else if (p->x10DC) {
        if (GetY(ConvertAloVec(0, p, Sub(func_001A62E8(p->x10DC), func_001A62E8(p)))) > 0.0f) k = 0;
        else k = 1;
    } else
        k = 1;
    SetMurrayAttack(p, k);
}
