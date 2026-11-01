extern int D_002E2050, D_002E2054;
extern void *D_002DD644;
extern int D_002BE538;
extern char D_003DD490[], D_003D9E70[], D_002DFC00[], D_002DDF40[], D_002DDED0[];
void func_0023B7D8(void);
void func_001A7970(void);
void StartupGame(void);
void ExecuteSwTransition(void *);
void func_001871C0(void *);
void func_0014BA38(void);
void func_0014D470(void *);
void func_0014D0B8(void *, int);
void UpdateSw(void *);
void func_001D9648(void *);
void func_00160828(void);
int func_001477B0(void *);
void func_001CDA80(void *);
void func_00157FA0(void *);
void func_001D9820(void *);
void func_001477D8(void *);
void func_00160A98(void);
/** @sly2-annot addr=0x1A7518
 * Purpose: Program entry: store argc/argv, Startup(), StartGame-like init 0x1613A8, then the infinite frame
 *   loop (see tu_docs/main.md)
 * Info: proposed name main (T1); TU main
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6+verdicts_A2
 * See: docs/engine/main-loop-startup.md#1-maincpp-0x1a7518-0x1a7c18,
 *   docs/engine/main-loop-startup.md#7-source-files, docs/engine/main-loop-startup.md#8-key-functions
 * @end-sly2-annot */
void func_001A7518(int a, int b)
{
    func_0023B7D8();
    D_002E2050 = a;
    D_002E2054 = b;
    func_001A7970();
    StartupGame();
    for (;;) {
        ExecuteSwTransition(D_003DD490);
        func_001871C0(D_002DFC00);
        func_0014BA38();
        func_0014D470(D_002DDF40);
        func_0014D0B8(D_002DDED0, 0);
        UpdateSw(D_003DD490);
        func_001D9648(D_003D9E70);
        func_00160828();
        if (!D_002DD644 || func_001477B0(D_002DD644)) {
            func_001CDA80(D_003DD490);
            func_00157FA0(D_003DD490);
            func_001D9820(D_003D9E70);
        }
        if (D_002DD644)
            func_001477D8(D_002DD644);
        func_00160A98();
        D_002BE538++;
    }
}
