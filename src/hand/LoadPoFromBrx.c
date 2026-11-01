void LoadSoFromBrx(void *a, void *b);
void CBinaryInputStream_SkipStringSw(void *b);
void LoadPoFromBrx(void *a, void *b) { LoadSoFromBrx(a, b); CBinaryInputStream_SkipStringSw(b); }
