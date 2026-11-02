struct O { char p0[8]; int *pn; char p1[0xF0-0xC]; int oid; float gF4; };
extern struct G D_003DD490;
void PostRipEmit(struct O *p, struct O *q);
int PloFindSwObject__FP2SWi3OIDP2LO(struct G *g, int k, int oid, int n);
/** @sly2-annot addr=0x1D1108
 * Purpose: REMIT (RIPT 2) PostEmit: resolve target emitter oid via SW lookup, copy rate
 * Info: proposed name PostRemitEmit (T1); TU rip
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void PostRemitEmit(struct O *p, struct O *q) {
    PostRipEmit(p, q);
    p->oid = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, q->oid, *q->pn);
    p->gF4 = q->gF4;
}
