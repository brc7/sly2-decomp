void func_0018C250(void *p, int n, int m);
/** @sly2-annot addr=0x18D568
 * Purpose: JT vt88: map generic player state 0..8 -> SetJtJts(0,2,7,4,0x12,0x13,0x14,0x17,8)
 * Info: proposed name SetJtPlayerState (T1); class JT slot88; TU jt
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018D568(void *p, int n)
{
    switch (n) {
    case 0: func_0018C250(p, 0, -1); break;
    case 1: func_0018C250(p, 2, -1); break;
    case 2: func_0018C250(p, 7, -1); break;
    case 3: func_0018C250(p, 4, -1); break;
    case 4: func_0018C250(p, 0x12, -1); break;
    case 5: func_0018C250(p, 0x13, -1); break;
    case 6: func_0018C250(p, 0x14, -1); break;
    case 7: func_0018C250(p, 0x17, -1); break;
    case 8: func_0018C250(p, 8, -1); break;
    }
}
