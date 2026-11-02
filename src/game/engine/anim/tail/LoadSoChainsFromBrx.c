typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct S;
struct N;
struct E;
struct O { struct M3 mat; char pad[0x70-0x30]; qword pos; char pad1[0xA8-0x80]; struct E *entry; };
struct VE { short d; short pad; struct O *(*get)(void *,int); };
struct S { char pad[0xC]; struct VE *vt; };
struct E { struct M3 mat; qword pos; qword world; qword x50; struct N *node; struct O *object; int x68; int x6C; };
struct N { int count; struct E *entries; int x8; float xC; float x10; float x14; float x18; int x1C; qword q20; qword q30; qword q40; };
struct Hook { int kind; struct S *owner; struct O *object; int xC; int x10; int flags; void *vt; struct E *entry; };
struct Alloc { int cb; int align; };
extern char D_002DB338[];
int func_00117D00(void *);
int func_00117BD0(void *);
float func_00117E38(void *);
void func_00117EB0(void *,qword *);
int func_00117D50(void *);
void *func_001AE2A0(struct Alloc *);
struct Hook *func_0012E1F0(int);
void AddXfAct(struct S *,struct O *,struct Hook *);
qword func_0022FC48(struct O *,struct O *,qword);
qword ConvertXfVec(struct O *,struct O *,qword);
static inline void *Alloc(int cb) { struct Alloc req; req.cb=cb; req.align=16; return func_001AE2A0(&req); }
/** @sly2-annot addr=0x21C888
 * Purpose: reads N chains: per chain S8 count, U8, 4 floats, 2 vectors, S16 count of links; per link resolves
 *   the XF via vt slot51, creates a kind-3 hook (vt D_002DB338) and registers it on the XF (0x230018);
 *   computes rest segments
 * Info: proposed name LoadSoChainsFromBrx (T1); TU tail
 * Notes: names_v2 C proposed LoadAloFromBrx__FP3ALOP18CBinaryInputStream (0.438) | level_parser
 *   CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static naming pass; sources:
 *   r8_21BB88+verdicts_tu_r7r8+parse-verified
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#6-so-payload-loadsofrombrx-0x1f31e0-cparse,
 *   docs/reference/functions/animation.md#tail-0x21c820-0x21d278
 * @end-sly2-annot */
void LoadSoChainsFromBrx(struct S *p,void *stream) {
    int count=func_00117D00(stream);
    int i,j;
    struct E *e;
    for(i=0;i<count;i++) {
        struct N *n=(struct N *)Alloc(0x50);
        n->x8=func_00117BD0(stream);
        n->xC=func_00117E38(stream);
        n->x10=func_00117E38(stream);
        n->x14=func_00117E38(stream);
        n->x18=func_00117E38(stream);
        func_00117EB0(stream,&n->q20);
        func_00117EB0(stream,&n->q30);
        n->count=func_00117D00(stream);
        n->entries=(struct E *)Alloc(n->count*sizeof(struct E));
        for(j=0;j<n->count;j++) {
            struct Hook *h;
            e=&n->entries[j];
            e->node=n;
            { struct VE *ve=&p->vt[51]; e->object=ve->get((char *)p+ve->d,func_00117D50(stream)); }
            e->mat=e->object->mat;
            h=func_0012E1F0(0x20);
            h->vt=D_002DB338;
            h->kind=3;
            h->owner=p;
            h->object=e->object;
            h->xC=-1;
            h->entry=e;
            h->flags=16;
            AddXfAct(p,e->object,h);
            e->object->entry=e;
        }
        for(j=0;j<n->count;j++) {
            e=&n->entries[j];
            if(j<n->count-1) e->pos=func_0022FC48(0,e->object,n->entries[j+1].object->pos);
            else if(j>0) e->pos=ConvertXfVec(((struct E *)((char *)e-0x70))->object,e->object,((struct E *)((char *)e-0x70))->pos);
            else e->pos=n->q30;
            e->world=func_0022FC48(e->object,0,e->pos);
        }
    }
}
