typedef struct T { int a; int b; int c; void *ap[1]; } T;
void func_00256EE0(void *pp, int n);
void func_001AE5A0(void *pvDst, void *pvSrc, unsigned int cb);
/** @sly2-annot addr=0x127B70
 * Purpose: Removes a parent pointer from apframe[] (CopyAb shift, cframe--)
 * Info: proposed name CFrame_RemoveParent (T1); class CFrame; TU frame
 * Notes: names_v2 v1 proposed RefAddBinding__6CFrameUiP4CRef (0.022)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138
 * @end-sly2-annot */
void CFrame_RemoveParent(T *p, void *pv)
{
    int i;
    for (i = 0; i < p->c; i++) {
        if (p->ap[i] == pv) {
            func_00256EE0(&p->ap[i], 0);
            p->c--;
            func_001AE5A0(p->ap[i], p->ap[i + 1], (p->c - i) * 4);
            return;
        }
    }
}
