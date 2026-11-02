#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
#include "vifs.h"
#include "bis.h"
struct OWNER{char pad[0x50];ulong flags;};
struct D{char kind,count;ushort flags;void*pad4;QW*chain;int base,words;};
struct BONE{int value;char pad4[4];int index;char padC[0x44];};
struct E{char pad[0x10];QW*chain;int pad14;BONE*bones;};
struct SPHERE;struct FACE;
struct OBJECT{char pad[0x1C];int count;void*segments;char pad24[8];};
struct HEADER{ushort flags,base,pad4,pad6,position,weights,color,end,extra,extra2;ushort segments[2],bones[4];};
struct REQUEST{int bytes,alignment;REQUEST(int cb,int a):bytes(cb),alignment(a){}};
extern "C"{
void*STACKSCOPE_ctor(void*);
void STACKSCOPE_ctor_1AE4A0(void*,int);
void*func_001AE2A0(const REQUEST&);
OBJECT*func_001EC8F8(int);
int func_001EC688(OBJECT*,int,int,VIFS*);
void func_001699E8(OWNER*,D*,E*,SPHERE*,int,int,const int*,FACE**,CBinaryInputStream*);
void*memset(void*,int,uint);
extern QW D_002DEB60;
extern uint D_002BE540[];
extern UPK D_002BE558[];
extern char D_00000000[],D_00001910[],D_00001A68[];
}
struct MARK{MARK(){STACKSCOPE_ctor(this);}~MARK(){STACKSCOPE_ctor_1AE4A0(this,2);}};
struct LEASE{LEASE(){STACKSCOPE_ctor(this);}void Release(){STACKSCOPE_ctor_1AE4A0(this,2);}};
static inline void*Allocate(int bytes){return func_001AE2A0(REQUEST(bytes,16));}
typedef char D_Size[sizeof(D)==20?1:-1];
typedef char BONE_Size[sizeof(BONE)==80?1:-1];
typedef char E_Size[sizeof(E)==28?1:-1];
typedef char OBJECT_Size[sizeof(OBJECT)==44?1:-1];
typedef char HEADER_Size[sizeof(HEADER)==32?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char OWNER_flags_Offset[FIELD_OFFSET(OWNER,flags)==80?1:-1];
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
typedef char HEADER_position_Offset[FIELD_OFFSET(HEADER,position)==8?1:-1];
typedef char HEADER_weights_Offset[FIELD_OFFSET(HEADER,weights)==10?1:-1];
typedef char HEADER_color_Offset[FIELD_OFFSET(HEADER,color)==12?1:-1];
typedef char HEADER_end_Offset[FIELD_OFFSET(HEADER,end)==14?1:-1];
typedef char HEADER_extra_Offset[FIELD_OFFSET(HEADER,extra)==16?1:-1];
typedef char HEADER_extra2_Offset[FIELD_OFFSET(HEADER,extra2)==18?1:-1];
typedef char HEADER_segments_Offset[FIELD_OFFSET(HEADER,segments)==20?1:-1];
typedef char HEADER_bones_Offset[FIELD_OFFSET(HEADER,bones)==24?1:-1];
typedef char REQUEST_bytes_Offset[FIELD_OFFSET(REQUEST,bytes)==0?1:-1];
typedef char REQUEST_alignment_Offset[FIELD_OFFSET(REQUEST,alignment)==4?1:-1];
/** @sly2-annot addr=0x16CAA0
 * Purpose: Sub-glob type 2 loader: U16 shader + vertices + colours; HYPOTHESIS cel/outline
 * Info: proposed name LoadSubglobType2FromBrx (T1); TU glob
 * Notes: superseded: LoadSubglobType1FromBrx (r3_1539A8) | sub-glob type->loader correction (CONFLICTS #16,
 *   MESH_FORMAT, level_parser type switch 0x167748): v1 had the types permuted | level_parser
 *   CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r3_1539A8+meshes(MESH_FORMAT jal switch
 *   0x167750)+parse-verified
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/meshes-scene.md#2-globset-loadglobsetfrombrx-0x167320-matched-cparse,
 *   docs/formats/meshes-scene.md#types-1-3-byte-layouts-cparse-roles-h-except-where-noted
 * @end-sly2-annot */
extern "C" void LoadSubglobType2FromBrx(OWNER*p,D*desc,E*entry,void*,SPHERE*sphere,int groups,CBinaryInputStream*stream){
 int i=0;
 p->flags|=1UL<<33;
 VIFS vifs;
 MARK outer;
 vifs.AllocSw(0x2000);
 vifs.AddDmaCnt();
 int count=stream->U8Read();
 desc->count=count;
 entry->bones=(BONE*)Allocate((char)count*sizeof(BONE));
 for(;i<desc->count;i++){
  BONE*bone=&entry->bones[i];
  bone->value=stream->S32Read();bone->index=stream->S16Read();
 }
 int base=0x1B;
 if(desc->count){desc->base=base;base+=desc->count*4;}
 int subCount=stream->S16Read();
 QW*data;
 for(int part=0;part<subCount;part++){
  LEASE inner;
  ushort*header;
  vifs.UnpackHelper(UPK_V4_16,4,10,0,(uint**)&header);
  memset(header,0,32);
  switch(part){case 0:header[0]|=1;if(subCount==1)header[0]|=8;break;case 1:header[0]|=2;break;}
  if(desc->count)header[0]|=4;
  int current=base;
  header[1]=current;
  OBJECT*object=func_001EC8F8(stream->U16Read());
  for(int segment=0;segment<object->count;segment++){
   HEADER*slot=(HEADER*)((uint)header+segment*2);slot->segments[0]=current;
   current+=func_001EC688(object,segment,current,&vifs);
  }
  int vertices=stream->U8Read();
  vifs.AddVifStcycl(1,2);
  vifs.AddVifUnpack(UPK_V3_32,vertices,stream,current+1);
  header[4]=current;current+=vertices*2;header[5]=current;
  if(desc->count){
   int bones=stream->U8Read();int k=0;
   for(;k<bones;k++){int bone=stream->U8Read();HEADER*slot=(HEADER*)((uint)header+k*2);slot->bones[0]=(ushort)desc->base+bone*4;}
   int small=bones<4;
   for(;k<4;k++){HEADER*slot=(HEADER*)((uint)header+k*2);slot->bones[0]=header[12];}
   if(small){vifs.AddVifStrow(&D_002DEB60);vifs.AddVifStmask(D_002BE540[bones]);}
   vifs.AddVifUnpack(D_002BE558[bones],vertices,stream,header[4]);
   if(small)vifs.AddVifStmask(0);
  }
  vifs.AddVifStcycl(1,1);
  int colors=stream->U8Read();
  vifs.AddVifUnpack(UPK_V4_8,colors,stream,current);
  header[6]=current;current+=colors;header[7]=current;current++;
  header[8]=header[9]=current;
  vifs.AddVifMscal(D_00000000);vifs.AddVifMscal(D_00001910);vifs.AddVifMscal(D_00001A68);inner.Release();
 }
 vifs.EndDmaCnt();vifs.AddDmaRet();
 vifs.DetachCopySw(&desc->words,&data,&entry->chain);
 func_001699E8(p,desc,entry,sphere,groups,0,0,0,stream);
}
