// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { int x0; } E;
typedef struct { char pad[0xE0]; int xE0; E *xE4; } S;
typedef struct { int a, b; } P;
E *func_001AE238(P *, E *);
E *func_002590F0(S *p, int x)
{
    if (x == 1) {
        p->xE0 = x;
        if (p->xE4->x0 >= 2) {
            P t;
            p->xE4->x0--;
            t.a = 0x100; t.b = 0x10;
            p->xE4 = func_001AE238(&t, p->xE4);
            p->xE4->x0 = x;
        }
    }
    return p->xE4;
}
