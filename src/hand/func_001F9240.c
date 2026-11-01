typedef int qword __attribute__((mode(TI)));
struct C {char pad[0x80];qword normal;float distance;int pad94[2];int kind;char padA0[0x10];qword velocity;};
struct P;
int func_001F90D0(struct P*,int,int,void**,int,struct C**);
void func_001F9410(struct P*,int,int,void**,int,struct C**);
static inline float Abs(float f){return __builtin_fabsf(f);}
static inline float Dot(qword a,qword b){qword r,one;float f;
__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"0"(a),"j"(b));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
/** @sly2-annot addr=0x1F9240
 * Purpose: Builds XP array for a group then runs SolveSwGroup in one or two passes depending on flags (1
 *   resting, 4 position pass)
 * Info: proposed name SplitSwGroup (T2); TU solve
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
void func_001F9240(struct P*p,int flags,int count,void**objects)
{
 int i;
 struct C *contacts[64];
 int n=func_001F90D0(p,flags,count,objects,64,contacts);
 if(flags&1){if(!n)flags=0;}
 else {
   if(flags&4){
     i=0;
     for(;i<n;i++){
       struct C*c=contacts[i];
       if(c->kind==11){if(0.1f<Abs(c->distance))break;}
       else {if(c->distance<-2.0f)break;}
     }
     if(i>=n)flags&=~4;
   }
   if(flags&2){
     i=0;
     if(n<=0)goto clearVelocityFlag;
     do{
       struct C*c=contacts[i];
       qword normal=c->normal;
       if(c->kind==11){if(5.0f<Abs(Dot(c->velocity,normal)))break;}
       else {if(Dot(c->velocity,normal)<-5.0f)break;}
     }while(++i<n);
     if(i<n)goto applyFlags;
   clearVelocityFlag:
     flags&=~2;
   }
 }
applyFlags:
 if(flags)func_001F9410(p,flags,count,objects,n,contacts);
}
