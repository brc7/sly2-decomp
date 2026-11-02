#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
#include "vifs.h"
#include "bis.h"
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];};
struct P;struct SPHERE;struct FACE;
struct PARAM{float value;int pad,kind;};
struct D{char channel,count;ushort flags;float radius;QW*chain;int base,words;};
struct BONE{int value;char pad4[4];int index;char padC[0x44];};
struct E{char pad[0x10];QW*chain;int base;BONE*bones;};
struct OBJECT{char pad[0x1C];int count;void*segments;};
struct HEADER{ushort flags,base,vertices,pad6,pad8,padA,padC,padE,weight,end,segments[2],bones[4];};
struct REQUEST{int bytes,alignment;REQUEST(int cb,int a):bytes(cb),alignment(a){}void*Allocate();void*AllocateZero();};
extern "C"{
void*STACKSCOPE_ctor(void*);
void STACKSCOPE_ctor_1AE4A0(void*,int);
void*func_001AE2A0(const REQUEST&);
void*func_001AE318(const REQUEST&);
void*func_002333B4(void*,int,uint);
void func_001AE5A0(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");
OBJECT*func_001EC8F8(int);
int func_001EC688(OBJECT*,int,int,VIFS*);
void func_001699E8(P*,D*,E*,SPHERE*,int,int,const int*,FACE**,CBinaryInputStream*);
float func_002599D8(const U*);
float func_002599E0(const U*);
float func_002599E8(const U*);
extern QW D_002DEB60;
extern uint D_002BE540[];
extern UPK D_002BE558[];
extern char D_00000000[],D_00001910[],D_00001A68[];
}
struct MARK{MARK(){STACKSCOPE_ctor(this);}~MARK(){STACKSCOPE_ctor_1AE4A0(this,2);}};
struct LEASE{LEASE(){STACKSCOPE_ctor(this);}void Release(){STACKSCOPE_ctor_1AE4A0(this,2);}};
inline void*REQUEST::Allocate(){return func_001AE318(*this);}
inline void*REQUEST::AllocateZero(){return func_001AE2A0(*this);}
static inline void*Allocate(int bytes){return REQUEST(bytes,16).Allocate();}
static inline void*AllocateZero(int bytes){return REQUEST(bytes,16).AllocateZero();}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char PARAM_Size[sizeof(PARAM)==12?1:-1];
typedef char D_Size[sizeof(D)==20?1:-1];
typedef char BONE_Size[sizeof(BONE)==80?1:-1];
typedef char E_Size[sizeof(E)==28?1:-1];
typedef char HEADER_Size[sizeof(HEADER)==32?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char MARK_Size[sizeof(MARK)==1?1:-1];
typedef char LEASE_Size[sizeof(LEASE)==1?1:-1];
typedef char PARAM_value_Offset[FIELD_OFFSET(PARAM,value)==0?1:-1];
typedef char PARAM_kind_Offset[FIELD_OFFSET(PARAM,kind)==8?1:-1];
typedef char D_count_Offset[FIELD_OFFSET(D,count)==1?1:-1];
typedef char D_chain_Offset[FIELD_OFFSET(D,chain)==8?1:-1];
typedef char D_base_Offset[FIELD_OFFSET(D,base)==12?1:-1];
typedef char D_words_Offset[FIELD_OFFSET(D,words)==16?1:-1];
typedef char BONE_value_Offset[FIELD_OFFSET(BONE,value)==0?1:-1];
typedef char BONE_index_Offset[FIELD_OFFSET(BONE,index)==8?1:-1];
typedef char E_chain_Offset[FIELD_OFFSET(E,chain)==16?1:-1];
typedef char E_bones_Offset[FIELD_OFFSET(E,bones)==24?1:-1];
typedef char OBJECT_count_Offset[FIELD_OFFSET(OBJECT,count)==28?1:-1];
typedef char OBJECT_segments_Offset[FIELD_OFFSET(OBJECT,segments)==32?1:-1];
typedef char HEADER_flags_Offset[FIELD_OFFSET(HEADER,flags)==0?1:-1];
typedef char HEADER_base_Offset[FIELD_OFFSET(HEADER,base)==2?1:-1];
typedef char HEADER_vertices_Offset[FIELD_OFFSET(HEADER,vertices)==4?1:-1];
typedef char HEADER_weight_Offset[FIELD_OFFSET(HEADER,weight)==16?1:-1];
typedef char HEADER_end_Offset[FIELD_OFFSET(HEADER,end)==18?1:-1];
typedef char HEADER_segments_Offset[FIELD_OFFSET(HEADER,segments)==20?1:-1];
typedef char HEADER_bones_Offset[FIELD_OFFSET(HEADER,bones)==24?1:-1];
typedef char REQUEST_bytes_Offset[FIELD_OFFSET(REQUEST,bytes)==0?1:-1];
typedef char REQUEST_alignment_Offset[FIELD_OFFSET(REQUEST,alignment)==4?1:-1];
typedef char DMAS_m_ab_Offset[FIELD_OFFSET(DMAS,m_ab)==4?1:-1];
typedef char DMAS_m_pb_Offset[FIELD_OFFSET(DMAS,m_pb)==12?1:-1];
// empty asms: hide the flag constants from cse/loop.c so each flag store keeps its own short-lived constant
// (case 0: param form; case 1: volatile, header read first, int result keeps the gcse table size of the game)
static inline ushort Flags(ushort v){__asm__("" : "+r"(v));return v;}
template <int V> static inline int Flags(){int v=V;__asm__ __volatile__("" : "+r"(v));return v;}
static inline void WriteVertex(U *destination,U value){
 float x=func_002599D8(&value);
 float y=func_002599E0(&value);
 float z=func_002599E8(&value);
 destination->f[0]=x;destination->f[1]=y;destination->f[2]=z;
}
/** @sly2-annot addr=0x16CF78
 * Purpose: Sub-glob type 3 loader: U16 shader + indexed vectors + strip bits; HYPOTHESIS cel edge strips
 * Info: proposed name LoadSubglobType3FromBrx (T1); TU glob
 * Notes: superseded: LoadSubglobType2FromBrx (r3_1539A8) | sub-glob type->loader correction (CONFLICTS #16,
 *   MESH_FORMAT, level_parser type switch 0x167748): v1 had the types permuted | level_parser
 *   CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r3_1539A8+meshes(MESH_FORMAT jal switch
 *   0x167750)+parse-verified
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/meshes-scene.md#2-globset-loadglobsetfrombrx-0x167320-matched-cparse,
 *   docs/formats/meshes-scene.md#types-1-3-byte-layouts-cparse-roles-h-except-where-noted
 * @end-sly2-annot */
extern "C" void func_0016CF78(P*p,D*desc,E*entry,PARAM*param,SPHERE*sphere,int groups,CBinaryInputStream*stream){
 register CBinaryInputStream *input asm("$23")=stream; // pin: the game keeps the stream in s7 for the whole function
 int i=0;
 int vertex;
 int index;
 VIFS vifs;
 MARK outer;
 vifs.AllocSw(0x2000);vifs.AddDmaCnt();
 int count=input->U8Read();desc->count=count;
 entry->bones=(BONE*)AllocateZero((char)count*sizeof(BONE));
 for(;i<desc->count;i++){
  BONE*bone=&entry->bones[i];bone->value=input->S32Read();bone->index=input->S16Read();
 }
 int base=0x1B;
 if(desc->count){desc->base=base;base+=desc->count*4;}
 int subCount=input->S16Read();
 QW*data;
 for(int part=0;part<subCount;){
  LEASE inner;
  int vertices;
  U*vectors;
  HEADER*header;
  char*weights;
  U*positions;
  vifs.UnpackHelper(UPK_V4_16,4,10,0,(uint**)&header);
  func_002333B4(header,0,32);
  switch(part){
  case 0:
   header->flags=Flags(1);
   if(subCount==1)header->flags|=8;
   if(param->kind!=2)break;
   header->flags|=0x400;
   break;
  case 1:
   {HEADER*h=header;h->flags=Flags<2>();}
   break;
  }
  header->base=base;
  OBJECT*object=func_001EC8F8(input->U16Read());
  vertices=input->U8Read();
  int vectorCount=input->U8Read();
  vectors=(U*)func_001AE318(REQUEST(vectorCount*sizeof(U),16));
  if(vectorCount>0){U*cursor=vectors;index=vectorCount;do{input->ReadVector((VECTOR*)cursor);--index;++cursor;}while(index);}
  char*boneWeights;
  int bones=desc->count<4?desc->count:4;
  if(desc->count){
   for(int b=0;b<4;b++){
    int bone=input->U8Read();HEADER*slot=(HEADER*)((uint)header+b*2);slot->bones[0]=(ushort)desc->base+bone*4;
   }
   int bytes=bones*vectorCount;
   boneWeights=(char*)Allocate(bytes);input->Read(bytes,boneWeights);
  }else boneWeights=0;
  byte*indices=(byte*)Allocate(vertices);input->Read(vertices,indices);
  int flagBytes=((vertices+31)/32)*4;
  uint*flags=(uint*)Allocate(flagBytes);input->Read(flagBytes,flags);
  uint*flagCursor=flags;
  uint mask=1;
  int current=base;
  for(int segment=0;segment<object->count;segment++){
   HEADER*slot=(HEADER*)((uint)header+segment*2);slot->segments[0]=current;
   current+=func_001EC688(object,segment,current,&vifs);
  }
  ++current;header->weight=current;current+=2*vertices;header->end=current;header->vertices=vertices;
  vifs.AddVifStcycl(1,2);
  if(desc->count){
   int small=bones<4;
   if(small){vifs.AddVifStrow(&D_002DEB60);vifs.AddVifStmask(D_002BE540[bones]);}
   vifs.UnpackHelper(D_002BE558[bones],vertices,header->weight,0,(uint**)&weights);
   if(small)vifs.AddVifStmask(0);
  }
  vifs.UnpackHelper(UPK_V4_32,vertices,header->weight+1,0,(uint**)&positions);
  vifs.AddVifStcycl(1,1);
  for(vertex=0;vertex<vertices;vertex++){
   index=indices[vertex];
   WriteVertex(&positions[vertex],vectors[index]);
   if(*flagCursor&mask)positions[vertex].f[3]=8421376.0f;else positions[vertex].f[3]=8417280.0f;
   mask<<=1;if(!mask){++flagCursor;mask=1;}
  }
  if(desc->count){
   for(vertex=0;vertex<vertices;vertex++){index=indices[vertex];func_001AE5A0(weights+vertex*bones,boneWeights+index*bones,bones);}
  }
  vifs.AddVifMscal(D_00000000);
  part++;
  vifs.AddVifMscal(D_00001910);vifs.AddVifMscal(D_00001A68);inner.Release();
 }
 vifs.EndDmaCnt();vifs.AddDmaRet();
 vifs.DetachCopySw(&desc->words,&data,&entry->chain);
 func_001699E8(p,desc,entry,sphere,groups,0,0,0,input);
}
