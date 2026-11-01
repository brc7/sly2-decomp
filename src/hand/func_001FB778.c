typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];int i[4];} V;
struct RANGE {float low,high;};
struct M3 {qword r[3];};
struct CAMERA {char pad[0x90];qword position;};
extern char D_003DD490[],D_002BBB60[];
extern struct CAMERA D_002DDF40;
extern struct M3 D_002DDFA0;
void *func_001C4A48(void*);
qword func_001A62E8(void*);
qword func_001A7348(const struct M3*,const struct M3*,qword);
float func_0011D948(void*,float);
float func_002458F0(float,float);
static inline qword Sub(qword a,qword b){register qword r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=&j"(r):"j"(a),"j"(b));return r;}
static inline float Length(qword a,V*save){qword d,one;float g;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));
save->q=a;
__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(d));return g;}
static inline float X(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Y(qword q){int bits;float f;__asm__("prot3w %0,%1":"=r"(bits):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}
static inline qword Bits(qword v){qword q;__asm__("qmfc2.ni %0,%1":"=r"(q):"j"(v));return q;}
static inline void Rounded(int*out,float f){float bits;__asm__("cvt.w.s %0,%1":"=&f"(bits):"f"(f));__asm__("swc1 %1,%0":"=m"(*out):"f"(bits));}
/** @sly2-annot addr=0x1FB778
 * Purpose: Distance from ear (player PosLoWorld or camera) -> volume via (sStop-d)/(sStop-sFull) clamped;
 *   atan2 -> pan degrees
 * Info: proposed name CalculateSndVolPan (T1); TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FB778(void *unused,qword point,const struct RANGE*limits,float*outValue,int*outAngle)
{
 V source,delta;
 void *player;
 register qword reference __asm__("vf1");
 float high,low,distance,value;
 register qword difference __asm__("vf3");
 source.q=point;
 player=func_001C4A48(D_003DD490);
 if(player)reference=func_001A62E8(player);
 else reference=D_002DDF40.position;
 high=limits->high;low=limits->low;
 {register qword input __asm__("vf2")=source.q;
  distance=Length(Sub(input,reference),&delta);}
 value=func_0011D948(D_002BBB60,(high-distance)/(high-low));
 difference=delta.q;
 if(outValue)*outValue=value;
 if(outAngle){
   if(value>0.0f){
     qword direction;
     float angle;
     direction=func_001A7348(0,&D_002DDFA0,Bits(difference));
     angle=func_002458F0(-Y(direction),X(direction));
     if(limits->high<limits->low)angle+=3.1415927f;
     Rounded(outAngle,angle*57.295776f);return;
   }else *outAngle=0;
 }
}
