typedef struct {
    char Head[4];
    unsigned short Reserv1;
    unsigned short OffsLF;
    unsigned int Reserv2;
    unsigned int TransRate;
    int BgColor[4][4];
    float LightDir[3][4];
    float LightColor[3][4];
    float Ambient[4];
    unsigned char TitleName[68];
    unsigned char FnameView[64];
    unsigned char FnameCopy[64];
    unsigned char FnameDel[64];
    unsigned char Reserve3[512];
} ICONSYS;
char *strcpy(char *, const char *);
void *func_002333B4(void *, int, unsigned int);
void ConvertPchzToSjis(char *, unsigned char *);
int func_0021ECD8(char *, const char *, ...);
int McWriteFile(void *s, char *name, void *b, int n);

/** @sly2-annot addr=0x1AC008
 * Purpose: Build PS2 icon.sys (memset, title 'Sly 2', sprintf) and write it
 * Info: proposed name WriteIconSys (EXISTING); TU memcard
 * Notes: agent/verifier proposes WriteIconSys for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> WriteIconSys (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+verdicts_tu_r4r6+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int WriteIconSys(void *p, char *dir, char *title)
{
    ICONSYS icon;
    char name[0x50];
    func_002333B4(&icon, 0, sizeof(icon));
    strcpy(icon.Head, "PS2D");
    icon.OffsLF = 0x20;
    icon.TransRate = 0x60;
    icon.BgColor[0][0] = 0;
    icon.BgColor[0][1] = 0x87;
    icon.BgColor[0][2] = 0xFF;
    icon.BgColor[1][0] = 0;
    icon.BgColor[1][1] = 0x29;
    icon.BgColor[1][2] = 0x6F;
    icon.BgColor[2][0] = 0;
    icon.BgColor[2][1] = 0x29;
    icon.BgColor[2][2] = 0x6F;
    icon.BgColor[3][0] = 0;
    icon.BgColor[3][1] = 0x87;
    icon.BgColor[3][2] = 0xFF;
    icon.LightDir[0][0] = 0.0f;
    icon.LightDir[0][1] = 0.5f;
    icon.LightDir[0][2] = -0.5f;
    icon.LightDir[1][0] = 0.5f;
    icon.LightDir[1][1] = -0.4f;
    icon.LightDir[1][2] = 0.1f;
    icon.LightDir[2][0] = -0.5f;
    icon.LightDir[2][1] = 0.5f;
    icon.LightDir[2][2] = 0.5f;
    icon.LightColor[0][0] = 0.14f;
    icon.LightColor[0][1] = 0.03f;
    icon.LightColor[0][2] = 0.48f;
    icon.LightColor[1][0] = 0.33f;
    icon.LightColor[1][1] = 0.0f;
    icon.LightColor[1][2] = 0.33f;
    icon.LightColor[2][0] = 0.14f;
    icon.LightColor[2][1] = 0.14f;
    icon.LightColor[2][2] = 0.38f;
    icon.Ambient[0] = 0.5f;
    icon.Ambient[1] = 0.5f;
    icon.Ambient[2] = 0.5f;
    ConvertPchzToSjis(title, icon.TitleName);
    strcpy((char *)icon.FnameView, "memcard_icon.ico");
    strcpy((char *)icon.FnameCopy, "memcard_icon.ico");
    strcpy((char *)icon.FnameDel, "memcard_icon.ico");
    func_0021ECD8(name, "%s/%s", dir, "icon.sys");
    return McWriteFile(p, name, &icon, sizeof(icon));
}
