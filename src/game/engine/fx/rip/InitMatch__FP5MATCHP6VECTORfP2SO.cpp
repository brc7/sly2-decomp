typedef int qword __attribute__((mode(TI)));
struct VECTOR;
struct SO;
struct MATCH { char pad[0x20]; float g20; float g24; int n28; int pad2; qword q30; };
extern qword D_002BBB50;
extern "C" void InitRip(MATCH *p, VECTOR *pv, float g, SO *pso);
/** @sly2-annot addr=0x1CFCD8
 * Purpose: MATCH (RIPT 6) Init
 * Info: proposed name InitMatch__FP5MATCHP6VECTORfP2SO (EXISTING); TU rip
 * Confidence: high [EXISTING 0.80] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void InitMatch(MATCH *pmatch, VECTOR *pv, float g, SO *pso)
{
    InitRip(pmatch, pv, g, pso);
    pmatch->q30 = D_002BBB50;
    pmatch->g20 = g;
    pmatch->g24 = 1.0f - g;
    pmatch->n28 = 0;
}
