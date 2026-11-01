extern char D_002E6550[], D_003DD490[];
int func_001ACC80(void *p);
char *PchzFindSwLangstr__FP2SWi(void *p, int n);
char *strcpy(char *, const char *);
void func_0021EDA0(char *a, const char *b);
void func_001AD820(char *buf)
{
    switch (func_001ACC80(D_002E6550)) {
    case 1:
        strcpy(buf, "&2S&.: ");
        func_0021EDA0(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x38));
        break;
    case 2:
        strcpy(buf, "&2S&. : ");
        func_0021EDA0(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x36));
        func_0021EDA0(buf, "       ");
        func_0021EDA0(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x37));
        func_0021EDA0(buf, " : &2T&.");
        break;
    }
}
