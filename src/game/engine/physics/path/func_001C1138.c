struct LO { char pad[0x124]; int x124; char p1[0x150 - 0x128]; int x150; };
struct AL { int cb; int cbAlign;  };
struct S { char pad[0x10]; int c; int *aid; struct LO **aplo; int x1C; int c2; struct LO **aplo2; int x28; int x2C; };
extern char D_003DD490[];
struct LO *PloFindSwObject__FP2SWi3OIDP2LO(void *p, int a, int id, void *scope);
int CploFindSwObjectsByClass(void *p, int a, int b, void *scope, int d, struct LO **ap);
void *func_001AE238(const struct AL *pal, const void *pv);
static __inline__ void*QueryScope(void){register void*value __asm__("$7");__asm__("daddu %0,$zero,$zero":"=r"(value));return value;}
static __inline__ const struct AL*PrepareRequest(struct AL*request,struct S*owner,int count,int alignment){register int requiredAlignment __asm__("$7")=alignment;register const struct AL*result __asm__("$4");register unsigned bytes __asm__("$2");__asm__("sll %1,%5,2\n\tdaddu %0,%4,$zero\n\tsw %1,%2\n\tsw %6,4(%4)":"=&r"(result),"=&r"(bytes),"=m"(request->cb),"=m"(request->cbAlign):"r"(request),"r"(count),"r"(requiredAlignment));owner->c2=count;return result;}
static __inline__ char*WorldPage(void){register char*page __asm__("$19");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(D_003DD490));return page;}
static __inline__ void*WorldAddress(char*page){void*world;__asm__("addiu %0,%1,%%lo(%2)":"=r"(world):"r"(page),"i"(D_003DD490));return world;}
/** @sly2-annot addr=0x1C1138
 * Purpose: Resolve leaf zone ids to PATHZONE objects (index at +0x124), collect all CID 124 path links (index
 *   at +0x150)
 * Info: proposed name BindSwPathzones (T2); TU path
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C1138(struct S *p)
{
    int i,n;register int c __asm__("$6");
    struct LO *ap[0x400];
    struct AL al;
    char*worldPage=WorldPage();
    for (i = 0; i < p->c; i++) {
        struct LO *plo = PloFindSwObject__FP2SWi3OIDP2LO(WorldAddress(worldPage), 5, p->aid[i], 0);
        plo->x124 = i;
        p->aplo[i] = plo;
    }
    n = CploFindSwObjectsByClass(WorldAddress(worldPage), 0x205, 0x7C, QueryScope(), 0x400, ap);
    {register int alignment __asm__("$7")=0x10;
    c = 0x400;
    if (n < 0x400) c = n;
    p->aplo2=(struct LO**)func_001AE238(PrepareRequest(&al,p,c,alignment),ap);}
    {
        int j;
        for (j = 0; j < p->c2; j++) p->aplo2[j]->x150 = j;
    }
    p->x2C = 1;
}

#define OFF(t,m) ((int)&((t*)0)->m)
typedef char actor_layout[(sizeof(void*)==4 && sizeof(int)==4 && sizeof(struct AL)==8 && OFF(struct AL,cb)==0 && OFF(struct AL,cbAlign)==4 && OFF(struct LO,x124)==0x124 && OFF(struct LO,x150)==0x150 && OFF(struct S,c)==0x10 && OFF(struct S,aid)==0x14 && OFF(struct S,aplo)==0x18 && OFF(struct S,c2)==0x20 && OFF(struct S,aplo2)==0x24 && OFF(struct S,x2C)==0x2C)?1:-1];
