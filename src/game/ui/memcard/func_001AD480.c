struct CRef { int m_tagk; float m_tag; };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B a[3]; char pad2[0xFD58 - 0x24 - 3 * 0x5464]; struct B *xFD58; };
struct H { char pad[0x4C]; int x4C; };
struct S { int x0, x4, x8; char pad[0x10]; int x1C; };
extern struct G D_002E65E0;
extern struct H D_002E6550;
extern int D_003E0FAC;
void func_002581C0(struct CRef *ret, int a, void *b);
void func_00129EC8(struct CRef *);
void SetMcDialogState(struct H *h, int n);
void func_001AD3E0(void);
/** @sly2-annot addr=0x1AD480
 * Purpose: Menu action slot i (group 3): confirm overwrite or SaveToSlot
 * Info: proposed name SelectSaveSlotSave (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD480(struct S *s)
{
    struct CRef ref;
    struct B *p = &D_002E65E0.a[s->x8];
    float f;
    func_002581C0(&ref, D_003E0FAC, p);
    f = ref.m_tag;
    func_00129EC8(&ref);
    D_002E65E0.xFD58 = p;
    if (f == 0.0f)
        func_001AD3E0();
    else
        SetMcDialogState(&D_002E6550, 0x1C);
    if (s)
        s->x1C |= 2;
}
