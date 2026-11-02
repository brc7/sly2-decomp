// Deserialize sprite groups, reusable frames and their animation keys.
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
struct BIS;
struct HEAP;
struct ROOT;
struct GROUP;
struct FRAME {char fields[0x70];};
struct V3 {float x,y,z;} __attribute__((aligned(16)));
struct M3 {V3 row[3];};
struct M23 {float a[6];} __attribute__((aligned(16)));
struct RGBA_RAW {uchar r,g,b,a;};
union RGBA {uint word;RGBA_RAW raw;RGBA(uint value){word=value;}} __attribute__((aligned(16)));
struct COLORS {RGBA_RAW first,second;};
struct ITEM {ushort a0,a2,a4;short a6;char pad8[8];M23 matrix;uchar flags,mode;char pad32[2];int id;char pad38[8];};
struct TAG {uint kind;union {int signedBits;uint bits;} value;};
struct KEY {float time;uchar itemCount,colorCount,shortCount,byteCount;uchar *bytes;ITEM *items;COLORS *colors;ushort *shorts;uchar tagCount;char pad19[3];TAG *tags;};
struct ANIM {ushort count,pad2;KEY *keys;float duration;ROOT *owner;GROUP *group;int id;};
struct REUSE {int count;FRAME *frames;short index,phase;void *previous;float elapsed;int pad14;};
struct GROUP {ushort animCount,reuseCount;ANIM *animations;REUSE *reuse;char padC[0x14];int index;ROOT *owner;int first;int pad2C;};
struct ROOT {char pad[0x80];ushort groupCount,pad82;GROUP *groups;};
struct OWNER {char pad[0xC4];HEAP *heap;};
extern "C" {
int U16Read__18CBinaryInputStream(BIS *);
uint U32Read__18CBinaryInputStream(BIS *);
int U8Read__18CBinaryInputStream(BIS *);
int S16Read__18CBinaryInputStream(BIS *);
int S32Read__18CBinaryInputStream(BIS *);
float F32Read__18CBinaryInputStream(BIS *);
void ReadMatrix__18CBinaryInputStreamP7MATRIX3(BIS *,M3 *);
void *PvAllocClear__FP4HEAPii(HEAP *,int,int);
void InitSpriteItem(FRAME *);
}
static inline void CopyXY(M23 *dst,const M3 &src){
 dst->a[0]=src.row[0].x;dst->a[1]=src.row[0].y;
 dst->a[2]=src.row[1].x;dst->a[3]=src.row[1].y;
 dst->a[4]=src.row[2].x;dst->a[5]=src.row[2].y;
}
/** @sly2-annot addr=0x214460
 * Purpose: deserialize sprite groups, reusable frames and animation keys
 * Info: proposed name LoadSpriteGroups (T1); TU hud
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7c_hud+parse-verified
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/formats/level-objects.md#8-widget-payload-loadwidgetfrombrx-0x212298-cparse,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void LoadSpriteGroups(OWNER *owner,BIS *stream,ROOT *root){
 int i,j,k,m;
 root->groupCount=U16Read__18CBinaryInputStream(stream);
 root->groups=(GROUP *)PvAllocClear__FP4HEAPii(owner->heap,(int)root->groupCount*0x30,0x12);
 for(i=0;i<root->groupCount;i++){
  GROUP *group=&root->groups[i];
  group->first=(i==0);group->owner=root;
  group->reuseCount=U16Read__18CBinaryInputStream(stream);
  group->reuse=(REUSE *)PvAllocClear__FP4HEAPii(owner->heap,(int)group->reuseCount*0x18,0x12);
  int count=U16Read__18CBinaryInputStream(stream);
  for(j=0;j<group->reuseCount;){
   int next=j+1;
   REUSE *reuse=&group->reuse[j];
   reuse->previous=0;reuse->index=-1;reuse->phase=0;reuse->elapsed=0.0f;
   reuse->count=count;
   reuse->frames=(FRAME *)PvAllocClear__FP4HEAPii(owner->heap,count*0x70,0x10);
   for(m=0;m<reuse->count;m++)InitSpriteItem(&reuse->frames[m]);
   j=next;
  }
  group->animCount=U16Read__18CBinaryInputStream(stream);
  group->animations=(ANIM *)PvAllocClear__FP4HEAPii(owner->heap,(int)group->animCount*0x18,0x12);
  group->index=-1;
  for(j=0;j<group->animCount;j++){
   ANIM *animation=&group->animations[j];
   animation->owner=root;animation->group=group;
   animation->duration=F32Read__18CBinaryInputStream(stream);
   animation->id=S32Read__18CBinaryInputStream(stream);
   animation->count=U16Read__18CBinaryInputStream(stream);
   animation->keys=(KEY *)PvAllocClear__FP4HEAPii(owner->heap,(int)animation->count*0x20,0x12);
   for(k=0;k<animation->count;k++){
    KEY *key=&animation->keys[k];
    key->time=F32Read__18CBinaryInputStream(stream);
    key->itemCount=U8Read__18CBinaryInputStream(stream);
    key->items=(ITEM *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->itemCount*0x40,0x12);
    for(m=0;m<key->itemCount;m++){
     ITEM *item=&key->items[m];
     item->a0=U16Read__18CBinaryInputStream(stream);item->a2=U16Read__18CBinaryInputStream(stream);item->a4=U16Read__18CBinaryInputStream(stream);item->a6=S16Read__18CBinaryInputStream(stream);
     item->flags=U8Read__18CBinaryInputStream(stream);item->mode=U8Read__18CBinaryInputStream(stream);item->id=S32Read__18CBinaryInputStream(stream);
     M3 raw;ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,&raw);M3 matrix=raw;CopyXY(&item->matrix,matrix);
    }
    key->colorCount=U8Read__18CBinaryInputStream(stream);
    key->colors=(COLORS *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->colorCount*8,0x12);
    for(m=0;m<key->colorCount;m++){
     COLORS *colors=&key->colors[m];
     RGBA second(U32Read__18CBinaryInputStream(stream));RGBA first(U32Read__18CBinaryInputStream(stream));
     colors->second=*(RGBA_RAW *)&second;colors->first=*(RGBA_RAW *)&first;
    }
    key->shortCount=U8Read__18CBinaryInputStream(stream);
    key->shorts=(ushort *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->shortCount*2,0x12);
    for(m=0;m<key->shortCount;m++){
     ushort *dst=&key->shorts[m];*dst=U16Read__18CBinaryInputStream(stream);
    }
    key->byteCount=U8Read__18CBinaryInputStream(stream);
    key->bytes=(uchar *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->byteCount,0x12);
    for(int byteIndex=0;byteIndex<key->byteCount;byteIndex++)key->bytes[byteIndex]=U8Read__18CBinaryInputStream(stream);
    key->tagCount=U8Read__18CBinaryInputStream(stream);
    key->tags=(TAG *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->tagCount*8,0x12);
    for(m=0;m<key->tagCount;m++){
     key->tags[m].kind=U8Read__18CBinaryInputStream(stream);
     if(key->tags[m].kind<2)key->tags[m].value.signedBits=S32Read__18CBinaryInputStream(stream);
     else key->tags[m].value.bits=(uint)S32Read__18CBinaryInputStream(stream);
    }
   }
  }
 }
}
