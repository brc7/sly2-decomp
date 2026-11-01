typedef struct { float m[4][4]; } __attribute__((aligned(16))) M4;
typedef struct Y { char pad[0x40]; M4 m40; char pad2[0x24]; int xA4; } Y;
typedef struct X { char pad[0x50]; Y *y; int x54; int pad2[2]; M4 m60; M4 mA0; } X;
M4 __ml__FRC7MATRIX4T0(M4 *, M4 *);

void UpdateXfmMatWorld(X *p)
{
    Y *y = p->y;
    if (y) {
        if (p->x54 < y->xA4) {
            p->x54 = y->xA4;
            p->mA0 = __ml__FRC7MATRIX4T0(&y->m40, &p->m60);
        }
    } else if (p->x54 < 0) {
        p->mA0 = p->m60;
    }
}
