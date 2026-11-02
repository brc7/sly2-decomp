typedef struct { char pad[0x30]; int x30; int x34; int x38; char pad3c[0x48 - 0x3C]; int x48; } GS;
typedef struct { int pad[5]; int x14; int x18; int x1C; } REC;
typedef struct { char pad[0x1C]; int x1C; } OBJ;
extern GS D_003DA0F0;
extern char D_003DD490[];
extern REC D_002BCD00[];
extern char *D_002C3268[];
extern char D_002A7A10[], D_002A7A18[], D_002A7A28[];
char *PchzFindSwLangstr__FP2SWi(void *, int);
char *strcpy(char *, char *);
int sprintf(char *, char *, ...);
OBJ *func_001DAE20(GS *,int);
void *PpoHero(void *);
int IGadgetSlotFromId(void *, int);
/** @sly2-annot addr=0x1DC598
 * Purpose: Text callback for widgets 0x37A/0x37B: popup message, "none", or the selected power-up's name,
 *   slot button and description
 * Info: proposed name BuildGadgetMenuInfoText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildGadgetMenuInfoText(char *out)
{
    char buf[256];
    unsigned char *s;
    if (D_003DA0F0.x30 != -1) {
        strcpy(out, PchzFindSwLangstr__FP2SWi(D_003DD490, D_003DA0F0.x38));
    } else if (D_003DA0F0.x48 == -1) {
        strcpy(out, PchzFindSwLangstr__FP2SWi(D_003DD490, 0xDD));
    } else {
        OBJ *po = func_001DAE20(&D_003DA0F0,D_003DA0F0.x48);
        int v = IGadgetSlotFromId(PpoHero(D_003DD490), D_003DA0F0.x48);
        if ((po->x1C & 1) && v < 0) {
            strcpy(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0xE1));
            s = (unsigned char *)D_002A7A10;
        } else {
            char *sz;
            if (v >= 0)
                sz = D_002C3268[v];
            else
                sz = D_002A7A10;
            sprintf(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, D_002BCD00[D_003DA0F0.x48].x1C), sz, sz, sz);
            s = (unsigned char *)PchzFindSwLangstr__FP2SWi(D_003DD490, D_002BCD00[D_003DA0F0.x48].x18);
        }
        sprintf(out, D_002A7A18, PchzFindSwLangstr__FP2SWi(D_003DD490, D_002BCD00[D_003DA0F0.x48].x14),
                      *s ? D_002A7A28 : D_002A7A10, s, buf);
    }
}
