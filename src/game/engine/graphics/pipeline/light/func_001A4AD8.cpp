// func_001A4AD8: builds four 48-byte shader parameter packets (tag word + 12 payload words each) in the global VIFS
// packet buffer: one per object in ap[0..n) (payload at +0x120 of each, filled in by func_001A3968), the rest of the
// four slots from the default block D_002BFC60. Stores the packet length in qwords in o->x14 (start cursor in o->x18).
//
// Matching notes:
//  - Copy is an ordinary up-counting `for (k = 0; k < 12; k += 4)` over a pointer pair that the loop optimiser
//    reverses (counter 8 / bgez). d_/s_ are initialised OUTSIDE the do{}while(0): the macro's loop-begin note is a
//    scheduling barrier, which places the PRE-inserted `i+1` and the reversed counter init after the copy setup.
//  - In the second loop `g` and `src` are locals declared before the Write so the hoisted lui/addiu pairs come
//    out in the original order (D_002DEA28, D_002BFC60, tag constant).
#include "vifs.h"
struct BLOCK {union {uint words[12];struct {int kind;float coefficients[11];} values;};};
struct P { char pad[0x120]; struct BLOCK payload; };
struct O { char pad[0x14]; int x14; unsigned char *x18; };
extern VIFS D_002DEA28;
extern struct BLOCK D_002BFC60;
extern "C" void func_001A3968(struct P *p);
static inline uint* Write(VIFS*g,uint x){unsigned char*d;*(uint*)g->m_pb=x;d=g->m_pb+4;g->m_pb=d;return(uint*)d;}
#define Copy(D,S,W) uint*d_=(D);const uint*s_=(S);do{int k;for(k=0;k<(W);k+=4){uint a=s_[0],b=s_[1],c=s_[2],e=s_[3];d_[0]=a;d_[1]=b;d_[2]=c;d_[3]=e;s_+=4;d_+=4;}}while(0);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char BLOCK_words_Offset[FIELD_OFFSET(BLOCK,words)==0?1:-1];
typedef char BLOCK_values_Offset[FIELD_OFFSET(BLOCK,values)==0?1:-1];
typedef char P_payload_Offset[FIELD_OFFSET(P,payload)==288?1:-1];
typedef char O_x14_Offset[FIELD_OFFSET(O,x14)==20?1:-1];
typedef char O_x18_Offset[FIELD_OFFSET(O,x18)==24?1:-1];
typedef char VIFS_m_pb_Offset[FIELD_OFFSET(VIFS,m_pb)==12?1:-1];
typedef char uint_PrefixSize[sizeof(uint)==4?1:-1];
typedef char BLOCK_PrefixSize[sizeof(BLOCK)==48?1:-1];
typedef char P_PrefixSize[sizeof(P)==336?1:-1];
typedef char O_PrefixSize[sizeof(O)==28?1:-1];
typedef char VIFS_PrefixSize[sizeof(VIFS)==36?1:-1];
/** @sly2-annot addr=0x1A4AD8
 * Purpose: Same packet build for an explicit light list (rebuilds each light first)
 * Info: proposed name BuildLightPackets (T2); TU light
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
extern "C" void func_001A4AD8(void *a0, int n, struct P **ap, struct O *o)
{
    int i;
    o->x18 = D_002DEA28.m_pb;
    for (i = 0; i < n; i++) {
        struct P *p = ap[i];
        func_001A3968(p);
        uint tag=0x6C03C000 | (i*3+0x15);
        uint*d=Write(&D_002DEA28,tag);
        Copy(d,p->payload.words,12);
        D_002DEA28.m_pb += 48;
    }
    for (; i < 4; i++) {
        VIFS *g = &D_002DEA28;
        const uint *src = D_002BFC60.words;
        uint*d=Write(g,0x6C03C000 | (i*3+0x15));
        Copy(d,src,12);
        g->m_pb += 48;
    }
    o->x14 = ((char *)D_002DEA28.m_pb - (char *)o->x18) >> 4;
}
