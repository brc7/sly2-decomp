struct G;
extern struct G D_003DD490;
struct S { char pad[0xEC]; void *o; int n; };
void RequestTaskState(void *o, int n, void *p);
char *PpoHero(struct G *pg);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
void func_0019D948(void *pv);
/** @sly2-annot addr=0x2036E8
 * Purpose: mark target photographed (task +0xEC or local flag)
 * Info: proposed name SetReconTargetDone (T2); TU tu_201D78
 * Notes: names_v2 C proposed FIgnoreBrkpIntersection__FP4BRKPP2SO (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void func_002036E8(struct S *p, int n)
{
    if (p->o) {
        char *o;
        RequestTaskState(p->o, 2, p);
        o = PpoHero(&D_003DD490);
        if (o && FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x34)) func_0019D948(o + 0xF24);
    } else p->n = n;
}
