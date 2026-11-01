void func_001EEF78(void *p);
void func_001B6078(void *p, int n);
/** @sly2-annot addr=0x146EE0
 * Purpose: BHV198 slot 9: clear +0x1AC0, Murray state 0
 * Info: proposed name CloseBhv198 (T2); class BHV198 slot9; TU tu_144B28
 * Notes: superseded: OnBhv198Pop (r2b_rest) | BHV vtable slot 9 role per r9 (CONFLICTS #15); was OnBhv198Pop
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00146EE0(char *p)
{
    func_001EEF78(p);
    *(int *)(*(char **)(p + 0x10) + 0x1AC0) = 0;
    func_001B6078(*(void **)(p + 0x10), 0);
}
