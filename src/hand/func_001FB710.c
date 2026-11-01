struct VT { int pad[4]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *pvt; };
struct DL;
extern struct DL D_003DD08C;
int func_0011A380(struct DL *, void *);
void func_0011A2E8(struct DL *, void *);
/** @sly2-annot addr=0x1FB710
 * Purpose: If in pending DL, remove and Decref (vt2)
 * Info: proposed name RemoveSndFromDirtyList (T1); TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FB710(struct X *px)
{
    if (func_0011A380(&D_003DD08C, px)) {
        func_0011A2E8(&D_003DD08C, px);
        px->pvt->pfn((char *)px + px->pvt->d);
    }
}
