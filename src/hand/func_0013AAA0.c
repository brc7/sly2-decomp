struct K { int a; int b; };
struct S { char pad[0x1f4]; int f1f4; int f1f8; int f1fc; };
void func_001F36A8(struct S *);
int func_001AE1D0(struct K *);
/** @sly2-annot addr=0x13AAA0
 * Purpose: BOMB vt15: PostSoLoad, and with fCustomXps (+0x1F4) allocate the custom-XP buffer (0xC0)
 * Info: proposed name PostBombLoad (T1); class BOMB slot15; TU bomb
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void func_0013AAA0(struct S *p) {
    func_001F36A8(p);
    if (p->f1f4) { struct K k; k.a = 0xc0; k.b = 0x10; p->f1fc = func_001AE1D0(&k); }
}
