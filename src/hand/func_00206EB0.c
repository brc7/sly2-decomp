struct X { char pad[0x4C]; int n4C; };
int func_00209A68(void *p, int n);
struct X *func_00205458(void *p, int n);
int func_00206D48(void *p);
int func_00132B10(void *p, int n);

/** @sly2-annot addr=0x206EB0
 * Purpose: Whether special state k is allowed: -1 always; never while dead (flag 7) or BHV169 busy; 8/0xD/0xE
 *   yes; 0x15 not for cids 0x3E/0x42/0x43; 0x22/0x25 not for rage-immune; 0xB only for plain guards
 * Info: proposed name FCanSetStepguardSpecial (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00206EB0(void *p, int k)
{
    struct X *x;
    if (k == -1)
        return 1;
    if (func_00209A68(p, 7))
        return 0;
    x = func_00205458(p, 0xA9);
    if (x && x->n4C)
        return 0;
    switch (k) {
    case 8:
    case 0xD:
    case 0xE:
        return 1;
    case 0x15:
        return !(func_00132B10(p, 0x3E) || func_00132B10(p, 0x42) || func_00132B10(p, 0x43));
    case 0x22:
    case 0x25:
        return !func_00206D48(p);
    case 0xB:
        return !(func_00132B10(p, 0x3B) || func_00132B10(p, 0x3D) || func_00132B10(p, 0x40) ||
                 func_00132B10(p, 0x3E) || func_00132B10(p, 0x41) || func_00132B10(p, 0x3F) ||
                 func_00132B10(p, 0x3C) || func_00132B10(p, 0x42) || func_00132B10(p, 0x43));
    }
    return 0;
}
