extern char D_002A6BB0[], D_002A6BC8[], D_002A6BF8[];
void *McMkdir(void *s, char *a);
void *McWriteFile(void *s, char *a, char *b, int n);
void *WriteIconSys(void *s, char *a, char *b);
void *load_sly_icon_from_disc(void *s, char *a);
/** @sly2-annot addr=0x1AB7D0
 * Purpose: mkdir /BASCUS-97316YAOTWTD!, write data file, write icon.sys (0x1AC008), copy icon from disc
 *   (load_sly_icon_from_disc)
 * Info: proposed name CreateSaveDir (T1); TU memcard
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void *CreateSaveDir(void *s)
{
    void *p = McMkdir(s, D_002A6BB0);
    if (p)
        return p;
    p = McWriteFile(s, D_002A6BC8, D_002A6BF8, 5);
    if (p)
        return p;
    p = WriteIconSys(s, D_002A6BB0, D_002A6BF8);
    if (p)
        return p;
    return load_sly_icon_from_disc(s, D_002A6BB0);
}
