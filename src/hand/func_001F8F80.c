typedef unsigned long ulong;
struct H { char pad[0x98]; short h98; };
struct E { char pad[0xA0]; int nA0; char pad2[0xC0 - 0xA4]; int nC0; };
struct O { char pad[0x58]; struct H *p58; struct H *p5C; char pad2[0xE8 - 0x60]; struct E *pe; char pad3[0x148 - 0xEC]; ulong fl; };
struct L { char pad[0x40]; struct L *pNext; char pad2[0x60 - 0x44]; struct O *po; };
struct P { char pad[0x3D1C]; struct L *pl; };
struct A { void *objects[0x301]; int n; };
int func_00230348(int);
void BuildContactGroup(struct P *, struct O *, int *, void **);
void func_001F9240(struct P *, int, int, void **);
/** @sly2-annot addr=0x1F8F80
 * Purpose: For each dynamic root LO in the SW list: build its contact group (BuildContactGroup) and solve it
 *   (0x1F9240)
 * Info: proposed name SolveSwContactGroups (T2); TU solve
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
void func_001F8F80(struct P *p, int k)
{
    struct A a;
    struct L *pl;
    struct O *po;
    for (pl = p->pl; pl; pl = pl->pNext) {
        struct E *pe;
        po = pl->po;
        pe = po->pe;
        if (!pe) continue;
        if (po->fl & 0x8000000) continue;
        if (pe->nC0 == 3 && pe->nA0 == 3) continue;
        {
            register struct O *receiver asm("a1")=po;
            if(k&1){
                int first=func_00230348(po->p58->h98);
                receiver=po;
                if(first){
                    int second=func_00230348(po->p5C->h98);
                    if(second)continue;
                    receiver=po;
                }
            }
            a.n=0;
            BuildContactGroup(p,receiver,&a.n,a.objects);
            func_001F9240(p,k,a.n,a.objects);
        }
    }
    for (pl = p->pl; pl; pl = pl->pNext) {
        po = pl->po;
        po->fl &= ~0x8000000L;
        po->fl &= ~0x10000000L;
    }
}
