struct S { char pad[0x13D0]; int k; };
void func_001F9B20(struct S *p);
void PresetStepAccel(struct S *p);
void PresetJtAccelJump(void *p);
void PresetJtAccelHide(void *p);
void PresetJtAccelZap(void *p);
void func_001B6CC0(struct S *p);
void PresetJtAccelPipe(void *p);
/** @sly2-annot addr=0x1B67F8
 * Purpose: MURRAY vt70: base accel (SO 0x1F9B20 for state 20 else STEP 0x204108), then per-state component
 *   accel (jump +0x2C0, 13 +0x4C0, 14 +0xDF0, 20 +0xF50, flop 0x1B6CC0)
 * Info: proposed name PresetMurrayAccel (T1); class MURRAY slot70; TU murray
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void PresetMurrayAccel(struct S *p)
{
    char *b = (char *)p;
    if (p->k == 20) func_001F9B20(p);
    else PresetStepAccel(p);
    switch (p->k) {
    case 2: PresetJtAccelJump(b + 0x2C0); break;
    case 13: PresetJtAccelHide(b + 0x4C0); break;
    case 14: PresetJtAccelZap(b + 0xDF0); break;
    case 3: func_001B6CC0(p); break;
    case 20: PresetJtAccelPipe(b + 0xF50); break;
    }
}
