struct CRef { int m_tagk; float m_tag; };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B a[3]; char pad2[0xFD50 - 0x24 - 3 * 0x5464]; struct B *xFD50; int xFD54; struct B *xFD58; };
struct S { int x0, x4, x8; char pad[0x10]; int x1C; };
extern struct G D_002E65E0;
extern int D_003E0FAC;
void RefGetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void func_001D8D80(struct B *b, int n);
void func_00165E38(struct B *b);
/** @sly2-annot addr=0x1AD320
 * Purpose: Menu action slot i: make current, ResetGs, start (0x165E38)
 * Info: proposed name SelectSaveSlotLoad (T2); TU memcard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD320(struct S *s)
{
    struct CRef ref;
    struct B *p = &D_002E65E0.a[s->x8];
    float f;
    RefGetGlobalSaveVar(&ref, D_003E0FAC, p);
    f = ref.m_tag;
    Decref__4CRef(&ref);
    D_002E65E0.xFD50 = p;
    if (f == 0.0f) {
        func_001D8D80(p, 1);
        func_00165E38(D_002E65E0.xFD50);
    } else {
        func_00165E38(p);
        if (s)
            s->x1C |= 2;
    }
}
