// Bounding radius from transformed path vertices and Bezier controls.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M4 {qword r[4];};
struct BASIS3 {qword r[3];};
struct P {char padA0[0xA0];struct M4 matrix;int mode;char padE4[4];int count;float *times,*distances;qword *points;char padF8[0x48];qword *incoming,*outgoing;};
extern void UpdateXfmMatWorld(struct P*);
static inline qword ReadPoint(const qword*source){register qword value __asm__("vf1");__asm__("lqc2 %0,%1":"=j"(value):"m"(*source));return value;}
static inline qword DirInPlace(qword m0,qword m1,qword m2,qword a){__asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(a):"j"(m0),"j"(m1),"j"(m2));return a;}
static inline float LenSq(qword a){qword one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one));__asm__("mtc1 %1,%0":"=f"(f):"r"(a));return f;}
static inline void Max(float*out,float value){if(*out<value)*out=value;}
static inline qword Scalar(float f,union U*out){register int bits __asm__("$10");register qword q __asm__("vf4");__asm__("mfc1 %0,%1":"=r"(bits):"f"(f));__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(bits));out->q=q;return out->q;}
static inline float Sqrt(float f){return __builtin_sqrtf(f);}
float func_001E7D90(struct P*p)
{
    register float squared __asm__("$f2");
    UpdateXfmMatWorld(p);squared=0.0f;
    if(p->mode==0){
        register int remaining __asm__("$3")=p->count;
        if(remaining>0){
            register qword r0 __asm__("vf3"),r2 __asm__("vf5"),r1 __asm__("vf4");
            register qword *point __asm__("$4");
            register struct M4 *basis __asm__("$2")=&p->matrix;
            __asm__("lqc2 %0,%4\n\tlqc2 %1,0x20(%8)\n\tlqc2 %2,0x10(%8)\n\tlw %3,%7":"=j"(r0),"=j"(r2),"=j"(r1),"=r"(point):"m"(p->matrix.r[0]),"m"(basis->r[2]),"m"(basis->r[1]),"m"(p->points),"r"(basis));
            do {qword v=DirInPlace(r0,r1,r2,*point);Max(&squared,LenSq(v));point++;}while(--remaining);
        }
    }else{
        register struct M4 *incomingBasis __asm__("$8");
        register qword r0 __asm__("vf3"),r1 __asm__("vf4"),r2 __asm__("vf5");
        register qword firstPoint __asm__("vf1"),previous __asm__("vf6");
        register qword *first __asm__("$5")=p->points;
        register int i __asm__("$4"),limit __asm__("$2");
        __asm__("addiu $2,%6,%9\n\tlqc2 %0,0x10($2)\n\tdaddu %1,$2,$zero\n\tlqc2 %2,0x20($2)\n\tdaddu %3,$zero,$zero\n\tlqc2 %4,%8\n\tlqc2 %5,%7"
          :"=j"(r1),"=&r"(incomingBasis),"=j"(r2),"=&r"(i),"=j"(firstPoint),"=j"(r0)
          :"r"(p),"m"(*(const struct BASIS3*)&p->matrix),"m"(*first),"I"((unsigned)&((struct P*)0)->matrix):"$2");
        __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
          :"=j"(previous):"j"(r0),"j"(r1),"j"(r2),"j"(firstPoint));
        __asm__("lw %0,%3\n\tvmul.xyz $vf1,%2,%2\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf2,$vf1z\n\tqmfc2.ni $3,$vf1\n\taddiu %0,%0,%4\n\tmtc1 $3,%1"
          :"=r"(limit),"=f"(squared):"j"(previous),"m"(p->count),"I"(-1):"$3","vf1","vf2");
        if(limit>0){
            register float positive __asm__("$f4"),negative __asm__("$f3");
            register qword a __asm__("vf10"),b __asm__("vf9"),c __asm__("vf8");
            register qword *outgoing __asm__("$7"),*point __asm__("$6");
            register float *time __asm__("$3");
            register int iterations __asm__("$9");
            positive=0.333333343f;
            __asm__("vmove.xyzw %0,%1":"=j"(a):"j"(r0));
            negative=-0.333333343f;
            __asm__("vmove.xyzw %0,%3\n\tlw %1,%4\n\tvmove.xyzw %2,%5"
              :"=j"(b),"=r"(outgoing),"=j"(c):"j"(r1),"m"(p->outgoing),"j"(r2));
            __asm__("lw %0,%3\n\tdaddu %1,%4,$zero\n\taddiu %2,%5,%6"
              :"=&r"(time),"=&r"(iterations),"=r"(point):"m"(p->times),"r"(limit),"r"(first),"I"(sizeof(qword)));
            do{
                union U scalar;
                register qword old __asm__("vf5"),value __asm__("vf1"),next __asm__("vf7"),k __asm__("vf4");
                register float interval __asm__("$f1"),measured __asm__("$f0");
                register unsigned offset __asm__("$5");
                register qword *incoming __asm__("$2");
                register qword row0 __asm__("vf5"),row1 __asm__("vf3"),row2 __asm__("vf2");
                __asm__("lwc1 $f0,%4\n\tvmove.xyzw %0,%6\n\tlwc1 %1,%5\n\tsll %2,%7,%9\n\tlqc2 %3,%8\n\tsub.s %1,%1,$f0"
                  :"=j"(old),"=f"(interval),"=&r"(offset),"=j"(value)
                  :"m"(time[0]),"m"(time[1]),"j"(previous),"r"(i),"m"(*point),"I"(4):"$f0");
                {register int bits __asm__("$10");
                __asm__("vmulax.xyz ACC,%3,%6x\n\tvmadday.xyz ACC,%4,%6y\n\tvmaddz.xyz %0,%5,%6z\n\tvmove.xyzw %1,%0\n\tmul.s $f0,%7,%8\n\tmfc1 %2,$f0"
                  :"=j"(next),"=j"(previous),"=r"(bits)
                  :"j"(a),"j"(b),"j"(c),"j"(value),"f"(interval),"f"(positive):"$f0");
                __asm__("qmtc2.ni %1,%0":"=j"(k):"r"(bits));scalar.q=k;}
                value=ReadPoint(outgoing);
                __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
                  :"+j"(value):"j"(a),"j"(b),"j"(c));
                __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x":"+j"(value):"j"(old),"j"(k));
                __asm__("vmul.xyz $vf2,%1,%1\n\tvaddw.x $vf3,$vf0,$vf0w\n\tvadday.x ACC,$vf2,$vf2y\n\tvmaddz.x $vf2,$vf3,$vf2z\n\tqmfc2.ni $2,$vf2\n\tmtc1 $2,%0"
                  :"=f"(measured):"j"(value):"$2","vf2","vf3");
                Max(&squared,measured);
                __asm__("mul.s %0,%3,%4\n\tlw %1,%5\n\tlqc2 %2,%6\n\taddu %1,%7,%1"
                  :"=f"(measured),"=&r"(incoming),"=j"(row0)
                  :"f"(interval),"f"(negative),"m"(p->incoming),"m"(p->matrix.r[0]),"r"(offset));
                k=Scalar(measured,&scalar);
                __asm__("lqc2 %0,0x10(%6)\n\tlqc2 %1,0x10(%7)\n\tlqc2 %2,0x20(%6)"
                  :"=j"(row1),"=j"(value),"=j"(row2)
                  :"m"(incomingBasis->r[1]),"m"(incoming[1]),"m"(incomingBasis->r[2]),"r"(incomingBasis),"r"(incoming));
                __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
                  :"+j"(value):"j"(row0),"j"(row1),"j"(row2));
                __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x":"+j"(value):"j"(next),"j"(k));
                __asm__("vmul.xyz $vf2,%1,%1\n\tvaddw.x $vf3,$vf0,$vf0w\n\tvadday.x ACC,$vf2,$vf2y\n\tvmaddz.x $vf2,$vf3,$vf2z\n\tqmfc2.ni $2,$vf2\n\tmtc1 $2,%0"
                  :"=f"(measured):"j"(value):"$2","vf2","vf3");
                Max(&squared,measured);
                __asm__("vmul.xyz $vf1,%1,%1\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf2,$vf1z\n\tqmfc2.ni $10,$vf1\n\tmtc1 $10,%0"
                  :"=f"(measured):"j"(previous):"$10","vf1","vf2");
                Max(&squared,measured);
                ++i;++outgoing;++time;++point;
            }while(i<iterations);
        }
    }
    return Sqrt(squared);
}

#define OFF(T,f) ((unsigned)&((T*)0)->f)
typedef char verified_radius_0[(sizeof(qword)==16)?1:-1];
typedef char verified_radius_1[(__alignof__(qword)==16)?1:-1];
typedef char verified_radius_2[(sizeof(union U)==16)?1:-1];
typedef char verified_radius_3[(sizeof(struct M4)==0x40)?1:-1];
typedef char verified_radius_4[(OFF(struct M4,r)==0)?1:-1];
typedef char verified_radius_5[(OFF(struct P,matrix)==0xA0)?1:-1];
typedef char verified_radius_6[(OFF(struct P,mode)==0xE0)?1:-1];
typedef char verified_radius_7[(OFF(struct P,count)==0xE8)?1:-1];
typedef char verified_radius_8[(OFF(struct P,times)==0xEC)?1:-1];
typedef char verified_radius_9[(OFF(struct P,distances)==0xF0)?1:-1];
typedef char verified_radius_10[(OFF(struct P,points)==0xF4)?1:-1];
typedef char verified_radius_11[(OFF(struct P,incoming)==0x140)?1:-1];
typedef char verified_radius_12[(OFF(struct P,outgoing)==0x144)?1:-1];
typedef char verified_radius_13[(sizeof(void*)==4)?1:-1];
typedef char verified_radius_14[(sizeof(int)==4)?1:-1];
typedef char verified_radius_15[(sizeof(float)==4)?1:-1];
typedef char verified_radius_16[(__alignof__(struct M4)==16)?1:-1];
typedef char verified_radius_17[(__alignof__(union U)==16)?1:-1];
typedef char verified_radius_18[(sizeof(((struct M4*)0)->r)==64)?1:-1];
typedef char verified_radius_19[(sizeof(((struct M4*)0)->r[0])==16)?1:-1];
typedef char verified_radius_20[(sizeof(((struct P*)0)->count)==4)?1:-1];

typedef char radius_basis_prefix_size[(sizeof(struct BASIS3)==48)?1:-1];
typedef char radius_basis_prefix_align[(__alignof__(struct BASIS3)==16)?1:-1];
