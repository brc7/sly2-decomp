typedef int qword __attribute__((mode(TI)));
typedef struct { qword a[3]; } M3;
typedef struct { void *x0; void *x4; } S;
typedef struct { char pad[0x40]; M3 m; } R;
typedef struct { char pad[0x1C]; void *x1C; } OBJ;
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
R *CreateStuck(OBJ *, void *, void *, qword, qword);
void ConvertAloMat(M3 *, int, void *, M3 *);
void RemoveRip__FP3RIP(OBJ *);
/** @sly2-annot addr=0x1D0BF8
 * Purpose: LEAF FBounce: CreateStuck/RemoveRip on contact
 * Info: proposed name FBounceLeaf_1D0BF8 (T3); TU rip
 * Notes: COLLISION: FBounceLeaf already used at 0x1CF968; renamed FBounceLeaf_1D0BF8 | arbitration assigned
 *   FBounceLeaf to 0x1CF968; this body needs another name
 * Confidence: low (draft) [T3 0.70] - evidence: arbitration; sources: r5_1AE130
 * See: docs/open-questions.md#names-and-classes-cross-cutting,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
int func_001D0BF8(OBJ *p, S *s, qword a, qword b)
{
    R *r;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(s->x4, 0x33))
        return 0;
    r = CreateStuck(p, p->x1C, s->x0, a, b);
    if (r) {
        M3 m;
        ConvertAloMat(&m, 0, s->x4, &r->m);
        r->m = m;
    }
    RemoveRip__FP3RIP(p);
    return 1;
}
