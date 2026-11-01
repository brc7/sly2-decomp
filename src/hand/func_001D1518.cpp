// Resolve object entries, compute spans and attach linked resources.
typedef int qword __attribute__((mode(TI)));
union F {float value;int bits;};
union U {qword q;float a[4];};
struct P;
struct REF {char pad[0x70];qword position;};
struct E {int id;REF *reference;int pad8;float x,y;char pad14[0x10];F lower,upper;int pad2C;U position; qword top,bottom;};
struct SIDE {int id;REF *reference;void *binding;int padC,pad10;};
struct ATTACH {char pad[0x18];int id;void *object;void *binding;char pad24[8];int meshId;REF *mesh;void *meshBinding;int extraId;void *extra;};
union CURRENT {E*entry;SIDE*side;ATTACH*attachment;};
union SCALE {const qword*axis;int stride;};
struct P {char pad[0x190];int count;char pad194[0xC];E entries[8];float width;int sideCount;SIDE sides[4];int attachCount;ATTACH*attachments[8];char pad51C[0x50];void *asset;void *binding;char pad574[0x38];void *path;};
extern "C" {
extern qword D_002A2DC0,D_002A2DD0;
extern char D_003DD490[];
void func_001F36A8(P *);
REF *func_001F4C58(P *,int,int);
void *func_002332B8(void *,const void *,int);
qword ConvertAloPos(void *,P *,qword);
void *func_0012DC18(P *,REF *,int,int,int,qword);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *,int,int,P *);
void *func_001F0380(void *,P *,int,int);
void *func_001E94B0(int,int);
void func_001F1448(void *,P *,int,int,void **);
void func_001D14C0(P *);
}
static inline qword Zero(){qword r;asm("vsub.xyzw %0,$vf0,$vf0":"=j"(r));return r;}
static inline qword CPU(qword value){qword result;asm("qmfc2.ni %0,%1":"=r"(result):"j"(value));return result;}
extern "C" void func_001D1518(P *p){
 U scalar,maximum,minimum;
 register int i __asm__("$18")=0;CURRENT current;SCALE scale;
 register int count __asm__("$2");
 {qword zero=Zero();minimum.q=CPU(zero);maximum.q=zero;}
 func_001F36A8(p);
 if(p->count>0){
 int stride=0x60;
 scale.axis=&D_002A2DC0;
 E *next=p->entries+1,*cursor=p->entries;
 do {
  current.entry=cursor;
  REF *reference=func_001F4C58(p,current.entry->id,13);
  current.entry->reference=reference;
  if(!reference){
   unsigned bytes=(p->count-i)*stride-sizeof(E);
   E*source=next;
   cursor=current.entry-1;--next;--i;
   func_002332B8(current.entry,source,bytes);
   --p->count;
  }else{
   qword returned=ConvertAloPos(0,p,reference->position);
   register qword position __asm__("vf2");register int upper __asm__("$2");
   asm("qmtc2.ni %3,%0\n\tsqc2 %0,%1\n\tlw %2,%4":"=j"(position),"=m"(current.entry->position.q),"=r"(upper):"r"(returned),"m"(current.entry->upper.bits));
   register qword high __asm__("vf3"),low __asm__("vf4");
   asm("qmtc2.ni %1,%0":"=j"(high):"r"(upper));
   register int lower __asm__("$2");
   register qword small __asm__("vf1"),large __asm__("vf5"),axis __asm__("vf1");
   asm("lw %0,%4\n\tsqc2 %5,%1\n\tqmtc2.ni %0,%2\n\tlqc2 %3,%6":"=&r"(lower),"=m"(scalar.q),"=j"(low),"=j"(small):"m"(current.entry->lower.bits),"j"(high),"m"(minimum.q));
   asm("vmini.xyz %0,%3,%0\n\tlqc2 %1,%4\n\tsqc2 %0,%2":"+j"(small),"=&j"(large),"=m"(minimum.q):"j"(position),"m"(maximum.q));
   asm("vmax.xyz %0,%3,%0\n\tlqc2 %1,%4\n\tsqc2 %0,%2":"+j"(large),"=&j"(axis),"=m"(maximum.q):"j"(position),"m"(*scale.axis));
   asm("vadda.xyzw $ACC,%2,$vf0\n\tvmaddx.xyzw %0,%3,%0x\n\tsqc2 %0,%1":"+j"(high),"=m"(current.entry->top):"j"(position),"j"(axis));
   asm("sqc2 %4,%1\n\tlqc2 %3,%5\n\tvadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%3,%4x\n\tsqc2 %0,%2":"+j"(position),"=m"(scalar.q),"=m"(current.entry->bottom),"=&j"(axis):"j"(low),"m"(*scale.axis));
  }
  count=p->count;
  ++i;++next;++cursor;
  count=i<count;
 }while(count);}
 void**binding;
 register qword maximumWord __asm__("$2");
 asm("lq %0,%1":"=r"(maximumWord):"m"(maximum.q));
 i=0;
 register float maxX __asm__("$f3");
 asm("mtc1 %2,%0\n\tla %1,%a3":"=f"(maxX),"=r"(binding):"r"(maximumWord),"p"(&p->binding));
 register qword minimumWord __asm__("$2");register float minX __asm__("$f2");
 asm("lq %0,%2\n\tmtc1 %0,%1":"=r"(minimumWord),"=f"(minX):"m"(minimum.q));
 float width;
 asm("sub.s %0,%2,%3\n\tlw %1,%4":"=f"(width),"=r"(count):"f"(maxX),"f"(minX),"m"(p->count));
 p->width=width;
 if(count>0){
  register qword word __asm__("$2");register E*coordinate __asm__("$3");
  register float xScale __asm__("$f2")=minX,minY __asm__("$f0");
  asm("lq %0,%4\n\tneg.s %1,%1\n\tprot3w %0,%0\n\tla %2,%a5\n\tmtc1 %0,%3":"=&r"(word),"+f"(xScale),"=&r"(coordinate),"=f"(minY):"m"(minimum.q),"p"(p->entries));
  register float maxY __asm__("$f1"),yScale __asm__("$f3");
  asm("lq %0,%4\n\tmax.s %1,%1,%5\n\tprot3w %0,%0\n\tneg.s %2,%2\n\tmtc1 %0,%3":"=&r"(word),"+f"(xScale),"+f"(minY),"=f"(maxY):"m"(maximum.q),"f"(maxX));
  asm("max.s %0,%1,%2":"=f"(yScale):"f"(minY),"f"(maxY));
  do {
   float x=coordinate->position.a[0];++i;
   float y=coordinate->position.a[1];
   coordinate->x=x/xScale;coordinate->y=y/yScale;
   count=p->count;count=i<count;
   ++coordinate;
  }while(count);
 }
 i=0;
 if(p->sideCount>0){
 scale.stride=0x14;
 SIDE*next=p->sides+1;current.side=p->sides;
 do {
  REF *reference=func_001F4C58(p,current.side->id,6);
  current.side->reference=reference;
  if(!reference){
   unsigned bytes=(p->sideCount-i)*scale.stride-sizeof(SIDE);
   SIDE*destination=current.side;SIDE*source=next;
   --current.side;--next;--i;
   func_002332B8(destination,source,bytes);
   --p->sideCount;
  }else current.side->binding=func_0012DC18(p,reference,1,7,3,D_002A2DD0);
  count=p->sideCount;
  ++i;++next;++current.side;
  count=i<count;
 }while(count);}
 int j=0;
 if(p->attachCount>0){
 int invalid=-1;
 ATTACH **cursor=p->attachments;int byteOffset=0;
 do {
  current.attachment=*cursor;
  current.attachment->object=PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490,1,current.attachment->id,p);
  if(current.attachment->object){
   current.attachment->mesh=func_001F4C58(p,current.attachment->meshId,6);
   if(current.attachment->mesh || current.attachment->meshId==invalid){
    current.attachment->extra=PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490,1,current.attachment->extraId,p);
    if(current.attachment->extra || current.attachment->extraId==invalid){
     current.attachment->binding=func_001F0380(current.attachment->object,p,-1,0);
     if(current.attachment->mesh)current.attachment->meshBinding=func_0012DC18(p,current.attachment->mesh,1,7,3,D_002A2DD0);
     goto attachment_next;
    }
   }
  }
  {ATTACH**destination=cursor;
  char*source;
  asm("addu %0,%1,%2":"=r"(source):"r"(byteOffset),"r"(p));
  source+=0x500;
  int bytes=(p->attachCount-j)*4-4;
  --cursor;byteOffset-=4;--j;
  func_002332B8(destination,source,bytes);
  register int remaining __asm__("$3");
  asm("lw %0,%1\n\taddiu %0,%0,-1\n\tsw %0,%1":"=&r"(remaining),"+m"(p->attachCount));}
 attachment_next:
  ++j;++cursor;byteOffset+=4;
 }while(j<p->attachCount);}

 p->path=func_001E94B0(0x2C9,16);
 func_001F1448(p->asset,p,-1,0,binding);
 func_001D14C0(p);
}

#define OFF(T,F) ((unsigned int)&((T*)0)->F)
typedef char native_layout_0[(sizeof(void*)==4)?1:-1];
typedef char native_layout_1[(sizeof(int)==4)?1:-1];
typedef char native_layout_2[(sizeof(float)==4)?1:-1];
typedef char native_layout_3[(sizeof(qword)==16)?1:-1];
typedef char native_layout_4[(__alignof__(qword)==16)?1:-1];
typedef char native_layout_5[(sizeof(F)==4)?1:-1];
typedef char native_layout_6[(__alignof__(F)==4)?1:-1];
typedef char native_layout_7[(sizeof(U)==16)?1:-1];
typedef char native_layout_8[(__alignof__(U)==16)?1:-1];
typedef char native_layout_9[(sizeof(REF)==0x80)?1:-1];
typedef char native_layout_10[(__alignof__(REF)==16)?1:-1];
typedef char native_layout_11[(OFF(REF,position)==0x70)?1:-1];
typedef char native_layout_12[(sizeof(E)==0x60)?1:-1];
typedef char native_layout_13[(__alignof__(E)==16)?1:-1];
typedef char native_layout_14[(OFF(E,id)==0)?1:-1];
typedef char native_layout_15[(OFF(E,reference)==4)?1:-1];
typedef char native_layout_16[(OFF(E,x)==0xC)?1:-1];
typedef char native_layout_17[(OFF(E,y)==0x10)?1:-1];
typedef char native_layout_18[(OFF(E,lower)==0x24)?1:-1];
typedef char native_layout_19[(OFF(E,upper)==0x28)?1:-1];
typedef char native_layout_20[(OFF(E,position)==0x30)?1:-1];
typedef char native_layout_21[(OFF(E,top)==0x40)?1:-1];
typedef char native_layout_22[(OFF(E,bottom)==0x50)?1:-1];
typedef char native_layout_23[(sizeof(SIDE)==0x14)?1:-1];
typedef char native_layout_24[(__alignof__(SIDE)==4)?1:-1];
typedef char native_layout_25[(OFF(SIDE,id)==0)?1:-1];
typedef char native_layout_26[(OFF(SIDE,reference)==4)?1:-1];
typedef char native_layout_27[(OFF(SIDE,binding)==8)?1:-1];
typedef char native_layout_28[(sizeof(ATTACH)==0x40)?1:-1];
typedef char native_layout_29[(__alignof__(ATTACH)==4)?1:-1];
typedef char native_layout_30[(OFF(ATTACH,id)==0x18)?1:-1];
typedef char native_layout_31[(OFF(ATTACH,object)==0x1C)?1:-1];
typedef char native_layout_32[(OFF(ATTACH,binding)==0x20)?1:-1];
typedef char native_layout_33[(OFF(ATTACH,meshId)==0x2C)?1:-1];
typedef char native_layout_34[(OFF(ATTACH,mesh)==0x30)?1:-1];
typedef char native_layout_35[(OFF(ATTACH,meshBinding)==0x34)?1:-1];
typedef char native_layout_36[(OFF(ATTACH,extraId)==0x38)?1:-1];
typedef char native_layout_37[(OFF(ATTACH,extra)==0x3C)?1:-1];
typedef char native_layout_38[(sizeof(CURRENT)==4)?1:-1];
typedef char native_layout_39[(__alignof__(CURRENT)==4)?1:-1];
typedef char native_layout_40[(OFF(CURRENT,entry)==0)?1:-1];
typedef char native_layout_41[(OFF(CURRENT,side)==0)?1:-1];
typedef char native_layout_42[(OFF(CURRENT,attachment)==0)?1:-1];
typedef char native_layout_43[(sizeof(SCALE)==4)?1:-1];
typedef char native_layout_44[(__alignof__(SCALE)==4)?1:-1];
typedef char native_layout_45[(OFF(SCALE,axis)==0)?1:-1];
typedef char native_layout_46[(OFF(SCALE,stride)==0)?1:-1];
typedef char native_layout_47[(sizeof(P)==0x5B0)?1:-1];
typedef char native_layout_48[(__alignof__(P)==16)?1:-1];
typedef char native_layout_49[(OFF(P,count)==0x190)?1:-1];
typedef char native_layout_50[(OFF(P,entries)==0x1A0)?1:-1];
typedef char native_layout_51[(OFF(P,width)==0x4A0)?1:-1];
typedef char native_layout_52[(OFF(P,sideCount)==0x4A4)?1:-1];
typedef char native_layout_53[(OFF(P,sides)==0x4A8)?1:-1];
typedef char native_layout_54[(OFF(P,attachCount)==0x4F8)?1:-1];
typedef char native_layout_55[(OFF(P,attachments)==0x4FC)?1:-1];
typedef char native_layout_56[(OFF(P,asset)==0x56C)?1:-1];
typedef char native_layout_57[(OFF(P,binding)==0x570)?1:-1];
typedef char native_layout_58[(OFF(P,path)==0x5AC)?1:-1];
