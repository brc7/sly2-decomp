typedef struct CB { int a; int b; } CB;
typedef struct E { float x; float y; int n; } E;
typedef struct T { char pad[0xB0]; E *pe; } T;
E *func_001AE1D0(CB *pcb);
/** @sly2-annot addr=0x12FDE8
 * Purpose: Allocates a 12-byte {x, y, n} record on the ASEG (consumed later)
 * Info: proposed name SetAsegPendingApply (T2); TU aseg
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void func_0012FDE8(T *t, int n, float x, float y)
{
    CB cb;
    E *pe;
    cb.a = 0xC;
    cb.b = 0x10;
    pe = func_001AE1D0(&cb);
    t->pe = pe;
    pe->x = x;
    pe->y = y;
    pe->n = n;
}
