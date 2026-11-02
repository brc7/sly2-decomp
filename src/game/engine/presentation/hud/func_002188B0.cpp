// Update reusable sprite entries at their cached animation time.
// func_00215850's float (frame duration) is its THIRD argument in the source: that order of the call's argument moves is what matches.
typedef unsigned char uchar;
typedef unsigned short ushort;
struct RGBA_RAW {uchar r,g,b,a;};
struct RGBA {uchar r,g,b,a;RGBA(){}RGBA(const RGBA &c){*(RGBA_RAW *)this=*(const RGBA_RAW *)&c;}RGBA(const RGBA_RAW &c){*(RGBA_RAW *)this=c;}RGBA &operator=(const RGBA &c){*(RGBA_RAW *)this=*(const RGBA_RAW *)&c;return *this;}} __attribute__((aligned(16)));
struct M23 {float a[6];} __attribute__((aligned(16)));
struct SPRITE;
struct ITEM {RGBA_RAW additive,multiply;char pad8[8];M23 matrix;SPRITE *sprite;int kind;float previousTime;RGBA_RAW previousAdditive,previousMultiply;M23 *previousMatrix;float nextTime;RGBA_RAW nextAdditive,nextMultiply;M23 *nextMatrix;int index;float cachedTime;uchar interpolate;char pad61[7];int flags,pad6C;};
struct REUSE {int count;ITEM *items;ushort index,phase;float time;int field10,field14;};
struct ANIM;
struct GROUP {ushort animCount,reuseCount;ANIM *animations;REUSE *reuse;char padC[0x14];int index;void *owner;int first,pad2C;};
struct ANIM {ushort count,pad2;void *keys;float duration;void *owner;GROUP *group;int id;};
extern "C" {
void ClearSle(int *);
void func_00218790(RGBA *,RGBA *,RGBA *,RGBA *,RGBA *,RGBA *);
void func_00215850(SPRITE *,ITEM *,float,M23 *,RGBA,RGBA);
}
static inline int NLerp(float t,int previous,int next){
 if(previous==next)return next;
 if(previous<next)return previous+(int)(t*((float)next+0.999f-(float)previous));
 return next+(int)((1.0f-t)*((float)previous+0.999f-(float)next));
}
static inline RGBA BlendColors(float t,RGBA previous,RGBA next){
 RGBA result;
 result.r=NLerp(t,previous.r,next.r);result.g=NLerp(t,previous.g,next.g);result.b=NLerp(t,previous.b,next.b);result.a=NLerp(t,previous.a,next.a);
 return result;
}
static inline M23 BlendMatrix(float t,const M23 &previous,const M23 &next){
 M23 result;
 for(int n=0;n<6;n++)result.a[n]=(1.0f-t)*previous.a[n]+t*next.a[n];
 return result;
}
static inline M23 Mul(const M23 &q,const M23 &n){
 float m0=q.a[0],m1=q.a[1],m2=q.a[2],m3=q.a[3],m4=q.a[4],m5=q.a[5];M23 r;
 r.a[0]=m0*n.a[0]+m1*n.a[2];r.a[1]=m0*n.a[1]+m1*n.a[3];r.a[2]=m2*n.a[0]+m3*n.a[2];r.a[3]=m2*n.a[1]+m3*n.a[3];r.a[4]=m4*n.a[0]+m5*n.a[2]+n.a[4];r.a[5]=m4*n.a[1]+m5*n.a[3]+n.a[5];return r;
}
/** @sly2-annot addr=0x2188B0
 * Purpose: update reusable sprite entries at cached time
 * Info: proposed name InterpolateSpriteSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_002188B0(float duration,ANIM *animation,int index,M23 *parent,RGBA additive,RGBA multiply){
 int i=0;
 REUSE *reuse=(REUSE *)(index*sizeof(REUSE)+(int)animation->group->reuse);
 int count=reuse->count;ITEM *items=reuse->items;float time=reuse->time;
 M23 matrix;RGBA outputAdditive,outputMultiply,currentMultiply,currentAdditive;
 for(;i<count;i++){
  ITEM *item=&items[i];
  if(!item->sprite){item->cachedTime=time;ClearSle(&item->flags);continue;}
  if(item->interpolate&&item->nextMatrix){
   float fraction=(time-item->previousTime)/(item->nextTime-item->previousTime);
   matrix=BlendMatrix(fraction,*item->previousMatrix,*item->nextMatrix);
   currentAdditive=BlendColors(fraction,RGBA(item->previousAdditive),RGBA(item->nextAdditive));
   currentMultiply=BlendColors(fraction,RGBA(item->previousMultiply),RGBA(item->nextMultiply));
  }
  else{matrix=*item->previousMatrix;*(RGBA_RAW *)&currentAdditive=item->previousAdditive;*(RGBA_RAW *)&currentMultiply=item->previousMultiply;}
  matrix=Mul(matrix,*parent);
  {
   RGBA inputMultiply(multiply),inputAdditive(additive),localMultiply(currentMultiply),localAdditive(currentAdditive);
   func_00218790(&inputMultiply,&inputAdditive,&localMultiply,&localAdditive,&outputMultiply,&outputAdditive);
  }
  if(item->sprite)func_00215850(item->sprite,item,duration,&matrix,outputAdditive,outputMultiply);
  item->cachedTime=time;item->matrix=matrix;
  item->additive=*(RGBA_RAW *)&outputAdditive;item->multiply=*(RGBA_RAW *)&outputMultiply;
  ClearSle(&item->flags);
 }
}
