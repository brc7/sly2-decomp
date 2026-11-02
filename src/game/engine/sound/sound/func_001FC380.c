struct P { char pad[0x88]; int a[6]; char pad2[0x3BC - 0xA0]; int fl; };
void func_001FB658(struct P *, int, int);
static inline int Norm(int deg) { int n = deg % 360; return n > -1 ? n : n + 360; }
/** @sly2-annot addr=0x1FC380
 * Purpose: Set pan of sub-group k<6, dirty 0x100
 * Info: proposed name SetStreamsndGroupPan (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC380(struct P *p, unsigned int k, int deg)
{
    if (k < 6) {
        int n = Norm(deg);
        if (n != p->a[k]) {
            p->fl |= 1 << k;
            p->a[k] = n;
            func_001FB658(p, 0x100, 0x100);
        }
    }
}
