struct P { char pad[0x40]; int fl; char pad2[0x17C - 0x44]; void *px; };
void *func_001AE4F8(int cb);
void *func_001E81C0(void *pv, struct P *p);
void func_001E6180(struct P *p);
void func_001A6CA8(struct P *p, int n);
/** @sly2-annot addr=0x1E7840
 * Purpose: Create (0x90 bytes, 0x1E81C0) the shape's chain if missing, measure, set LO busy flags; f==1
 *   re-arms an existing one; returns it
 * Info: proposed name PchainEnsureShape (T2); TU shape
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void *func_001E7840(struct P *p, int f)
{
    if (!p->px) {
        p->px = func_001E81C0(func_001AE4F8(0x90), p);
        func_001E6180(p);
        func_001A6CA8(p, 0);
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v &= ~0x20; else p->fl = v & ~0x20; }
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v |= 0xC; else p->fl = v | 0xC; }
    } else if (f == 1) {
        func_001A6CA8(p, 0);
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v |= 0x4; else p->fl = v | 0x4; }
    }
    return p->px;
}
