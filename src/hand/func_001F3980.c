typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct E { char pad[0x10]; ulong fl; char pad2[0xB0 - 0x18]; };
struct Q { char pad[0x98]; short h98; char pad2[0xB0 - 0x9A]; qword vB0; char padC0[0xF0 - 0xC0]; int aF0[3]; };
struct R { char pad[0x60]; qword v60; qword v70; };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0x58 - 0x10]; struct Q *p58; struct Q *p5C; char pad3[0xE0 - 0x60]; int nE0; struct E *aE4; struct R *pE8; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void UpdateSoPartMatrices(struct P *);
void func_001F6EF8(struct E *, float);
int func_00230380(int);
void UpdateSoSlots(struct P *);
void UpdateSoPartGeom(struct E *);
static inline void Clear(int *a)
{
    a[2] = 0;
    a[1] = 0;
    a[0] = 0;
}
void func_001F3980(struct P *p)
{
    int i;
    UpdateSoPartMatrices(p);
    for (i = 0; i < p->nE0; i++) {
        struct E *pe = &p->aE4[i];
        if (*(int *)&pe->fl) func_001F6EF8(pe, D_002DDED0.dt);
    }
    if (p->pE8) {
        if (p->p58) p->pE8->v60 = p->p58->vB0;
        if (p->p5C) p->pE8->v70 = p->p5C->vB0;
        if ((unsigned short)(p->p58->h98 - 1) < 2 || p->p5C->h98 == 1 || p->p5C->h98 == 2) {
            if (func_00230380(p->p58->h98)) {
                Clear(p->p58->aF0);
            }
            if (func_00230380(p->p5C->h98)) {
                Clear(p->p5C->aF0);
            }
            p->pvt[70].pfn((char *)p + p->pvt[70].d);
        }
    }
    UpdateSoSlots(p);
    for (i = 0; i < p->nE0; i++) {
        struct E *pe = &p->aE4[i];
        if (!(pe->fl & (1L << 50))) UpdateSoPartGeom(pe);
    }
}
