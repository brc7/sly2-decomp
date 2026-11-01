struct P { char pad[0x40]; int n40; char pad2[0x50 - 0x44]; unsigned short fl; unsigned short c52; char pad3[0x68 - 0x54]; long l68; char pad4[0x3BC - 0x70]; int n3BC; };
void func_001FB0C0(struct P *);
/** @sly2-annot addr=0x1FC270
 * Purpose: vt7: base Start then reset play time and status mask 0xF
 * Info: proposed name StartStreamsnd (T1); class STREAMSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC270(struct P *p)
{
    int f = 0; if (!p->n40) { f = p->c52 || (p->fl & 1); f ^= 1; } if (f) {
        func_001FB0C0(p);
        p->l68 = 0;
        p->n3BC = 0xF;
    }
}
