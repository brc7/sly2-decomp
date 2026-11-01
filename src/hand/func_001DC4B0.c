extern struct CLOCK { float a[16]; } D_002DDED0;
typedef struct {
    char pad[0x28];
    int state;
    float t;
    char pad30[0x48 - 0x30];
    int c;
    char pad4c[0x24C - 0x4C];
    int i;
} OBJ;
void func_001DC260(OBJ *, int);
void func_001DBBE0(OBJ *, int);
void *func_001FBB90(int);
void func_001FB2F8(void *, int);
/** @sly2-annot addr=0x1DC4B0
 * Purpose: ThiefNet slot 14: dir 0 -> sell tab, 4 -> buy tab, 1-3 previous row, 5-7 next row (with sounds)
 * Info: proposed name SetThiefnetStickDir (T2); TU screen
 * Notes: verifier PLAUSIBLE: Screen vtable slot 14 (vtable 0x2D8520 region). State setter for an 8-way
 *   direction: 0 -> 0x1DC260(1), 4 -> 0x1DC260(0), 1-3 select row i-1, 5-7 row i+1, with sounds 0x35 (moved)
 *   / 0x36 (at end). The | T2 forced: verifier: role word not supported by the code
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DC4B0(OBJ *p, unsigned int n)
{
    if (n == p->state)
        return;
    p->state = n;
    p->t = D_002DDED0.a[15];
    switch (n) {
    case 0:
        func_001DC260(p, 1);
        break;
    case 4:
        func_001DC260(p, 0);
        break;
    case 1:
    case 2:
    case 3:
        if (p->i > 0) {
            func_001DBBE0(p, p->i - 1);
            func_001FB2F8(func_001FBB90(0x35), 0);
        } else {
            func_001FB2F8(func_001FBB90(0x36), 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        if (p->i < p->c - 1) {
            func_001DBBE0(p, p->i + 1);
            func_001FB2F8(func_001FBB90(0x35), 0);
        } else {
            func_001FB2F8(func_001FBB90(0x36), 0);
        }
        break;
    }
}
