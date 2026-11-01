typedef int ti128 __attribute__((mode(TI)));
struct S { char pad[0x10b8]; int f10b8; char pad2[0x1320 - 0x10bc]; char a1320[0xc0]; char a13e0[0x100]; char a14e0[0x10];
    char a14f0[0x100]; char a15f0[0x10]; char a1600[0x110]; char a1710[0x10]; };
extern ti128 D_002BC920;
void func_001400B8(struct S *);
int func_001F4C58(struct S *, int, int);
void func_001A0300(struct S *, void *, void *);
void func_0020B868(void *);
void func_00136D28(void *, struct S *, ti128, float, float);
void func_001426E8(struct S *, int);
void func_00135248(struct S *, int);
/** @sly2-annot addr=0x1347A8
 * Purpose: BENTLEY vt15: PostLoadStepplayer, find child SO 0xB, bind anim blends, init crosshair/aim helper
 *   (0x136D28), AttachStepplayerToStartMarker(0x16), SetBentleyState(0)
 * Info: proposed name PostBentleyLoad (T1); class BENTLEY slot15; TU bentley
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_001347A8(struct S *p) {
    func_001400B8(p);
    p->f10b8 = func_001F4C58(p, 0xb, 1);
    func_001A0300(p, p->a13e0, p->a14e0);
    func_001A0300(p, p->a14f0, p->a15f0);
    func_0020B868(p->a1320);
    func_0020B868(p->a1600);
    func_00136D28(p->a1710, p, D_002BC920, -50.0f, 50.0f);
    func_001426E8(p, 0x16);
    func_00135248(p, 0);
}
