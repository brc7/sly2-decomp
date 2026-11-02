typedef int qword __attribute__((mode(TI)));
struct F { char pad[0x28]; long fl; };
struct S;
typedef struct { short d; short pad; struct F *(*pfn)(void *); } VT;
struct S { char pad[0x20]; VT *vt; };
typedef struct E { struct F *x0; struct S *x4; int x8; float xc; float x10; int pad[3]; } __attribute__((aligned(16))) E;
struct P { int x0; int c; int pad[2]; E ae[32]; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern "C" void *memmove(void *dst, void *src, int cb);
extern "C" void *func_002333B4(void *p, int v, int cb);
/** @sly2-annot addr=0x19FD30
 * Purpose: Move-to-front insert of (object, param s) into the place table, merging entries within 50 of s
 * Info: proposed name AddJtPlaceOnCurve (T2); TU places
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grab-0x191090-0x195950,
 *   docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8,
 *   docs/reference/functions/player.md#places-0x19f3e8-0x1a0300
 * @end-sly2-annot */
extern "C" void func_0019FD30(struct P *p, struct S *s, float g)
{
    int i;
    E *e;
    for (i = 1; i < p->c; i++) {
        e = &p->ae[i];
        if (s == e->x4 && __builtin_fabsf(g - e->x10) < 50.0f) {
            if (i != 1) {
                E t = *e;
                memmove(&p->ae[2], &p->ae[1], (i - 1) * sizeof(E));
                p->ae[1] = t;
            }
            break;
        }
    }
    if (i >= p->c) {
        if ((unsigned int)p->c < 32)
            p->c++;
        memmove(&p->ae[1], &p->ae[0], p->c * sizeof(E) - sizeof(E));
    }
    e = &p->ae[0];
    func_002333B4(e, 0, sizeof(E));
    p->ae[0].x0 = s->vt[6].pfn((char *)s + s->vt[6].d);
    e->x4 = s;
    e->x10 = g;
    e->xc = D_002DDED0.t;
    p->ae[0].x0->fl |= 1L << 59;
}
