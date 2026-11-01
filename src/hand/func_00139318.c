struct G { char pad[0x4070]; int f4070; };
extern struct G D_003DD490;
void *PdlFromSwCid(struct G *, int);
void RemoveSlEntry(void *, void *);
void func_0011CD98(void *, void *);
/** @sly2-annot addr=0x139318
 * Purpose: BLIPG vt2: remove from the cid list and return to pool g_sw+0x4070
 * Info: proposed name DeleteBlipg (T2); class BLIPG slot2; TU blip
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void func_00139318(void *p) {
    RemoveSlEntry(PdlFromSwCid(&D_003DD490, 0x1c), p);
    func_0011CD98(&D_003DD490.f4070, p);
}
