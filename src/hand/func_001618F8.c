struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x8C - 0x10]; int f8c; };
struct S { char pad[0x3BC4]; struct O *po; char pad2[0x476C - 0x3BC8]; float t; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
/** @sly2-annot addr=0x1618F8
 * Purpose: Shows HUD widget g_sw+0x3BC4 (clears +0x8C, vt49 Show(-1)) and sets its hide time g_sw+0x476C =
 *   now + dt
 * Info: proposed name ShowSwWidget3BC4Timed (T2); TU game
 * Notes: names_v2 C proposed StopSound__FP3AMBi (0.638) | verifier PLAUSIBLE: Offset-in-name placeholder.
 *   Body: widget g_sw+0x3BC4: +0x8C=0, vt49(-1); g_sw+0x476C = g_clock.t + dt. | T2 forced: verifier:
 *   placeholder name
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r3b_rest+verdicts_followups
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_001618F8(struct S *p, float dt)
{
    struct O *po = p->po;
    if (po) {
        struct VTE *e;
        po->f8c = 0;
        e = &po->vt[49];
        e->pfn((char *)po + e->d, -1);
    }
    p->t = D_002DDED0.t + dt;
}
