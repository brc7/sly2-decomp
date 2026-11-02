struct VTE { short d; short pad; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0x40 - 0x10]; int n40; char pad3[0x4E - 0x44]; short n4E; unsigned short f50; unsigned short n52; char pad4[0x68 - 0x54]; long l68; };
struct G { char pad[0x1F0]; long l1F0; long l1F8; };
extern struct G D_003DD040;
void OnStreamsndStatus(void);
void func_001FBB30(void);
void func_002330D0(int, int, int, int, int, int, int, void (*)(void), long);
void SetSndFlags(struct P *, int, int);
/** @sly2-annot addr=0x1FC878
 * Purpose: vt14: async SUKR cmd 7/8 status queries with callbacks 0x1FCC30/0x1FBB30; accumulate paused time
 * Info: proposed name STREAMSND_QueryStatus (T2); class STREAMSND; TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC878(struct P *p)
{
    if (p->f50 & 0x1000) {
        p->pvt[1].pfn((char *)p + p->pvt[1].d);
        func_002330D0(0x524B5553, 7, p->n40, 0, 0, 0, 0, OnStreamsndStatus, (long)p);
        SetSndFlags(p, 0x1000, 0);
    }
    if (p->n52) return;
    if (p->f50 & 0x400) {
        p->pvt[1].pfn((char *)p + p->pvt[1].d);
        func_002330D0(0x524B5553, 8, p->n40, 0, 0, 0, 0, func_001FBB30, (long)p);
        SetSndFlags(p, 0x400, 0);
    }
    if ((p->f50 & 0x800) && p->n4E <= 0)
        p->l68 += D_003DD040.l1F0 - D_003DD040.l1F8;
}
