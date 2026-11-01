struct P { char pad[0x14]; int n14; float f18; char pad2[0x40 - 0x1C]; int n40; float f44; char pad3[0x50 - 0x48]; unsigned short f50;
    char pad4[0x70 - 0x52]; float af70[6]; int a88[6]; char pad5[0x3BC - 0xA0]; int n3BC; };
void func_00233088(int, int, int, int, int, int, int);
/** @sly2-annot addr=0x1FC740
 * Purpose: vt13: SUKR cmd 5 vol/pan/pitch, then per-group params
 * Info: proposed name ApplyStreamsndParams (T1); class STREAMSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC740(struct P *p)
{
    int i;
    if (p->f50 & 0x70) {
        int fl = 0;
        int vol, pitch;
        if (p->f50 & 0x10) {
            fl = 1;
            vol = (int)(p->f44 * 1024.0f);
        }
        if (p->f50 & 0x20) fl |= 4;
        if (p->f50 & 0x40) {
            fl |= 8;
            pitch = (int)(p->f18 * 12.0f);
        }
        func_00233088(0x524B5553, 5, p->n40, fl, vol, p->n14, pitch);
    }
    if (p->f50 & 0x180) {
        for (i = 0; i < 6; i++) {
            int m = 1 << i;
            if (p->n3BC & m)
                func_00233088(0x524B5553, 6, p->n40, i, (int)(p->af70[i] * 1024.0f), p->a88[i], 0);
        }
        p->n3BC = 0;
    }
}
