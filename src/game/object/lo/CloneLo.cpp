#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef unsigned int uint;
typedef unsigned long ulong;
struct P;
struct BINDING{int refs,pad4,pad8;int cref[2];int next;};
struct FIELD{int refs,pad4;BINDING*binding;};
struct ROOT{int refs,pad4,count;void*items[4];BINDING*binding;};
struct RF;
struct RR;
struct REQUEST{int bytes,alignment;};
struct NODE{int pad0;NODE*next;int key,chain;P*owner;int link;char list[12];int pad24;};
struct META{char pad[0xC];uint bytes;};
struct VE{short delta,pad;META*(*type)(P*);};
union FLAGS{ulong word;struct{ulong pad:48;int kind:4,mode:4;uint reserved:1,autoA:1,autoB:1,end:5;}bits;};
struct TAG{int word;int Get()const{register int v __asm__("$4")=word;if(!(v&1))v=*(int*)v;return v;}void Set(int v){v&=~1;int d=word;if(!(d&1))*(int*)d=v|(*(int*)d&1);else word=v|(d&1);}};
struct P{ROOT*root;FIELD*field;int pad8;VE*vt;char pad10[0x18];FLAGS flags;int pad30;int word34;float scale;NODE*nodes;TAG tag;char pad44[12];};
struct G{char pad[0x3F10];int phase;};
typedef char BINDING_Size[sizeof(BINDING)==24?1:-1];
typedef char FIELD_Size[sizeof(FIELD)==12?1:-1];
typedef char ROOT_Size[sizeof(ROOT)==32?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char NODE_Size[sizeof(NODE)==40?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char META_Size[sizeof(META)==16?1:-1];
typedef char P_Size[sizeof(P)==80?1:-1];
typedef char FLAGS_Size[sizeof(FLAGS)==8?1:-1];
typedef char BINDING_refs_Offset[FIELD_OFFSET(BINDING,refs)==0?1:-1];
typedef char BINDING_cref_Offset[FIELD_OFFSET(BINDING,cref)==12?1:-1];
typedef char BINDING_next_Offset[FIELD_OFFSET(BINDING,next)==20?1:-1];
typedef char FIELD_refs_Offset[FIELD_OFFSET(FIELD,refs)==0?1:-1];
typedef char FIELD_binding_Offset[FIELD_OFFSET(FIELD,binding)==8?1:-1];
typedef char ROOT_refs_Offset[FIELD_OFFSET(ROOT,refs)==0?1:-1];
typedef char ROOT_items_Offset[FIELD_OFFSET(ROOT,items)==12?1:-1];
typedef char ROOT_binding_Offset[FIELD_OFFSET(ROOT,binding)==28?1:-1];
typedef char NODE_next_Offset[FIELD_OFFSET(NODE,next)==4?1:-1];
typedef char NODE_key_Offset[FIELD_OFFSET(NODE,key)==8?1:-1];
typedef char NODE_chain_Offset[FIELD_OFFSET(NODE,chain)==12?1:-1];
typedef char NODE_owner_Offset[FIELD_OFFSET(NODE,owner)==16?1:-1];
typedef char NODE_link_Offset[FIELD_OFFSET(NODE,link)==20?1:-1];
typedef char NODE_list_Offset[FIELD_OFFSET(NODE,list)==24?1:-1];
typedef char P_root_Offset[FIELD_OFFSET(P,root)==0?1:-1];
typedef char P_field_Offset[FIELD_OFFSET(P,field)==4?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_flags_Offset[FIELD_OFFSET(P,flags)==40?1:-1];
typedef char P_word34_Offset[FIELD_OFFSET(P,word34)==52?1:-1];
typedef char P_scale_Offset[FIELD_OFFSET(P,scale)==56?1:-1];
typedef char P_nodes_Offset[FIELD_OFFSET(P,nodes)==60?1:-1];
typedef char P_tag_Offset[FIELD_OFFSET(P,tag)==64?1:-1];
typedef char VE_delta_Offset[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char VE_type_Offset[FIELD_OFFSET(VE,type)==4?1:-1];
typedef char META_bytes_Offset[FIELD_OFFSET(META,bytes)==12?1:-1];
typedef char G_phase_Offset[FIELD_OFFSET(G,phase)==16144?1:-1];
extern "C"{
extern char D_002DD598[],D_004520D0[],D_00452148[];
extern G D_003DD490;
void CGc_UpdateRecyclable(void*);
void func_00257250(void*,int);
void Decref__4CRef(void*);
void GcBinding_Unlink(void*);
void GcSidebag_Unlink(void*);
void func_0011CD98(void*,void*);
void func_00257078(ROOT*,int);
void func_002580A8(FIELD**,FIELD*);
void FramePtr_Assign(ROOT**,ROOT*);
int func_00258178(FIELD*);
int func_00259A88(ROOT*);
RF CSidebag_CloneTo(FIELD*,P*);
RR CFrame_CloneTo(ROOT*,P*);
void*func_001AE238(REQUEST*,void*);
void ClearSle(int*);
void ClearSl(void*);
void func_0011A070(void*,NODE*);
void*PdlFromSwPxrOid(G*,int);
void CopyAb__FPvT0Ui(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");
}
struct RF{FIELD*p;RF(){}RF(const RF&r):p(r.p){if(p)++p->refs;}~RF(){FIELD*f=p;if(f&&!--f->refs){BINDING*b=f->binding;if(b&&!--b->refs){func_00257250(&b->next,2);Decref__4CRef(b->cref);GcBinding_Unlink(b);func_0011CD98(D_004520D0,b);}GcSidebag_Unlink(f);func_0011CD98(D_00452148,f);}}};
struct RR{ROOT*p;RR(){}RR(const RR&r):p(r.p){if(p)++p->refs;}~RR(){ROOT*r=p;if(r&&!--r->refs)func_00257078(r,3);}};
typedef char FieldRefSize[sizeof(RF)==4?1:-1];
typedef char RootRefSize[sizeof(RR)==4?1:-1];
static inline NODE*CopyNode(int bytes,REQUEST*r,NODE*n){r->bytes=bytes;r->alignment=16;return(NODE*)func_001AE238(r,n);}
static inline META*Type(P*p){VE*v=&p->vt[6];return v->type((P*)((char*)p+v->delta));}
/** @sly2-annot addr=0x1A6730
 * Purpose: LO vt41 Clone(ploBase): copy Splice frame/sidebag refs, scale, flag bits, tag word, x34; clone
 *   proxy nodes when g_sw phase==-2; CopyAb the class body past 0x50 (size from classinfo)
 * Info: proposed name CloneLo (T1); class LO slot41; TU lo
 * Notes: runtime: LO slot 41 = CloneLo 0x1A6730 confirmed
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources: r4_17F090+runtime
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface, docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
extern "C" void CloneLo(P*p,const P*source){
    CGc_UpdateRecyclable(D_002DD598);
    if(source->field){
        {RF r=CSidebag_CloneTo(source->field,p);func_002580A8(&p->field,r.p);}
        (void)func_00258178(p->field);
    }
    if(source->root){
        {RR r=CFrame_CloneTo(source->root,p);FramePtr_Assign(&p->root,r.p);}
        (void)func_00259A88(p->root);
    }
    p->scale=source->scale;
    p->flags.bits.mode=source->flags.bits.mode;
    p->flags.bits.autoA=source->flags.bits.autoA;
    p->flags.bits.kind=source->flags.bits.kind;
    p->flags.bits.autoB=source->flags.bits.autoB;
    p->tag.Set(source->tag.Get());
    p->word34=source->word34;
    char*dst;const char*src;
    if(D_003DD490.phase!=-2){src=(const char*)source+0x50;dst=(char*)p+0x50;p->nodes=source->nodes;}
    else{
        src=(const char*)source+0x50;dst=(char*)p+0x50;REQUEST request;
        for(NODE*n=source->nodes;n&&n->owner==source;n=n->next){
            NODE*copy=CopyNode(sizeof(NODE),&request,n);
            copy->owner=p;
            ClearSle(&copy->chain);
            ClearSle(&copy->link);
            ClearSl(copy->list);
            func_0011A070(copy->next->list,copy);
            void*list=PdlFromSwPxrOid(&D_003DD490,copy->key);
            func_0011A070(list,copy);
        }
    }
    CopyAb__FPvT0Ui(dst,src,Type(p)->bytes-0x50);
}
