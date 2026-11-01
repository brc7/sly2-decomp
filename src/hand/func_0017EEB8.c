struct O { char pad[0xE4]; int nE4; };
struct S { char pad[0x30]; struct O *p30; int n34; float g38; };
extern float D_002BE7B8;
float SMaxShape(struct O *p);
float SMinShape(struct O *p);
int func_0017EEB8(struct S *p) {
    float g;
    if (p->p30->nE4) return 0;
    if (p->n34) g = SMaxShape(p->p30); else g = SMinShape(p->p30);
    if (D_002BE7B8 < __builtin_fabsf(p->g38 - g)) return 0;
    return 1;
}
