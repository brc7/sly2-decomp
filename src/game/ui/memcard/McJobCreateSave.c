struct B { char data[0x5464]; };
struct S { int x0; int x4; int x8; int xC; int x10, x14, x18; int x1C; int x20; struct B a[3]; char pad2[0xFD5C - 0x24 - 3 * 0x5464]; char xFD5C[1]; };
extern char D_002A6C00[], D_002A38E8[], D_002A6C08[], D_002A6C28[], D_002A6BC8[], D_002A6BB0[], D_002E6550[];
int sceMcGetEntSpace(int a, int b, char *c);
int McSyncResult(void *p, int a, int b, int *c);
int FOpenSaveIconFile(void *p);
int CreateSaveDir(struct S *s);
int WriteSaveSlotFileReset(struct S *s, struct B *b);
void BuildSaveFilePath(struct S *s, struct B *b, char *name);
void McDelete(struct S *s, char *name);
void func_001ACC38(void *p, int n);
/** @sly2-annot addr=0x1AB868
 * Purpose: Thread job: check space (sceMcGetEntSpace), delete stale, CreateSaveDir, write slot files
 * Info: proposed name McJobCreateSave (T1); TU memcard
 * Notes: names_v2 v1 proposed FUN_0018c250 (0.000)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int McJobCreateSave(struct S *s)
{
    char name[0x50];
    int n;
    int k;
    int r;
    int i;
    void *p = s->xFD5C;
    k = 1;
    r = McSyncResult(p, sceMcGetEntSpace(s->x0, s->x4, D_002A6C00), 0x12, &n);
    if (r == 0) {
        if (!n)
            k = 2;
        k += 5;
        k += (FOpenSaveIconFile(D_002A38E8) + 0x3FF) / 0x400;
        k += 0x42;
        if (s->x1C < k) {
            r = 1;
        } else {
            r = CreateSaveDir(s);
            if (r == 0) {
                for (i = 0; i < 3; i++) {
                    r = WriteSaveSlotFileReset(s, &s->a[i]);
                    if (r)
                        goto cleanup;
                }
                if (s->xC < 5)
                    s->xC = 5;
                goto done;
            }
cleanup:
            for (i = 0; i < 3; i++) {
                BuildSaveFilePath(s, &s->a[i], name);
                McDelete(s, name);
            }
            McDelete(s, D_002A6C08);
            McDelete(s, D_002A6C28);
            McDelete(s, D_002A6BC8);
            McDelete(s, D_002A6BB0);
        }
    }
    if (r)
        func_001ACC38(D_002E6550, 3);
done:
    return r;
}
