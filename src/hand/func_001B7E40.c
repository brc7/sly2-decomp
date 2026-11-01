struct P { char pad[0x2C4]; int x2C4; char pad2[0x13F0 - 0x2C8]; void *x13F0; };
void func_001B6078(struct P *p, int n);
void func_001B7BA8(struct P *p);
/** @sly2-annot addr=0x1B7E40
 * Purpose: MURRAY vt88: map generic player state 0..8 to Murray states (0->carry 5 or idle 0, 1->jump 2,
 *   2->13, 3->14, 4->16, 5..8->17..20), dropping carried object where needed
 * Info: proposed name SetMurrayPlayerState (T1); class MURRAY slot88; TU murray
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B7E40(struct P *p, unsigned int k)
{
    switch (k) {
    case 0:
        if (p->x13F0) func_001B6078(p, 5);
        else func_001B6078(p, 0);
        break;
    case 1:
        switch (p->x2C4) { case 15: case 16: break; default: func_001B7BA8(p); }
        func_001B6078(p, 2);
        break;
    case 2: func_001B6078(p, 0xD); break;
    case 3: func_001B7BA8(p); func_001B6078(p, 0xE); break;
    case 4: func_001B6078(p, 0x10); break;
    case 5: func_001B7BA8(p); func_001B6078(p, 0x11); break;
    case 6: func_001B7BA8(p); func_001B6078(p, 0x12); break;
    case 7: func_001B7BA8(p); func_001B6078(p, 0x13); break;
    case 8: func_001B7BA8(p); func_001B6078(p, 0x14); break;
    }
}
