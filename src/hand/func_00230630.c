typedef struct S { char pad[0x98]; short k; short x9A; } S;
void func_002303A0(void *, S *, int);

/** @sly2-annot addr=0x230630
 * Purpose: role/phase transitions via SlotSetRole
 * Info: proposed name SlotSettle (T2); TU xf
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_00230630(void *p, S *s)
{
    switch (s->k) {
    case 7:
    case 9:
        if (s->x9A == 2)
            func_002303A0(p, s, 10);
        else if (s->x9A == 1)
            func_002303A0(p, s, 6);
        break;
    case 11:
        if (s->x9A == 2)
            func_002303A0(p, s, 0xC);
        break;
    case 18:
        if (s->x9A == 2)
            func_002303A0(p, s, 0x13);
        break;
    case 1:
        func_002303A0(p, s, 3);
        break;
    }
}
