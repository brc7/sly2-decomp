struct K { int a, b; };
extern struct K D_002A6C90;
extern char D_003E0EA8[];
struct S { char pad[0xFD50]; int xfd50; int pad2[2]; char xfd5c[4]; };
void func_001D54B8(struct S *p);
void func_001D84B0(void *p, int n);
void CreateMemcardThread(void *p, struct K k);
/** @sly2-annot addr=0x1ABD40
 * Purpose: Same as 0x1ABCE8 with other job
 * Info: proposed name StartSaveJobB (T2); TU memcard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ABD40(struct S *p)
{
    func_001D54B8(p);
    func_001D84B0(D_003E0EA8, p->xfd50);
    CreateMemcardThread(p->xfd5c, D_002A6C90);
}
