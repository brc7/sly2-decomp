struct VT { int pad[4]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *pvt; };
struct DL;
extern struct DL D_003DD08C;
int FFindDlEntry__FP2DLPv(struct DL *, void *);
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
/** @sly2-annot addr=0x1FB710
 * Purpose: If in pending DL, remove and Decref (vt2)
 * Info: proposed name RemoveSndFromDirtyList (T1); TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void RemoveSndFromDirtyList(struct X *px)
{
    if (FFindDlEntry__FP2DLPv(&D_003DD08C, px)) {
        RemoveDlEntry__FP2DLPv(&D_003DD08C, px);
        px->pvt->pfn((char *)px + px->pvt->d);
    }
}
