struct MSG { int cb; int nKind; };
struct G { char pad[0x45C8]; void *p45c8; void *p45cc; };
void *PloNew__F3CIDP2SO3OIDi(int a, int b, int c, int d);
void InitDefaultPathzoneCg(void *p);
void *func_001AE2A0(struct MSG *pmsg);
void func_001C1138(void *p);
/** @sly2-annot addr=0x1C10C8
 * Purpose: If no zone BSP: create default pathzone (PloNew cid 17 oid 0x2CA) + InitDefaultPathzoneCg; then
 *   bind zones
 * Info: proposed name PostLoadSwPathzones (T1); TU path
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void PostLoadSwPathzones(struct G *p)
{
    if (!p->p45cc) {
        struct MSG msg;
        p->p45c8 = PloNew__F3CIDP2SO3OIDi(0x11, 0, 0x2CA, -1);
        InitDefaultPathzoneCg(p->p45c8);
        msg.cb = 0x30;
        msg.nKind = 0x10;
        p->p45cc = func_001AE2A0(&msg);
    }
    func_001C1138(p->p45cc);
}
