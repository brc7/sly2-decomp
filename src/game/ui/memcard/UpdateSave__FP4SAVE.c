struct PMF { short delta; short pad; int (*pfn)(void *); };
struct H { char pad[0x44]; int x44; };
struct S { int x0, x4; int x8; int xC; float x10; float x14; char pad[0xFD5C - 0x18]; char xFD5C[1]; };
extern char D_003DD490[], D_002F66B0[], D_003D9ED0[];
extern int D_002DEB4C;
extern struct H D_002E6550;
extern float D_002DDF0C;
extern struct PMF D_002A6BA0;
int func_0020E040(void *g);
int FMemcardThreadValid__FP7MEMCARD(void *p);
void CreateMemcardThread(void *t, struct PMF pmf);
void func_001AB630(struct S *s);
void SetSaveState(struct S *s, int);
int func_001B42C0(void *p);
int func_001B01E8(void *p);
void SetMcDialogState(struct H *h, int n);
/** @sly2-annot addr=0x1AB1A0
 * Purpose: Per-frame save state machine: skip during transitions/when thread busy; poll card (job D_002A6BA0)
 *   or advance state (0x1AB418); autosave trigger with 0.3s debounce
 * Info: proposed name UpdateSave (T1); TU memcard
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r4_17F090+verdicts_A2
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void UpdateSave__FP4SAVE(struct S *s)
{
    if (func_0020E040(D_003DD490))
        return;
    if (D_002DEB4C)
        return;
    if (D_002E6550.x44 == 3)
        return;
    if (FMemcardThreadValid__FP7MEMCARD(s->xFD5C))
        return;
    if (s->x8 == s->xC) {
        if (s->x14 <= D_002DDF0C)
            CreateMemcardThread(s->xFD5C, D_002A6BA0);
        else
            func_001AB630(s);
    } else {
        SetSaveState(s, s->xC);
    }
    if (FMemcardThreadValid__FP7MEMCARD(s->xFD5C))
        return;
    if (func_001B42C0(D_002F66B0))
        return;
    if (D_002E6550.x44 != 1)
        return;
    if (0.3f <= D_002DDF0C - s->x10) {
    if (func_001B01E8(D_003D9ED0)) {
        if (s->x8 != 6)
            SetMcDialogState(&D_002E6550, 0x2E);
        return;
    }
    switch (s->x8) {
    case 1:
        SetMcDialogState(&D_002E6550, 0x11);
        break;
    case 2:
        SetMcDialogState(&D_002E6550, 0x12);
        break;
    case 3:
        SetMcDialogState(&D_002E6550, 0x15);
        break;
    case 4:
        SetMcDialogState(&D_002E6550, 0x18);
        break;
    }
    }
}
