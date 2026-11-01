typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct LO { char pad[0xC]; VT *vt; } LO;
typedef struct L { int x0, x4; int c; LO **ap; } L;
typedef struct X { char pad[0x10]; void *x10; char pad2[0x3C]; void *x50; char pad3[0xC]; char m60[0x40]; } X;
void func_0022D670(void *);
void func_0022D698(void *);
void LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(void *, void *, L *);

void LoadXfmChildObjectsFromBrx(X *p, void *pv, L *pl)
{
    int i;
    func_0022D670(p->x50);
    LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(p->x10, pv, pl);
    func_0022D698(p->x50);
    for (i = 0; i < pl->c; i++) {
        LO *lo = pl->ap[i];
        ((void (*)(void *, void *))lo->vt[37].pfn)((char *)lo + lo->vt[37].d, p->m60);
    }
}
