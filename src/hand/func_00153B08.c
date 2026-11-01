typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE2A0(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
struct E { char *pchz; int n4; int n8; int nC; char ab[0x40]; };
struct S { char pad[0x50]; int c; struct E *ae; };
extern char D_002DD590[];
void func_001A5C28(struct S *p, void *pbis);
int func_00117BD0(void *pbis);
int func_00117D00(void *pbis);
int func_00117DB8(void *pbis);
void func_00118380(void *pbis, char **ppchz, void *palloc, int nKind);
void func_00118458(void *pbis, void *pv);
/** @sly2-annot addr=0x153B08
 * Purpose: Reads the base LO then a u8-counted table of {string (symbol table D_002DD590), s16, s32...}
 *   records
 * Info: proposed name LoadCid122FromBrx (T2); class CID122 slot14; TU tu_153B08
 * Notes: Cid7A->Cid122 (CID 0x7A=122, hex-literal) | level_parser CONFIRMED-code (layout read from the
 *   matched function; class absent from samples)
 * Confidence: low (draft) [T2 0.70] - evidence: static analysis; sources: r3_1539A8+parse-code
 * See: docs/engine/gameplay-objects.md#source-files,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/gameplay-objects.md#tu_153b08-0x153b08-0x153d58
 * @end-sly2-annot */
void func_00153B08(struct S *p, void *pbis)
{
    MSG msg;
    int i;
    func_001A5C28(p, pbis);
    p->c = func_00117BD0(pbis);
    p->ae = PvAllocMsg(&msg, p->c * sizeof(struct E), 0x10);
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        func_00118380(pbis, &pe->pchz, D_002DD590, 7);
        pe->n4 = func_00117D00(pbis);
        pe->n8 = func_00117DB8(pbis);
        pe->nC = func_00117DB8(pbis);
        func_00118458(pbis, pe->ab);
    }
}
