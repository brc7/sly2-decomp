typedef int qword __attribute__((mode(TI)));
struct V3 { float x, y, z; };
struct LM { float gMin, gMax; };
struct SHRAPNEL {
    char pad[0x18]; float g18;
    char pad2[0x90 - 0x1C]; qword v;
    char pad3[0xD0 - 0xA0]; float ag[3];
    char pad4[0xF0 - 0xDC]; V3 dir;
};
struct EMITB { char pad[0xF0]; V3 dir; };
extern "C" {
extern LM D_002C1F78;
extern float D_002C1F80[3];
extern LM D_002BBB60;
float func_0011D448(float gMin, float gMax);
void func_001CE9B8(SHRAPNEL *psh, EMITB *pemitb);
}
static inline float GClamp(float g, float lo, float hi)
{
    float r;
    if (g < lo) r = lo; else if (hi < g) r = hi; else r = g;
    return r;
}
static inline qword VLen(qword v)
{
    qword t, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(t), "=&j"(one) : "j"(v));
    return t;
}
static inline float LowerLimit(char*page){return *(const float*)(page-0x44A0);}
static inline float ReadLengthAndPublish(qword length,char**out,LM**limits,float*duration,float life){register char*page __asm__("$2");register LM*address __asm__("$3");register float scalar __asm__("$f3");__asm__("lui %0,%%hi(%5)\n\tqmfc2.ni %1,%4\n\tswc1 %6,%3\n\tmtc1 %1,%2\n\taddiu %1,%0,%%lo(%5)":"=&r"(page),"=&r"(address),"=f"(scalar),"=m"(*duration):"j"(length),"i"(&D_002BBB60),"f"(life));*out=page;*limits=address;return scalar;}
static inline void CopyDirection(SHRAPNEL*dest,const EMITB*source){register float xz __asm__("$f0");register float y __asm__("$f1");__asm__("lwc1 %0,%5\n\tswc1 %0,%2\n\tlwc1 %1,%6\n\tswc1 %1,%3\n\tlwc1 %0,%7\n\tswc1 %0,%4":"=&f"(xz),"=&f"(y),"=m"(dest->dir.x),"=m"(dest->dir.y),"=m"(dest->dir.z):"m"(source->dir.x),"m"(source->dir.y),"m"(source->dir.z));}
/** @sly2-annot addr=0x1CFAB8
 * Purpose: SHRAPNEL (RIPT 8) PostEmit
 * Info: proposed name PostShrapnelEmit__FP8SHRAPNELP5EMITB (EXISTING); TU rip
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void PostShrapnelEmit(SHRAPNEL *psh, EMITB *pemitb)
{
    float len, g;
    int i;
    func_001CE9B8(psh, pemitb);
    float gain=func_0011D448(D_002C1F78.gMin,D_002C1F78.gMax);
    float life=psh->g18*gain;
    qword lengthValue=VLen(psh->v);
    char*page;LM*limits;
    len=ReadLengthAndPublish(lengthValue,&page,&limits,&psh->g18,life);
    CopyDirection(psh,pemitb);
    g = GClamp(D_002C1F80[0] + len * (D_002C1F80[1] + len * D_002C1F80[2]), LowerLimit(page), limits->gMax) * 10.0f;
    for (i = 0; i < 3; i++)
        psh->ag[i] = func_0011D448(-g, g);
}

#define OFF(t,m) ((unsigned)&((t*)0)->m)
typedef char post_layout[(sizeof(qword)==16 && sizeof(float)==4 && sizeof(void*)==4 && sizeof(V3)==12 && sizeof(LM)==8 && OFF(LM,gMin)==0 && OFF(LM,gMax)==4 && OFF(SHRAPNEL,g18)==0x18 && OFF(SHRAPNEL,v)==0x90 && OFF(SHRAPNEL,ag)==0xD0 && OFF(SHRAPNEL,dir)==0xF0 && OFF(EMITB,dir)==0xF0 && sizeof(((SHRAPNEL*)0)->ag)==12 && sizeof(D_002C1F80)==12)?1:-1];
