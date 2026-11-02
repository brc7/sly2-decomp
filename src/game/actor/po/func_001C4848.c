struct MSG { int cb; int nKind; };
struct V { int a, b; void *p8; };
struct G { char pad[0x4778]; unsigned int c; void *a[4]; };
struct V *func_00258658(void *p, int n);
void *func_001AE2A0(struct MSG *pmsg);
/** @sly2-annot addr=0x1C4848
 * Purpose: Append LO to SW 4-entry list (+0x4778) and give its sidebag/record (0x258658) a 0x14-byte block
 * Info: proposed name AddSwPoFollower (T2); TU po
 * Notes: names_v2 B proposed PostRostLoad__FP4ROST (0.923)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void func_001C4848(struct G *p, void *q)
{
    if (p->c < 4) {
        struct V *pv;
        p->a[p->c++] = q;
        pv = func_00258658(q, 1);
        if (!pv->p8) {
            struct MSG msg;
            msg.cb = 0x14;
            msg.nKind = 0x10;
            pv->p8 = func_001AE2A0(&msg);
        }
    }
}
