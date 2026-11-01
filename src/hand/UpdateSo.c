typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[10]; };
struct E { char pad[0xDC]; void *pDC; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xE8 - 0x10]; struct E *pe; char pad3[0x12C - 0xEC]; float g12C; float g130; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void UpdateLo(struct P *);
qword func_001A62E8(struct P *);
void *func_001C0D70(qword, int);
void func_001F37C8(struct P *, void *);
void UpdateSo(struct P *p)
{
    void *q;
    float d;
    UpdateLo(p);
    q = p->pe ? p->pe->pDC : 0;
    if (q) func_001F37C8(p, func_001C0D70(func_001A62E8(p), 1));
    d = p->g130;
    if (d != 0.0f) {
        float v = p->g12C + D_002DDED0.dt * d;
        p->g12C = v;
        if (v <= 0.0f) {
            p->g12C = 0.0f;
            p->g130 = 0.0f;
            p->pvt->a[9].pfn((char *)p + p->pvt->a[9].d);
        } else if (1.0f <= v) {
            p->g12C = 1.0f;
            p->g130 = 0.0f;
        }
    }
}
