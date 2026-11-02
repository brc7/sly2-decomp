// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
extern char D_002D9978[], D_002DA568[], D_002CB1F0[], D_002DA9D0[];
typedef struct { int x0, x4, x8; void *xC; } B;
typedef struct { int x0, x4, x8; void *xC; char pad[0x70]; char x80[0xF4]; int x174; char pad2[0xC38]; char xDB0[0x70]; B xE20; } S;
void *func_00167310(void *); void *LEAP_ctor(void *);
/** @sly2-annot addr=0x256360
 * Purpose: inline constructor (SO base vtable 0x2D9978 -> STEPGUARD vtable 0x2DA568, LEAP sub-object) [game
 *   linkonce (inline/template)]
 * Info: proposed name STEPGUARD_ctor (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
S *STEPGUARD_ctor(S *p)
{
    B *q;
    char *v = D_002CB1F0;
    p->x0 = 0;
    p->x4 = 0;
    p->xC = D_002D9978;
    func_00167310(p->x80);
    p->x174 = 0;
    p->xC = D_002DA568;
    LEAP_ctor(p->xDB0);
    q = &p->xE20;
    q->xC = v;
    p->xE20.x0 = 0;
    p->xE20.x4 = 0;
    q->xC = D_002DA9D0;
    return p;
}
