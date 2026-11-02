// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct AREQ { int cb; int cbAlign; } AREQ;
int func_001AE2A0(AREQ *preq);
static inline int New(unsigned int cb) { AREQ req; req.cb = cb; req.cbAlign = 0x10; return func_001AE2A0(&req); }
typedef struct { char pad[0x10]; int x10; char pad2[0x1C]; int x30; } T;
typedef struct { char pad[0xE4]; T *xE4; } S;
T *PemitbEnsureEmitter(S *, int);
/** @sly2-annot addr=0x258A00
 * Purpose: set EMITO kind (pemitb+0x10) and allocate its data (COW first) (EMITTER copy) [game linkonce
 *   (inline/template)]
 * Info: proposed name SetEmitterEmitoKind (T2); TU game_linkonce
 * Notes: level_parser CONFIRMED-code (layout read from the matched function; class absent from samples)
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names+parse-code
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00258A00(S *p, int k)
{
    T *t;
    if (p->xE4->x10 == k) return;
    if (p->xE4->x10 != 0) return;
    t = PemitbEnsureEmitter(p, 1);
    t->x10 = k;
    switch (k) {
    case 1: t->x30 = New(0x20); break;
    case 2: t->x30 = -1; break;
    case 3: t->x30 = New(0x30); break;
    }
}
