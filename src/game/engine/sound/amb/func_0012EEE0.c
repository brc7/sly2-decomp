typedef struct E { int n; float x; float y; float g; float h; } E;
typedef struct T { char pad[0xE4]; int c; E *a; } T;
/** @sly2-annot addr=0x12EEE0
 * Purpose: Script-settable parameter of the ambient sound object
 * Info: proposed name SetCid43SoundParam (T2); class CID43; TU amb
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0
 * @end-sly2-annot */
void func_0012EEE0(T *t, int n, int x, int y, float g, float h)
{
    if (t->c < 8) {
        E *e = &t->a[t->c++];
        e->n = n;
        e->x = x;
        e->y = y;
        e->g = g;
        e->h = h;
    }
}
