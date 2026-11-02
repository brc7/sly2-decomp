struct A { char pad[0xA0]; int f; };
void func_001FCF88(void);
void func_002330D0(int id, int n, int a, int b, int c, int d, int e, void (*pfn)(void), long p);
/** @sly2-annot addr=0x1FDBA8
 * Purpose: Issue SUKR cmd 0xC once (busy flag) with callback
 * Info: proposed name PollSndmgrStatus (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FDBA8(struct A *p)
{
    if (!p->f) {
        p->f = 1;
        func_002330D0(0x524B5553, 0xC, 0, 0, 0, 0, 0, func_001FCF88, (long)p);
    }
}
