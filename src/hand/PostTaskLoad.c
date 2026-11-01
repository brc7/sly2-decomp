typedef struct CRef { int m_tagk; int m_tag; } CRef;
struct E16 {int oidA,oidB,flags,level;};
struct E24 {int state,oid,index,id,enabled,level;};
struct SL {void*first;void*last;int nextOffset;};
struct BF { unsigned long pad:52; long k:4; unsigned long pad2:8; };
struct S {
    char pad[0x28]; struct BF bf;
    char pad0[0x40 - 0x30]; int fl;
    char pad1[0x64 - 0x44]; int f64; int f68;
    char pad2[0x78 - 0x6C]; float f78; struct S *f7c; struct SL children;
    char pad3[0x90 - 0x8C]; int *a90; int c94; void **a98; int *a9C; int cA0; void **aA4; int fa8;struct E16*fac;int fb0;struct E24*fb4;
    char pad4[0xC8 - 0xB8]; int fc8; int pad5; int fd0;
};
extern char D_003D4A60[];
extern char D_003DD490[];
void PostLoLoad(struct S *p);
void func_0011A070(struct SL *psl, struct S *p);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, void *owner);
int FIsCurrentTask(struct S *p);
int FTaskLive(struct S *p);
void ApplyStateLoVisibility(struct S *p, int n, int c, struct E16 *a);
void TriggerStateRsmgs(struct S *p, int n, int c, struct E24 *a, int b);
CRef *func_002581C0(CRef *pref, void *a, void *b);
void func_00129EC8(CRef *pref);
void SetLoBusyk(struct S *p, int n);
void PostTaskLoad(struct S *p)
{
    CRef ref;
    int i;
    register int j asm("$16"); /* allocation: i/j tie in global alloc */
    int *pnC8;
    int *pnD0;
    PostLoLoad(p);
    if (p->f7c)
        func_0011A070(&p->f7c->children, p);
    i = 0;
    j = 0;
    pnC8 = &p->fc8;
    pnD0 = &p->fd0;
    if (p->c94 > 0) {
        do {
            void *pv = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 0x105, p->a90[i], 0);
            if (pv)
                p->a98[j++] = pv;
        } while (++i < p->c94);
    }
    p->c94 = j;
    j = 0;
    for (i = 0; i < p->cA0; i++) {
        void *pv = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 0x105, p->a9C[i], 0);
        if (pv)
            p->aA4[j++] = pv;
    }
    p->cA0 = j;
    if (!FIsCurrentTask(p) && FTaskLive(p)) {
        ApplyStateLoVisibility(p, 5, p->fa8, p->fac);
        TriggerStateRsmgs(p, 5, p->fb0, p->fb4, 1);
    }
    if (p->fc8) {
        int n;
        func_002581C0(&ref, pnC8, D_003D4A60);
        n = ref.m_tag;
        func_00129EC8(&ref);
        p->f64 = n;
    }
    if (p->fd0) {
        int n;
        func_002581C0(&ref, pnD0, D_003D4A60);
        n = ref.m_tag;
        func_00129EC8(&ref);
        p->f68 = n;
    }
    if (p->f78 != 0.0f || p->children.first) {
        int v;
        if (p->bf.k == 2)
            SetLoBusyk(p, 0);
        v = p->fl;
        if ((v ^ 1) & 1)
            *(int *)v |= 2;
        else
            p->fl = v | 2;
    }
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_ulong[(sizeof(unsigned long)==8)?1:-1];
typedef char layout_CRef[(sizeof(CRef)==8)?1:-1];
typedef char layout_BF[(sizeof(struct BF)==8)?1:-1];
typedef char layout_SL[(sizeof(struct SL)==12)?1:-1];
typedef char layout_E16[(sizeof(struct E16)==16)?1:-1];
typedef char layout_E24[(sizeof(struct E24)==24)?1:-1];
typedef char layout_S_prefix[(sizeof(struct S)==0xD8)?1:-1];
typedef char layout_S_bf[(((unsigned)&((struct S*)0)->bf==0x28))?1:-1];
typedef char layout_S_fl[(((unsigned)&((struct S*)0)->fl==0x40))?1:-1];
typedef char layout_S_f64[(((unsigned)&((struct S*)0)->f64==0x64))?1:-1];
typedef char layout_S_f68[(((unsigned)&((struct S*)0)->f68==0x68))?1:-1];
typedef char layout_S_f78[(((unsigned)&((struct S*)0)->f78==0x78))?1:-1];
typedef char layout_S_f7c[(((unsigned)&((struct S*)0)->f7c==0x7c))?1:-1];
typedef char layout_S_children[(((unsigned)&((struct S*)0)->children==0x80))?1:-1];
typedef char layout_S_a90[(((unsigned)&((struct S*)0)->a90==0x90))?1:-1];
typedef char layout_S_c94[(((unsigned)&((struct S*)0)->c94==0x94))?1:-1];
typedef char layout_S_a98[(((unsigned)&((struct S*)0)->a98==0x98))?1:-1];
typedef char layout_S_a9C[(((unsigned)&((struct S*)0)->a9C==0x9c))?1:-1];
typedef char layout_S_cA0[(((unsigned)&((struct S*)0)->cA0==0xa0))?1:-1];
typedef char layout_S_aA4[(((unsigned)&((struct S*)0)->aA4==0xa4))?1:-1];
typedef char layout_S_fa8[(((unsigned)&((struct S*)0)->fa8==0xa8))?1:-1];
typedef char layout_S_fac[(((unsigned)&((struct S*)0)->fac==0xac))?1:-1];
typedef char layout_S_fb0[(((unsigned)&((struct S*)0)->fb0==0xb0))?1:-1];
typedef char layout_S_fb4[(((unsigned)&((struct S*)0)->fb4==0xb4))?1:-1];
typedef char layout_S_fc8[(((unsigned)&((struct S*)0)->fc8==0xc8))?1:-1];
typedef char layout_S_fd0[(((unsigned)&((struct S*)0)->fd0==0xd0))?1:-1];
typedef char layout_SL_first[(((unsigned)&((struct SL*)0)->first==0))?1:-1];
typedef char layout_SL_last[(((unsigned)&((struct SL*)0)->last==4))?1:-1];
typedef char layout_SL_nextOffset[(((unsigned)&((struct SL*)0)->nextOffset==8))?1:-1];
