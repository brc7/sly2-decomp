/* func_00140DB0 (Claude agent for Lilly, T1644): saves s->v, scales it by 0.01 when the
   other entry's k (s->ae[1 - i].k) is one of three known values and p allows it
   (p->p150->x80 != 1 or func_00144068(p)), calls func_001FAD00 with the same arguments,
   then restores s->v.
   Matching notes: `save` is a plain qword that is only read by "j" asm operands, so it
   becomes a VU-class pseudo that is spilled across the call (the game's lq/sq v1 + lqc2
   vf3 from 0x10(sp)); the byte offset in its own variable makes the address
   (ae + off), not (off * 64 + ae). */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct E { int a, b, k; char pad[0x34]; };
struct S { char pad[0x40]; qword v; struct E *ae; };
struct X150 { char pad[0x80]; int x80; };
struct P { char pad[0x150]; struct X150 *p150; };
extern int D_002E1E40;
extern int D_002DD5BC;
extern int D_002F7900;
int func_00144068(struct P *p);
void func_001FAD00(struct P *p, void *a, struct S *s, void *b, int i);
/** @sly2-annot addr=0x140DB0
 * Purpose: vt68 (SO VUnk67 contact impulse): when the other body is the current player/D_002DD5BC/D_002F7900
 *   and the player is not controlled or is pushing (0x144068), scale the contact velocity by 0.01 before
 *   calling base 0x1FAD00
 * Info: proposed name STEPPLAYER_VUnk67 (T2); class STEPPLAYER slot68; TU stepplayer
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140DB0(struct P *p, void *a, struct S *s, void *b, int i)
{
    U c;
    qword save;
    int kk, off;
    save = s->v;
    off = (1 - i) * 64;
    kk = ((struct E *)((char *)s->ae + off))->k;
    if (kk == D_002E1E40 || kk == D_002DD5BC || kk == D_002F7900) {
        if (p->p150->x80 != 1 || func_00144068(p)) {
            qword vo;
            __asm__("qmtc2.ni %1, %0" : "=j"(c.q) : "r"(0.01f));
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(vo) : "j"(save), "j"(c.q));
            s->v = vo;
        }
    }
    func_001FAD00(p, a, s, b, i);
    s->v = save;
}
