struct G { char pad[0x4764]; int a; int b; };
extern struct G D_003DD490;
struct H;
extern struct H D_003DD040;
struct S { char pad[0x190]; int n190; char pad2[0x64]; int n1f8; };
void func_001C8730(struct S *p, int n);
void func_001618B0(struct G *pg);
void StartDialog(struct H *ph, int n, int m);
/** @sly2-annot addr=0x1C8AF0
 * Purpose: calls SetVaultState
 * Info: proposed name VaultUnk8AF0 (T2); TU prize
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C8AF0(struct S *p, int n)
{
    struct G *pg = &D_003DD490;
    if (pg->a >= pg->b) {
        p->n190 = n;
        func_001C8730(p, 1);
    } else {
        func_001618B0(pg);
        if (p->n1f8 != -1) StartDialog(&D_003DD040, p->n1f8, 1);
    }
}
