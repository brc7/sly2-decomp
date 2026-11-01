typedef int qword __attribute__((mode(TI)));
typedef struct E { union { qword q[12]; struct { void *x0; int pad[2]; void *xC; char pad2[0xAC - 0x10]; struct E *next; } s; } u; } E;
struct VTE { short d; short pad; void (*pfn)(void *, void *, int, void *, void *, E **); };
struct VO { int x0, x4, x8; struct VTE *vt; };
struct O { char pad[0x60]; struct VO *x60; };
struct K { char pad[0x18]; int x18; };
struct L { char pad[0x6C]; E *x6C; };
struct S { char pad[0x3D40]; char pool[1]; };
struct FILTER;
struct FILTER *func_001F7530(void *a, int mask);
E *func_0011CCC0(void *pool);
void func_001A9FE0(struct S *s, E *e, E **pp);
static inline int Index(E *e,struct K *k) { int one=1; int neq=e->u.s.x0!=k; return one-neq; }
/** @sly2-annot addr=0x1AA0B0
 * Purpose: Query the zap responder (PloFindZapResponder, vt slot 0x3C) and append its contacts
 * Info: proposed name AddSoZapXps (T2); TU mark
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
void func_001AA0B0(struct S *s, struct O *o, struct L *l, struct K *k, int a4, void *a5, void *a6, E **pp)
{
    E *res = 0;
    E *e;
    struct VO *vo = o->x60;
    struct VTE *v = &vo->vt[0x1E0 / 8];
    v->pfn((char *)vo + v->d, k, a4, a5, a6, k->x18 ? &res : pp);
    if(res) { e=res; do {
        if (func_001F7530(((E *)((char *)e + (Index(e,k) << 6)))->u.s.xC, k->x18)) {
            E *n = func_0011CCC0(s->pool);
            *n = *e;
            n->u.s.next = l->x6C;
            l->x6C = n;
        }
    }while((e=e->u.s.next)!=0);
    func_001A9FE0(s, res, pp); }
}

typedef char E_Size[sizeof(E)==0xC0?1:-1];
typedef char K_Mask_Offset[(unsigned int)&((struct K*)0)->x18==0x18?1:-1];
typedef char O_Collector_Offset[(unsigned int)&((struct O*)0)->x60==0x60?1:-1];
typedef char L_Contacts_Offset[(unsigned int)&((struct L*)0)->x6C==0x6C?1:-1];
