struct S { char pad[0x34]; int n34; };
struct G { int a; };
extern struct G D_003DD490;
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *pg, int kind, int oid, int owner);
/** @sly2-annot addr=0x1715B8
 * Purpose: PloFindSwObject relative to the script's current context object
 * Info: proposed name PloFindBhv169Target (T2); TU tu_1709A8
 * Notes: v2: demoted to T2 (placeholder token in name: 169)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void *func_001715B8(struct S *p, int oid) { int o = p->n34; int k = o ? 4 : 5; return PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, k, oid, o); }
