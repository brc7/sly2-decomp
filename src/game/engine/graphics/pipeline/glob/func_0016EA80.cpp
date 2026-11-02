#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef unsigned int uint;
struct TRANSFORM;
struct STREAM;
struct SO;
struct VE{short delta,pad;void(*readTransform)(SO*,STREAM*,TRANSFORM**);};
struct SO{char pad[0xC];VE*vt;char pad10[0x10];uint flags;char position[0x10];char list[0x10];int vectorCount;void*vectors;};
struct D{char channel,segments;int flags:8;char kind;char pad4[4];void*chain;char padC[4];int words;};
union FLAGS{uint word;struct{uint pad:11,feature:1,matrix:1,first:9,last:9,reserved:1;}bits;};
struct E{int value;TRANSFORM*transform;void*parent;FLAGS flags;char*data;uint base;char*segments;int word1C;SO*object;void*sphere;};
struct P{SO*owner;char pad4[0x20];int objectCount;SO**objects;char pad2C[12];int count;D*descriptors;E*entries;char pad44[0x14];int word58;};
struct LINK{int pad0;void*bounds;int pad8;SO*object;int next;};
struct REQUEST{int bytes,alignment;};
typedef char D_Size[sizeof(D)==20?1:-1];
typedef char E_Size[sizeof(E)==40?1:-1];
typedef char LINK_Size[sizeof(LINK)==20?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char FLAGS_Size[sizeof(FLAGS)==4?1:-1];
typedef char D_channel_Offset[FIELD_OFFSET(D,channel)==0?1:-1];
typedef char D_segments_Offset[FIELD_OFFSET(D,segments)==1?1:-1];
typedef char D_kind_Offset[FIELD_OFFSET(D,kind)==3?1:-1];
typedef char D_chain_Offset[FIELD_OFFSET(D,chain)==8?1:-1];
typedef char D_words_Offset[FIELD_OFFSET(D,words)==16?1:-1];
typedef char E_transform_Offset[FIELD_OFFSET(E,transform)==4?1:-1];
typedef char E_parent_Offset[FIELD_OFFSET(E,parent)==8?1:-1];
typedef char E_flags_Offset[FIELD_OFFSET(E,flags)==12?1:-1];
typedef char E_data_Offset[FIELD_OFFSET(E,data)==16?1:-1];
typedef char E_base_Offset[FIELD_OFFSET(E,base)==20?1:-1];
typedef char E_segments_Offset[FIELD_OFFSET(E,segments)==24?1:-1];
typedef char E_object_Offset[FIELD_OFFSET(E,object)==32?1:-1];
typedef char E_sphere_Offset[FIELD_OFFSET(E,sphere)==36?1:-1];
typedef char P_owner_Offset[FIELD_OFFSET(P,owner)==0?1:-1];
typedef char P_objectCount_Offset[FIELD_OFFSET(P,objectCount)==36?1:-1];
typedef char P_objects_Offset[FIELD_OFFSET(P,objects)==40?1:-1];
typedef char P_count_Offset[FIELD_OFFSET(P,count)==56?1:-1];
typedef char P_descriptors_Offset[FIELD_OFFSET(P,descriptors)==60?1:-1];
typedef char P_entries_Offset[FIELD_OFFSET(P,entries)==64?1:-1];
typedef char P_word58_Offset[FIELD_OFFSET(P,word58)==88?1:-1];
typedef char SO_vt_Offset[FIELD_OFFSET(SO,vt)==12?1:-1];
typedef char SO_flags_Offset[FIELD_OFFSET(SO,flags)==32?1:-1];
typedef char SO_position_Offset[FIELD_OFFSET(SO,position)==36?1:-1];
typedef char SO_list_Offset[FIELD_OFFSET(SO,list)==52?1:-1];
typedef char SO_vectorCount_Offset[FIELD_OFFSET(SO,vectorCount)==68?1:-1];
typedef char SO_vectors_Offset[FIELD_OFFSET(SO,vectors)==72?1:-1];
typedef char LINK_bounds_Offset[FIELD_OFFSET(LINK,bounds)==4?1:-1];
typedef char LINK_object_Offset[FIELD_OFFSET(LINK,object)==12?1:-1];
typedef char LINK_next_Offset[FIELD_OFFSET(LINK,next)==16?1:-1];
typedef char VE_delta_Offset[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char VE_readTransform_Offset[FIELD_OFFSET(VE,readTransform)==4?1:-1];
extern "C"{
extern int D_003DA790;
extern LINK D_003DA798[];
void*func_001AE238(REQUEST*,void*);
SO*func_001ED850(SO*);
void CopyRelocateDmaChain(D*,E*,int,void*,int,void*,void*);
void func_001AE5A0(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");
void ClearSle(int*);
void PrependSlEntry(void*,LINK*);
void func_001EC740(int,int,int,uint,uint);
}
static inline void*Copy(int bytes,void*source){REQUEST request;request.bytes=bytes;request.alignment=16;return func_001AE238(&request,source);}
static inline void ReadTransform(SO*object,STREAM*stream,TRANSFORM**output){VE*method=&object->vt[52];method->readTransform((SO*)((char*)object+method->delta),stream,output);}
static inline void CopyObjects(P*p){for(int i=0;i<p->objectCount;i++){if(p->objects[i]->flags&4){p->objects=(SO**)Copy(p->objectCount*4,p->objects);return;}}}
/** @sly2-annot addr=0x16EA80
 * Purpose: Clones a globset for a cloned SO: copies entries/segments, re-resolves transforms, swaps cloned
 *   objects, relocates packet copies
 * Info: proposed name CloneGlobset (T2); TU glob
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
extern "C" void func_0016EA80(P*p,SO*owner,STREAM*stream){
    int bytes;
    p->owner=owner;
    p->entries=(E*)Copy(p->count*sizeof(E),p->entries);
    p->word58=0;
    CopyObjects(p);
    for(int i=0;i<p->count;i++){
        D*descriptor=&p->descriptors[i];
        E*entry=&p->entries[i];
        ReadTransform(owner,stream,&entry->transform);
        entry->segments=(char*)Copy(descriptor->segments*0x50,entry->segments);
        for(int j=0;j<descriptor->segments;j++){uint address=(uint)entry->segments;address+=j*0x50;ReadTransform(owner,stream,(TRANSFORM**)(address+0xC));}
        int cloned=entry->object&&(entry->object->flags&4);
        if(cloned){
            SO*object=func_001ED850(entry->object);
            for(int j=0;j<p->objectCount;j++)if(p->objects[j]==entry->object){p->objects[j]=object;break;}
            entry->object=object;
        }
        if(!descriptor->chain&&!cloned)continue;
        int count=0;
        void*old=0;
        void*data=0;
        if(descriptor->flags&1){
            count=descriptor->words;old=entry->data;
            entry->data=(char*)Copy(count*16,old);data=entry->data;
        }else{
            if(entry->object){
                count=entry->object->vectorCount;
                old=data=entry->object->vectors;
                if(cloned){entry->object->vectors=Copy(count*16,data);data=entry->object->vectors;}
            }
            CopyRelocateDmaChain(descriptor,entry,descriptor->words,entry->data,count,data,old);
        }
        if(entry->object){
            bytes=count*16;
            int span=entry->flags.bits.last-entry->flags.bits.first;
            func_001AE5A0(&D_003DA798[D_003DA790],&D_003DA798[entry->flags.bits.first],span*sizeof(LINK));
            entry->flags.bits.first=D_003DA790;
            D_003DA790+=span;
            entry->flags.bits.last=D_003DA790;
            for(uint j=entry->flags.bits.first;j<entry->flags.bits.last;j++){
                LINK*link=&D_003DA798[j];
                if(cloned){link->bounds=&entry->object->position;link->object=entry->object;}
                ClearSle(&link->next);
                PrependSlEntry(link->object->list,link);
            }
            func_001EC740(entry->flags.bits.first,entry->flags.bits.last,bytes,(uint)old,(uint)data);
        }
    }
}
