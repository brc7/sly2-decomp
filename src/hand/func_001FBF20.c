struct X { char pad[0x14]; int n14; float g18; char pad2[0x40 - 0x1C]; void *p40; float g44; char pad3[0x50 - 0x48]; unsigned short fl; };
void func_00232500(void *, int, int, int, int, int);
/** @sly2-annot addr=0x1FBF20
 * Purpose: vt13: send vol(*1024)/pan/pitch(*12) for dirty bits 0x10/0x20/0x40
 * Info: proposed name ApplyBanksndParams (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FBF20(struct X *px)
{
    int fl = px->fl;
    if (fl & 0x70) {
        int m = 0;
        int a, b;
        if (fl & 0x10) { m = 1; a = (int)(px->g44 * 1024.0f); }
        if (fl & 0x20) m |= 4;
        if (fl & 0x40) { m |= 8; b = (int)(px->g18 * 12.0f); }
        func_00232500(px->p40, m, a, px->n14, b, 0);
    }
}
