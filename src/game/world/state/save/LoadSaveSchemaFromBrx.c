struct MSG { int cb; int nKind; };
typedef struct BIS BIS;
typedef struct { int c; void *pv; char ht[1]; } OBJ;
extern char D_002DD590[];
extern struct G D_003D4A60;
int func_00117C20(BIS *);
void func_00117968(BIS *, int, void *);
void CBinaryInputStream_ReadStringSw(BIS *, char **, void *, int);
void InitSaveSymbolTable(void *, int);
void AddSaveSymbol(void *, char *, int);
void *func_001AE1D0(struct MSG *);
void ResetGsIfSchemaChanged(OBJ *, struct G *);
/** @sly2-annot addr=0x1D7B88
 * Purpose: Load save-variable schema: U16 symbol count -> hash table of (U16 id, string) names; U16 entry
 *   count -> c*10-byte trie entries; then reset g_gs if schema checksum changed
 * Info: proposed name LoadSaveSchemaFromBrx (T1); TU save
 * Notes: runtime: schema at g_sw+0x3A18 (5461 entries, 960 symbols) confirmed | splice_scripts format doc
 *   cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources:
 *   r6_1D5150+runtime+format-doc:splice_scripts
 * See: docs/engine/save-memcard.md#1-the-save-blob-g_gs-savecpp-0x1d7b88-0x1d93d8,
 *   docs/engine/save-memcard.md#4-source-files, docs/engine/save-memcard.md#5-key-functions
 * @end-sly2-annot */
void LoadSaveSchemaFromBrx(OBJ *p, BIS *pbis)
{
    struct MSG msg;
    char *pchz;
    int i;
    int n;
    int c;
    n = func_00117C20(pbis);
    InitSaveSymbolTable(p->ht, n);
    for (i = 0; i < n; i++) {
        int v = func_00117C20(pbis);
        CBinaryInputStream_ReadStringSw(pbis, &pchz, D_002DD590, 0x2D);
        AddSaveSymbol(p->ht, pchz, v);
    }
    c = func_00117C20(pbis);
    p->c = c;
    msg.cb = c * 10;
    msg.nKind = 0x10;
    p->pv = func_001AE1D0(&msg);
    func_00117968(pbis, c * 10, p->pv);
    ResetGsIfSchemaChanged(p, &D_003D4A60);
}
