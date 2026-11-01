typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x18]; int x18; };
extern qword D_002A2DE0;
void func_001400B8(struct S *p);
void func_0020B868(void *p);
void func_001A0300(struct S *p, void *a, void *b);
void func_0020BD88(void *p, struct S *s);
void func_0021F388(void *p);
void func_001426E8(struct S *p, int n);
void *func_001F4C58(struct S *p, int a, int b);
int func_0012DC18(struct S *p, void *a, int b, int c, int d, qword q);
void func_001B6078(struct S *p, int n);
/** @sly2-annot addr=0x1B49E0
 * Purpose: MURRAY vt15: STEPPLAYER PostLoad, post-load the 4 locomotion blends, strafe direction table
 *   (0x1A0300 x16F0/x17F0), JTTHROW blends (0x21F388), optional attached effect (x1ACC), then
 *   SetMurrayState(0)
 * Info: proposed name PostMurrayLoad (T1); class MURRAY slot15; TU murray
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B49E0(struct S *p)
{
    char *b = (char *)p;
    void *v;
    func_001400B8(p);
    func_0020B868(b + 0x1320);
    func_0020B868(b + 0x1460);
    func_0020B868(b + 0x1630);
    func_0020B868(b + 0x1540);
    func_001A0300(p, b + 0x16F0, b + 0x17F0);
    func_0020BD88(b + 0x1850, p);
    func_0021F388(b + 0x1910);
    func_001426E8(p, 0x16);
    v = func_001F4C58(p, p->x18, 0xC);
    if (v) {
        *(int *)(b + 0x1ACC) = func_0012DC18(p, v, 0, 7, 2, D_002A2DE0);
    }
    func_001B6078(p, 0);
}
