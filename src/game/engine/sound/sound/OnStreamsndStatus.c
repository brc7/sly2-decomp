struct VT { int pad[4]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x52 - 0x10]; short s52; };
void ResumeSnd(struct X *);
void SetSndFlags(struct X *, int, int);
/** @sly2-annot addr=0x1FCC30
 * Purpose: SUKR status callback (cmd 7)
 * Info: proposed name OnStreamsndStatus (T1); TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/guides/port-plan.md#64-sound--coreaudio-k12,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void OnStreamsndStatus(int f, long l)
{
    struct X *px = (struct X *)l;
    if (f) { px->s52 = 0; ResumeSnd(px); }
    else SetSndFlags(px, 0x1000, 0x1000);
    px->pvt->pfn((char *)px + px->pvt->d);
}
