#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
#include "vifs.h"
#include "gifs.h"
#include "bis.h"
struct P;struct SPHERE;struct FACE;
struct ENV{char pad[0x240];float width,height,fade;};
struct PARAM{float value;int pad,kind;};
struct D{char channel,count;ushort flags;float radius;QW*chain;int base,words;};
struct BONE{int value;char pad4[4];int index;char padC[0x44];};
struct E{char pad[0x10];QW*chain;int base;BONE*bones;};
struct HEADER{ushort flags,base,weight,pad6,position,positionEnd,color,end,bones[4];};
struct REQUEST{int bytes,alignment;REQUEST(int cb,int a):bytes(cb),alignment(a){}};
extern "C"{
void*func_001AE478(void*);
void func_001AE4A0(void*,int);
void*func_001AE2A0(const REQUEST&);
void func_0014E270(ENV*,float*,float);
void*func_002333B4(void*,int,uint);
void func_001699E8(P*,D*,E*,SPHERE*,int,int,const int*,FACE**,CBinaryInputStream*);
extern ENV D_002DDF40;
extern uint D_002DEB50;
extern QW D_002DEB60;
extern uint D_002BE540[];
extern UPK D_002BE558[];
extern char D_00000000[],D_00000738[],D_000007F0[];
}
struct MARK{MARK(){func_001AE478(this);}~MARK(){func_001AE4A0(this,2);}};
struct LEASE{~LEASE(){}void Acquire(){func_001AE478(this);}void Release(){func_001AE4A0(this,2);}};
static inline void*Allocate(int bytes){return func_001AE2A0(REQUEST(bytes,16));}
typedef char PARAM_Size[sizeof(PARAM)==12?1:-1];
typedef char D_Size[sizeof(D)==20?1:-1];
typedef char BONE_Size[sizeof(BONE)==80?1:-1];
typedef char E_Size[sizeof(E)==28?1:-1];
typedef char HEADER_Size[sizeof(HEADER)==24?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char GIFS_Size[sizeof(GIFS)==40?1:-1];
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
typedef char HEADER_flags_Offset[FIELD_OFFSET(HEADER,flags)==0?1:-1];
typedef char HEADER_base_Offset[FIELD_OFFSET(HEADER,base)==2?1:-1];
typedef char HEADER_weight_Offset[FIELD_OFFSET(HEADER,weight)==4?1:-1];
typedef char HEADER_position_Offset[FIELD_OFFSET(HEADER,position)==8?1:-1];
typedef char HEADER_positionEnd_Offset[FIELD_OFFSET(HEADER,positionEnd)==10?1:-1];
typedef char HEADER_color_Offset[FIELD_OFFSET(HEADER,color)==12?1:-1];
typedef char HEADER_end_Offset[FIELD_OFFSET(HEADER,end)==14?1:-1];
typedef char HEADER_bones_Offset[FIELD_OFFSET(HEADER,bones)==16?1:-1];
typedef char REQUEST_bytes_Offset[FIELD_OFFSET(REQUEST,bytes)==0?1:-1];
typedef char REQUEST_alignment_Offset[FIELD_OFFSET(REQUEST,alignment)==4?1:-1];
typedef char DMAS_m_ab_Offset[FIELD_OFFSET(DMAS,m_ab)==4?1:-1];
typedef char DMAS_m_pb_Offset[FIELD_OFFSET(DMAS,m_pb)==12?1:-1];
typedef char ENV_width_Offset[FIELD_OFFSET(ENV,width)==576?1:-1];
typedef char ENV_height_Offset[FIELD_OFFSET(ENV,height)==580?1:-1];
typedef char ENV_fade_Offset[FIELD_OFFSET(ENV,fade)==584?1:-1];
/** @sly2-annot addr=0x16E438
 * Purpose: Sub-glob type 1 loader: vertices + colours, no shader (GIF prims); HYPOTHESIS shadow/cel-no-shader
 * Info: proposed name LoadSubglobType1FromBrx (T1); TU glob
 * Notes: superseded: LoadSubglobType3FromBrx (r3_1539A8) | sub-glob type->loader correction (CONFLICTS #16,
 *   MESH_FORMAT, level_parser type switch 0x167748): v1 had the types permuted | level_parser
 *   CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r3_1539A8+meshes(MESH_FORMAT jal switch
 *   0x167750)+parse-verified
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/meshes-scene.md#2-globset-loadglobsetfrombrx-0x167320-matched-cparse,
 *   docs/formats/meshes-scene.md#types-1-3-byte-layouts-cparse-roles-h-except-where-noted
 * @end-sly2-annot */
extern "C" void func_0016E438(P*p,D*desc,E*entry,PARAM*param,SPHERE*sphere,int groups,CBinaryInputStream*stream){
 int i=0;
 VIFS vifs;
 MARK outer;
 vifs.AllocSw(0x2000);vifs.AddDmaCnt();
 float*environment;
 vifs.UnpackHelper(UPK_V4_32,1,9,0,(uint**)&environment);
 func_0014E270(&D_002DDF40,environment,param->value);
 int count=stream->U8Read();desc->count=count;
 entry->bones=(BONE*)Allocate((char)count*sizeof(BONE));
 for(;i<desc->count;i++){
  BONE*bone=&entry->bones[i];
  bone->value=stream->S32Read();bone->index=stream->S16Read();
 }
 int base=0x22;
 GIFS gifs;
 LEASE inner;
 gifs.AllocSw(8);
 gifs.AddPrimPack(0,2,0x1EUL);gifs.PackAD(0x42L,0x44L);gifs.PackRGBA(D_002DEB50);gifs.EndPrim(1);
 vifs.AddVifUnpack(UPK_V4_32,((uint)gifs.m_pb-(uint)gifs.m_ab)>>4,gifs.m_ab,0xD);
 gifs.Reset();
 gifs.AddPrimPack(0xC1,3,0x44EUL);gifs.PackAD(0x47L,0x73001L);
 gifs.PackXYZF(0,0,0,0);gifs.PackXYZF(0,0,0,0);gifs.EndPrim(0);
 gifs.AddPrimPack(1,3,0x44EUL);gifs.PackAD(0x47L,0x72001L);
 gifs.PackXYZF(0,0,0,0);gifs.PackXYZF(0,0,0,0);gifs.EndPrim(1);
 vifs.AddVifUnpack(UPK_V4_32,2,gifs.m_ab,0x12);
 vifs.AddVifUnpack(UPK_V4_32,2,gifs.m_ab+0x40,0x16);
 vifs.AddVifUnpack(UPK_V4_32,2,gifs.m_ab,0x1A);
 vifs.AddVifUnpack(UPK_V4_32,2,gifs.m_ab+0x40,0x1E);
 if(desc->count){desc->base=base;base+=desc->count*4;}
 int subCount=stream->S16Read();
 QW*data;
 for(int part=0;part<subCount;){
  {LEASE*lease=&inner;lease->Acquire();}
  ushort*header;
  vifs.UnpackHelper(UPK_V4_16,3,10,0,(uint**)&header);
  func_002333B4(header,0,24);
  switch(part){case 0:header[0]|=1;break;case 1:header[0]|=2;break;}
  int current=base;
  header[1]=current;
  int nextPart=part+1;
  int vertices=stream->U8Read();
  vifs.AddVifUnpack(UPK_V3_32,vertices,stream,current);
  header[4]=current;current+=vertices;header[5]=current;
  if(desc->count){
   part=stream->U8Read();int k=0;
   for(;k<part;k++){int bone=stream->U8Read();HEADER*slot=(HEADER*)((uint)header+k*2);slot->bones[0]=(ushort)desc->base+bone*4;}
   int next=current+vertices;
   int small=part<4;
   for(;k<4;k++){HEADER*slot=(HEADER*)((uint)header+k*2);slot->bones[0]=header[8];}
   if(small){vifs.AddVifStrow(&D_002DEB60);vifs.AddVifStmask(D_002BE540[part]);}
   vifs.AddVifUnpack(D_002BE558[part],vertices,stream,current);
   header[2]=current;current=next;
   if(small)vifs.AddVifStmask(0);
  }
  part=nextPart;int colors=stream->U8Read();
  vifs.AddVifUnpack(UPK_V4_8,colors,stream,current);
  header[6]=current;current+=colors;header[7]=current;
  vifs.AddVifMscal(D_00000000);vifs.AddVifMscal(D_00000738);vifs.AddVifMscal(D_000007F0);
  {LEASE*lease=&inner;lease->Release();}
 }
 vifs.EndDmaCnt();vifs.AddDmaRet();
 vifs.DetachCopySw(&desc->words,&data,&entry->chain);
 func_001699E8(p,desc,entry,sphere,groups,0,0,0,stream);
}
