typedef int qword __attribute__((mode(TI)));
struct Y { char pad[0x28]; unsigned long fl; };
struct X { char pad[0x1010]; qword pos; };
void func_00195A00(void *p, int a, int b, int c);
void AddStepAxisXps(struct X *p, void *a, struct Y *b, qword d);
/** @sly2-annot addr=0x140C30
 * Purpose: vt82 (STEP VUnk81(grf,plo,qw..)): if other LO not in world (bit60 of +0x28) reset jtair (0x195A00
 *   state 0), else call STEP base 0x205008 with v - pos
 * Info: proposed name STEPPLAYER_VUnk81 (T2); class STEPPLAYER slot82; TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140C30(struct X *p, void *a, struct Y *b, qword v)
{
    qword d;
    if (!((b->fl >> 60) & 1)) {
        func_00195A00((char *)p + 0x2C0, 0, 0, 0);
        return;
    }
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(v), "j"(p->pos));
    AddStepAxisXps(p, a, b, d);
}
