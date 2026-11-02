struct X { int k; char pad[0xEC - 4]; void *vt; };
extern char D_002D7738[];
extern char D_002D76E8[];
extern char D_002D7698[];
extern char D_002D7648[];
extern char D_002D75F8[];
extern char D_002D75A8[];
extern char D_002D74B8[];
extern char D_002D7558[];
extern char D_002D7508[];
extern char D_002D7788[];
extern char D_002D7468[];
void *func_002333B4(void *p, int c, int cb);
/** @sly2-annot addr=0x1CE6D0
 * Purpose: Placement-construct a RIP of type ript: memset to type size (0xF0/0x100/0x110) and set the type's
 *   vtable (11 types, vtables 0x2D7468..0x2D7788)
 * Info: proposed name PvtripFromRipt (T1); TU rip
 * Notes: names_v2 C proposed PvtripFromRipt__F4RIPT (0.638) | returns the VTRIP vtable for a RIPT (table of
 *   11 RIP vtables); r5 PripConstructRipt (was PripConstructRipt)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: verify_r3r5_summary
 * See: docs/engine/particles-effects.md#2-rip-particles-ripcpp-0x1cdcd0-0x1d1370,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
struct X *PvtripFromRipt(struct X *p, unsigned int k)
{
    struct X *px;
    switch (k) {
    case 3:
        func_002333B4(p, 0, 0x100);
        p->vt = D_002D7738;
        px = p;
        break;
    case 10:
        func_002333B4(p, 0, 0x100);
        p->vt = D_002D76E8;
        px = p;
        break;
    case 8:
        func_002333B4(p, 0, 0x100);
        p->vt = D_002D7698;
        px = p;
        break;
    case 6:
        func_002333B4(p, 0, 0x110);
        p->vt = D_002D7648;
        px = p;
        break;
    case 4:
        func_002333B4(p, 0, 0x100);
        p->vt = D_002D75F8;
        px = p;
        break;
    case 9:
        func_002333B4(p, 0, 0x110);
        p->vt = D_002D75A8;
        px = p;
        break;
    case 5:
        func_002333B4(p, 0, 0xF0);
        p->vt = D_002D74B8;
        px = p;
        break;
    case 1:
        func_002333B4(p, 0, 0x100);
        p->vt = D_002D7558;
        px = p;
        break;
    case 0:
        func_002333B4(p, 0, 0xF0);
        p->vt = D_002D7508;
        px = p;
        break;
    case 7:
        func_002333B4(p, 0, 0xF0);
        p->vt = D_002D7788;
        px = p;
        break;
    case 2:
        func_002333B4(p, 0, 0x100);
        p->vt = D_002D7468;
        px = p;
        break;
    }
    px->k = k;
    return px;
}
