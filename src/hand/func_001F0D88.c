struct ANIMATION;
struct T { char pad[0x74]; float f74; };
struct A { struct T *pt; int n4; int n8; };
struct E8 { int n0; int n4; };
struct G { char pad[0x54]; struct E8 *a54; };
struct P { char pad[0x24]; struct G *pg; int n28; int n2C; struct ANIMATION *n30; int n34; int n38; int n3C; struct A *p40; float f44; int n48; };
int func_001F0638(struct P *);
struct A *func_001F0CF0(struct G *, int, int, int);
struct A *func_001F0B80(struct G *, int, int, int, int);
void func_0012F9B8(struct T *, int, float, float, int, struct ANIMATION **);
void func_001333C0(struct ANIMATION *, struct P *);
void func_00130B18(struct ANIMATION *);
void func_001F13A8(struct P *, int, int);
static inline struct P *Receiver(struct P *p){register struct P *r __asm__("$4");__asm__("daddu %0,%1,$zero":"=r"(r):"r"(p));return r;}
static inline unsigned EntryOffset(int index){register unsigned bytes __asm__("$3");__asm__("sll %0,%1,3":"=r"(bytes):"r"(index));return bytes;}
/** @sly2-annot addr=0x1F0D88
 * Purpose: Main driver: if no goal or at goal, random wander; else toward goal. Starts the transition's ASEG
 *   via ApplyAseg (0x12F9B8), sets ismsNext, notifies
 * Info: proposed name ChooseSmaTransition (T1); TU sm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void func_001F0D88(struct P *p, int k)
{
    struct A *pa;
    int nx;
    register int fl __asm__("$17");
    if (func_001F0638(p)) return;
    {int finished=-1;
    int random=-2;
    goto test;
select:
    if(nx==random){
        pa=func_001F0CF0(p->pg,p->n48,p->n34,0);
        if(pa)p->n3C=(pa->n4!=p->n34)?pa->n4:pa->n8;
    }else{
        p->n3C=nx;
        pa=func_001F0B80(p->pg,p->n48,p->n34,nx,1);
    }
    k=0;
apply:
        p->p40 = pa;
        if (!pa) return;
        fl = 0;
        if (pa->pt) {
            float a, b;
            int f;
            if (pa->n4 == p->n34) {
                p->n38 = pa->n8;
                a = 0.0f;
                b = p->f44;
            } else {
                p->n38 = pa->n4;
                b = -p->f44;
                a = pa->pt->f74;
            }
            f = p->n2C;
            if (k == 1) {
                f |= 2;
                f &= random;
                fl = 1;
                a = pa->pt->f74 - a;
            } else if (pa->pt->f74 == 0.0f) {
                fl = 1;
            }
            func_0012F9B8(pa->pt, p->n28, a, b, f, &p->n30);
        }
        {
            int oldArgument,newArgument;
            struct P *pArgument;
            if(!p->n30){
                int old=p->n34;
                int nw=pa->n4;
                if(nw==old)nw=pa->n8;
                p->n34=nw;
                oldArgument=old;
                newArgument=nw;
                pArgument=Receiver(p);
            }else{
                int old;
                if(!fl){func_001333C0(p->n30,p);return;}
                fl=p->n38;
                old=p->n34;
                p->n34=fl;
                p->n38=finished;
                func_00130B18(p->n30);
                p->n30=0;
                oldArgument=old;
                newArgument=fl;
                pArgument=Receiver(p);
            }
            func_001F13A8(pArgument,oldArgument,newArgument);
        }
    if(func_001F0638(p))return;
    if(p->n30)return;
test:
    if(p->n34!=p->n3C){
        pa=func_001F0B80(p->pg,p->n48,p->n34,p->n3C,1);
        goto apply;
    }
    nx=((struct E8*)((unsigned)p->pg->a54+EntryOffset(p->n34)))->n4;
    if(nx!=finished)goto select;
}
}

#define FIELD_OFFSET(t,m) ((unsigned)&(((struct t*)0x10000)->m)-0x10000)
typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char T_Prefix_Size[sizeof(struct T)==0x78?1:-1];
typedef char A_Prefix_Size[sizeof(struct A)==0xC?1:-1];
typedef char E8_Size[sizeof(struct E8)==8?1:-1];
typedef char G_Prefix_Size[sizeof(struct G)==0x58?1:-1];
typedef char P_Prefix_Size[sizeof(struct P)==0x4C?1:-1];
typedef char T_f74_Offset[FIELD_OFFSET(T,f74)==0x74?1:-1];
typedef char A_pt_Offset[FIELD_OFFSET(A,pt)==0?1:-1];
typedef char A_n4_Offset[FIELD_OFFSET(A,n4)==4?1:-1];
typedef char A_n8_Offset[FIELD_OFFSET(A,n8)==8?1:-1];
typedef char E8_n0_Offset[FIELD_OFFSET(E8,n0)==0?1:-1];
typedef char E8_n4_Offset[FIELD_OFFSET(E8,n4)==4?1:-1];
typedef char G_a54_Offset[FIELD_OFFSET(G,a54)==0x54?1:-1];
typedef char P_pg_Offset[FIELD_OFFSET(P,pg)==0x24?1:-1];
typedef char P_n28_Offset[FIELD_OFFSET(P,n28)==0x28?1:-1];
typedef char P_n2C_Offset[FIELD_OFFSET(P,n2C)==0x2C?1:-1];
typedef char P_n30_Offset[FIELD_OFFSET(P,n30)==0x30?1:-1];
typedef char P_n34_Offset[FIELD_OFFSET(P,n34)==0x34?1:-1];
typedef char P_n38_Offset[FIELD_OFFSET(P,n38)==0x38?1:-1];
typedef char P_n3C_Offset[FIELD_OFFSET(P,n3C)==0x3C?1:-1];
typedef char P_p40_Offset[FIELD_OFFSET(P,p40)==0x40?1:-1];
typedef char P_f44_Offset[FIELD_OFFSET(P,f44)==0x44?1:-1];
typedef char P_n48_Offset[FIELD_OFFSET(P,n48)==0x48?1:-1];
