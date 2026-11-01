typedef struct VT { struct { short d; short pad; int (*pfn)(void *); } a[64]; } VT;
typedef struct { char pad[0xC]; VT *vt; } VOBJ;
typedef struct { char pad[0x60]; VOBJ *p; } C;
typedef struct { char pad[0xEC]; C *pc; } B;
typedef struct { int pad; B *pb; } A;
typedef struct { char a[8]; char b[0x58]; } ENT;
typedef struct {
    char pad[0x190];
    int c;
    char pad2[0xC];
    ENT ae[8];
} OBJ;
void func_001F5990(void);
void func_001D3450(OBJ *, ENT *, A *, int, int, int, void *);
void AddRobottankCustomXps(OBJ *p, A *pa, int x, int y, int z)
{
    int i;
    VOBJ *po;
    func_001F5990();
    po = pa->pb->pc->p;
    if (po->vt->a[62].pfn((char *)po + po->vt->a[62].d)) {
        for (i = 0; i < p->c; i++)
            func_001D3450(p, &p->ae[i], pa, x, y, z, p->ae[i].b);
    }
}
