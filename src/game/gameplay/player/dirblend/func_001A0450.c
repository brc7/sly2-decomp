// Blend directional vectors and update the animation request.
typedef int qword __attribute__((mode(TI)));
typedef union{qword q;float f[4];int i[4];}U;
struct ANIM{char pad[0x40];float rate;};
struct P{char pad[0x190];U acceleration;char pad1A0[0xE90];struct ANIM*anim;int pad1034;int side;};
struct ENTRY{union{float value;int bits;}weight;int active;char pad8[8];U direction;};
void func_001384C0(void*);
static inline qword Dot(qword a,qword b){qword d,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a),"j"(b));return d;}
static inline float FloatX(qword q){int bits;float f;__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}
static inline float Max(float a,float b){float f;__asm__("max.s %0,%1,%2":"=f"(f):"f"(a),"f"(b));return f;}
static inline qword AddScaled(qword v,qword x,qword k){qword q;__asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(q):"j"(v),"j"(x),"j"(k));return q;}

static inline float Float3(qword q){register int bits __asm__("$7");float f;__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}
static inline qword ScalarBits(int bits){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(bits));return q;}

static inline float Float1(qword q){register int bits __asm__("$7");register float f __asm__("$f1");__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}
/** @sly2-annot addr=0x1A0450
 * Purpose: Choose/blend directional clip weights from the movement vector and update the animation request
 * Info: proposed name UpdateDirBlend (T2); TU dirblend
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/animation.md#dirblend-0x1a0300-0x1a0b18,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001A0450(struct P*p,struct ENTRY*axes,void*request){
 int indices[5];
 U total,scalar;
 int i,count,clear,accum;
 register float rate __asm__("$f4");float sum;
 register qword rk __asm__("vf6");
 if(!request)return;
 rate=p->anim->rate;
 if(!axes[6].active||!axes[7].active)p->side=-1;
 else if(p->side==-1){
  if(FloatX(Dot(p->acceleration.q,axes[0].direction.q))<0.0f)p->side=1;
  else p->side=0;
 }
 {register int ratebits __asm__("$7");__asm__("mfc1 %0,%1":"=r"(ratebits):"f"(rate));__asm__("qmtc2.ni %1,%0":"=j"(rk):"r"(ratebits));}
 for(clear=7;clear>=0;clear--)axes[clear].weight.value=0.0f;
 switch(p->side){
 case -1:
  if(!axes[7].active){indices[0]=6;indices[1]=2;indices[2]=4;}
  else{indices[0]=7;indices[1]=3;indices[2]=5;}
  indices[3]=0;indices[4]=1;count=5;
  break;
 case 0:indices[0]=6;indices[1]=2;indices[2]=4;indices[3]=0;count=4;break;
 case 1:indices[0]=7;indices[1]=3;indices[2]=5;indices[3]=1;count=4;break;
 }
 sum=0.0f;
 if(rate>0.0f){
  for(i=1;i<count;i++){
   struct ENTRY*entry=&axes[indices[i]];
   float denominator=rate*Float1(Dot(entry->direction.q,entry->direction.q));
   float along=Float3(Dot(p->acceleration.q,entry->direction.q));
   float w=Max(along/denominator,0.0f);
   entry->weight.value=w;
   sum+=w;
  }
 }
 {struct ENTRY*head=&axes[indices[0]];
 if(sum<1.0f)head->weight.value=1.0f-sum;
 else{
  head->weight.value=0.0f;
  for(i=1;i<count;i++)axes[indices[i]].weight.value/=sum;
 }
 }
 total.i[0]=total.i[1]=total.i[2]=0;
 for(accum=7;accum>=0;accum--){
  qword k=ScalarBits(axes->weight.bits);
  total.q=AddScaled(total.q,axes->direction.q,k);
  scalar.q=k;
  axes++;
 }
 {qword q;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(q):"j"(total.q),"j"(rk));{register void*argument __asm__("$4")=request;scalar.q=rk;p->acceleration.q=q;func_001384C0(argument);}}
}
