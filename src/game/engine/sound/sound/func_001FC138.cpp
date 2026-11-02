struct A; struct B;
struct X {
    static void *operator new(unsigned int cb);
    X(A *pa, B *pb);
    char pad[0x3C0];
};
/** @sly2-annot addr=0x1FC138
 * Purpose: new STREAMSND(SNDSTREAM*, kind) (0x3C0 bytes) or NULL
 * Info: proposed name PstreamsndNew (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" X *func_001FC138(A *pa, B *pb)
{
    X *px;
    if (pa)
        px = new X(pa, pb);
    else
        px = 0;
    return px;
}
