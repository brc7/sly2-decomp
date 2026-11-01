typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct VTE { short delta,pad; void (*fn)(void *,qword,float,int); };
struct LO { char pad[0xB0]; qword scale; };
struct S {
    char pad[0x5C]; struct LO *lo;
    char pad60[0xEC-0x60]; struct VTE *vt; void *emit;
    char padF4[0x1A0-0xF4]; float probability; int pad1A4; void *owner;
    int pad1AC; void *left; int pad1B4; void *right; int previous; float last;
};
struct Q { char pad[0x18]; float duration; void *owner; char pad20[0xEC-0x20]; struct VTE *vt; void *emit; };
struct CAMERA { char pad[0x60]; qword a,b; };
struct CLOCK { float t; };
extern struct CAMERA D_002DDF40;
extern struct CLOCK D_002DDED0;
extern qword D_002C0D80;
extern float D_002C0D90;
extern struct G D_003DD490;
qword func_001A6FC0(struct S *,void *,qword);
float GRandInRange__Fff(float,float);
struct Q *func_001CE888(struct G *,void *,void *);
qword func_001A62E8(void *);
static inline float Dot(qword a,qword b)
{
    qword d,one;
    float f;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w":"=j"(d),"=j"(one):"j"(a),"j"(b));
    __asm__("vadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(d):"j"(one));
    __asm__("mtc1 %1,%0":"=f"(f):"r"(d));
    return f;
}
static inline float Max(float a,float b)
{
    __asm__("max.s %0,%0,%1":"+f"(a):"f"(b));
    return a;
}
void func_001C6308(struct S *p)
{
    union U scale;
    qword direction;
    int side;
    void *emitter;
    struct Q *q;
    if(!p->owner||p->probability<=0)return;
    direction=func_001A6FC0(p,0,D_002C0D80);
    side=Dot(direction,D_002DDF40.b)<0;
    if(side!=p->previous) {
        if(GRandInRange__Fff(0,1)<p->probability) {
            if(D_002C0D90<D_002DDED0.t-p->last) {
                if(Dot(direction,D_002DDF40.a)<0)emitter=p->left;
                else emitter=p->right;
                if(!emitter)goto changed;
                q=func_001CE888(&D_003DD490,(void *)4,0);
                if(!q)goto changed;
                p->last=D_002DDED0.t;
                {
                    struct VTE *method=&q->vt[1];
                    void *ctx=(char *)q+method->delta;
                    qword pos=func_001A62E8(emitter);
                    method->fn(ctx,pos,1,0);
                }
                {
                    void *owner=p->owner;
                    q->emit=emitter;
                    q->owner=owner;
                }
                scale.q=p->lo->scale;
                q->duration=1.5f/Max(1,__builtin_fabsf(scale.f[2]));
            }
        }
changed:
        p->previous=side;
    }
}
