struct T { int a; int *ap; };
struct S { char pad[0xE0]; struct T *pt; int x; void *pe8; };
int IcopyFromPspare__FP5PROXYP10PROXYSPARE(struct S *p, void *q);
/** @sly2-annot addr=0x1CB2E0
 * Purpose: first object of current spare (+0xE8) or NULL
 * Info: proposed name PloCurProxy (T2); TU proxy
 * Notes: names_v2 C proposed PshadowAloEnsure__FP3ALO (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
int func_001CB2E0(struct S *p)
{
    /*B*/int i; if (!p->pe8) return 0; i = IcopyFromPspare__FP5PROXYP10PROXYSPARE(p, p->pe8); return p->pt->ap[i];/*E*/
}
