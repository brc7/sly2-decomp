struct E { int a; int b; int pad[13]; int c; };
struct S { unsigned int n; E ae[4]; };
/** @sly2-annot addr=0x1B3A48
 * Purpose: Append a movie request (0x40-byte entry) to CMpeg queue (max 4) if valid
 * Info: proposed name CMpeg_QueueMovie (T1); class CMpeg; TU mpeg
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#open-questions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
extern "C" void CMpeg_QueueMovie(S *p, E *pe)
{
    if (pe && pe->b != pe->c && p->n < 4) {
        p->ae[p->n++] = *pe;
    }
}
