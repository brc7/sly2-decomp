struct PMF { short delta; short pad; int (*pfn)(void *); };
struct S { char pad[0x40]; int x40; };
extern struct PMF D_002A6E88, D_002A6D88, D_002A6E90;
extern char D_002F633C[];
void func_001AD3E0(void);
void func_001AAF90(void *t, struct PMF pmf);
void func_001ACB58(struct S *s, int n);
/** @sly2-annot addr=0x1AD168
 * Purpose: Confirm path: 0x1AD3E0 or CreateMemcardThread
 * Info: proposed name McDialogConfirm (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD168(struct S *s)
{
    switch (s->x40) {
    case 0x1C:
        func_001AD3E0();
        break;
    case 0x12:
        func_001AAF90(D_002F633C, D_002A6E88);
        func_001ACB58(s, 0x13);
        break;
    case 0x15:
        func_001AAF90(D_002F633C, D_002A6D88);
        func_001ACB58(s, 0x16);
        break;
    case 0x20:
        func_001AAF90(D_002F633C, D_002A6E90);
        func_001ACB58(s, 0x21);
        break;
    }
}
