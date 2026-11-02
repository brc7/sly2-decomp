typedef int qword __attribute__((mode(TI)));
struct CMD { char pad[0x10]; qword q; float x, y, z; int pad2; float f; int pad3[3]; union { float g; int n; } u40; int n44; };
struct CMD *PrelaycmdAddRelay(void *pq, int kind, int b);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
/** @sly2-annot addr=0x1E1238
 * Purpose: Splice builtin: append a RELAYCK 0x38 (CamBlendFrom) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x38,fAsync)) and fill its args
 * Info: proposed name AddRelayCamBlendFrom (T1); TU relay
 * Notes: names_v2 C proposed SetJsgFocus__FP3JSGP2LO (0.438)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayCamBlendFrom(void *pq, qword a, qword v, int n, float f, float g)
{
    struct CMD *p = PrelaycmdAddRelay(pq, 0x38, 1);
    p->q = a;
    p->x = GetX(v) * 0.017453294f;
    p->y = GetY(v) * 0.017453294f;
    p->z = GetZ(v) * 0.017453294f;
    p->f = f;
    p->u40.g = g;
    p->n44 = n;
}
