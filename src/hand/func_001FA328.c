typedef int qword __attribute__((mode(TI)));
union V {qword q;float f[4];};
struct LO;
struct B {char padA0[0xA0];struct LO *parent;char padA4[0xC];union V velocity;};
struct P {char pad58[0x58];struct B *b58,*b5C;char pad60[0x88];void *data;char padEC[4];union V force0,force1;};
extern qword D_002A2DD0;
int func_0011D4D0(float,float,float);
void SlotUnsettle(struct P *,struct B *);
qword ConvertXfVec(struct LO *,struct LO *,qword);
void func_001F4BF8(struct P *);
static __inline__ float X(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static __inline__ float Y(qword q){float f;__asm__("prot3w %0,%1":"=r"(f):"r"(q));return f;}
static __inline__ float Z(qword q){float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
static __inline__ int Near(qword value,qword reference)
{
    int same=0;
    if(func_0011D4D0(X(reference),X(value),0.0001f) &&
       func_0011D4D0(Y(reference),Y(value),0.0001f) &&
       func_0011D4D0(Z(reference),Z(value),0.0001f))same=1;
    return same;
}
static __inline__ void AddTo(union V *out,qword q)
{
    qword r;
    __asm__("lqc2 %0,%1":"=j"(r):"m"(*out));
    __asm__("vadd.xyzw %0,%0,%1":"+j"(r):"j"(q));
    out->q=r;
}
void func_001FA328(struct P *p)
{
    if(p->data){
        qword value=p->force0.q;
        qword reference=D_002A2DD0;
        if(!Near(value,reference)){
            struct B *b=p->b58;
            union V *force=&p->force0;
            qword increment;
            SlotUnsettle(p,b);
            increment=ConvertXfVec(0,b->parent,p->force0.q);
            force->f[2]=0;force->f[1]=0;force->f[0]=0;
            AddTo(&b->velocity,increment);
            func_001F4BF8(p);
        }
        value=p->force1.q;
        reference=D_002A2DD0;
        if(!Near(value,reference)){
            struct B *b=p->b5C;
            union V *force=&p->force1;
            qword increment;
            SlotUnsettle(p,b);
            increment=ConvertXfVec(0,b->parent,p->force1.q);
            force->f[2]=0;force->f[1]=0;force->f[0]=0;
            AddTo(&b->velocity,increment);
            func_001F4BF8(p);
        }
    }
}
