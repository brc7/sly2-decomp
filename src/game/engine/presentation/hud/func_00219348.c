struct Y { char pad[0x30]; void *x30; char pad2[0x3C]; };
struct F { int x0; struct Y *ay; char pad[0x10]; };
struct Q { char pad[8]; struct F *af; };
struct E { char pad[6]; unsigned char c; char pad7[0xD]; unsigned short *pi; char pad18[8]; };
struct S { int x0; struct E *ae; char pad8[8]; struct Q *pq; };
void func_002158C0(void *p, struct Y *y);
void func_002156B8(struct Y *y);
/** @sly2-annot addr=0x219348
 * Purpose: reset items referenced by a key
 * Info: proposed name ResetSpriteKeyItems (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00219348(struct S *p, int i, int j)
{
    struct E *e = &p->ae[i];
    struct Y *ay = p->pq->af[j].ay;
    int k;
    for (k = 0; k < e->c; k++) {
        struct Y *y = &ay[e->pi[k]];
        if (y->x30)
            func_002158C0(y->x30, y);
        else
            func_002156B8(y);
    }
}
