struct VECTOR { float x, y, z; } __attribute__((aligned(16)));
extern "C" void ConvertUserRgbToUserHsv(VECTOR *pvecHsv, VECTOR *pvecRgb);

/** @sly2-annot addr=0x11DFA0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
void ConvertUserHsvToUserRgb(VECTOR *pvecHsv, VECTOR *pvecRgb)
{
    VECTOR vecHsv = *pvecHsv;
    vecHsv.y *= 1.0f / 255.0f;
    vecHsv.z *= 1.0f / 255.0f;
    ConvertUserRgbToUserHsv(&vecHsv, pvecRgb);
    pvecRgb->x *= 255.0f;
    pvecRgb->y *= 255.0f;
    pvecRgb->z *= 255.0f;
}
