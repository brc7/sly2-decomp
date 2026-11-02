struct G { char pad[0x3B0C]; void *p3b0c; };
extern struct G D_003DD490;
extern char D_002A3928[];
struct S { char pad[0x1C]; int f; };
void WipeToWorldWarp(struct G *pg, void *s, int a, int b, int c, int d);
void func_00162E38(void *p);
/** @sly2-annot addr=0x1B09A8
 * Purpose: WipeToWorldWarp(...,0x189) and fail current job
 * Info: proposed name PauseItemReturnToHideout (T1); TU menu
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5b_rest+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/formats/disc-records.md#3-static-records-in-the-elf,
 *   docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void PauseItemReturnToHideout(struct S *p)
{
    struct G *pg = &D_003DD490;
    WipeToWorldWarp(pg, D_002A3928, 0x189, -1, 0x80, -1);
    if (pg->p3b0c) func_00162E38(pg->p3b0c);
    if (p) p->f |= 2;
}
