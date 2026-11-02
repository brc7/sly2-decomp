typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];int i[4];};
struct M3 {U r[3];M3(){} M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3&operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;}};
struct OWNER {M3 matrix;U position;char pad40[0x60];OWNER *parent;int stamp;char padA8[8];U velocity;};
struct VT {short delta,pad;void (*fn)(void *);};
struct P {char padC[0xC];VT *vt;char pad10[0x48];OWNER *position,*rotation;char pad60[0x88];void *enabled;char padEC[4];U force[2];};
struct G {char pad3D58[0x3D58];int stamp;};
struct CLOCK {float time,dt;};
extern CLOCK D_002DDED0;
extern G D_003DD490;
extern "C" void SlotUnsettle(P *,OWNER *);
extern "C" qword ConvertXfVec(void *,OWNER *,qword);
extern "C" int func_0011D4D0(float,float,float);
extern qword D_002A2DD0;
extern "C" void func_001F4BF8(P *);
extern "C" qword *func_00256D18(qword *,qword);
extern "C" M3 *func_0011B570(M3 *,qword);
static inline M3 BuildRotation(qword vector) return matrix {func_0011B570(&matrix,vector);}
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
static inline qword Scalar(float f,U *out){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));out->q=q;return out->q;}
static inline qword AddScaled(qword p,qword v,qword k){__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x":"+j"(v):"j"(p),"j"(k));return v;}
static inline qword Scale(qword v,qword k){__asm__("vmulx.xyzw %0,%0,%1x":"+j"(v):"j"(k));return v;}
static inline M3 Mul(const M3 &a,const M3 &b) return r
{
    qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
static inline float X(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Y(qword q){register float f __asm__("$2");__asm__("prot3w %0,%1":"=r"(f):"r"(q));return f;}
static inline float Z(qword q){register float f __asm__("$2");__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
static inline int NearFlag(int code){register int r __asm__("$21");__asm__("sltu %0,$zero,%1":"=r"(r):"r"(code));return r;}
static inline void Near(int *out,qword a,qword b){if(func_0011D4D0(X(a),X(b),0.0001f))if(func_0011D4D0(Y(a),Y(b),0.0001f))*out=NearFlag(func_0011D4D0(Z(a),Z(b),0.0001f));}
static inline qword Zero(){register qword r __asm__("vf3");__asm__("vsub.xyzw %0,$vf0,$vf0":"=j"(r));return r;}
static inline void Consume(P *p,OWNER *object,U &pending,qword*out){SlotUnsettle(p,object);qword world=ConvertXfVec(0,object->parent,pending.q);__asm__("sw $zero,%0\n\tsq %3,%1\n\tsw $zero,%2":"=m"(pending.i[1]),"=m"(*out),"=m"(pending.i[2]):"r"(world));}
static inline float YFromHome(const qword &q){register qword bits __asm__("$2");register float r __asm__("$f13");__asm__("lq %0,%2\n\tprot3w %0,%0\n\tmtc1 %0,%1":"=r"(bits),"=f"(r):"m"(q));return r;}
static inline float ZFromHome(const qword &q){register qword bits __asm__("$2");register float r __asm__("$f13");__asm__("lq %0,%2\n\tpextuw %0,$zero,%0\n\tmtc1 %0,%1":"=r"(bits),"=f"(r):"m"(q));return r;}
static inline void NearVector(int *out,const qword &first,const qword &b){register const qword *address __asm__("$2")=&first;register qword a __asm__("$16");__asm__("lq %0,0(%1)":"=r"(a):"r"(address),"m"(first));register qword bits __asm__("$2");register float ax __asm__("$f12"),bx __asm__("$f13");__asm__("lq %0,%4\n\tmtc1 %3,%1\n\tmtc1 %0,%2":"=&r"(bits),"=f"(ax),"=f"(bx):"r"(a),"m"(b));if(func_0011D4D0(ax,bx,0.0001f))if(func_0011D4D0(Y(a),YFromHome(b),0.0001f))*out=func_0011D4D0(Z(a),ZFromHome(b),0.0001f)!=0;}
static inline void AddVector(U *out,const qword &q){qword r,v;__asm__("lqc2 %0,%2\n\tlqc2 %1,%3":"=j"(r),"=j"(v):"m"(*out),"m"(q));__asm__("vadd.xyzw %0,%0,%1":"+j"(r):"j"(v));out->q=r;}
static inline qword*PointAddress(OWNER*p,qword*value){register qword*address __asm__("$4");register qword point __asm__("vf2");__asm__("addiu %0,%2,0x30\n\tlqc2 %1,%3":"=&r"(address),"=j"(point):"r"(p),"m"(p->position));*value=point;return address;}
static inline float HalfAndDelta(float dt,float half,const qword&source,qword*delta){register float product __asm__("$f0");register qword d __asm__("vf3");__asm__("mul.s %0,%2,%3\n\tlqc2 %1,%4":"=f"(product),"=j"(d):"f"(dt),"f"(half),"m"(source));*delta=d;return product;}
/** @sly2-annot addr=0x1F9E70
 * Purpose: Applies two queued force/torque vectors to an SO transform (rotation build 0x11B570, normalize);
 *   no direct callers (vtable/callback)
 * Info: proposed name ApplySoForcesXf (T2); class SO slot65; TU solve
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
extern "C" void func_001F9E70(P *input)
{
    register P *p __asm__("$18");
    __asm__("daddu %0,%1,$zero":"=r"(p):"r"(input));
    U scalar;
    M3 frame;
    qword shift,turn;
    register void*enabled __asm__("$2");
    __asm__("lw %0,0xE8(%1)":"=r"(enabled):"r"(p),"m"(p->enabled));
    if(enabled){
        qword value;__asm__("lq %0,0xF0(%1)":"=r"(value):"r"(p),"m"(p->force[0].q));
        qword reference=D_002A2DD0;
        int changed=0;
        OWNER *position=p->position,*rotation=p->rotation;
        int near=0;
        Near(&near,reference,value);
        if(!near){
            Consume(p,position,p->force[0],&shift);
            changed=1;
            p->force[0].i[0]=0;
            AddVector(&position->velocity,shift);
        }else shift=Zero();
        value=p->force[1].q;reference=D_002A2DD0;
        near=0;Near(&near,reference,value);
        if(!near){
            Consume(p,rotation,p->force[1],&turn);
            changed=1;
            p->force[1].i[0]=0;
            AddVector(&rotation->velocity,turn);
        }else turn=Zero();
        if(changed){
            func_001F4BF8(p);
            int incrementNear=0;NearVector(&incrementNear,D_002A2DD0,shift);
            if(!incrementNear){
                qword point;qword *positionAddress=PointAddress(position,&point);
                qword delta;float halfDt=HalfAndDelta(D_002DDED0.dt,0.5f,shift,&delta);
                qword k=Scalar(halfDt,&scalar);
                func_00256D18(positionAddress,AddScaled(point,delta,k));
                position->position.f[3]=1.0f;
                position->stamp=++D_003DD490.stamp;
            }
            incrementNear=0;NearVector(&incrementNear,D_002A2DD0,turn);
            if(!incrementNear){
                frame=Mul(BuildRotation(Scale(turn,Scalar(D_002DDED0.dt*0.5f,&scalar))),rotation->matrix);
                NormalizeRotateMatrix3__FP7MATRIX3(&frame);
                rotation->matrix=frame;
                rotation->matrix.r[1].i[3]=0;
                rotation->matrix.r[2].i[3]=0;
                rotation->matrix.r[0].i[3]=0;
                rotation->stamp=++D_003DD490.stamp;
            }
            p->vt[18].fn((char *)p+p->vt[18].delta);
        }
    }
}

typedef char layout_pointer_Width[(sizeof(void*)==4)?1:-1];
typedef char layout_integer_Width[(sizeof(int)==4)?1:-1];
typedef char layout_float_Width[(sizeof(float)==4)?1:-1];
typedef char layout_short_Width[(sizeof(short)==2)?1:-1];
typedef char layout_qword_Size[(sizeof(qword)==16)?1:-1];
typedef char layout_qword_Alignment[(__alignof__(qword)==16)?1:-1];
typedef char layout_U_Size[(sizeof(U)==16)?1:-1];
typedef char layout_U_Alignment[(__alignof__(U)==16)?1:-1];
typedef char layout_U_q_Offset[((unsigned)&((U*)0)->q==0)?1:-1];
typedef char layout_U_f_Offset[((unsigned)&((U*)0)->f==0)?1:-1];
typedef char layout_U_i_Offset[((unsigned)&((U*)0)->i==0)?1:-1];
typedef char layout_U_f_Span[(sizeof(((U*)0)->f)==16)?1:-1];
typedef char layout_U_i_Span[(sizeof(((U*)0)->i)==16)?1:-1];
typedef char layout_M3_Size[(sizeof(M3)==0x30)?1:-1];
typedef char layout_M3_Alignment[(__alignof__(M3)==16)?1:-1];
typedef char layout_M3_rows_Offset[((unsigned)&((M3*)0)->r==0)?1:-1];
typedef char layout_M3_rows_Span[(sizeof(((M3*)0)->r)==0x30)?1:-1];
typedef char layout_M3_row_Stride[(sizeof(((M3*)0)->r[0])==0x10)?1:-1];
typedef char layout_OWNER_PrefixSize[(sizeof(OWNER)==0xC0)?1:-1];
typedef char layout_OWNER_Alignment[(__alignof__(OWNER)==16)?1:-1];
typedef char layout_OWNER_matrix_Offset[((unsigned)&((OWNER*)0)->matrix==0)?1:-1];
typedef char layout_OWNER_position_Offset[((unsigned)&((OWNER*)0)->position==0x30)?1:-1];
typedef char layout_OWNER_parent_Offset[((unsigned)&((OWNER*)0)->parent==0xA0)?1:-1];
typedef char layout_OWNER_stamp_Offset[((unsigned)&((OWNER*)0)->stamp==0xA4)?1:-1];
typedef char layout_OWNER_velocity_Offset[((unsigned)&((OWNER*)0)->velocity==0xB0)?1:-1];
typedef char layout_VT_Size[(sizeof(VT)==8)?1:-1];
typedef char layout_VT_Alignment[(__alignof__(VT)==4)?1:-1];
typedef char layout_VT_delta_Offset[((unsigned)&((VT*)0)->delta==0)?1:-1];
typedef char layout_VT_delta_Size[(sizeof(((VT*)0)->delta)==2)?1:-1];
typedef char layout_VT_fn_Offset[((unsigned)&((VT*)0)->fn==4)?1:-1];
typedef char layout_P_PrefixSize[(sizeof(P)==0x110)?1:-1];
typedef char layout_P_Alignment[(__alignof__(P)==16)?1:-1];
typedef char layout_P_vt_Offset[((unsigned)&((P*)0)->vt==0xC)?1:-1];
typedef char layout_P_position_Offset[((unsigned)&((P*)0)->position==0x58)?1:-1];
typedef char layout_P_rotation_Offset[((unsigned)&((P*)0)->rotation==0x5C)?1:-1];
typedef char layout_P_enabled_Offset[((unsigned)&((P*)0)->enabled==0xE8)?1:-1];
typedef char layout_P_force_Offset[((unsigned)&((P*)0)->force==0xF0)?1:-1];
typedef char layout_P_second_force_Offset[((unsigned)&((P*)0)->force[1]==0x100)?1:-1];
typedef char layout_P_force_Span[(sizeof(((P*)0)->force)==0x20)?1:-1];
typedef char layout_G_PrefixSize[(sizeof(G)==0x3D5C)?1:-1];
typedef char layout_G_Alignment[(__alignof__(G)==4)?1:-1];
typedef char layout_G_stamp_Offset[((unsigned)&((G*)0)->stamp==0x3D58)?1:-1];
typedef char layout_CLOCK_Size[(sizeof(CLOCK)==8)?1:-1];
typedef char layout_CLOCK_Alignment[(__alignof__(CLOCK)==4)?1:-1];
typedef char layout_CLOCK_time_Offset[((unsigned)&((CLOCK*)0)->time==0)?1:-1];
typedef char layout_CLOCK_dt_Offset[((unsigned)&((CLOCK*)0)->dt==4)?1:-1];
