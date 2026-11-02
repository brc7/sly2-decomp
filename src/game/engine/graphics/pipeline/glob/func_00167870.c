typedef unsigned int uint;
typedef unsigned long ulong;
struct META{float limit;long kind:4;ulong count:8;ulong channel:6;ulong feature:1;ulong index:13;};
struct RECORD{float position[3];float radius;struct META meta;void*link;int pad1C;};
struct GROUP{struct RECORD records[4];};
struct D{signed char channel;char pad1[7];void*chain;char padC[8];};
struct E{char pad[0xC];uint flags;void*matrix;int word;char pad18[0xC];struct RECORD*sphere;};
struct P{char pad[0x3C];struct D*descriptors;struct E*entries;void*localSpheres;struct GROUP*groups;};
struct STREAM;
struct REQUEST{int bytes,alignment;};
typedef char MetaSize[sizeof(struct META)==8?1:-1];
typedef char RecordSize[sizeof(struct RECORD)==0x20?1:-1];
typedef char EntrySize[sizeof(struct E)==0x28?1:-1];
short S16Read__18CBinaryInputStream(struct STREAM*) __asm__("S16Read__18CBinaryInputStream");
char S8Read__18CBinaryInputStream(struct STREAM*) __asm__("S8Read__18CBinaryInputStream");
struct VECTOR;
void ReadVector__18CBinaryInputStreamP6VECTOR(struct STREAM*,struct VECTOR*) __asm__("ReadVector__18CBinaryInputStreamP6VECTOR");
float F32Read__18CBinaryInputStream(struct STREAM*) __asm__("F32Read__18CBinaryInputStream");
unsigned char U8Read__18CBinaryInputStream(struct STREAM*) __asm__("U8Read__18CBinaryInputStream");
void*func_001AE2A0(struct REQUEST*);
static inline float Min(float a,float b){float r;__asm__("min.s %0,%1,%2":"=f"(r):"f"(a),"f"(b));return r;}
/** @sly2-annot addr=0x167870
 * Purpose: Reads groups of up to 4 distance/LOD records (kind, position, radius, limit<=1e10, count) per
 *   globset descriptor
 * Info: proposed name LoadPoseSetsFromBrx (T1); TU glob
 * Notes: superseded: LoadGlobsetLodsFromBrx (r3_1539A8) | name from the hand source of 0x167320
 *   (LoadPoseSetsFromBrx) + MESH_FORMAT "posesets"; v1 LoadGlobsetLodsFromBrx | level_parser CONFIRMED-parse:
 *   payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources:
 *   r3_1539A8+format-docs+parse-verified
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/meshes-scene.md#2-globset-loadglobsetfrombrx-0x167320-matched-cparse,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void func_00167870(struct P*p,struct STREAM*stream){
    int count=S16Read__18CBinaryInputStream(stream);
    if(count){
        struct REQUEST request;
        struct GROUP*groups;
        int i;
        request.bytes=count*sizeof(struct GROUP);request.alignment=0x10;
        groups=(struct GROUP*)func_001AE2A0(&request);
        for(i=0;i<count;i++){
            struct GROUP*group=&groups[i];
            uint j;
            for(j=0;j<4;j++){
                struct RECORD*record=&group->records[j];
                record->meta.kind=S8Read__18CBinaryInputStream(stream);
                if(record->meta.kind==-1){
                    for(;j<4;j++)group->records[j].meta.kind=-1;
                    break;
                }
                ReadVector__18CBinaryInputStreamP6VECTOR(stream,(struct VECTOR*)record->position);
                record->radius=F32Read__18CBinaryInputStream(stream);
                record->meta.limit=F32Read__18CBinaryInputStream(stream);
                record->meta.count=U8Read__18CBinaryInputStream(stream);
                record->meta.limit=Min(1.0e10f,record->meta.limit);
                switch(record->meta.kind){
                case 0:{
                    int index=S16Read__18CBinaryInputStream(stream);
                    struct D*descriptor=&p->descriptors[index];
                    struct E*entry=&p->entries[index];
                    if(entry->flags&0x1000){record->meta.kind=3;record->link=entry->matrix;}
                    record->meta.channel=descriptor->channel;
                    record->meta.feature=(entry->flags>>11)&1;
                    record->meta.index=index;
                    entry->sphere=record;
                    break;
                }
                case 1:
                    record->link=&groups[S16Read__18CBinaryInputStream(stream)];
                    break;
                }
            }
        }
        p->groups=groups;
    }
}
