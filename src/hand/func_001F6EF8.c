typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f;} V;
struct TRI {int index[3];};
struct MESH {qword *velocity,*acceleration;struct TRI *triangles;};
struct P {void *x0;int x4;float maximum;int xC;struct MESH*mesh;char pad14[0x64];int count;qword *positions;int triCount;void *planes;char pad88[0x20];int state;};
void func_00119308(void*,qword,qword,qword);
static inline qword Scalar(float f){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));return q;}
static inline qword Step(qword a,qword b,qword s){register qword r __asm__("vf5");
__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=&j"(r):"j"(a),"j"(b),"j"(s));return r;}
static inline qword Position(qword p,qword old,qword next,qword half,V *scalar){register qword r __asm__("vf3");
__asm__("sqc2 %5,%1\n\tvadda.xyzw $ACC,%2,$vf0\n\tvmaddax.xyzw $ACC,%3,%5x\n\tvmaddx.xyzw %0,%4,%5x":"=&j"(r),"=m"(scalar->q):"j"(p),"j"(old),"j"(next),"j"(half));return r;}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float LengthSquared(qword q){qword r,one;float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"j"(q));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
static inline float Root(float x){float r;__asm__ __volatile__("sqrt.s %0,%1":"=f"(r):"f"(x));return r;}
static inline qword *VelocityCursor(qword *p){register qword *initial __asm__("$2")=p;register qword *r __asm__("$4");__asm__("daddu %0,%1,$zero":"=r"(r):"r"(initial));return r;}
static inline int PlaneLess(int a,int b){register int r __asm__("$2");__asm__("slt %0,%1,%2":"=r"(r):"r"(a),"r"(b));return r;}
/** @sly2-annot addr=0x1F6EF8
 * Purpose: Integrate soft-mesh vertices by dt (semi-implicit), track max radius, invalidate on large moves
 * Info: proposed name UpdateSoPartMesh (T2); TU so
 * Notes: names_v2 v1 proposed ImpactSo__FP2SOi (0.167)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F6EF8(struct P*p,float dt)
{
 V scalar;
 register int vertexIndex __asm__("a2")=0;
 qword *velocity=p->mesh->velocity;
 register int initialCount __asm__("$3");
 register qword *acceleration __asm__("$5")=({register qword *r __asm__("$5");__asm__("lw %0,%2\n\tlw %1,%3":"=&r"(initialCount),"=r"(r):"m"(p->count),"m"(p->mesh->acceleration));r;});
 register qword *positions __asm__("s4");
 float maximum=0.0f;
 positions=p->positions;
 if(initialCount>0){
   register qword time __asm__("vf7")=Scalar(dt);
   register int invalid __asm__("$7")=-1;
   register qword half __asm__("vf6")=Scalar(dt*0.5f);
   register qword *position __asm__("$3")=positions;
   register qword *velocityCursor __asm__("$4")=VelocityCursor(velocity);
   register int limit __asm__("$2");
   do{
     register qword old __asm__("vf1")=*velocityCursor,force __asm__("vf2"),point __asm__("vf4")=*position;
     int state;
     qword next,newpoint;
     float square;
     __asm__("sqc2 %4,%2\n\tlqc2 %0,%3\n\tlw %1,%5":"=j"(force),"=r"(state),"=m"(scalar.q):"m"(*acceleration),"j"(time),"m"(p->state));
     next=Step(old,force,time);
     newpoint=Position(point,old,next,half,&scalar);
     if(state!=invalid && 1.0f<LengthSquared(Sub(point,newpoint)))p->state=invalid;
     square=LengthSquared(newpoint);
     *position=newpoint;
     *velocityCursor=next;
     if(maximum<square)maximum=square;
     limit=p->count;
   }while(++velocityCursor,++position,++acceleration,++vertexIndex<limit);
 }
 p->maximum=Root(maximum);
 {
   struct TRI*firstTri=p->mesh->triangles;
   char *firstPlane=(char*)p->planes;
   register int planeIndex __asm__("s3")=0;
   if(p->triCount>0){struct TRI*tri=firstTri;char*plane=firstPlane;do{
     qword a=positions[tri->index[0]],b=positions[tri->index[1]],c=positions[tri->index[2]];
     __asm__("addiu %0,%0,12":"+r"(tri));
     func_00119308(plane,a,b,c);
     plane+=0x20;
   }while(PlaneLess(++planeIndex,p->triCount));}
 }
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_V[(sizeof(V)==16)?1:-1];
typedef char layout_V_alignment[(__alignof__(V)==16)?1:-1];
typedef char layout_TRI[(sizeof(struct TRI)==12)?1:-1];
typedef char layout_MESH[(sizeof(struct MESH)==12)?1:-1];
typedef char layout_P[(sizeof(struct P)==0xAC)?1:-1];
typedef char layout_P_maximum[((unsigned)&((struct P*)0)->maximum==8)?1:-1];
typedef char layout_P_mesh[((unsigned)&((struct P*)0)->mesh==0x10)?1:-1];
typedef char layout_P_count[((unsigned)&((struct P*)0)->count==0x78)?1:-1];
typedef char layout_P_positions[((unsigned)&((struct P*)0)->positions==0x7C)?1:-1];
typedef char layout_P_triCount[((unsigned)&((struct P*)0)->triCount==0x80)?1:-1];
typedef char layout_P_planes[((unsigned)&((struct P*)0)->planes==0x84)?1:-1];
typedef char layout_P_state[((unsigned)&((struct P*)0)->state==0xA8)?1:-1];
typedef char layout_MESH_velocity[((unsigned)&((struct MESH*)0)->velocity==0)?1:-1];
typedef char layout_MESH_acceleration[((unsigned)&((struct MESH*)0)->acceleration==4)?1:-1];
typedef char layout_MESH_triangles[((unsigned)&((struct MESH*)0)->triangles==8)?1:-1];
