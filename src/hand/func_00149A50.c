struct S8 { int a, b; };
struct N { int a; struct S8 s; int d; };
struct N *func_0011CCC0(void *p);
void func_0011A070(void *p, struct N *n);
/** @sly2-annot addr=0x149A50
 * Purpose: Append a deferred callback {object, pointer-to-member, arg} from pool g_sw+0x3C90 to the SW
 *   callback list (+0x3CA8)
 * Info: proposed name PostSwCallback (T1); TU callback
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/gameplay-objects.md#source-files,
 *   docs/engine/world-sw.md#6-deferred-callbacks-and-the-splice-schedule,
 *   docs/engine/world-sw.md#9-source-files
 * @end-sly2-annot */
void func_00149A50(char *p, int a, struct S8 s, int d)
{
    struct N *n = func_0011CCC0(p + 0x3C90);
    n->a = a;
    n->s = s;
    n->d = d;
    func_0011A070(p + 0x3CA8, n);
}
