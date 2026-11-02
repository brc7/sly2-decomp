extern char D_002A2E50[];
extern char D_002A2E58[];
extern int D_002DDE80;
extern int D_002DDE84;
int func_00242840(unsigned int lsn, unsigned int c, void *buf, void *mode);
int func_241E88(int n);
int func_00241E88(int n);
int func_00242AE0(void);
/** @sly2-annot addr=0x14BC78
 * Purpose: Loop sceCdRead until 1, else sceCdSync(0)+error, as Sly1 ReadCdDirect; Sly2 adds a 4th int arg
 *   choosing the read mode
 * Info: proposed name ReadCdDirect (T1); TU cd
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: names_v2:A+verdicts_A1
 * See: docs/engine/core-library.md#4-binary-input-streams-and-cd,
 *   docs/engine/core-library.md#7-key-functions, docs/formats/disc-records.md#2-the-read-path
 * @end-sly2-annot */
void func_0014BC78(unsigned int lsn, unsigned int c, void *buf, int fMode)
{
    void *mode;
    if (fMode == 1) mode = D_002A2E50;
    else mode = D_002A2E58;
    do {
        D_002DDE80 = func_00242840(lsn, c, buf, mode);
        if (D_002DDE80 == 1) break;
        func_00241E88(0);
        D_002DDE84 = func_00242AE0();
    } while (1);
}
