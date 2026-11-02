typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} V;
struct O;
struct VT {short delta,pad;void(*fn)(void*,qword*);};
struct O {char pad[0xC];struct VT *vt;char pad10[0x28];float height;char pad3C[0xA4];float radius;char padE4[0x24];int state;};
struct REF {struct O*o;};
struct P {char pad[0x20];V position;int count;struct O *items[20];char pad84[8];struct O*best;char pad90[0x18];struct REF *ref;};
struct CAMERA {char pad[0x90];qword position;};
extern struct CAMERA D_002DDF40;
extern const float D_002A8550;
int FReconTargetSelectable(struct O*);
qword func_001A62E8(struct O*);
int FInsideCmFrustum(struct CAMERA*,qword,qword);
int FInsideCmMrd(struct CAMERA*,qword,float,float*);
void UpdateReconTargetVisibility(struct P*,struct O*);
void SetReconSelState(struct P*,int);
static inline void PlaneArguments(qword *eye,qword *threshold,qword *point,qword scalar,const qword *camera,const qword *position) {
    register qword view __asm__("vf1");
    register qword value __asm__("$6");
    register qword local __asm__("vf2");
    __asm__("lqc2 %0,%3\n\tpor %1,$zero,%5\n\tlqc2 %2,%4"
            : "=j"(view),"=&r"(value),"=j"(local) : "m"(*camera),"m"(*position),"r"(scalar));
    *eye=view;*threshold=value;*point=local;
}
static inline qword PlaneDelta(qword position,qword camera) {
    register qword eye __asm__("vf1")=camera;
    register qword point __asm__("vf2")=position;
    __asm__("vsub.xyzw %0,%0,%1" : "+j"(point) : "j"(eye));return point;
}
static inline qword SubSecond(qword a,qword b){__asm__("vsub.xyzw %0,%1,%2":"=j"(b):"j"(a),"0"(b));return b;}
static inline float LengthXY(qword a){float g;
__asm__("vmul.xy %0,%0,%0\n\tvaddy.x %0,%0,%0y":"=j"(a):"0"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(a):"0"(a));
__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(a));return g;}
static inline void Store(qword*p,qword q){__asm__("sq %1,%0":"=m"(*p):"r"(q));}
static inline qword ScalarBits(const float*p){qword q;__asm__("lw %0,%1":"=r"(q):"m"(*p));return q;}
/** @sly2-annot addr=0x202540
 * Purpose: choose closest unblocked target in frustum; on change SetReconSelState(2)
 * Info: proposed name SelectReconTarget (T1); TU tu_201D78
 * Notes: names_v2 C proposed FAbsorbBrkWkr__FP3BRKP3WKR (0.638)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void SelectReconTarget(struct P*p)
{
 V radius,screen,position;
 int i=0;
 register struct O *best __asm__("$20")=0;
 struct O *owner=p->ref->o;
 float minimum;
 struct O **slot;
 owner->vt[97].fn((char*)owner+owner->vt[97].delta,&p->position.q);
 p->position.f[2]+=20.0f;
 minimum=D_002A8550;
 if(p->count>0){slot=p->items;do{
   struct O *o=*slot;
   struct CAMERA *camera;
   float distance; qword eye,threshold,point;
   if(!FReconTargetSelectable(o))continue;
   Store(&position.q,func_001A62E8(o));
   camera=&D_002DDF40;
   radius.q=ScalarBits(&o->radius);
   PlaneArguments(&eye,&threshold,&point,radius.q,&camera->position,&position.q);
   if(!FInsideCmFrustum(camera,PlaneDelta(point,eye),threshold))continue;
   if(!FInsideCmMrd(camera,func_001A62E8(o),o->height,&screen.f[0]))continue;
   distance=LengthXY(SubSecond(position.q,camera->position));
   if(distance<minimum){
     UpdateReconTargetVisibility(p,o);
     if(o->state!=2){best=o;minimum=distance;}
   }
 }while(++slot,++i<p->count);}
 if(best!=p->best){p->best=best;SetReconSelState(p,2);}
}
