struct K { int a; int b; };
extern struct K D_002A4B58;
extern char D_003DD490[];
struct E { int f0; int f4; int f8; char pad[0x34]; };
struct S { char pad[0x184]; int st; };
void func_001F4FB0(struct S *, struct E *, int);
int FIsBasicDerivedFrom__FP5BASIC3CID(int, int);
void EnsureSwCallback(void *, void *, struct K, int);
/** @sly2-annot addr=0x13C2D8
 * Purpose: cid 0x67 vt55: base, and when dropped and touched by a non-player post callback D_002A4B58
 * Info: proposed name AdjustBootyNewXp (T1); class BOOTY slot55; TU booty
 * Notes: names_v2 C proposed EnableAlarmSensors__FP5ALARM (0.167)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void AdjustBootyNewXp(struct S *p, struct E *ae, int i) {
    func_001F4FB0(p, ae, i);
    if (p->st == 5) { struct E *pe = &ae[1 - i]; if (!FIsBasicDerivedFrom__FP5BASIC3CID(pe->f8, 0x34)) EnsureSwCallback(D_003DD490, p, D_002A4B58, 0); }
}
