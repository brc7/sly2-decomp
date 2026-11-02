typedef int qword __attribute__((mode(TI)));
struct S { int x0; int x4; int pad[2]; long x10; };
typedef struct E { int x0; int x4; struct S *x8; float xc; qword x10; } E;
struct P { int x0; int c; int pad[2]; E ae[32]; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void *memmove(void *dst, void *src, int cb);
void *func_002333B4(void *p, int v, int cb);
static __inline__ float Dist(qword a, qword b)
{
    qword r, t;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "0"(b));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q"
            : "+j"(r), "=&j"(t));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(r));
    return g;
}
/** @sly2-annot addr=0x19FEB0
 * Purpose: Move-to-front insert of (object, position) merging entries within 50 units
 * Info: proposed name AddJtPlaceAtPos (T2); TU places
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8,
 *   docs/reference/functions/player.md#grab-0x191090-0x195950,
 *   docs/reference/functions/player.md#places-0x19f3e8-0x1a0300
 * @end-sly2-annot */
void func_0019FEB0(struct P *p, struct S *s, qword q)
{
    int i;
    E *e;
    for (i = 1; i < p->c; i++) {
        e = &p->ae[i];
        if (s == e->x8 && Dist(q, e->x10) < 50.0f) {
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
    if (s) {
        p->ae[0].x0 = s->x4;
        e->x8 = s;
        s->x10 |= 1L << 51;
    }
    p->ae[0].x10 = q;
    e->xc = D_002DDED0.t;
}
