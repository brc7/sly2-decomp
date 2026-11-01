extern int D_003DA180;
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *g, int n);
void func_0021ED58(char *p, void *q);
void func_001DCA30(char *pch) {
    int id;
    switch (D_003DA180) {
    case -1: id = 0xDE; break;
    case 0: id = 0xDF; break;
    case 1: id = 0xE0; break;
    default: *pch = 0; return;
    }
    func_0021ED58(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, id));
}
