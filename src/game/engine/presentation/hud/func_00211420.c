struct E { int x0; int x4; int x8; };
extern struct E D_002C3288[];
struct Z { int x0; int x4; };
struct Y { char pad[0x224]; int x224; char pad228[0x1C]; struct Z *x244; };
struct X { char pad[0x4C8]; int x4C8; char pad4cc[0x9A0 - 0x4CC]; struct Y *x9A0; char pad9a4[0x1318 - 0x9A4]; int x1318; };
extern char D_003DD490[];
struct X *PpoHero(void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
char *PchzFindSwLangstr__FP2SWi(void *, int);
void func_0021ED58(char *, char *);
/** @sly2-annot addr=0x211420
 * Purpose: STRINGGEN id 0x2F4: name of the object the player operates in mode x4C8==0xC (cid 0x5E table
 *   D_002C3288 / cid 0x5C field / player x1318)
 * Info: proposed name BuildOperatedObjectText (T2); TU hud
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211420(char *pch)
{
    int id = -1;
    struct X *x = PpoHero(D_003DD490);
    if (x && FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x34)) {
        struct Y *y = x->x9A0;
        if (y) {
            int k = x->x4C8;
            if (k == 0xC) {
                if (FIsBasicDerivedFrom__FP5BASIC3CID(y, 0x5E))
                    id = D_002C3288[y->x244->x4].x4;
                else if (FIsBasicDerivedFrom__FP5BASIC3CID(y, 0x5C))
                    id = y->x224;
                else
                    id = x->x1318;
            }
        }
    }
    if (id != -1)
        func_0021ED58(pch, PchzFindSwLangstr__FP2SWi(D_003DD490, id));
    else
        *pch = 0;
}
