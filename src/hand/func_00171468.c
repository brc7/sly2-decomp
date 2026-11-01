struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct E { int k; int pad; float g8; };
struct O { char pad[0xF80]; int nF80; char pad2[0xFD0 - 0xF84]; char aFD0[4]; };
struct S { char pad[0x10]; struct O *p10; int pad2; void *p18; char pad3[0x1C]; struct E *pe; float g3C; int n40; int pad4; float g48; };
int func_0017EE78(void *p);
int func_0017CE30(void *p);
int func_0017D188(void *p);
int func_0017DCB8(void *p);
int func_00174FB8(void *p);
void func_00209818(void *p, float g);
void func_001EF0D8(void *p);
/** @sly2-annot addr=0x171468
 * Purpose: Whether the running command's sub-behaviour is still active (goto/look/wait/13/14 kinds); pops it
 *   when done
 * Info: proposed name FBhv169CmdBusy (T2); TU tu_1709A8
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
int func_00171468(struct S *p) {
    if (!p->n40) return 0;
    switch (p->pe->k) {
    case 6:
        switch (p->p10->nF80) {
        case -1:
            if (func_0017EE78(p->p18)) p->n40 = 0;
            break;
        case 0:
            if (func_0017CE30(p->p18)) p->n40 = 0;
            break;
        case 1:
            if (func_0017D188(p->p18)) p->n40 = 0;
            break;
        }
        break;
    case 7:
        if (func_0017DCB8(p->p18)) {
            p->n40 = 0;
            func_00209818(p->p10->aFD0, p->g48);
        }
        break;
    case 5:
        if (p->pe->g8 < D_002DDED0.t - p->g3C) p->n40 = 0;
        break;
    case 13: case 14:
        if (func_00174FB8(p->p18)) p->n40 = 0;
        break;
    }
    if (!p->n40) func_001EF0D8(&p->p18);
    return p->n40;
}
