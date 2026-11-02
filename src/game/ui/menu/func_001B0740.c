struct S { char pad[0x1C]; int f; };
extern char D_002DEB30[], D_003DD490[], D_003D9ED0[];
extern char D_002C01D0[], D_002C01F0[], D_002C0210[];
void BroadcastGameEvent(void *p, int a, int b);
void *PpoHero(void *p);
int CidBasic(void *p);
void func_001AF4D8(void *p, int a, int b, int c, char *s);
/** @sly2-annot addr=0x1B0740
 * Purpose: Game event 0x29; push page for the current player class (JT 0x35 / BENTLEY 0x37 / MURRAY 0x36)
 * Info: proposed name OpenPauseCharacterPage (T2); TU menu
 * Notes: names_v2 C proposed UpdateTnCallback__FP2TN5MSGIDPv (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001B0740(struct S *p)
{
    char *s;
    BroadcastGameEvent(D_002DEB30, 0x29, 0);
    switch (CidBasic(PpoHero(D_003DD490))) {
    case 0x35: s = D_002C01D0; break;
    case 0x37: s = D_002C01F0; break;
    case 0x36: s = D_002C0210; break;
    default: return;
    }
    func_001AF4D8(D_003D9ED0, 0, 0, 0, s);
    if (p) p->f |= 2;
}
