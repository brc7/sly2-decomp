typedef struct { int x0,x4,cost,xC,x10,ids1,ids2,x1C; } REC;
typedef struct { char *name; int ids,pad; } REC2;
typedef struct {
    char pad[0x30];
    int x30,pad34,x38,pad3C,x40;
    char pad44[0x254-0x44];
    int x254,x258;
} TBL;
extern TBL D_003DA150;
extern REC D_002BCD00[];
extern REC2 D_002C3288[];
extern struct G D_003DD490;
extern char D_002DEB30[],D_002A7A38[];
char *PchzFindSwLangstr__FP2SWi(struct G *,int);
int func_00161798(void *);
int sprintf(char *,const char *,...);
char *strcpy(char *,const char *);
static inline void CopyId(char *p,int id)
{
    strcpy(p,PchzFindSwLangstr__FP2SWi(&D_003DD490,id));
}
/** @sly2-annot addr=0x1DCAB0
 * Purpose: Text callback 0x435: popup message, or the selected power-up's name, description and affordability
 *   (0xD9/0xDA), or the loot sell line (0xDB)
 * Info: proposed name BuildThiefnetInfoText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/minigames.md#8-safehouse-and-thiefnet-ccode,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildThiefnetInfoText(char *pch)
{
    TBL *pt=&D_003DA150;
    if(pt->x30!=-1) {
        CopyId(pch,pt->x38);
        return;
    } else {
        switch(pt->x40) {
        case 0: {
            unsigned int i=pt->x254;
            if(i<0x2B) {
                if(D_002BCD00[i].ids2!=pt->x30) {
                    int *pc=&D_002BCD00[i].cost;
                    int count=func_00161798(D_002DEB30);
                    int idsT=count<*pc?0xD9:0xDA;
                    struct G *pg=&D_003DD490;
                    char *fmt=D_002A7A38;
                    char *pch1=PchzFindSwLangstr__FP2SWi(pg,D_002BCD00[i].ids1);
                    char *pch2=PchzFindSwLangstr__FP2SWi(pg,D_002BCD00[i].ids2);
                    sprintf(pch,fmt,pch1,pch2,PchzFindSwLangstr__FP2SWi(pg,idsT));
                    return;
                }
            }
            CopyId(pch,0xDC);
            return;
        }
        case 1: {
            unsigned int i=pt->x258;
            if(i<0x40) {
                sprintf(pch,PchzFindSwLangstr__FP2SWi(&D_003DD490,0xDB),D_002C3288[i].name);
                return;
            }
            CopyId(pch,0xE3);
            return;
        }
        default:
            *pch=0;
            return;
        }
    }
}
