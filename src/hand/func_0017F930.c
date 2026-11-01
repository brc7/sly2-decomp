typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE7E8;
extern char D_002BE7F0[];
struct T { char pad[0x18]; float g18; float g1C; char pad2[8]; float g28; float g2C; };
struct E { char pad[0xC]; void *pC; char pad2[4]; void *p14; };
struct M { char pad[0x4E]; short s4E; };
struct S {
    char pad[0x150]; struct T *p150;
    char pad1[0x1C0 - 0x154]; int n1C0;
    char pad2[0x218 - 0x1C4]; float g218;
    char pad3[4]; qword v220;
    int n230; struct E a234[1];
    char pad4[0x2A4 - 0x234 - 0x18]; struct M *p2A4; int pad5; float g2AC;
};
void UpdateSo(struct S *p);
int func_0017F550(struct S *p);
void func_0017FD08(struct S *p, int n);
qword SetVectorCylind(float a, float b, float c);
void SetActPositionGoal(void *p, qword q);
void SetSmaGoal(void *p, int n);
float GSmooth__FfffP3SMPPf(void *, int, float, float, float);
void func_001FB1F8(struct M *p, float g);
void func_001FB060(struct M *p);
void func_001FB018(struct M *p);
struct V3 { float x, y, z; };
static __inline__ qword Make(float x, float y, float z) { qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z)); return q; }
void func_0017F930(struct S *p) {
    int s;
    UpdateSo(p);
    do {
        s = p->n1C0;
        if (s == 2 && func_0017F550(p)) func_0017FD08(p, 1);
    } while (p->n1C0 != s);
    switch (p->n1C0) {
    case 0: { struct V3 *pv = (struct V3 *)&p->v220; pv->z = 0; pv->y = 0; pv->x = 0; break; }
    case 2: { struct V3 *pv = (struct V3 *)&p->v220; pv->z = 0; pv->y = 0; pv->x = 0; break; }
    case 3: { struct V3 *pv = (struct V3 *)&p->v220; pv->z = 0; pv->y = 0; pv->x = 0; break; }
    case 4: { struct V3 *pv = (struct V3 *)&p->v220; pv->z = 0; pv->y = 0; pv->x = 0; break; }
    case 1: {
        struct T *t = p->p150;
        union { qword q; } u; qword v;
        int i;
        p->g218 = t->g18;
        p->v220 = SetVectorCylind(t->g18, t->g1C * D_002BE7E8, 0.0f);
        v = Make(0.0f, 0.0f, p->p150->g28); u.q = v;
        for (i = 0; i < p->n230; i++) {
            struct E *e = &p->a234[i];
            if (e->pC) SetActPositionGoal(e->pC, v);
            if (e->p14) {
                if (0.0f < p->p150->g2C) SetSmaGoal(e->p14, 0x68);
                else SetSmaGoal(e->p14, 0x66);
            }
        }
        break;
    }
    }
    if (p->p2A4) {
        float g = (p->n1C0 == 1) ? p->p150->g1C : 0.0f;
        p->g2AC = GSmooth__FfffP3SMPPf(D_002BE7F0, 0, p->g2AC, g, D_002DDED0.dt);
        func_001FB1F8(p->p2A4, p->g2AC);
        if (0.0f < p->g2AC) {
            if (p->p2A4->s4E > 0) func_001FB060(p->p2A4);
        } else {
            if (p->p2A4->s4E <= 0) func_001FB018(p->p2A4);
        }
    }
}
