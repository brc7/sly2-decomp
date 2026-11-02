struct S { char pad[0x180]; int x180; char p1[0x198 - 0x184]; int x198; void *x19C; char p2[0x1F0 - 0x1A0]; int x1F0; };
void UpdateSo(struct S *p);
int FRelayFiring(void *p);
void func_001C8730(struct S *p, int n);
void func_001C84D8(struct S *p);
void func_001C8188(int f);
/** @sly2-annot addr=0x1C8350
 * Purpose: CID 78 vt16: per-state update (input / open)
 * Info: proposed name UpdateVault (T2); class VAULT slot16; TU prize
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C8350(struct S *p)
{
    int s;
    UpdateSo(p);
    do {
        s = p->x180;
        switch (s) {
        case 1:
            if (!p->x19C || !FRelayFiring(p->x19C))
                func_001C8730(p, 2);
            break;
        case 2:
            if (!p->x19C || !FRelayFiring(p->x19C)) {
                if (p->x198 == p->x1F0)
                    func_001C8730(p, 3);
                else
                    func_001C8730(p, 4);
            } else {
                func_001C84D8(p);
            }
            break;
        case 4:
            if (!p->x19C || !FRelayFiring(p->x19C))
                func_001C8730(p, 0);
            break;
        case 3:
            if (!p->x19C || !FRelayFiring(p->x19C)) {
                func_001C8188(1);
                func_001C8730(p, 5);
            }
            break;
        }
    } while (p->x180 != s);
}
