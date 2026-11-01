typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int i[4]; };
struct M3 { U r[3]; M3(){} M3(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3 &operator=(const M3 &m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;} };
struct LO { M3 matrix; U position; M3 rotation; char pad[0x30]; LO *parent; int stamp; };
struct VTE { short delta,pad; void (*fn)(void *); };
struct P { char pad[0xC]; VTE *vt; char pad10[0x4C]; LO *object; };
struct G { char pad[0x3D58]; int stamp; };
extern G D_003DD490;
extern "C" M3 InvertMatrix3Cofactor(const M3 *);
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" void *func_002333B4(void *,int,int);
extern "C" void func_001F4E28(P *);
extern "C" void SlotUnsettle(P *,LO *);
extern "C" void UpdateSoPartMatrices(P *);
static inline M3 Mul(const M3 &a,const M3 &b) return r
{
    qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
extern "C" void RotateSoToMat(P *p,const M3 *basis)
{
    LO *object=p->object;
    if(!object)return;
    {
        M3 m;
        if(object->parent) {
            m=object->parent->rotation;
            NormalizeRotateMatrix3__FP7MATRIX3(&m);
        } else {
            func_002333B4(&m,0,sizeof(m));
            m.r[2].f[2]=1.0f;
            m.r[1].f[1]=1.0f;
            m.r[0].f[0]=1.0f;
        }
        M3 transformed=Mul(InvertMatrix3Cofactor(&m),*basis);
        M3 *dest=&p->object->matrix;
        *dest=transformed;
        dest->r[0].i[3]=0;
        dest->r[1].i[3]=0;
        dest->r[2].i[3]=0;
        int stamp=++D_003DD490.stamp;
        p->object->stamp=stamp;
        p->vt[18].fn((char *)p+p->vt[18].delta);
        func_001F4E28(p);
        SlotUnsettle(p,p->object);
        UpdateSoPartMatrices(p);
    }
}
