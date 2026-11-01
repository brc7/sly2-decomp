typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT { qword q; };
struct VU_VECTOR { qword q; };
/** @sly2-annot addr=0x256D28
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
qword operator*(VU_FLOAT f, VU_VECTOR v)
{
    qword r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f.q), "j"(v.q));
    return r;
}
