struct A { int n; };
struct M { char pad[0x14]; struct A *pa; };
struct S { char pad[0x184]; int st; };
int FIsBasicDerivedFrom__FP5BASIC3CID(int, int);
int FIgnoreSoIntersection(struct S *, struct M *);
/** @sly2-annot addr=0x13C278
 * Purpose: cid 0x67 vt61: while dropped (5) ignore STEPPLAYERs (3), else base
 * Info: proposed name FIgnoreBootyIntersection (T1); class BOOTY slot61; TU booty
 * Notes: names_v2 v1 proposed TriggerAlarm__FP5ALARM4ALTK (0.002)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
int FIgnoreBootyIntersection(struct S *p, struct M *pm) {
    if (p->st == 5 && FIsBasicDerivedFrom__FP5BASIC3CID(pm->pa->n, 0x34)) return 3;
    return FIgnoreSoIntersection(p, pm);
}
