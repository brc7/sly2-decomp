// LoadSpriteDescriptor: reads one sprite descriptor from the stream into sprites[id] and builds its packet data.
// Kinds: 4 index, 8 nested children + word/float tables, 3 shader, 2 polygon (2x3 matrix + vertices),
// 6/7 textured rectangle (GIF packet; 7 also posts the owner's callback), 1 geometry records grouped
// into VU1 packets (microprogram 0x140).
// How it matched (Lilly's agent, T1615), from codex18fresh1 (17 diffs):
// - the scopes are real out-of-line STACKSCOPEs (0x1AE478 / 0x1AE4A0), which keeps &inner in s3;
// - owner is a const parameter: its spill slot is unchanging, so its reloads move above the stores;
// - the texture id is stored in both rectangle arms (a global pseudo: s2), float literals, natural loops,
//   bytes[n*3+k] indexing, packetCount / samples / groups update order;
// - j is declared in case 8 after the float count is read and reused by case 1's packet loop: its pseudo
//   number puts the gcse temp for j+1 between &packetCount and &packet, which gives their spill slots.
#include "gifs.h"
#include "vifs.h"
typedef int qword __attribute__((mode(TI)));
struct BIS;
struct OWNER {char pad[0xC4];HEAP *heap;};
struct ROOT;
struct WORLD;
struct SPRITE;
struct M3 {qword r[3];};
struct M23 {float a[6];} __attribute__((aligned(16)));
struct PAIR {short x,y;};
struct RAWCOLOR {uchar r,g,b,a;};
struct VERTEX {uchar data[0x18];};
struct NESTED {int count;SPRITE *children;int floatCount;float *floats;int wordCount;uint *words;};
struct SHADER {int tag,mode,id;uint slot;float size,value,scale;int flag,special;};
struct POLYGON {M23 matrix;ushort count,pad22;VERTEX *vertices;};
struct EFFECT {int id;void *handle;int count;QW *packet;};
struct PICTURE {int id,count;QW *packet;};
struct GEOMETRY {ushort count,groups;int samples,packetCount;QW *packet;};
union PAYLOAD {int index;NESTED nested;SHADER shader;POLYGON polygon;EFFECT effect;PICTURE picture;GEOMETRY geometry;};
struct SPRITE {int id,kind;ROOT *root;int fieldC;PAYLOAD data;};
struct RECORD {int kind,id;RAWCOLOR color;uchar count,padD[3];int budget;uchar format,pad15[3];PAIR *coordinates;uchar extra,pad1D[3];uchar *extras;};
struct GROUP {int count,budget,samples;RECORD *begin,*end;};
// Native two-word member callback, stored as {delta,index,function}.
typedef void (OWNER::*PMF)(SPRITE *);
extern "C" {
int U16Read__18CBinaryInputStream(BIS *);
uint U32Read__18CBinaryInputStream(BIS *);
int U8Read__18CBinaryInputStream(BIS *);
int S16Read__18CBinaryInputStream(BIS *);
int S8Read__18CBinaryInputStream(BIS *);
int S32Read__18CBinaryInputStream(BIS *);
float F32Read__18CBinaryInputStream(BIS *);
void ReadMatrix__18CBinaryInputStreamP7MATRIX3(BIS *,M3 *);
void Read__18CBinaryInputStreamiPv(BIS *,int,void *);
void *PvAllocClear__FP4HEAPii(HEAP *,int,int);
void *func_001AE378(ALLOCREQ *);
void LoadSpriteTable(OWNER *,BIS *,ROOT *,int *,SPRITE **);
void PostSwCallback(WORLD *,OWNER *,PMF,SPRITE *);
}
// The current stack-allocation scope (out of line: 0x1AE478 / 0x1AE4A0)
struct STACKSCOPE {STACKSCOPE();~STACKSCOPE();};
extern int D_002C40A4;
extern PMF D_002A8B08;
extern WORLD D_003DD490;
extern QW D_002A2E10;
extern char D_00000650[],D_00000140[];
static inline void *Acquire(ALLOCREQ *request,int bytes,int alignment){request->cb=bytes;request->cbAlign=alignment;return func_001AE378(request);}
static inline void CopyXY(M23 &dst,const M3 &src){
 const float *a=(const float *)&src.r[0],*b=(const float *)&src.r[1],*c=(const float *)&src.r[2];
 dst.a[0]=a[0];dst.a[1]=a[1];dst.a[2]=b[0];dst.a[3]=b[1];dst.a[4]=c[0];dst.a[5]=c[1];
}
/** @sly2-annot addr=0x2149D8
 * Purpose: read one sprite descriptor (kinds 1 geometry,2 polygon,3 shader,4 index,6/7 picture,8 nested) and
 *   build packets
 * Info: proposed name LoadSpriteDescriptor (T1); TU hud
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7c_hud+parse-verified
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/formats/level-objects.md#8-widget-payload-loadwidgetfrombrx-0x212298-cparse,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void LoadSpriteDescriptor(OWNER *const owner,BIS *stream,ROOT *root,int,SPRITE *sprites){
 STACKSCOPE outer;M3 raw;
 int id=U16Read__18CBinaryInputStream(stream);SPRITE *sprite=&sprites[id];sprite->root=root;sprite->id=id;
 sprite->kind=U32Read__18CBinaryInputStream(stream);
 switch(sprite->kind){
 case 0:case 5:default:break;
 case 4:sprite->data.index=U16Read__18CBinaryInputStream(stream);break;
 case 8:
  D_002C40A4=1;
  LoadSpriteTable(owner,stream,root,&sprite->data.nested.count,&sprite->data.nested.children);
  sprite->data.nested.wordCount=U32Read__18CBinaryInputStream(stream);
  sprite->data.nested.words=(uint *)PvAllocClear__FP4HEAPii(owner->heap,sprite->data.nested.wordCount*4,0x13);
  int i;
  for(i=0;i<sprite->data.nested.wordCount;i++)sprite->data.nested.words[i]=U32Read__18CBinaryInputStream(stream);
  sprite->data.nested.floatCount=U32Read__18CBinaryInputStream(stream);
  int j;
  sprite->data.nested.floats=(float *)PvAllocClear__FP4HEAPii(owner->heap,sprite->data.nested.floatCount*4,0x13);
  for(j=0;j<sprite->data.nested.floatCount;j++)sprite->data.nested.floats[j]=F32Read__18CBinaryInputStream(stream);
  D_002C40A4=0;break;
 case 3:{
  sprite->data.shader.tag=U32Read__18CBinaryInputStream(stream);sprite->data.shader.slot=U32Read__18CBinaryInputStream(stream);
  sprite->data.shader.size=F32Read__18CBinaryInputStream(stream);sprite->data.shader.scale=F32Read__18CBinaryInputStream(stream);sprite->data.shader.value=F32Read__18CBinaryInputStream(stream);
  sprite->data.shader.flag=S8Read__18CBinaryInputStream(stream);sprite->data.shader.mode=S8Read__18CBinaryInputStream(stream);
  if(sprite->data.shader.mode==1)sprite->data.shader.id=S32Read__18CBinaryInputStream(stream);else if(sprite->data.shader.mode==0)sprite->data.shader.id=S32Read__18CBinaryInputStream(stream);
  if(sprite->data.shader.mode==1&&sprite->data.shader.id==0x2F2)sprite->data.shader.special=sprite->data.shader.mode;
  else sprite->data.shader.special=0;
  break;
 }
 case 2:{
  ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,&raw);M3 matrix=raw;CopyXY(sprite->data.polygon.matrix,matrix);
  sprite->data.polygon.count=U16Read__18CBinaryInputStream(stream);
  sprite->data.polygon.vertices=(VERTEX *)PvAllocClear__FP4HEAPii(owner->heap,sprite->data.polygon.count*sizeof(VERTEX),0x14);
  Read__18CBinaryInputStreamiPv(stream,sprite->data.polygon.count*sizeof(VERTEX),sprite->data.polygon.vertices);break;
 }
 case 6:case 7:{
  PAIR first,second;
  first.x=S16Read__18CBinaryInputStream(stream);first.y=S16Read__18CBinaryInputStream(stream);second.x=S16Read__18CBinaryInputStream(stream);second.y=S16Read__18CBinaryInputStream(stream);
  int texture=S32Read__18CBinaryInputStream(stream);
  STACKSCOPE inner;GIFS gif;
  gif.AllocSw(16);gif.AddPrimPack(0,1,0xE);
  if(sprite->kind==6){sprite->data.effect.id=texture;sprite->data.effect.handle=0;gif.PackAD(0x47,0x5000F);gif.PackAD(0x42,0x80000000A8L);}
  else{sprite->data.picture.id=texture;gif.PackAD(0x42,0x44);gif.PackAD(0x47,0x50000);}
  gif.AddPrimPack(0x54,9,0x525252521L);gif.PackRGBA(0x80808080);
  if(sprite->kind==6){
   gif.PackSTQ(0.0f,0.0f,0.0f);gif.PackXYZ(first.x,first.y,0);
   gif.PackSTQ(1.0f,0.0f,0.0f);gif.PackXYZ(second.x,first.y,0);
   gif.PackSTQ(0.0f,1.0f,0.0f);gif.PackXYZ(first.x,second.y,0);
   gif.PackSTQ(1.0f,1.0f,0.0f);gif.PackXYZ(second.x,second.y,0);
   gif.AddPrimPack(0,1,0xE);gif.PackAD(0x42,0x44);gif.PackAD(0x47,0x50000);
  }
  else{
   gif.PackSTQ(0.0f,1.0f,0.0f);gif.PackXYZ(first.x,first.y,0);
   gif.PackSTQ(1.0f,1.0f,0.0f);gif.PackXYZ(second.x,first.y,0);
   gif.PackSTQ(0.0f,0.0f,0.0f);gif.PackXYZ(first.x,second.y,0);
   gif.PackSTQ(1.0f,0.0f,0.0f);gif.PackXYZ(second.x,second.y,0);
  }
  gif.EndPrim(1);
  VIFS vif;vif.AllocSw(20);vif.AddDmaCnt();
  vif.AddVifUnpack(UPK_V4_32,((uint)gif.m_pb-(uint)gif.m_ab)>>4,gif.m_ab,7);
  vif.AddVifMscal(D_00000650);vif.EndDmaCnt();vif.AddDmaRet();
  if(sprite->kind==6)vif.DetachCopySw(owner->heap,&sprite->data.effect.count,0,&sprite->data.effect.packet);
  else{vif.DetachCopySw(owner->heap,&sprite->data.picture.count,0,&sprite->data.picture.packet);PostSwCallback(&D_003DD490,owner,D_002A8B08,sprite);}
  break;
 }
 case 1:{
  sprite->data.geometry.count=U16Read__18CBinaryInputStream(stream);
  RECORD *records;GROUP *groups;int i;
  { ALLOCREQ request;
  records=(RECORD *)Acquire(&request,sprite->data.geometry.count*sizeof(RECORD),16);
  groups=(GROUP *)Acquire(&request,sprite->data.geometry.count*sizeof(GROUP),16);
  for(i=0;i<sprite->data.geometry.count;i++){
   RECORD *record=&records[i];
   record->count=U8Read__18CBinaryInputStream(stream);record->format=U8Read__18CBinaryInputStream(stream);
   record->kind=U8Read__18CBinaryInputStream(stream);record->id=U8Read__18CBinaryInputStream(stream);record->extra=U8Read__18CBinaryInputStream(stream);
   record->budget=record->count+2;
   record->coordinates=(PAIR *)Acquire(&request,record->count*sizeof(PAIR),16);
   record->extras=(uchar *)Acquire(&request,record->count,16);
   for(int k=0;k<record->count;k++){
    PAIR *coordinate=&record->coordinates[k];*(uint *)&record->color=U32Read__18CBinaryInputStream(stream);
    record->extras[k]=U8Read__18CBinaryInputStream(stream);
    coordinate->x=S16Read__18CBinaryInputStream(stream);coordinate->y=S16Read__18CBinaryInputStream(stream);
   }
  }
  }
  sprite->data.geometry.groups=0;sprite->data.geometry.packetCount=0;sprite->data.geometry.samples=0;
  for(i=0;i<sprite->data.geometry.count;){
   GROUP *group=&groups[sprite->data.geometry.groups];
   group->count=0;group->samples=0;group->budget=0;group->begin=&records[i];
   while(i<sprite->data.geometry.count&&group->budget+records[i].budget<0xBD){
    group->budget+=records[i].budget;group->samples+=records[i].count;group->count++;i++;group->end=&records[i];
   }
   int estimate=(group->samples+1)/4+group->samples/16+(group->count*3+3)/4+3;
   sprite->data.geometry.packetCount+=estimate;
   sprite->data.geometry.samples+=group->samples;
   sprite->data.geometry.groups++;
  }
  VIFS vif;j=0;vif.AllocSw(sprite->data.geometry.packetCount*2);vif.AddDmaCnt();vif.AddVifStrow(&D_002A2E10);
  for(;j<sprite->data.geometry.groups;j++){
   GROUP *group=&groups[j];
   // V2_32 copies eight bytes; only the first header component is written.
   int n=0;QW header;header.an[0]=group->count;vif.AddVifUnpack(UPK_V2_32,1,&header,7);vif.AddVifStcycl(1,2);
   uint *bytes,*colors;
   vif.UnpackHelper(UPK_V3_8,group->count,8,0,&bytes);vif.UnpackHelper(UPK_V4_8,group->count,9,0,&colors);
   int address=group->count*2+8;
   vif.AddVifStcycl(1,1);
   int base=address;
   vif.AddVifStmask(0x50505050);
   for(RECORD *record=group->begin;record!=group->end;record++){
    ((uchar *)bytes)[n*3]=record->kind;((uchar *)bytes)[n*3+1]=record->count;((uchar *)bytes)[n*3+2]=record->format;
    *(RAWCOLOR *)&colors[n]=record->color;n++;
    vif.AddVifUnpack((UPK)(UPK_V2_16+16),record->count,record->coordinates,address);address+=record->count;
   }
   vif.AddVifStmask(0x3F3F3F3F);address=base;
   for(RECORD *record=group->begin;record!=group->end;record++){
    if(record->extra)vif.AddVifUnpack((UPK)(UPK_S8+16),record->count,record->extras,address);
    address+=record->count;
   }
   vif.AddVifStmask(0);vif.AddVifMscal(D_00000140);
  }
  vif.EndDmaCnt();vif.AddDmaRet();vif.DetachCopySw(owner->heap,&sprite->data.geometry.packetCount,0,&sprite->data.geometry.packet);break;
 }
 }
}
