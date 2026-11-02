struct SAVE {
    int a;
    int b;
    int n8;
    int nC;
    float g10;
    float g14;
    int n18;
    int n1C;
    int n20;
};
extern "C" void sceMcInit(void);
extern "C" void InitMemcard__FP7MEMCARD(void *);
/** @sly2-annot addr=0x1AB128
 * Purpose: sceMcInit, InitMemcard(+0xFD5C), zero SAVE header
 * Info: proposed name InitSave__FP4SAVEii (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void InitSave(SAVE *psave, int a, int b)
{
    sceMcInit();
    InitMemcard__FP7MEMCARD((char *)psave + 0xFD5C);
    psave->a = a;
    psave->b = b;
    psave->n8 = -1;
    psave->nC = 0;
    psave->g10 = psave->g14 = 0.0f;
    psave->n18 = 0;
    psave->n1C = 0;
    psave->n20 = 0;
}
