struct G { char pad[0x228]; void *p228; };
extern struct G D_003DD040;
int func_001FE990(struct G *);
void func_001FC2E0(void *, int, float);
void func_001FC380(void *, int, int);
/** @sly2-annot addr=0x1FF108
 * Purpose: Splice: set volume/pan of ambience sub-group selected by id
 * Info: proposed name SetAmbienceGroup (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FF108(int a, int b, int c, float g)
{
    int r = func_001FE990(&D_003DD040);
    void *p = D_003DD040.p228;
    func_001FC2E0(p, r, g);
    func_001FC380(p, r, c);
}
