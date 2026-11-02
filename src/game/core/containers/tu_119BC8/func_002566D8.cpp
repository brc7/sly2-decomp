struct VT { short d; short pad; void *pfn; };
struct O { char pad[0xC]; VT *vt; char x10[4]; };
struct DESC { int cb; int align; DESC(int c, int a) { cb = c; align = a; } };
struct MARK { int a[4]; };
extern "C" {
void *STACKSCOPE_ctor(MARK *);
void STACKSCOPE_ctor_1AE4A0(MARK *, int);
void *func_001AE318(DESC *);
int func_00117DB8(void *);
void func_00116D70(void *, int);
void *func_002333B4(void *, int, int);
}

/** @sly2-annot addr=0x2566D8
 * Purpose: virtual: read count, then per element memset/vt4 load/vt5 add (stack scratch scope) [game linkonce
 *   (inline/template)]
 * Info: proposed name LoadListFromBrx_vt (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
extern "C" void func_002566D8(O *p, void *pv)
{
    ((void (*)(void *))p->vt[2].pfn)((char *)p + p->vt[2].d);
    MARK m;
    STACKSCOPE_ctor(&m);
    DESC d(4, 0x10);
    void *buf = func_001AE318(&d);
    int n = func_00117DB8(pv);
    func_00116D70(p->x10, n);
    if (n > 0) do {
        func_002333B4(buf, 0, 4);
        ((void (*)(void *, void *, void *))p->vt[4].pfn)((char *)p + p->vt[4].d, buf, pv);
        ((void (*)(void *, void *))p->vt[5].pfn)((char *)p + p->vt[5].d, buf);
    } while (--n);
    STACKSCOPE_ctor_1AE4A0(&m, 2);
}
