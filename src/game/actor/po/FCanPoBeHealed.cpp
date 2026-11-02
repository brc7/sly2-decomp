extern "C" void func_00129EC8(struct CRef *pref);
struct CRef {
    int tagk; int tag;
};
class OBase { public: int a, b, c; virtual void V1(); virtual void V2(); virtual void Msg(int, void *, int, void **, CRef *); };
extern "C" int FCanPoBeHealed(OBase *p);
/** @sly2-annot addr=0x1C41B8
 * Purpose: PO virtual slot 80 (Lilly FCanBeHealed): sends message 0xD with a bool CRef (default 1) and
 *   returns the answer
 * Info: proposed name FCanPoBeHealed (T1); class PO slot80; TU po
 * Notes: names_v2 C proposed InitRoh__FP3ROH (0.638)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
int FCanPoBeHealed(OBase *p)
{
    CRef ref;
    int f = 1;
    ref.tagk = 0x17;
    ref.tag = f;
    p->Msg(0xD, 0, 0, 0, &ref);
    if (ref.tagk == 0x17) f = ref.tag;
    func_00129EC8(&ref);
    return f;
}
