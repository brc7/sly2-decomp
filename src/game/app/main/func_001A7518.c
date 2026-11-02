extern int D_002E2050, D_002E2054;
extern void *D_002DD644;
extern int D_002BE538;
extern char D_003DD490[], D_003D9E70[], D_002DFC00[], D_002DDF40[], D_002DDED0[];
void __main__Fv(void);
void Startup__Fv(void);
void StartupGame(void);
void ExecuteSwTransition(void *);
void UpdateJoy__FP3JOY(void *);
void UpdateCd__Fv(void);
void SetupCm__FP2CM(void *);
void MarkClockTick(void *, int);
void UpdateSw(void *);
void func_001D9648(void *);
void OpenFrame__Fv(void);
int func_001477B0(void *);
void RenderSw(void *);
void DrawSw(void *);
void DrawUi(void *);
void func_001477D8(void *);
void CloseFrame__Fv(void);
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
    __main__Fv();
    D_002E2050 = a;
    D_002E2054 = b;
    Startup__Fv();
    StartupGame();
    for (;;) {
        ExecuteSwTransition(D_003DD490);
        UpdateJoy__FP3JOY(D_002DFC00);
        UpdateCd__Fv();
        SetupCm__FP2CM(D_002DDF40);
        MarkClockTick(D_002DDED0, 0);
        UpdateSw(D_003DD490);
        func_001D9648(D_003D9E70);
        OpenFrame__Fv();
        if (!D_002DD644 || func_001477B0(D_002DD644)) {
            RenderSw(D_003DD490);
            DrawSw(D_003DD490);
            DrawUi(D_003D9E70);
        }
        if (D_002DD644)
            func_001477D8(D_002DD644);
        CloseFrame__Fv();
        D_002BE538++;
    }
}
