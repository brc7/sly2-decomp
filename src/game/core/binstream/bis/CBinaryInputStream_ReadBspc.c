typedef struct CBinaryInputStream CBinaryInputStream;
typedef struct MSG { int cb; int nKind; } MSG;
typedef struct ITEM { char ab[0x20]; } ITEM;
typedef struct OWNER { char pad[0xC]; ITEM *aitem; } OWNER;
typedef struct NODE { struct NODE *pnodeLeft; struct NODE *pnodeRight; void *pv; } NODE;
typedef struct TREE { int cnode; int n4; int n8; NODE *anode; } TREE;

int U32Read__18CBinaryInputStream(CBinaryInputStream *pbis);
int U16Read__18CBinaryInputStream(CBinaryInputStream *pbis);
void *func_001AE2A0(MSG *pmsg);
extern char D_002BBAF0[];

/** @sly2-annot addr=0x118220
 * Purpose: Reads a BSP collision tree: U32 node count, allocate (0x1AE2A0), per node U16 plane/children
 *   indices
 * Info: proposed name CBinaryInputStream_ReadBspc (T1); class CBinaryInputStream; TU bis
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r1_116918+parse-verified
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/formats/level-objects.md#4-common-building-blocks-cparse,
 *   docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream_ReadBspc(CBinaryInputStream *pbis, OWNER *powner, TREE *ptree)
{
    int i;
    MSG msg;

    ptree->cnode = U32Read__18CBinaryInputStream(pbis);
    ptree->n4 = U32Read__18CBinaryInputStream(pbis);
    ptree->n8 = U32Read__18CBinaryInputStream(pbis);
    msg.cb = ptree->cnode * sizeof(NODE);
    msg.nKind = 0x10;
    ptree->anode = (NODE *)func_001AE2A0(&msg);

    for (i = 0; i < ptree->cnode; i++) {
        NODE *pnode = &ptree->anode[i];
        unsigned int iitem = U16Read__18CBinaryInputStream(pbis);
        if (iitem == 0xFFFF) {
            pnode->pv = D_002BBAF0;
        } else {
            unsigned int inode;
            pnode->pv = &powner->aitem[iitem];
            inode = U32Read__18CBinaryInputStream(pbis);
            if (inode != 0xFFFFFFFF) {
                pnode->pnodeLeft = &ptree->anode[inode];
            }
            inode = U32Read__18CBinaryInputStream(pbis);
            if (inode != 0xFFFFFFFF) {
                pnode->pnodeRight = &ptree->anode[inode];
            }
        }
    }
}
