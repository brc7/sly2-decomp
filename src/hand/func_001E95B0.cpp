/* func_001E95B0 (Claude agent, wave-2 lane 7): append a time sample to a sampled path (two U arrays a/b, a
 * path with positions/distances, a time array). If the new time is earlier than the last sample the tail is
 * truncated; otherwise samples are inserted until the new time is within a step of the last one or the
 * capacity is reached, interpolating a/b/pos/dist for the sample that becomes the second-to-last.
 *
 * Loop trick: the target loop is a real loop (its refs are weighted x2 for register allocation, which is what
 * puts the right-hand pointer of each Lerp in $a2 and n*4 in $a3) but loop.c did NOT optimize it (it hoists the
 * 1.0f and the %hi of the step constant otherwise). The first sample is tested before the loop and the loop is
 * entered with a goto to its first label, so loop.c sees a jump into the loop and leaves it alone. */
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct PATH {char pad[0xE8];int count;int padEC;float*dist;U*pos;};
struct S {char pad[0x48];PATH*path;int active;int pad50;U*a;U*b;float*time;char pad60[0x84-0x60];int capacity;float step;};
extern float D_002C3678;
extern "C" void InvalidateShapeCache(PATH*);
extern "C" void func_001E95B0(S*,float);
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
/* pd = p0 * cf + p1 * f */
static inline void Lerp(U*pd,const U*p0,const U*p1,float f,float cf){pd->q=VecCombo(VU_FLOAT(cf),VU_VECTOR(p0->q),VU_FLOAT(f),VU_VECTOR(p1->q));}
/** @sly2-annot addr=0x1E95B0
 * Purpose: Append a time sample to a sampled path (truncate on going back, interpolate inserted samples)
 * Info: proposed name AddChainSample (T2); TU shape
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E95B0(S*p,float time){
 int count=p->path->count;
 int n;
 if(time<p->time[count-1]){
  int i=count;
  for(;;){--i;if(i<0)break;if(p->time[i]<time)break;}
  i++;
  p->path->count=i+1;
  p->time[i]=time;
  goto refresh;
 }
 n=count;
 if(time<p->time[count-2]+D_002C3678*p->step){p->time[count-1]=time;goto finished;}
 goto top;
 for(;;){
 top:
  if(n<p->capacity){
   n++;
   p->a[n-1]=p->a[n-2];
   p->b[n-1]=p->b[n-2];
   p->time[n-1]=time;
   p->path->pos[n-1]=p->path->pos[n-2];
   p->path->dist[n-1]=p->path->dist[n-2];
   p->time[n-2]=p->time[n-3]+p->step;
   float f=(p->time[n-2]-p->time[n-3])/(p->time[n-1]-p->time[n-3]);
   float cf=1-f;
   Lerp(&p->a[n-2],&p->a[n-3],&p->a[n-1],f,cf);
   Lerp(&p->b[n-2],&p->b[n-3],&p->b[n-1],f,cf);
   Lerp(&p->path->pos[n-2],&p->path->pos[n-3],&p->path->pos[n-1],f,cf);
   p->path->dist[n-2]=cf*p->path->dist[n-3]+f*p->path->dist[n-1];
   if(time<p->time[n-2]+D_002C3678*p->step){p->time[n-1]=time;break;}
  }else{p->active=0;break;}
 }
finished:
 p->path->count=n;
refresh:
 InvalidateShapeCache(p->path);
}
