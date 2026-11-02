struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &);
    void Decref();
};
struct FRAME {
    int a[4];
    FRAME();
    ~FRAME();
};
struct KEY;
struct TRIE;
struct KEYS { KEY *a[11]; };
struct GS { char pad[0x3B2C]; KEY *k[11]; };
extern "C" {
extern GS D_003DD490;
extern TRIE D_003E0EA8;
int FGsMatchesSchema(TRIE *, char *);
CRef RefGetSaveVar(TRIE *, KEY *, char *);
CRef func_00258200(KEY *, CRef *, char *);
void func_002333B4(void *, int, unsigned int);
void ResetRumbleSaveVar(void);
}
static inline CRef Get(KEY *k, char *pb) { return RefGetSaveVar(&D_003E0EA8, k, pb); }
static inline CRef Set(KEY *k, CRef *r, char *pb) { return func_00258200(k, r, pb); }
/** @sly2-annot addr=0x1D8D80
 * Purpose: Clear GS blob (0x5400) and reset rumble var; if f and schema matches, preserve 11 settings vars
 *   (keys in global +0x3B2C) across the clear
 * Info: proposed name ResetSaveGs (T2); TU save
 * Notes: save-TU g_gs blob reset; ResetGs__Fv (0x16F868, GS graphics) is a different function; integrator
 *   name (was ResetGs)
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
extern "C" void func_001D8D80(char *pb, int f)
{
    FRAME frame;
    KEY *keys[11] = { D_003DD490.k[0], D_003DD490.k[7], D_003DD490.k[10], D_003DD490.k[8], D_003DD490.k[9], D_003DD490.k[1], D_003DD490.k[2], D_003DD490.k[3], D_003DD490.k[4], D_003DD490.k[5], D_003DD490.k[6] };
    CRef aref[11];
    unsigned int i;
    if (f && !FGsMatchesSchema(&D_003E0EA8, pb))
        f = 0;
    if (f) {
        for (i = 0; i < 11; i++)
            aref[i] = Get(keys[i], pb);
    }
    func_002333B4(pb, 0, 0x5400);
    ResetRumbleSaveVar();
    if (f) {
        for (i = 0; i < 11; i++)
        {
            CRef ref(aref[i]);
            const CRef &r = Set(keys[i], &ref, pb);
        }
    }
}
