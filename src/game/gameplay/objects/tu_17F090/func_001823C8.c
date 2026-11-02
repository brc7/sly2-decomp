// func_001823C8: contact generator. From lane H3v1 (no pins); the plane distance's scalar leaves the VU via gcc's qmfc2 plus an mtc1 asm (GX), as in Lilly's GXOfQw.

typedef int qword __attribute__((mode(TI)));
typedef union U{qword q;float f[4];int i[4];}U;
typedef struct M4{U r[4];}M4;
typedef struct NODE{U position;char pad10[0x50];struct NODE*next;}NODE;
typedef struct ENTRY{char pad0[0x10];NODE*head;char pad14[0xC];float radius;char pad24[0xC];}ENTRY;
typedef struct OWNER{char pad0[0x70];qword position;}OWNER;
typedef struct REQUEST{OWNER*owner;int unused4;float radius;int unusedC;long flags;int unused18,start;M4 matrix;char pad60[0x1C];qword*vertices;char pad80[0x10];int count;qword*world;}REQUEST;
typedef struct CONTACT{char pad[0xB0];}CONTACT;
typedef struct P{char pad0[0xE0];int contactCount;CONTACT*contacts;char padE8[0xE0];int entryCount;ENTRY entries[1];}P;
typedef struct SEGMENT{qword a,b,plane;char pad30[0x30];}SEGMENT;
typedef struct WORK{U direction,newEndpoint,oldEndpoint;SEGMENT segments[2];U scale,dot,one;}WORK;
extern qword D_002A2DA0;extern int D_003DD490;
extern void func_001F5990(P*,REQUEST*,int,int,int,CONTACT**);
extern qword*func_002564B8(qword*);
extern int func_0014C748(qword,float,qword,qword,int,SEGMENT*);
extern void func_001A7C18(void*,CONTACT*,REQUEST*,qword,qword,int,NODE*,int,CONTACT**,float);

static inline qword Sub(qword a,qword b){__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
static inline qword Broadcast(float f){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(f));return r;}
static inline qword BroadcastBits(int b){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(b));return r;}
static inline float Scalar(qword a){float f;__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(a));return f;}
static inline qword MAdd(qword a,qword b,qword s){__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(a):"j"(b),"j"(s));return a;}
static inline qword MAdd2(qword a,qword b,qword s){qword r;__asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(a),"j"(b),"j"(s));return r;}
static inline qword Point(const M4*m,qword p){qword r=m->r[0].q;__asm__("vmulax.xyz ACC,%0,%4x\n\tvmadday.xyz ACC,%1,%4y\n\tvmaddaz.xyz ACC,%2,%4z\n\tvmaddw.xyz %0,%3,$vf0w":"+j"(r):"j"(m->r[1].q),"j"(m->r[2].q),"j"(m->r[3].q),"j"(p));return r;}
static inline float Planar(U*p){
 qword direction=p->q;
 qword d,root;
 qword dv; /* allocation-only "=j"(dv) tied to direction: the asm leaves it unchanged, it gives the vdiv a VF copy so direction's GPR dies here */
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(root),"=j"(dv):"2"(direction),"r"(direction),"r"(direction)); /* allocation-only "r": direction is reloaded through a GPR (lq+qmtc2) */
 __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(root):"j"(d));
 if(Scalar(root)<0.0001f)p->q=D_002A2DA0;
 else{__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=j"(d):"j"(dv),"j"(root));p->q=d;}
 return Scalar(root);
}
static inline float GX(qword qw){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(qw));return x;}
static inline float Distance(U*dot,U*one,qword plane,qword point){
 qword d=point,o;
 __asm__("vmul.xyz %0,%2,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddaz.x ACC,%1,%0z\n\tvmsubw.x %0,%1,%2w":"+j"(d),"=&j"(o):"j"(plane));
 one->q=o;dot->q=d;return GX(d);
}
/** @sly2-annot addr=0x1823C8
 * Purpose: vt60: SO base then contacts against each segment (PxpAdd) for requests with flag bit 49
 * Info: proposed name AddHackwallXps (T2); class HACKWALL slot60; TU tu_17F090
 * Notes: names_v2 C proposed AssignSggSearchPoints__FP3SGG (0.638) | CID4C->CID76 (CID 0x4C=76, hex-literal)
 *   | class identity from whole_game WE names/object strings: AddCid76Xps -> AddHackwallXps
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001823C8(P*p,REQUEST*q,int unused2,int unused3,int unused4,CONTACT**pout){
 WORK w;int i,count;qword center;CONTACT**out=pout;
 func_001F5990(p,q,unused2,unused3,unused4,pout);
 if(!(q->flags&0x2000000000000L))return;
 center=q->owner->position;
 count=p->entryCount;i=0;
 do{
  if(count<=0)goto done;
 top:
  {
  ENTRY*entry=&p->entries[i];NODE*n=entry->head;float radius=q->radius+entry->radius;
  for(;n;n=n->next){
   float distance;
   w.direction.q=Sub(n->position.q,center);w.direction.i[2]=0;
   distance=Planar(&w.direction);
   if(distance<radius){
    w.newEndpoint.q=Broadcast(-q->radius);
    func_001A7C18(&D_003DD490,&p->contacts[p->contactCount-1],q,MAdd(center,w.direction.q,w.newEndpoint.q),w.direction.q,10,n,0,out,distance-radius);
   }else{
    int j;
    for(j=q->start;j<q->count;j++){
     w.newEndpoint.q=Point(&q->matrix,q->vertices[j]);w.oldEndpoint.q=q->world[j];
     w.newEndpoint.f[2]=w.oldEndpoint.f[2]=n->position.f[2];
     if(func_0014C748(n->position.q,radius,w.newEndpoint.q,w.oldEndpoint.q,2,w.segments)>0){
      qword plane,point,result;
      w.scale.q=BroadcastBits(*(int*)&q->radius);
      plane=w.segments[0].plane;point=w.oldEndpoint.q;
      result=MAdd2(point,plane,w.scale.q);
      func_002564B8(&w.dot.q);func_002564B8(&w.one.q);
      distance=Distance(&w.dot,&w.one,plane,point)-q->radius;
      func_001A7C18(&D_003DD490,&p->contacts[p->contactCount-1],q,result,plane,10,n,0,out,distance);
      break;
     }
    }
   }
  }
  }
  ++i;
  if(i<p->entryCount)goto top;
 }while(0);
 done:;
}
typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char Long_Size[(sizeof(long)==8)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char M4_Size[(sizeof(M4)==0x40)?1:-1];
typedef char ENTRY_Size[(sizeof(ENTRY)==0x30)?1:-1];
typedef char CONTACT_Size[(sizeof(CONTACT)==0xB0)?1:-1];
typedef char SEGMENT_Size[(sizeof(SEGMENT)==0x60)?1:-1];
typedef char SegmentBuffer_Size[(sizeof(SEGMENT[2])==0xC0)?1:-1];
typedef char NODE_position_Offset[((unsigned)&((NODE*)0)->position==0)?1:-1];
typedef char NODE_next_Offset[((unsigned)&((NODE*)0)->next==96)?1:-1];
typedef char ENTRY_head_Offset[((unsigned)&((ENTRY*)0)->head==16)?1:-1];
typedef char ENTRY_radius_Offset[((unsigned)&((ENTRY*)0)->radius==32)?1:-1];
typedef char OWNER_position_Offset[((unsigned)&((OWNER*)0)->position==112)?1:-1];
typedef char REQUEST_owner_Offset[((unsigned)&((REQUEST*)0)->owner==0)?1:-1];
typedef char REQUEST_radius_Offset[((unsigned)&((REQUEST*)0)->radius==8)?1:-1];
typedef char REQUEST_flags_Offset[((unsigned)&((REQUEST*)0)->flags==16)?1:-1];
typedef char REQUEST_start_Offset[((unsigned)&((REQUEST*)0)->start==28)?1:-1];
typedef char REQUEST_matrix_Offset[((unsigned)&((REQUEST*)0)->matrix==32)?1:-1];
typedef char REQUEST_vertices_Offset[((unsigned)&((REQUEST*)0)->vertices==124)?1:-1];
typedef char REQUEST_count_Offset[((unsigned)&((REQUEST*)0)->count==144)?1:-1];
typedef char REQUEST_world_Offset[((unsigned)&((REQUEST*)0)->world==148)?1:-1];
typedef char P_contactCount_Offset[((unsigned)&((P*)0)->contactCount==224)?1:-1];
typedef char P_contacts_Offset[((unsigned)&((P*)0)->contacts==228)?1:-1];
typedef char P_entryCount_Offset[((unsigned)&((P*)0)->entryCount==456)?1:-1];
typedef char P_entries_Offset[((unsigned)&((P*)0)->entries==460)?1:-1];
typedef char SEGMENT_plane_Offset[((unsigned)&((SEGMENT*)0)->plane==32)?1:-1];
typedef char WORK_direction_Offset[((unsigned)&((WORK*)0)->direction==0)?1:-1];
typedef char WORK_newEndpoint_Offset[((unsigned)&((WORK*)0)->newEndpoint==16)?1:-1];
typedef char WORK_oldEndpoint_Offset[((unsigned)&((WORK*)0)->oldEndpoint==32)?1:-1];
typedef char WORK_segments_Offset[((unsigned)&((WORK*)0)->segments==48)?1:-1];
typedef char WORK_scale_Offset[((unsigned)&((WORK*)0)->scale==240)?1:-1];
typedef char WORK_dot_Offset[((unsigned)&((WORK*)0)->dot==256)?1:-1];
typedef char WORK_one_Offset[((unsigned)&((WORK*)0)->one==272)?1:-1];
