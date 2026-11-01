extern float D_002A7874;
struct V { float x, y, z; };
struct S { char p0[0x70]; struct V v70; int pad; int n80; float g84; char p1[8]; struct V v90; int pad2; int nA0; float gA4; char p2[8]; float gB0; };
void InitLo(struct S *p);
static inline void SetVector(struct V *pv, float x, float y, float z) { pv->x = x; pv->y = y; pv->z = z; }
void func_001D6A90(struct S *p) {
    InitLo(p);
    SetVector(&p->v70, 0, 0, 0);
    p->n80 = 0;
    p->g84 = D_002A7874;
    SetVector(&p->v90, 0, 1.0f, 0);
    p->gB0 = 0.5f;
    p->gA4 = D_002A7874;
    p->nA0 = 0;
}
