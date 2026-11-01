struct V { float x, y, z; };
struct S { char p0[0x180]; int n180; char p1[0x51C-0x184]; float g51C; struct V v520; int pad; int n530; struct V v534;
    char p2[0x55C-0x540]; int n55C; char p3[0x578-0x560]; int n578; char p4[8]; int n584; char p5[8]; int n590; char p6[8]; int n59C; };
void InitPo(struct S *p);
static inline void SetVector(struct V *pv, float x, float y, float z) { pv->x = x; pv->y = y; pv->z = z; }
void InitRobottank(struct S *p) {
    InitPo(p);
    p->g51C = 500.0f; //P
    p->n180 = -1; //P
    SetVector(&p->v520, 1500.0f, -1000.0f, 0.0f); //P
    p->n59C = -1; //P
    p->v534.x = 1500.0f;
    p->v534.y = 2500.0f;
    p->v534.z = 1.0f;
    p->n55C = 100;
    p->n590 = 30;
    p->n584 = 30;
    p->n578 = 30;
    p->n530 = 0;
}
