struct S { char *p0; char pad[0x64]; void *x68; };
void *func_0013E3C8(int, int, int, int);
void func_00221268(void *, int);
void func_00194118(struct S *);
void func_001A0300(char *, void *, void *);
void func_0020B868(void *);
void func_00137A30(char *, int, int, void *, int, int, int, void *);
void func_001CC5E8(void *, char *, char *);
/** @sly2-annot addr=0x1910F8
 * Purpose: PostLoad of the grab context: PloNew(CID 0xF TN, oid 0x148) as its private TN (SetTnTnsOverride
 *   0), reset TN params (0x194118), two place tables (0x1A0300), blend blocks, 3-entry swing blend (0x137A30
 *   cbBl 0xC), reach tracker bound to owner SMA x1030
 * Info: proposed name PostLoadJtGrab (T1); TU grab
 * Notes: runtime2: private TN oid 0x148
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources: r4b_rest+runtime
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void func_001910F8(struct S *p) {
    char *pb = (char *)p;
    p->x68 = func_0013E3C8(0xF, 0, 0x148, -1);
    func_00221268(p->x68, 0);
    func_00194118(p);
    func_001A0300(p->p0, pb + 0x70, pb + 0x170);
    func_001A0300(p->p0, pb + 0x330, pb + 0x430);
    func_0020B868(pb + 0x180);
    func_0020B868(pb + 0x240);
    func_00137A30(p->p0, 0xC, 3, pb + 0x2F0, 0, 0, 0, pb + 0x314);
    func_001CC5E8(pb + 0x440, p->p0, p->p0 + 0x1030);
}
