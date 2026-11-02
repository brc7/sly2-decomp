/** @sly2-annot addr=0x1DDD40
 * Purpose: World index 0-8 -> episode name string id 0x6D-0x75
 * Info: proposed name IdsFromWorld (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
int IdsFromWorld(int n) {
    int r = -1;
    switch (n) {
    case 0: r = 0x6D; break;
    case 1: r = 0x6E; break;
    case 2: r = 0x6F; break;
    case 3: r = 0x70; break;
    case 4: r = 0x71; break;
    case 5: r = 0x72; break;
    case 6: r = 0x73; break;
    case 7: r = 0x74; break;
    case 8: r = 0x75; break;
    }
    return r;
}
