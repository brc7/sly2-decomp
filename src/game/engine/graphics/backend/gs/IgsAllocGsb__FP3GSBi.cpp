extern "C" {
struct B { int n0; int n4; struct B *next; };
struct S { char pad0[4]; int n4; char pad8[4]; struct B *ap[8]; };
int IgsbBucketFromCgs__Fi(int n);
void PadGsbToPage__FP3GSB(struct S *p);
/** @sly2-annot addr=0x170548
 * Purpose: Allocates c units of GS memory from a GSB: first-fit from the size-bucket free lists, else bump
 *   igsMac (padding to a 0x80 boundary if it would straddle); returns the start index
 * Info: proposed name IgsAllocGsb (T1); TU gs
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic,
 *   docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
int IgsAllocGsb__FP3GSBi(struct S *input, int cb)
{
    register struct S *p __asm__("$19")=input;
    unsigned int i;
    int old;
    struct B *pb;
    int mask;
    i = IgsbBucketFromCgs__Fi(cb - 1) + 1;
    mask = ~0x7F;
    goto test;
loop:
    pb = p->ap[i];
    if (pb) {
        old = pb->n0;
        pb->n0 = old + cb;
        if (pb->n0 == pb->n4) {
            p->ap[i] = pb->next;
        } else {
            unsigned int j = IgsbBucketFromCgs__Fi(pb->n4 - pb->n0);
            if (j != i) {
                register struct B *next __asm__("$4")=pb->next;
                p->ap[i] = next;
                register struct B *replacement __asm__("$2")=p->ap[j];
                pb->next = replacement;
                p->ap[j] = pb;
            }
        }
        goto done;
    }
    i++;
test:
    if (i < 8) goto loop;
    if (((p->n4 + cb) & mask) != (p->n4 & mask)) PadGsbToPage__FP3GSB(p);
    old = p->n4;
    p->n4 = old + cb;
done:
    return old;
}

}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Integer_Size[sizeof(int)==4&&sizeof(unsigned int)==4?1:-1];
typedef char B_Size[sizeof(B)==12?1:-1];
typedef char B_Alignment[__alignof__(B)==4?1:-1];
typedef char B_Begin[FIELD_OFFSET(B,n0)==0?1:-1];
typedef char B_End[FIELD_OFFSET(B,n4)==4?1:-1];
typedef char B_Next[FIELD_OFFSET(B,next)==8?1:-1];
typedef char S_PrefixSize[sizeof(S)==0x2C?1:-1];
typedef char S_Alignment[__alignof__(S)==4?1:-1];
typedef char S_Header0[FIELD_OFFSET(S,pad0)==0?1:-1];
typedef char S_Current[FIELD_OFFSET(S,n4)==4?1:-1];
typedef char S_Header8[FIELD_OFFSET(S,pad8)==8?1:-1];
typedef char S_Buckets[FIELD_OFFSET(S,ap)==12?1:-1];
typedef char S_BucketCount[sizeof(((S*)0)->ap)/sizeof(B*)==8?1:-1];
