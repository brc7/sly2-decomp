struct LO;
typedef struct {short d,pad;void(*pfn)(struct LO*);} VTE;
struct LO {int x0,x4,x8;VTE*vt;};
struct SUB {char pad[0x9C];int x9C;char pad2[0x100-0xA0];};
struct ALO {char pad[0x50];int c;struct SUB*a;};
struct E {int grf,id,ib;};
extern char D_003DD490[];
void*func_001F6030(struct ALO*,int);
void func_001F2AD0(struct ALO*, void*);
void*func_001F5FF8(struct ALO*,int);
void func_001F69F8(struct ALO*,int);
struct LO*func_0015E938(void*,int,int,struct ALO*);
/** @sly2-annot addr=0x1A64E8
 * Purpose: Resolve a table {grf,id,ib} of OID refs into fields at plo+ib: grf&0x10 part by id, 0x20/0x40 SO
 *   sub-objects (0x1F6030/0x1F5FF8), else PloFindSwObject 0x15E938 (kind 4/5/1); grf&0x200 -> vt9 Remove the
 *   found object
 * Info: proposed name BindLoOidTable (T1); TU lo
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/engine/object-model.md#3-creation-hierarchy-and-removal, docs/engine/player.md#11-key-functions
 * @end-sly2-annot */
void func_001A64E8(struct ALO*palo,
int c,struct E*ae){
int i=0;
 while(i<c){
  struct E*pe=&ae[i];int grf=pe->grf;
  if(grf&0x10){
   /* The count and byte offset share the native search registers. */
   register int j __asm__("$7")=palo->c;register int off __asm__("$6")=j*0x100;struct SUB*p;
   i++;
again:
   j--;off-=0x100;
   if(j<0)goto done;
   p=(struct SUB*)(off+(int)palo->a);
   if(p->x9C!=pe->id)goto again;
   *(void**)((char*)palo+pe->ib)=p;
done:;
  }else if(grf&0x20){
   void*p=func_001F6030(palo,pe->id);
   if(p){i++;*(void**)((char*)palo+pe->ib)=p;if(pe->grf&0x200)func_001F2AD0(palo,p);}else i++;
  }else if(grf&0x40){
   void*p=func_001F5FF8(palo,pe->id);
   if(p){i++;*(void**)((char*)palo+pe->ib)=p;if(pe->grf&0x200)func_001F69F8(palo,pe->id);}else i++;
  }else{
   int k;struct LO*p;
   if(grf&2)k=4;else k=(grf&1)?5:1;
   p=func_0015E938(D_003DD490,(pe->grf&0x400)?k|0x400:k,pe->id,palo);
   if(!p){i++;continue;}
   if(pe->grf&0x200)p->vt[9].pfn((struct LO*)((char*)p+p->vt[9].d));
   i++;
   *(void**)((char*)palo+pe->ib)=p;
  }
 }
}

#define OFFSET(t,m) ((unsigned int)&(((t *)0x10000)->m)-0x10000)
typedef char layout_vte_delta[OFFSET(VTE,d)==0?1:-1];
typedef char layout_vte_function[OFFSET(VTE,pfn)==4?1:-1];
typedef char layout_lo_vtable[OFFSET(struct LO,vt)==12?1:-1];
typedef char layout_sub_id[OFFSET(struct SUB,x9C)==0x9C?1:-1];
typedef char layout_alo_count[OFFSET(struct ALO,c)==0x50?1:-1];
typedef char layout_alo_array[OFFSET(struct ALO,a)==0x54?1:-1];
typedef char layout_entry_flags[OFFSET(struct E,grf)==0?1:-1];
typedef char layout_entry_id[OFFSET(struct E,id)==4?1:-1];
typedef char layout_entry_offset[OFFSET(struct E,ib)==8?1:-1];
typedef char layout_vte_size[sizeof(VTE)==8?1:-1];
typedef char layout_lo_size[sizeof(struct LO)==16?1:-1];
typedef char layout_sub_size[sizeof(struct SUB)==0x100?1:-1];
typedef char layout_alo_size[sizeof(struct ALO)==0x58?1:-1];
typedef char layout_entry_size[sizeof(struct E)==12?1:-1];
typedef char layout_word_size[sizeof(int)==4?1:-1];
typedef char layout_pointer_size[sizeof(void*)==4?1:-1];
