typedef struct CBinaryInputStream CBinaryInputStream;
typedef struct MSG { int cb; int nKind; } MSG;
typedef struct ITEM { char ab[0x20]; } ITEM;
typedef struct OWNER { char pad[0xC]; ITEM *aitem; } OWNER;
typedef struct NODE { struct NODE *pnodeLeft; struct NODE *pnodeRight; void *pv; } NODE;
typedef struct TREE { int cnode; int n4; int n8; NODE *anode; } TREE;

int func_00117C80(CBinaryInputStream *pbis);
int func_00117C20(CBinaryInputStream *pbis);
void *func_001AE2A0(MSG *pmsg);
extern char D_002BBAF0[];

void CBinaryInputStream_ReadBspc(CBinaryInputStream *pbis, OWNER *powner, TREE *ptree)
{
    int i;
    MSG msg;

    ptree->cnode = func_00117C80(pbis);
    ptree->n4 = func_00117C80(pbis);
    ptree->n8 = func_00117C80(pbis);
    msg.cb = ptree->cnode * sizeof(NODE);
    msg.nKind = 0x10;
    ptree->anode = (NODE *)func_001AE2A0(&msg);

    for (i = 0; i < ptree->cnode; i++) {
        NODE *pnode = &ptree->anode[i];
        unsigned int iitem = func_00117C20(pbis);
        if (iitem == 0xFFFF) {
            pnode->pv = D_002BBAF0;
        } else {
            unsigned int inode;
            pnode->pv = &powner->aitem[iitem];
            inode = func_00117C80(pbis);
            if (inode != 0xFFFFFFFF) {
                pnode->pnodeLeft = &ptree->anode[inode];
            }
            inode = func_00117C80(pbis);
            if (inode != 0xFFFFFFFF) {
                pnode->pnodeRight = &ptree->anode[inode];
            }
        }
    }
}
