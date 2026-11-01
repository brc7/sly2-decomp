struct S { int f0; int f4; };
int func_00257D80(void);
void func_002580A8(int *, int);
void func_00258178(int);
/** @sly2-annot addr=0x132B78
 * Purpose: If psidebag (+4) is null create one (0x257D80) and addref
 * Info: proposed name EnsureBasicSidebag (T1); TU basic
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
void func_00132B78(struct S *p) {
    int *pf = &p->f4;
    if (p->f4 == 0) { func_002580A8(pf, func_00257D80()); func_00258178(p->f4); }
}
