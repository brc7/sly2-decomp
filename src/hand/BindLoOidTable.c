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
struct LO*PloFindSwObject__FP2SWi3OIDP2LO(void*,int,int,struct ALO*);
void BindLoOidTable(struct ALO*palo,
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
   p=PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490,(pe->grf&0x400)?k|0x400:k,pe->id,palo);
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
