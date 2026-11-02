struct E { char pad[7]; unsigned char c; unsigned char *pb; char padc[0x14]; };
struct S { int x0; struct E *ae; char pad8[8]; void *x10; };
void func_00218748(void *p, int k);
/** @sly2-annot addr=0x2190F8
 * Purpose: for action bytes ==7 call 0x218748
 * Info: proposed name FinishSpriteSoundEvents (T2); TU hud
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_002190F8(struct S *p, int i, int k)
{
    struct E *e = &p->ae[i];
    int j;
    for (j = 0; j < e->c; j++) {
        if (e->pb[j] == 7)
            func_00218748(p->x10, k);
    }
}
