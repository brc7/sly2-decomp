// SLY2_ASM: gnu-reorder-db
// Static initialization of the scratch buffers and shared pool prefixes.
typedef unsigned long ulong;
struct SBE {int cb;char *pv;};
extern "C" void *func_002333B4(void*,int,unsigned);
struct SPRBUF {
    ulong *aqw;char *pv;SBE a[8];int c;int state;
    SPRBUF(){func_002333B4(this,0,sizeof(*this));}
};
struct SLOTHEAP;
extern "C" SLOTHEAP *func_0011CCA8(SLOTHEAP*,int,int,int);
struct SLOTHEAP {
    int bytes,count,alignment;void *head;int allocated;void *block;
};
extern char D_003DD380[],D_003DD400[];
extern int D_002DD2E0,D_002DD2E4,D_002DD2E8,D_002DD2EC,D_002DD2F0,D_002DD2F4;
static inline SPRBUF MakeFirst(){SPRBUF b;b.pv=(char*)0x70000000;b.aqw=(ulong*)((int)D_003DD380|0x20000000);return b;}
static inline SPRBUF MakeSecond(){SPRBUF b;b.aqw=(ulong*)((int)D_003DD400|0x20000000);b.pv=(char*)0x70002000;return b;}
SPRBUF D_003DD2C0[2]={MakeFirst(),MakeSecond()};
struct POOLA;extern POOLA D_004520A8;
extern SLOTHEAP ViewA __asm__("D_004520A8");
struct POOLA:SLOTHEAP{POOLA(){if(++D_002DD2E0==1)func_0011CCA8(&ViewA,0x14,0x2000,0x10);}};
POOLA D_004520A8;
struct POOLB;extern POOLB D_004520D0;
extern SLOTHEAP ViewB __asm__("D_004520D0");
struct POOLB:SLOTHEAP{POOLB(){if(++D_002DD2E4==1)func_0011CCA8(&ViewB,0x18,0x800,0x10);}};
POOLB D_004520D0;
struct POOLC;extern POOLC D_004520F8;
extern SLOTHEAP ViewC __asm__("D_004520F8");
struct POOLC:SLOTHEAP{POOLC(){if(++D_002DD2E8==1)func_0011CCA8(&ViewC,0x28,0x200,0x10);}};
POOLC D_004520F8;
struct POOLD;extern POOLD D_00452120;
extern SLOTHEAP ViewD __asm__("D_00452120");
struct POOLD:SLOTHEAP{POOLD(){if(++D_002DD2EC==1)func_0011CCA8(&ViewD,0x1C,0x800,0x10);}};
POOLD D_00452120;
struct POOLE;extern POOLE D_00452148;
extern SLOTHEAP ViewE __asm__("D_00452148");
struct POOLE:SLOTHEAP{POOLE(){if(++D_002DD2F0==1)func_0011CCA8(&ViewE,0xC,0xC8,0x10);}};
POOLE D_00452148;
struct POOLF;extern POOLF D_00452170;
extern SLOTHEAP ViewF __asm__("D_00452170");
struct POOLF:SLOTHEAP{POOLF(){if(++D_002DD2F4==1)func_0011CCA8(&ViewF,0x14,0x100,0x10);}};
POOLF D_00452170;

typedef char check_pointer[(sizeof(void*)==4)?1:-1];
typedef char check_ulong[(sizeof(ulong)==8)?1:-1];
typedef char check_sbe[(sizeof(SBE)==8)?1:-1];
typedef char check_sbe_bytes[((unsigned)&((SBE*)0)->cb==0)?1:-1];
typedef char check_sbe_pointer[((unsigned)&((SBE*)0)->pv==4)?1:-1];
typedef char check_sprbuf[(sizeof(SPRBUF)==0x50)?1:-1];
typedef char check_sprbuf_align[(__alignof__(SPRBUF)==4)?1:-1];
typedef char check_sprbuf_tags[((unsigned)&((SPRBUF*)0)->aqw==0)?1:-1];
typedef char check_sprbuf_scratch[((unsigned)&((SPRBUF*)0)->pv==4)?1:-1];
typedef char check_sprbuf_entries[((unsigned)&((SPRBUF*)0)->a==8)?1:-1];
typedef char check_sprbuf_last[((unsigned)&((SPRBUF*)0)->a[7]==0x40)?1:-1];
typedef char check_sprbuf_count[((unsigned)&((SPRBUF*)0)->c==0x48)?1:-1];
typedef char check_sprbuf_state[((unsigned)&((SPRBUF*)0)->state==0x4C)?1:-1];
typedef char check_sprbuf_pair[(sizeof(D_003DD2C0)==0xA0)?1:-1];
typedef char check_heap_prefix[(sizeof(SLOTHEAP)==0x18)?1:-1];
typedef char check_heap_align[(__alignof__(SLOTHEAP)==4)?1:-1];
typedef char check_heap_bytes[((unsigned)&((SLOTHEAP*)0)->bytes==0)?1:-1];
typedef char check_heap_count[((unsigned)&((SLOTHEAP*)0)->count==4)?1:-1];
typedef char check_heap_alignment[((unsigned)&((SLOTHEAP*)0)->alignment==8)?1:-1];
typedef char check_heap_head[((unsigned)&((SLOTHEAP*)0)->head==0xC)?1:-1];
typedef char check_heap_allocated[((unsigned)&((SLOTHEAP*)0)->allocated==0x10)?1:-1];
typedef char check_heap_block[((unsigned)&((SLOTHEAP*)0)->block==0x14)?1:-1];
typedef char check_pool_A[(sizeof(POOLA)==sizeof(SLOTHEAP))?1:-1];
typedef char check_pool_B[(sizeof(POOLB)==sizeof(SLOTHEAP))?1:-1];
typedef char check_pool_C[(sizeof(POOLC)==sizeof(SLOTHEAP))?1:-1];
typedef char check_pool_D[(sizeof(POOLD)==sizeof(SLOTHEAP))?1:-1];
typedef char check_pool_E[(sizeof(POOLE)==sizeof(SLOTHEAP))?1:-1];
typedef char check_pool_F[(sizeof(POOLF)==sizeof(SLOTHEAP))?1:-1];

/** @sly2-annot addr=0x2019F0
 * Purpose: compiler-generated __static_initialization_and_destruction_0 of TU sprbuf
 * Info: proposed name StaticInit_sprbuf (T1); TU sprbuf
 * Notes: real local symbol name is __static_initialization_and_destruction_0 (not unique; renamed per TU)
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r7_1F8DD8
 * @end-sly2-annot */
__asm__(".text\n"
        ".globl func_002019F0\n"
        ".type func_002019F0,@function\n"
        "func_002019F0:\n"
        ".size func_002019F0,872\n");
