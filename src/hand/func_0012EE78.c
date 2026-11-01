typedef struct E { int a[5]; } E;
typedef struct T { char pad[0xE0]; E *pcur; int c; E *a; } T;
void func_0012EEE0(T *t, int n, int x, int y, float g, float h);
/** @sly2-annot addr=0x12EE78
 * Purpose: Callback in a 4-entry table (0x2AD86C..) forwarding to 0x12EEE0
 * Info: proposed name Cid43Callback (T2); class CID43; TU amb
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0
 * @end-sly2-annot */
E *func_0012EE78(T *t)
{
    if (!t->pcur) {
        t->pcur = &t->a[t->c];
        func_0012EEE0(t, -1, 300, 3000, 1.0f, 0.0f);
    }
    return t->pcur;
}
