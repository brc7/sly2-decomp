struct B { int a; void *p; };
int func_00132B10(void *p, int k);
int func_0020F948(void *a, struct B *b);
/** @sly2-annot addr=0x21FB18
 * Purpose: FindSwObjectsInBox filter: reject objects with flag 0x59 else default filter 0x20F948
 * Info: proposed name FFilterJtthrowArcObject (T2); TU throw
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/player.md#throw-0x21f2e0-0x220d40
 * @end-sly2-annot */
int func_0021FB18(void *a, struct B *b)
{
    if (func_00132B10(b->p, 0x59))
        return 1;
    return func_0020F948(a, b);
}
