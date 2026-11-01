struct K { short x; short y; int i; };
extern const struct K D_002A5630;
struct G { int a; };
extern struct G D_003DD490;
struct S { char pad[0x18C]; int n18C; };
void func_00149A50(struct G *g, void *p, struct K k, int n);
/** @sly2-annot addr=0x17F5B0
 * Purpose: Once: set dying flag x18C and PostSwCallback(D_002A5630, n)
 * Info: proposed name StartCid47Dying (T2); class CID47; TU tu_17F090
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F5B0(struct S *p, int n) { if (p->n18C) return; p->n18C = 1; func_00149A50(&D_003DD490, p, D_002A5630, n); }
