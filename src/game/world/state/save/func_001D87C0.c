typedef char *va_list;
#define va_arg_pch(ap) (*(char **)((ap += 8) - 8))
struct MSG { int cb; int nKind; };
typedef struct { int c; short *ash; } V;
typedef struct { int a[2]; char ht[1]; } H;
extern H D_003E0EA8;
void *func_001AE318(struct MSG *);
int func_001D8720(void *, char *);
static inline int NFind(H *ph, char *pch) { return func_001D8720(ph->ht, pch); }
short func_00233800(char *);
/** @sly2-annot addr=0x1D87C0
 * Purpose: Turn a NULL-terminated varargs list of strings into a key (short array of symbol ids; non-symbols
 *   via atoi as array indices)
 * Info: proposed name BuildSaveKeyV (T1); TU save
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
void func_001D87C0(char *pch, va_list ap, V *pv)
{
    va_list apSave = ap;
    char *pchT;
    int i;
    pv->c = 0;
    pv->ash = 0;
    for (pchT = pch; pchT; pchT = va_arg_pch(ap))
        pv->c++;
    if (pv->c) {
        struct MSG msg;
        msg.cb = pv->c * 2;
        msg.nKind = 0x10;
        pv->ash = func_001AE318(&msg);
        ap = apSave;
        for (pchT = pch, i = 0; pchT; pchT = va_arg_pch(ap), i++) {
            int n = NFind(&D_003E0EA8, pchT);
            if (n == -1)
                pv->ash[i] = func_00233800(pchT);
            else
                pv->ash[i] = n;
        }
    }
}
