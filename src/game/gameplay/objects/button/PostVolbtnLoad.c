struct P2 { int a, b; };
void PostLoLoad(void *p);
void PostBtnLoad(void *p);
void SetVolbtnButtons(void *p, int n);
int func_001AE1D0(struct P2 *p);
/** @sly2-annot addr=0x149398
 * Purpose: vt15: base, PostBtnLoad
 * Info: proposed name PostVolbtnLoad (T1); class VOLBTN slot15; TU button
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void PostVolbtnLoad(char *p)
{
    PostLoLoad(p);
    PostBtnLoad(p + 0x130);
    SetVolbtnButtons(p, 0);
    if (*(int *)(p + 0x2C8)) {
        struct P2 s;
        s.a = 0x100;
        s.b = 0x10;
        *(int *)(p + 0x2C4) = func_001AE1D0(&s);
    }
}
