struct RGBA { unsigned int u; RGBA() {} RGBA(unsigned int x) { u = x; } };
struct CP { RGBA c0, c1; };
struct S { CP x0; char pad[0x28]; int x30, x34, x38; CP x3c; void *x44; int x48; CP x4c; int x54, x58, x5c; int pad2; int x64; char x68[4]; };
extern "C" char D_002A3A10[];
extern "C" void ClearSle(void *p);
extern "C" void func_002156B8(S *p)
{
    p->x44 = D_002A3A10;
    p->x30 = 0;
    p->x54 = 0;
    p->x0.c0 = RGBA(0x7F7F7F7F); p->x0.c1 = RGBA(0xFFFFFFFF);
    p->x3c.c0 = RGBA(0x7F7F7F7F); p->x3c.c1 = RGBA(0xFFFFFFFF);
    p->x4c.c0 = RGBA(0x7F7F7F7F); p->x4c.c1 = RGBA(0xFFFFFFFF);
    p->x64 = -1;
    p->x58 = 0;
    p->x34 = 0;
    p->x5c = 0;
    p->x38 = 0;
    p->x48 = 0;
    ClearSle(p->x68);
}
