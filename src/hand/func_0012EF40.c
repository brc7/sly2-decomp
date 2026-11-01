typedef struct E { int n; float x; float y; float g; } E;
typedef struct T { char pad[0xF0]; int c; E *a; } T;
/** @sly2-annot addr=0x12EF40
 * Purpose: Small helper, no callers (proposal PtargetEnsureAlo rejected)
 * Info: proposed name Cid43Unk (T2); class CID43; TU amb
 * Notes: names_v2 C proposed PtargetEnsureAlo__FP3ALO (0.501)
 * Confidence: low (draft) [T2 0.20] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0
 * @end-sly2-annot */
void func_0012EF40(T *t, int n, int x, int y, float g)
{
    if (t->c < 8) {
        E *e = &t->a[t->c++];
        e->n = n;
        e->x = x;
        e->y = y;
        e->g = g;
    }
}
