struct CLOCK { float t; };
extern CLOCK D_002DDED0;
struct PO { char pad[0x1024]; int x1024; };
enum HFDK { HFDK_Nil };
struct JT { PO *p0; char pad[0x28]; HFDK hfdk; float tHfdk; int x34; char pad2[0x50-0x38]; int x50; char pad3[0x68-0x54]; int x68; };
/** @sly2-annot addr=0x1940B0
 * Purpose: (existing name) Set hide/grab facing kind hfdk (+0x2C) and tHfdk=g_clock.t; kind 3 -> x34=1; 1/2
 *   -> x34=2 if owner TN matches
 * Info: proposed name SetJtHfdk (EXISTING); TU grab
 * Notes: RENAME proposed -> SetJtHfdk (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.75] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4b_rest+arbitration
 * See: docs/engine/player.md#5-sly-jt-jtcpp-0x188488--component-tus, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void SetJtHfdk(JT *pjt, HFDK hfdk)
{
    if (hfdk == pjt->hfdk) return;
    pjt->hfdk = hfdk;
    pjt->tHfdk = D_002DDED0.t;
    pjt->x50 = 0;
    switch (hfdk) {
    case 3:
        pjt->x34 = 1;
        break;
    case 1: case 2:
        if (pjt->p0->x1024 == pjt->x68) pjt->x34 = 2;
        else pjt->x34 = 0;
        break;
    }
}
