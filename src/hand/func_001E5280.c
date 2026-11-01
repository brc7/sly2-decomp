struct VTE { short d; short pad; void (*pfn)(void *, int, int); };
struct O { int pad[3]; struct VTE *vt; };
struct P { char pad[0xA4]; int c; struct O *apo[4]; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void *func_002332B8(void *pdst, const void *psrc, unsigned int cb);
static inline unsigned ByteOffset(int index){register unsigned result __asm__("$4");__asm__("sll %0,%1,2":"=r"(result):"r"(index));return result;}
/** @sly2-annot addr=0x1E5280
 * Purpose: Remove a held STEP and call its vt84(a,b)
 * Info: proposed name RemoveJsgStepHold (T1); TU relay
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E5280(struct P *p, struct O *po, int a, int b)
{
    int i;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x33)) return;
    for(i=0;i<p->c;i++) {
        if(p->apo[i]==po)break;
    }
    if (i >= p->c) return;
    {
        register unsigned int addr __asm__("$5") = ByteOffset(i)+(unsigned int)p;
        unsigned int dest;
        __asm__("daddu %0,%1,$zero":"=r"(dest):"r"(addr));
        func_002332B8((void *)(dest+0xA8),(void *)(addr+0xAC),(p->c-i)*4-4);
    }
    p->c--;
    po->vt[0x54].pfn((char *)po + po->vt[0x54].d, a, b);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_VTE[(sizeof(struct VTE)==8)?1:-1];
typedef char layout_O_prefix[(sizeof(struct O)==16)?1:-1];
typedef char layout_P_array_prefix[(sizeof(struct P)==0xB8)?1:-1];
typedef char layout_VTE_delta[((unsigned)&((struct VTE*)0)->d==0)?1:-1];
typedef char layout_VTE_function[((unsigned)&((struct VTE*)0)->pfn==4)?1:-1];
typedef char layout_O_vtable[((unsigned)&((struct O*)0)->vt==0xC)?1:-1];
typedef char layout_P_count[((unsigned)&((struct P*)0)->c==0xA4)?1:-1];
typedef char layout_P_array[((unsigned)&((struct P*)0)->apo==0xA8)?1:-1];
