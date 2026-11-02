#include "mat.h"
typedef struct MATRIX4 M4;
typedef struct VECTOR V;
typedef struct VT { short d; short pad; void (*pfn)(void *, int); } VT;
typedef struct S { char pad[0xC]; VT *vt; } S;
typedef struct P {
    char pad0[0x94];
    int x94;
    short x98;
    char pad9A[0xB0 - 0x9A];
    V xB0;
    char padC0[0xF0 - 0xC0];
    V xF0;
} P;
static inline void SetV(V *pv, float x, float y, float z) { pv->x = x; pv->y = y; pv->z = z; }
int func_00230310(int);
int func_00230348(int);
int func_00230380(int);
M4 func_0022CDC0(P *);
M4 InvertMatrix4Rigid(P *);
M4 __ml__FRC7MATRIX4T0(M4 *, M4 *);
void func_0022C818(S *, P *);

/** @sly2-annot addr=0x2303A0
 * Purpose: Set the motion role (+0x98) of one XF slot of an SO; on leaving a static role capture the current
 *   value from the slot matrix; zero dv for velocity roles; wake the SO (vt47 SetSoGrf 4) unless the new role
 *   is an at-rest role
 * Info: proposed name SlotSetRole (T1); TU xf
 * Notes: top50_round (0.65): Lilly's Sly 2 headers declare 'void SlotSetRole(SO *pso, XF *pxf, int nRole)'
 *   (src/hand/SetJtJts__FP2JT3JTS4JTBS.cpp:1649, Lilly jtstate.c port) and call it with (pjt, pjt->pxfUnk58/5c, 1|2),
 *   matching this body's (so,xf,role). Body
 * Confidence: mid (draft) [T1 0.65] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r8_21BB88+top50_round
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/animation.md#tail-0x21c820-0x21d278,
 *   docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_002303A0(S *s, P *p, int n)
{
    if (n == p->x98)
        return;
    if (func_00230310(n) && !func_00230310(p->x98)) {
        M4 m;
        M4 t;
        t = func_0022CDC0(p);
        m = t;
        switch (p->x94) {
        case 1: case 4: case 5: case 9: case 10:
            p->xB0 = *(V *)m.mat[3];
            break;
        case 11:
            SetV(&p->xB0, m.mat[1][0], m.mat[2][0], m.mat[2][1]);
            break;
        case 12:
            SetV(&p->xB0, m.mat[0][0], m.mat[1][1], m.mat[2][2]);
            break;
        case 6: case 7: {
            M4 r = InvertMatrix4Rigid(p);
            t = __ml__FRC7MATRIX4T0(&m, &r);
            m = t;
            SetV(&p->xB0, m.mat[1][2], m.mat[2][0], m.mat[0][1]);
            break;
        }
        }
    }
    switch (n) {
    case 1: case 2: case 3:
        if (!func_00230380(p->x98)) {
            { V *pv = &p->xF0; pv->z = 0; pv->y = 0; pv->x = 0; }
        }
        func_0022C818(s, p);
        break;
    case 0: case 13: case 14: case 15: case 16: case 17: case 20: case 21:
        func_0022C818(s, p);
        break;
    }
    p->x98 = n;
    if (!func_00230348(n))
        s->vt[47].pfn((char *)s + s->vt[47].d, 4);
}
