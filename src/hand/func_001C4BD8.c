struct LO { char pad[0x28]; unsigned long x28; char p1[0x15C - 0x30]; char x15C[0x110]; char x26C[4]; };
extern struct LO **D_002C0D28[3];
extern char D_002C0D18[];
struct LO *PpoHero(void *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct LO *plo, int n);
int PbhvFindBhvstk(void *p, int n);
/** @sly2-annot addr=0x1C4BD8
 * Purpose: Fill up to cMax PZI pointers: current PO's pzi (+0x15C) or default pzi D_002C0D18, plus other
 *   in-world STEPPLAYERs from table D_002C0D28 passing check 0xC2; returns count
 * Info: proposed name CppziCur (T2); TU po
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
int func_001C4BD8(void *p, int cMax, void **ap)
{
    int c;
    unsigned int j;
    struct LO *plo, *plo2;
    if (cMax <= 0) return 0;
    c = 1;
    plo = PpoHero(p);
    if (plo) {
    *ap = plo->x15C;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x34)) {
        for (j = 0; j < sizeof(D_002C0D28) / sizeof(D_002C0D28[0]) && c < cMax; j++) {
            plo2 = *D_002C0D28[j];
            if (!plo2 || plo2 == plo || !PbhvFindBhvstk(plo2->x26C, 0xC2)) continue;
            if ((int)(plo2->x28 >> 60) & 1) ap[c++] = plo2->x15C;
        }
    }
    } else {
        *ap = D_002C0D18;
    }
    return c;
}
