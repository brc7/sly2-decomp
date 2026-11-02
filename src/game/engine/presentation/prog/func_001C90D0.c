struct TP { int status; void *func; void *stack; int stack_size; void *gp_reg; int initial_priority; int pad[6]; };
struct G { char pad[0x10]; int tid; };
extern struct G D_004425D0;
extern char D_003D3A50[];
extern char D_002E5270[];
extern int D_003D3A3C, D_003D3A40, D_003D3A44;
void func_001C9190(void *);
void func_002333B4(void *p, int c, int cb);
int func_00234820(struct TP *ptp);
int func_0021EFD0(int a, int b);
int func_002355E0(int tid, void *arg);
/** @sly2-annot addr=0x1C90D0
 * Purpose: create the two semaphores and start the loading render thread (0x1C9190)
 * Info: proposed name StartupLoadingScreen (T1); TU prog
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/hud-ui.md#3-fonts-language-letterbox-loading-screen,
 *   docs/engine/main-loop-startup.md#open-questions, docs/engine/overview.md#1-boot
 * @end-sly2-annot */
void func_001C90D0(void)
{
    struct TP tp;
    func_002333B4(&tp, 0, sizeof(tp));
    tp.stack_size = 0x1000;
    tp.stack = D_003D3A50;
    tp.gp_reg = D_002E5270;
    tp.initial_priority = 4;
    tp.func = func_001C9190;
    D_004425D0.tid = func_00234820(&tp);
    D_003D3A3C = func_0021EFD0(0, 1);
    D_003D3A40 = func_0021EFD0(0, 1);
    D_003D3A44 = func_0021EFD0(0, 1);
    func_002355E0(D_004425D0.tid, 0);
}
