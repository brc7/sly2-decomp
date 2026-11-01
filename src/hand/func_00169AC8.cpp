#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
struct M4{qword rows[4];};
struct D{char channel,segments;unsigned char flags;char kind;char pad4[4];void*chain;char padC[4];int words;};
struct TRANSFORM{char pad[0x40];M4 matrix;};
union FLAGS{uint word;struct{uint pad:11,feature:1,other:20;} bits;};
struct E{int value;TRANSFORM*transform;M4*parent;FLAGS flags;char*data;uint base;char pad18[12];void*sphere;};
struct SPHERE{float center[3],radius,limit;uint metadata;};
struct SO;
struct VE{short delta,pad;TRANSFORM*(*fn)(SO*,int);};
struct SO{char pad[0xC];VE*vt;};
struct P{SO*owner;};
struct REQUEST{int bytes,alignment;REQUEST(int cb,int a):bytes(cb),alignment(a){}};
struct MARK;
struct STREAM;
struct LINK{short first,second;unsigned char kind,pad;};
struct LINKS{int count;LINK*links;};
struct BINDING{int count;LINKS*sets;int*indices;};
struct FACE{int count,pad4;uint flags;char padC[8];int*indices;char pad18[12];int first,second,pad2C,kind;char pad34[12];};
typedef char MatrixSize[sizeof(M4)==64?1:-1];
typedef char DescriptorSize[sizeof(D)==20?1:-1];
typedef char EntrySize[sizeof(E)==40?1:-1];
typedef char SphereSize[sizeof(SPHERE)==24?1:-1];
typedef char FaceSize[sizeof(FACE)==64?1:-1];
typedef char LinkSize[sizeof(LINK)==6?1:-1];
typedef char LinkSetSize[sizeof(LINKS)==8?1:-1];
typedef char BindingSize[sizeof(BINDING)==12?1:-1];
typedef char VirtualEntrySize[sizeof(VE)==8?1:-1];
typedef char D_flags_Offset[FIELD_OFFSET(D,flags)==2?1:-1];
typedef char D_kind_Offset[FIELD_OFFSET(D,kind)==3?1:-1];
typedef char D_chain_Offset[FIELD_OFFSET(D,chain)==8?1:-1];
typedef char D_words_Offset[FIELD_OFFSET(D,words)==16?1:-1];
typedef char E_transform_Offset[FIELD_OFFSET(E,transform)==4?1:-1];
typedef char E_parent_Offset[FIELD_OFFSET(E,parent)==8?1:-1];
typedef char E_flags_Offset[FIELD_OFFSET(E,flags)==12?1:-1];
typedef char E_data_Offset[FIELD_OFFSET(E,data)==16?1:-1];
typedef char E_base_Offset[FIELD_OFFSET(E,base)==20?1:-1];
typedef char E_sphere_Offset[FIELD_OFFSET(E,sphere)==36?1:-1];
typedef char SPHERE_radius_Offset[FIELD_OFFSET(SPHERE,radius)==12?1:-1];
typedef char SPHERE_limit_Offset[FIELD_OFFSET(SPHERE,limit)==16?1:-1];
typedef char SPHERE_metadata_Offset[FIELD_OFFSET(SPHERE,metadata)==20?1:-1];
typedef char TRANSFORM_matrix_Offset[FIELD_OFFSET(TRANSFORM,matrix)==64?1:-1];
typedef char SO_vt_Offset[FIELD_OFFSET(SO,vt)==12?1:-1];
typedef char VE_delta_Offset[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char VE_fn_Offset[FIELD_OFFSET(VE,fn)==4?1:-1];
typedef char FACE_flags_Offset[FIELD_OFFSET(FACE,flags)==8?1:-1];
typedef char FACE_indices_Offset[FIELD_OFFSET(FACE,indices)==20?1:-1];
typedef char FACE_first_Offset[FIELD_OFFSET(FACE,first)==36?1:-1];
typedef char FACE_second_Offset[FIELD_OFFSET(FACE,second)==40?1:-1];
typedef char FACE_kind_Offset[FIELD_OFFSET(FACE,kind)==48?1:-1];
typedef char LINKS_links_Offset[FIELD_OFFSET(LINKS,links)==4?1:-1];
typedef char BINDING_sets_Offset[FIELD_OFFSET(BINDING,sets)==4?1:-1];
typedef char BINDING_indices_Offset[FIELD_OFFSET(BINDING,indices)==8?1:-1];
typedef char LINK_second_Offset[FIELD_OFFSET(LINK,second)==2?1:-1];
typedef char LINK_kind_Offset[FIELD_OFFSET(LINK,kind)==4?1:-1];
extern "C"{
char func_00117D00(STREAM*) __asm__("S8Read__18CBinaryInputStream");
uint func_00117C80(STREAM*) __asm__("U32Read__18CBinaryInputStream");
short func_00117D50(STREAM*) __asm__("S16Read__18CBinaryInputStream");
int func_00117DB8(STREAM*) __asm__("S32Read__18CBinaryInputStream");
void func_00117F18(STREAM*,M4*);
void*func_001AE1D0(const REQUEST&);
void*func_001AE238(const REQUEST&,void*);
MARK*func_001AE478(MARK*);
void func_001AE4A0(MARK*,int);
BINDING*func_0016C8E0(P*,STREAM*);
M4*func_0011C168(M4*,const M4*,const M4*);
void func_001AE5A0(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");
}
struct MARK{MARK(){func_001AE478(this);}~MARK(){func_001AE4A0(this,2);}};
static inline void*Allocate(int bytes){return func_001AE1D0(REQUEST(bytes,16));}
static inline void*Reallocate(int bytes,void*old){return func_001AE238(REQUEST(bytes,16),old);}
/** @sly2-annot addr=0x169AC8
 * Purpose: Reads one sub-glob instance: transform owner lookup (vtable slot 51), matrix, flags, optional
 *   binding table, relocates packet data
 * Info: proposed name LoadSubglobInstanceFromBrx (T1); TU glob
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8+parse-verified
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/meshes-scene.md#type-0-the-visible-textured-mesh-loadsubglobtype0frombrx-0x16a000-cparse,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
extern "C" void func_00169AC8(P*p,const D*sourceD,const E*sourceE,const SPHERE*sourceSphere,D*desc,E*entry,SPHERE*sphere,int groups,const int*counts,FACE**faces,STREAM*stream){
    M4 matrix;
    (void)func_00117D00(stream);
    *desc=*sourceD;*entry=*sourceE;
    if(sphere){*sphere=*sourceSphere;sphere->metadata=func_00117C80(stream);}
    {
        SO*object=p->owner;
        VE*method=&object->vt[51];
        SO*receiver=(SO*)((char*)object+method->delta);
        int index=func_00117D50(stream);
        entry->transform=method->fn(receiver,index);
    }
    if(entry->flags.word&0x1000)func_00117F18(stream,&matrix);
    else{entry->parent=(M4*)Allocate(sizeof(M4));func_00117F18(stream,entry->parent);}
    entry->value=func_00117DB8(stream);
    entry->flags.bits.feature=func_00117D00(stream);
    MARK scope;
    BINDING*binding=func_0016C8E0(p,stream);
    if(!(desc->flags&1)&&desc->kind==0){
        char*old=entry->data;
        char*data=(char*)Reallocate(desc->words*16,old);
        uint delta=(uint)data-(uint)old;
        entry->data=data;entry->base+=delta;
        if(entry->flags.word&0x1000){
            M4 product,temp;
            func_0011C168(&temp,&entry->transform->matrix,&matrix);
            product=temp;
            func_001AE5A0(entry->data+0x14,&product,0x40);
        }
        if(binding){
            for(int g=0;g<groups;g++){
                int count=counts[g];FACE*set=faces[g];
                for(int i=0;i<count;i++){
                    FACE*face=&set[i];
                    if(face->flags&0x20){
                        uint base=(uint)entry->base;
                        unsigned short*first=(unsigned short*)((uint)face->first+base);
                        unsigned short*second=(unsigned short*)((uint)face->second+base);
                        unsigned char*kind=(unsigned char*)((uint)face->kind+base);
                        for(int j=0;j<face->count;j++){
                            int index=face->indices[j];LINKS*links=&binding->sets[binding->indices[g]];
                            first[j]=links->links[index].first;second[j]=links->links[index].second;kind[j]=links->links[index].kind;
                        }
                    }
                }
            }
        }
    }
}
