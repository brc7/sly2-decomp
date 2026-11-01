typedef struct { float x, y, z; } V3;
struct S { int x0; int x4; char pad8[0xC]; float x14; float x18; float x1C; float x20; float x24; float x28; int pad2C;
  V3 v30; char pad3C[0x84]; V3 vC0; int padCC; V3 vD0; };
static inline void SetV(V3 *pv, float x, float y, float z) { pv->x = x; pv->y = y; pv->z = z; }
void InitThrow(struct S *p, int a)
{
    p->x0 = a;
    p->x4 = -1;
    p->x1C = 500.0f;
    p->x20 = 1500.0f;
    p->x24 = 1.5f;
    p->x14 = 1.0f;
    p->x18 = 1.0f;
    p->x28 = 1000.0f;
    SetV(&p->v30, 0.0f, 0.0f, 25.0f);
    SetV(&p->vC0, 1000.0f, 0.0f, 0.0f);
    SetV(&p->vD0, 42.0f, -73.0f, -50.0f);
}
