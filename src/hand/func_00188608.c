typedef int qword __attribute__((mode(TI)));
#define F(T, o) (*(T *)((char *)p + (o)))
#define A(o) ((char *)p + (o))

extern void func_001400B8(void *p);
extern void func_001A64E8(void *p, int n, void *pv);
extern int func_001F4C58(void *p, int a, int b);
extern void func_0020B868(void *p);
extern void func_001A0300(void *p, void *a, void *b);
extern void func_0018F908(void *p, void *a, void *b);
extern int func_0012DBB0(void *p, int a, int b);
extern int func_0012DC18(void *p, int a, int b, int c, int d, qword q);
extern void func_00230100(void *p, int a, int b);
extern void func_001426E8(void *p, int a);
extern void func_0018B348(void *p);
extern void func_00137A30(void *p, int a, int b, void *c, int d, int e, int f, void *g);
extern void func_001CC5E8(void *a, void *p, void *b);
extern void func_0018C250(void *p, int a, int b);
extern void func_001A6CA8(void *p, int a);
extern int func_001F0380(int a, int b, int c, int d);
extern char D_002BF1B0[];
extern qword D_002A2DD0;

/** @sly2-annot addr=0x188608
 * Purpose: JT vt15 PostLoad: base 0x1400B8, BindLoOidTable, look up ~10 named child ALOs/actuators (0x1F4C58
 *   by id+kind) into x10B8/x19C0..x19E0, sub-object postloads (0x1A0300, 0x18F908), build blend/aseg tables,
 *   SM applies
 * Info: proposed name PostJtLoad (T1); class JT slot15; TU jt
 * Notes: names_v2 C proposed PostStepLoadRun__FP4STEPP5BLRUNPP6ASEGBL (0.438)
 * Confidence: high [T1 0.80] - evidence: static naming pass; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_00188608(char *p)
{
    int n;
    func_001400B8(p);
    func_001A64E8(p, 0x12, D_002BF1B0);
    F(int, 0x10B8) = func_001F4C58(p, 0x138, 1);
    F(int, 0x19E0) = func_001F4C58(p, 0x18, 6);
    F(int, 0x19DC) = func_001F4C58(p, 0x19, 6);
    F(int, 0x19D8) = func_001F4C58(p, 0x13, 1);
    F(int, 0x19D4) = func_001F4C58(p, 0x12, 1);
    F(int, 0x19D0) = func_001F4C58(p, 0x1C, 6);
    F(int, 0x19CC) = func_001F4C58(p, 0x1D, 6);
    F(int, 0x19C8) = func_001F4C58(p, 0xD, 1);
    F(int, 0x19C4) = func_001F4C58(p, 0xC, 1);
    F(int, 0x19C0) = func_001F4C58(p, 0xC, 6);
    func_0020B868(A(0x1320));
    func_0020B868(A(0x1C30));
    func_001A0300(p, A(0x1AC0), A(0x1BC0));
    func_0018F908(p, A(0x1970), A(0x1994));
    func_001A0300(p, A(0x1E10), A(0x1F10));
    F(int, 0x19E4) = func_0012DBB0(p, F(int, 0x10B8), 5);
    F(int, 0x19E8) = func_0012DBB0(p, F(int, 0x19C8), 5);
    F(int, 0x19EC) = func_0012DBB0(p, F(int, 0x19C4), 5);
    F(int, 0x19F0) = func_0012DBB0(p, F(int, 0x19D8), 7);
    F(int, 0x19F4) = func_0012DBB0(p, F(int, 0x19D4), 7);
    F(int, 0x19FC) = func_0012DBB0(F(char *, 0x19A0), *(int *)(F(char *, 0x19A0) + 0x58), 5);
    F(int, 0x19F8) = func_0012DBB0(p, F(int, 0x19C0), 5);
    n = func_0012DC18(p, F(int, 0x19C0), 5, 5, 2, D_002A2DD0);
    F(int, 0x1A00) = n;
    func_00230100(p, F(int, 0x19C0), n);
    func_001426E8(p, 0x17);
    func_0018B348(p);
    func_00137A30(p, 0xC, 2, A(0x1A04), 0, 0, 0, A(0x1A1C));
    func_00137A30(p, 0xC, 2, A(0x1A20), 0, 0, 0, A(0x1A38));
    func_00137A30(p, 0xC, 2, A(0x1A3C), 0, 0, 0, A(0x1A54));
    func_00137A30(p, 0xC, 2, A(0x1A58), 0, 0, 0, A(0x1A70));
    func_001A0300(p, A(0x13D0), A(0x14D0));
    func_00137A30(p, 0xC, 4, A(0x1F60), 0, 0, 0, A(0x1F90));
    func_00137A30(p, 0xC, 3, A(0x2014), 0, 0, 0, A(0x2038));
    F(int, 0x1804) = F(int, 0x19A4);
    func_001CC5E8(A(0x17D0), p, A(0x18E0));
    func_0018C250(p, 0, -1);
    func_001A6CA8(p, 1);
    if (F(int, 0x1D24))
        F(int, 0x1D28) = func_001F0380(F(int, 0x1D24), F(int, 0x1D20), -1, 1);
}
