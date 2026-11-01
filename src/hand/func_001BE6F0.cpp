typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int i[4];};
struct M3{qword r[3];};
struct M4{U r[4];};
struct V3{float f[3];};
struct PLANE{V3 normal;float distance;};
struct O{char pad[0x118];O*parent;float fov,clipFar,clipNear;};
struct QW;
struct P{int width,height;char pad8[8];int directCount;QW*directData;char pad18[8];O*camera;};
struct V;
extern "C" V3 D_002C1F00,D_002C1F10;
extern "C" M3*func_001A6318(O*);
extern "C" qword func_001A62E8(O*);
extern "C" qword*func_00256D18(qword*,qword);
extern "C" float func_002450F0(float);
extern "C" void func_0011C448(float,float,float,float,float,float,M4*);
extern "C" void func_0014D8F8(M4*,M4*,M4*);
extern "C" void func_001574B8(V*);
extern "C" void func_00226D10(V*);
extern "C" void func_002270C8(V*,int,QW*,int);
extern "C" void func_00226D28(V*);
extern "C" void func_00226CA8(V*,int,int);
extern "C" void func_00226F28(V*,int,int,void*,int);
extern "C" void func_001576F0(V*);
static inline qword Bcast(float f){register qword q __asm__("vf2");__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));return q;}

static inline qword Scale0(qword v,qword k){register qword q __asm__("vf4");__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(q):"j"(v),"j"(k));return q;}
static inline qword Scale2(qword v,qword k){register qword q __asm__("vf1");__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(q):"j"(v),"j"(k));return q;}
/** @sly2-annot addr=0x1BE6F0
 * Purpose: Build look-at + projection for the slot's camera object and emit VIF/DMA setup (base/offset,
 *   unpack)
 * Info: proposed name SetupItemRenderView (T2); TU tu_1BE398
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
extern "C" void func_001BE6F0(P*p,V*v){
 U axis0,axis1,axis2,minus2,minus0,copy1;
 M4 view;
 U scalar;
 M4 projection,product;
 PLANE planes[2];
 O*object;
 M3*basis;
 register float aspect __asm__("$f20");
 float slope;
 if(!p->camera)return;
 object=p->camera->parent?p->camera->parent:p->camera;
 basis=func_001A6318(object);
 M4*pv=&view;
 axis0.q=basis->r[0];
 {qword neg=Bcast(-1.0f);
  axis1.q=basis->r[1];
  axis2.q=basis->r[2];
  qword nx=Scale0(axis0.q,neg);
  qword nz=Scale2(axis2.q,neg);
  copy1=axis1;
  scalar.q=neg;
  minus2.q=nz;
  minus0.q=nx;
 }
 {qword position=func_001A62E8(object);
 func_00256D18(&view.r[3].q,position);}
 aspect=(float)p->width;
 aspect/=(float)p->height;
 pv->r[0]=minus2;
 pv->r[1]=minus0;
 pv->r[2]=copy1;
 view.r[0].i[3]=view.r[1].i[3]=view.r[2].i[3]=0;
 pv->r[3].f[3]=1.0f;
 slope=func_002450F0(p->camera->fov*0.5f);
 aspect=slope*aspect;
 slope*=4096.0f;
 aspect*=4096.0f;
 func_0011C448((float)p->width/aspect,(float)p->height/slope,0.0f,0.0f,p->camera->clipFar,p->camera->clipNear,&projection);
 func_0014D8F8(pv,&projection,&product);
 {O*camera=p->camera;
  float z=(camera->clipFar+camera->clipNear)/(camera->clipFar-camera->clipNear);
  float k=1.0f-z;
  planes[0].normal=D_002C1F00;
  planes[0].distance=1.0f/(camera->clipFar*k);
  planes[1].normal=D_002C1F10;
  planes[1].distance=-z/(camera->clipFar*k);
 }
 func_001574B8(v);
 func_00226D10(v);
 func_002270C8(v,p->directCount,p->directData,0);
 func_00226D28(v);
 func_00226CA8(v,0,0);
 func_00226F28(v,0xC,4,&product,2);
 func_00226F28(v,0xC,4,pv,6);
 func_00226F28(v,0xC,2,planes,0);
 func_00226CA8(v,0x32,0x1E7);
 func_001576F0(v);
}
