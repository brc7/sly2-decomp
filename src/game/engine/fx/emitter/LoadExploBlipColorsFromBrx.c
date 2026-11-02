typedef struct { unsigned char r, g, b, a; } RGBA;
typedef struct { int n; RGBA ac[1]; } LIST;
typedef struct { char pad[0xC]; int xC; char pad2[0xC]; LIST *x1C; } Y;
typedef struct { char pad[0xC0]; Y y; } T;
typedef struct { char pad[0xE4]; T *xE4; } X;
typedef struct { int cb; int align; } DESC;
int func_00117C20(void *);
int func_00117BD0(void *);
int func_00117C80(void *);
void ColorCorrectRgba(RGBA *);
void *func_001AE1D0(DESC *);

static inline int Min(int a, int b) { return a < b ? a : b; }

/** @sly2-annot addr=0x258F40
 * Purpose: EXPLO twin of 0x2584A8 [game linkonce (inline/template)]
 * Info: proposed name LoadExploBlipColorsFromBrx (T1); TU game_linkonce
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: sdk_names+parse-verified
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void LoadExploBlipColorsFromBrx(X *p, void *ps)
{
    int n = func_00117C20(ps);
    int i;
    Y *y;
    LIST *l;
    union { DESC d; RGBA c; } u;
    if (n == 0)
        return;
    y = &p->xE4->y;
    u.d.cb = n * 4 + 4;
    u.d.align = 0x10;
    l = func_001AE1D0(&u.d);
    y->x1C = l;
    y->xC = func_00117BD0(ps);
    l->n = Min(n, 32);
    for (i = 0; i < n; i++) {
        *(int *)&u.c = func_00117C80(ps);
        ColorCorrectRgba(&u.c);
        if (i < l->n)
            l->ac[i] = u.c;
    }
}
