// save.cpp -- save variables: the schema loaded from the brx (a symbol table and a trie of named entries, each a
// typed slot at an offset of the save blob), lookup by key (a list of symbol ids built from strings), typed get/set
// as CRef values, the completion percentage, schema resets and applying the settings variables.
// TU 0x1D7B88..0x1D93D8 (24 functions, address order).
// Entry points: LoadSaveSchemaFromBrx (level load), GetSaveVarByName / SetSaveVarByName / GetSaveVarAddressV
// (named access from game code), GetCompletionPercent, ApplyGsSettings.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_save
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_save
#include "sly2/cref.h"
#include "sly2/save.h"
#include "sly2/slotheap.h"
#include "sly2/sndmgr.h"
#include "sly2/stackscope.h"
#include "sly2/sw.h"
#include "sly2/util.h"
#include "dmas.h"

// 0x1D9280 StaticInit_save and GlobalConstructors_save: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* EE (single-float EABI) stdarg, from gcc's va-mips.h. */
typedef char *VA_LIST;
#define VA_START(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
#define VA_ARG_PCH(ap) (*(char **)((ap += 8) - 8))

struct CBinaryInputStream;
struct MEMREQ { int cb; int cbAlign; };

/* A save key: symbol ids (or numbers), one per path element. */
struct SVKEY
{
    int c;
    short *ash;
};

/* The schema (g_svstore = SW::savevars) is sly2/sw.h's SAVEVARS: SVENT entries and the SVN symbol table (SVNHASH). */


/* The result of a lookup: the leaf and the array index. */
struct SVFIND
{
    SVENT *pent;
    int i;
};



/* GS display registers as ApplyGsSettings writes the screen offset (two copies of DISPLAY). */
struct DISP1 { unsigned long dx : 12; unsigned long dy : 11; unsigned long rest : 41; };
struct DISP2 { unsigned int dx : 12; unsigned int dy : 11; unsigned long rest : 41; };
struct GSREGS { char pad[0x18]; DISP1 d1; char pad20[0x40 - 0x20]; DISP2 d2; };

/* save's .data: bytes per element by kind, and the completion weight of each world's progress variable. */
int D_002C2D90[11] = {4, 4, 4, 4, 4, 16, 16, 8, 12, 4, 4};
int D_002C2DC0[9] = {13, 86, 97, 147, 141, 160, 106, 182, 166};
extern const char D_002A78F0[];   /* defined below where the ELF has it (.rodata order) */
extern const char D_002A78F8[];   /* defined below where the ELF has it (.rodata order) */
extern unsigned char D_002AA6A1[] __asm__("_ctype_+1");  /* ctype table */
extern GSREGS D_002DEB80;

void CopyAb(void *pvDst, void *pvSrc, unsigned int cb);
extern "C" {
unsigned short U16Read__18CBinaryInputStream(CBinaryInputStream *stream);
void Read__18CBinaryInputStreamiPv(CBinaryInputStream *stream, int cb, void *pv);
void CBinaryInputStream_ReadStringSw(CBinaryInputStream *stream, char **ppchz, void *pheap, int n);
void *AllocFromWorldHeap(MEMREQ *pmemreq);
void *AllocCopyFromWorldHeap(MEMREQ *pmemreq, void *pv);
void *AllocFromStack(MEMREQ *pmemreq);
void *memset(void *pv, int c, unsigned int cb);
int strcasecmp(const char *pchz1, const char *pchz2);
int atoi(const char *pchz);
CRef GetGlobalSaveVar(SVKEY *key, SAVEDATA *saveSet);
CRef SetGlobalSaveVar(SVKEY *key, const CRef &ref, SAVEDATA *saveSet);
void ResetRumbleSaveVar();
int CountRegionLanguages();
void SetLanguage(int lang);
void SetSndmgrPlaybackMode(void *psndmgr, int mode);
void SetSndmgrChannelVolume(void *psndmgr, int ch, int n, float u);
unsigned int fptoui(float g);
/* CRef setters with C names (splice/ref.cpp). */
CRef *CRef_SetBool(CRef *pref, int f);
CRef *CRef_SetVector(CRef *pref, const qword *pvec);
CRef *CRef_SetClq(CRef *pref, const qword *pclq);
CRef *CRef_SetLm(CRef *pref, const void *plm);
CRef *CRef_SetSmp(CRef *pref, const void *psmp);

void ResetGsIfSchemaChanged(SAVEVARS *schema, SAVEDATA *saveSet);
int DoesGsMatchSchema(SAVEVARS *schema, SAVEDATA *saveSet);
int FindSaveVar(SAVEVARS *schema, SVKEY *key, SVFIND *result);
void *GetSaveVarAddress(SAVEVARS *schema, SVKEY *key, SAVEDATA *saveSet);
CRef GetSaveVar(SAVEVARS *schema, SVKEY *key, SAVEDATA *saveSet);
CRef SetSaveVar(SAVEVARS *schema, SVKEY *key, const CRef &ref, SAVEDATA *saveSet);
void InitSaveSymbolTable(SVNHASH *symbolTable, int csym);
unsigned int HashSaveSymbolName(const char *pchz);
void AddSaveSymbol(SVNHASH *symbolTable, char *pchz, int symid);
int FindSaveSymbol(SVNHASH *symbolTable, const char *pchz);
void BuildSaveKeyV(const char *pchz, VA_LIST ap, SVKEY *key);
}

/* Sum of the schema bytes (its checksum). */
static inline int SumAb(unsigned char *pb, unsigned int cb)
{
    int sum = 0;
    while (cb)
    {
        sum += *pb++;
        cb--;
    }
    return sum;
}

// Index of the symbol pchz in the schema's symbol table.
static inline int FindSchemaSymbol(SAVEVARS *schema, const char *pchz) { return FindSaveSymbol(&schema->symtab, pchz); }

// Loads the save-variable schema from the BRX: the symbol names into a hash table, then the trie of entries; resets
// the save blob when the schema changed.
// Also known as: LoadProgressFromBrx (upstream readable / Sly 1 name; LoadProgressFromBrx__FP2SWP18CBinaryInputStream).
extern "C" void LoadSaveSchemaFromBrx(SAVEVARS *schema, CBinaryInputStream *stream)
{
    MEMREQ memreq;
    char *pchz;
    int i;
    int csym;
    int cent;
    csym = U16Read__18CBinaryInputStream(stream);
    InitSaveSymbolTable(&schema->symtab, csym);
    for (i = 0; i < csym; i++)
    {
        int symid = U16Read__18CBinaryInputStream(stream);
        CBinaryInputStream_ReadStringSw(stream, &pchz, &g_heapSw, 0x2D);
        AddSaveSymbol(&schema->symtab, pchz, symid);
    }
    cent = U16Read__18CBinaryInputStream(stream);
    schema->cent = cent;
    memreq.cb = cent * 10;
    memreq.cbAlign = 0x10;
    schema->aent = (SVENT *)AllocFromWorldHeap(&memreq);
    Read__18CBinaryInputStreamiPv(stream, cent * 10, schema->aent);
    ResetGsIfSchemaChanged(schema, &g_gs);
}

// Resets the save blob when its stored schema version or checksum differs from the loaded schema, and stores the new
// ones.
// Also known as: ValidateGameSave (upstream readable / Sly 1 name; ValidateGameSave__FPvT0).
extern "C" void ResetGsIfSchemaChanged(SAVEVARS *schema, SAVEDATA *saveSet)
{
    int sum = SumAb((unsigned char *)schema->aent, schema->cent * 10);
    int nSchema = *(unsigned short *)((unsigned char *)schema->aent + schema->cent * 10 - 6);
    if (nSchema == saveSet->nSchema && sum == saveSet->nSchemaSum)
        return;
    memset(saveSet, 0, 0x5400);
    saveSet->nSchema = nSchema;
    saveSet->nSchemaSum = sum;
}

// True when the save blob's stored schema version and checksum match the loaded schema.
extern "C" int DoesGsMatchSchema(SAVEVARS *schema, SAVEDATA *saveSet)
{
    int sum = SumAb((unsigned char *)schema->aent, schema->cent * 10);
    return *(unsigned short *)((unsigned char *)schema->aent + schema->cent * 10 - 6) == saveSet->nSchema &&
           sum == saveSet->nSchemaSum;
}

// Walks the schema trie along a key (symbol ids, numbers as array indices) and returns the entry and array index;
// false when the key does not name a variable.
// Also known as: FFindEntryKey (upstream readable / Sly 1 name; FFindEntryKey__FP4TRIEPP4PENTPi).
extern "C" int FindSaveVar(SAVEVARS *schema, SVKEY *key, SVFIND *result)
{
    SVENT *aent = schema->aent;
    SVENT *pent;
    int i;
    if (aent->ientChild == 0)
        return 0;
    pent = &aent[aent->ientChild];
    result->i = 0;
    i = 0;
    for (;;)
    {
        if (pent->symid == (unsigned short)key->ash[i])
        {
            i++;
            if (i == key->c)
            {
                result->pent = pent;
                return 1;
            }
            if (pent->fIndexed)
            {
                result->pent = pent;
                result->i = (unsigned short)key->ash[i++];
                return i == key->c;
            }
            if (pent->kind != -1)
                return 0;
            if (!pent->ientChild)
                return 0;
            pent = &schema->aent[pent->ientChild];
        }
        else
        {
            if (!pent->ientSibling)
                return 0;
            pent = &schema->aent[pent->ientSibling];
        }
    }
}

// Returns the address of a save variable in a save blob (blob + offset + index * size of its kind).
extern "C" void *GetSaveVarAddress(SAVEVARS *schema, SVKEY *key, SAVEDATA *saveSet)
{
    SVFIND find;
    FindSaveVar(schema, key, &find);
    return (char *)saveSet + (find.pent->ib + find.i * D_002C2D90[find.pent->kind]);
}

struct LMV { int a[2]; };
struct SMPV { int a[3]; };

// Reads a save variable as a Splice value, by kind (int, float, bool or packed bit, vectors...).
// Also known as: RefGetProgress (upstream readable / Sly 1 name; RefGetProgress__FP4TRIEP3KEYPc).
CRef GetSaveVar(SAVEVARS *schema, SVKEY *key, SAVEDATA *saveSet)
{
    CRef ref;
    SVFIND find;
    SVENT *pent;
    if (!FindSaveVar(schema, key, &find) || (pent = find.pent)->kind == -1)
        return ref;
    {
        unsigned char *pv = (unsigned char *)saveSet + pent->ib;
        switch (pent->kind)
        {
        case 0:
        case 1:
        case 2:
        case 10:
            ref.SetS32(((int *)pv)[find.i]);
            break;
        case 3:
            ref.SetF32(((float *)pv)[find.i]);
            break;
        case 4:
            if (pent->fIndexed)
            {
                int i = find.i;
                unsigned char b;
                pv += i / 8;
                b = 1 << (i % 8);
                CRef_SetBool(&ref, (bool)((*pv & b) != 0));
            }
            else
            {
                CRef_SetBool(&ref, *(bool *)pv);
            }
            break;
        case 5:
            CRef_SetVector(&ref, &((qword *)pv)[find.i]);
            break;
        case 6:
            CRef_SetClq(&ref, &((qword *)pv)[find.i]);
            break;
        case 7:
            CRef_SetLm(&ref, &((LMV *)pv)[find.i]);
            break;
        case 8:
            CRef_SetSmp(&ref, &((SMPV *)pv)[find.i]);
            break;
        case 9:
            ref.SetS32(((int *)pv)[find.i]);
            break;
        }
    }
    return ref;
}

// Writes a Splice value into a save variable, by kind (packed bools set or clear their bit).
// Also known as: FSetProgress (upstream readable / Sly 1 name; FSetProgress__FP4TRIEP4CRefT1Pc).
CRef SetSaveVar(SAVEVARS *schema, SVKEY *key, const CRef &refVal, SAVEDATA *saveSet)
{
    CRef ref(0x24);
    SVFIND find;
    SVENT *pent;
    if (!FindSaveVar(schema, key, &find) || (pent = find.pent)->kind == -1)
        return ref;
    {
        unsigned char *pv = (unsigned char *)saveSet + pent->ib;
        switch (pent->kind)
        {
        case 0:
        case 1:
        case 2:
        case 9:
        case 10:
            ((int *)pv)[find.i] = refVal.u.n;
            break;
        case 3:
            ((float *)pv)[find.i] = refVal.u.g;
            break;
        case 4:
            if (pent->fIndexed)
            {
                int i = find.i;
                unsigned char b;
                pv += i / 8;
                b = 1 << (i % 8);
                if (refVal.u.n)
                    *pv |= b;
                else
                    *pv &= ~b;
            }
            else
            {
                *(int *)pv = refVal.u.n;
            }
            break;
        case 5:
        case 6:
            ((qword *)pv)[find.i] = *(qword *)refVal.u.pv;
            break;
        case 7:
            ((LMV *)pv)[find.i] = *(LMV *)refVal.u.pv;
            break;
        case 8:
            ((SMPV *)pv)[find.i] = *(SMPV *)refVal.u.pv;
            break;
        }
    }
    return ref;
}

// Read the save variable key as an int.
static inline int GetSaveVarInt(SVKEY *key, SAVEDATA *saveSet)
{
    CRef ref = GetGlobalSaveVar(key, saveSet);
    return ref.u.n;
}


// The game completion percentage: a weighted sum of nine progress variables plus a bonus per vault flag; 0, 100 or
// clamped to 1..99.
extern "C" int GetCompletionPercent(SAVEVARS *schema, SAVEDATA *saveSet)
{
    int sum = 0;
    int total = 0;
    int i;
    int symid;
    for (i = 0; i < 9; i++)
    {
        SVKEY *key = (SVKEY *)g_sw.apsvpEpisodePercent[i];
        int w = D_002C2DC0[i];
        total += w * 100;
        if (key)
            sum += GetSaveVarInt(key, saveSet) * w;
    }
    symid = FindSaveSymbol(&schema->symtab, D_002A78F0);
    total += 0xD0;
    for (int ient = 0; ient < schema->cent; ient++)
    {
        SVENT *pent = &schema->aent[ient];
        if (pent->kind == 4 && pent->symid == symid)
        {
            unsigned char *pv = (unsigned char *)saveSet + pent->ib;
            if (pent->fIndexed)
            {
                int c = pent->fIndexed;
                int j;
                for (j = 0; j < c; j++)
                {
                    unsigned char b = 1 << (j % 8);
                    if (pv[j / 8] & b)
                        sum += 0x1A;
                }
            }
            else
            {
                if (*pv)
                    sum += 0x1A;
            }
        }
    }
    if (sum == 0)
        return 0;
    if (sum == total)
        return 100;
    {
        return ClampInt(sum * 100 / total, 1, 99);
    }
}

// Builds the memory card mask: copies the save blob into a buffer, then flags the bytes of every entry tagged
// "from_memcard". (inferred)
// Also known as: CopyGameSaveForSlot (upstream readable / Sly 1 name; CopyGameSaveForSlot__FP6LAYOUTP8GAMESAVE).
extern "C" void BuildMemcardGsMask(SAVEVARS *schema, SAVEDATA *saveSet)
{
    int symid;
    int i;
    *saveSet = g_gs;
    symid = FindSchemaSymbol(schema, D_002A78F8);
    if (symid == -1)
        return;
    for (i = 0; i < schema->cent; i++)
    {
        SVENT *pent = &schema->aent[i];
        if (pent->symid == symid)
            ((char *)saveSet)[pent->ib] = 1;
    }
}

// Allocates the save symbol table's entries.
extern "C" void InitSaveSymbolTable(SVNHASH *symbolTable, int csym)
{
    MEMREQ memreq;
    symbolTable->csym = csym;
    memreq.cb = csym * (int)sizeof(SVN);
    memreq.cbAlign = 0x10;
    symbolTable->asym = (SVN *)AllocFromWorldHeap(&memreq);
}

// Upper-case form of ch.
static inline int ChUpper(int ch) { return (D_002AA6A1[ch] & 2) ? ch - 0x20 : ch; }

// Save-name hash, save's copy of HashStringCaseInsensitive (core/containers) without the null check: h*33+toupper(c).
extern "C" unsigned int HashSaveSymbolName(const char *pchz)
{
    const unsigned char *pch = (const unsigned char *)pchz;
    int hash = 0;
    while (*pch)
    {
        hash = hash * 33 + ChUpper(*pch);
        pch++;
    }
    return hash;
}

// Adds a (name, symbol id) pair to the 53-bucket save symbol hash table.
extern "C" void AddSaveSymbol(SVNHASH *symbolTable, char *pchz, int symid)
{
    unsigned int hash = HashSaveSymbolName(pchz);
    unsigned int ihash = hash % 0x35;
    SVN *psym = &symbolTable->asym[symbolTable->csym - 1];
    symbolTable->csym--;
    psym->pchz = pchz;
    psym->symid = symid;
    psym->hash = hash;
    psym->psymNext = symbolTable->apsym[ihash];
    symbolTable->apsym[ihash] = psym;
}

// Returns the symbol id of a name in the save symbol table, or -1.
// Also known as: SymFromName (upstream readable / Sly 1 name; SymFromName__FP6LAYOUTPc).
extern "C" int FindSaveSymbol(SVNHASH *symbolTable, const char *pchz)
{
    unsigned int hash = HashSaveSymbolName(pchz);
    SVN *psym;
    for (psym = symbolTable->apsym[hash % 0x35]; psym; psym = psym->psymNext)
    {
        if (psym->hash == hash && strcasecmp(psym->pchz, pchz) == 0)
            return psym->symid;
    }
    return -1;
}

// Turns a null-terminated list of strings into a key: symbol names become their ids, numbers become array indices.
extern "C" void BuildSaveKeyV(const char *pchz, VA_LIST ap, SVKEY *key)
{
    VA_LIST apSave = ap;
    const char *pchzT;
    int i;
    key->c = 0;
    key->ash = 0;
    for (pchzT = pchz; pchzT; pchzT = VA_ARG_PCH(ap))
        key->c++;
    if (key->c)
    {
        MEMREQ memreq;
        memreq.cb = key->c * 2;
        memreq.cbAlign = 0x10;
        key->ash = (short *)AllocFromStack(&memreq);
        ap = apSave;
        for (pchzT = pchz, i = 0; pchzT; pchzT = VA_ARG_PCH(ap), i++)
        {
            int symid = FindSchemaSymbol(&g_sw.savevars, pchzT);
            if (symid == -1)
                key->ash[i] = atoi(pchzT);
            else
                key->ash[i] = symid;
        }
    }
}

// Builds a key from a varargs list of strings on the heap.
extern "C" SVKEY *NewSaveKey(const char *pchz, ...)
{
    STACKSCOPE stackscope;
    MEMREQ memreq;
    MEMREQ *pmemreq = &memreq;
    VA_LIST ap;
    SVKEY *key;
    VA_START(ap, pchz);
    pmemreq->cb = 8;
    pmemreq->cbAlign = 0x10;
    key = (SVKEY *)AllocFromWorldHeap(pmemreq);
    BuildSaveKeyV(pchz, ap, key);
    {
        short *ashOld = key->ash;
        pmemreq->cb = key->c * 2;
        pmemreq->cbAlign = 0x10;
        key->ash = (short *)AllocCopyFromWorldHeap(pmemreq, ashOld);
    }
    return key;
}

// Builds a key from a varargs list of strings in a slot-heap allocation.
extern "C" SVKEY *AllocSaveKey(const char *pchz, ...)
{
    MEMREQ memreq;
    VA_LIST ap;
    SVKEY *key;
    VA_START(ap, pchz);
    memreq.cb = 8;
    memreq.cbAlign = 0x10;
    key = (SVKEY *)AllocFromStack(&memreq);
    BuildSaveKeyV(pchz, ap, key);
    return key;
}

/* Lookups in the global schema; spelled as inline wrappers (the temporaries of a direct call differ). */
static inline CRef GetGlobalStoreSaveVar(SVKEY *key, SAVEDATA *saveSet) { return GetSaveVar(&g_sw.savevars, key, saveSet); }
// Write the save variable key into the world's global save store.
static inline CRef SetGlobalStoreSaveVar(SVKEY *key, const CRef &ref, SAVEDATA *saveSet)
{
    return SetSaveVar(&g_sw.savevars, key, ref, saveSet);
}

// Reads the named save variable (varargs path) from the global save blob as a Splice value.
extern "C" CRef GetSaveVarByName(const char *pchz, ...)
{
    CRef ref;
    STACKSCOPE stackscope;
    SVKEY key;
    VA_LIST ap;
    VA_START(ap, pchz);
    BuildSaveKeyV(pchz, ap, &key);
    ref = GetGlobalStoreSaveVar(&key, &g_gs);
    return ref;
}

// Writes a Splice value into the named save variable (varargs path) of the global save blob.
// Also known as: SetProgressPoCurrent (upstream readable / Sly 1 name; SetProgressPoCurrent__FP4CRefPci).
extern "C" CRef SetSaveVarByName(const CRef &refVal, const char *pchz, ...)
{
    CRef ref;
    STACKSCOPE stackscope;
    SVKEY key;
    VA_LIST ap;
    VA_START(ap, pchz);
    BuildSaveKeyV(pchz, ap, &key);
    ref = SetGlobalStoreSaveVar(&key, refVal, &g_gs);
    return ref;
}

// Returns the address of the named save variable (varargs path) in the global save blob.
// Also known as: PnProgressSlot (upstream readable / Sly 1 name; PnProgressSlot__FPvi).
extern "C" void *GetSaveVarAddressV(const char *pchz, ...)
{
    CRef ref;
    STACKSCOPE stackscope;
    struct { SVKEY key; int pad[2]; } key;
    VA_LIST ap;
    VA_START(ap, pchz);
    BuildSaveKeyV(pchz, ap, &key.key);
    return GetSaveVarAddress(&g_sw.savevars, &key.key, &g_gs);
}

// Returns a new key: the concatenation of two keys.
extern "C" SVKEY *ConcatSaveKeys(SVKEY *pkey1, SVKEY *pkey2)
{
    MEMREQ memreq;
    SVKEY *key;
    int c;
    memreq.cb = 8;
    memreq.cbAlign = 0x10;
    key = (SVKEY *)AllocFromStack(&memreq);
    c = pkey1->c + pkey2->c;
    key->c = c;
    memreq.cb = c * 2;
    memreq.cbAlign = 0x10;
    key->ash = (short *)AllocFromStack(&memreq);
    CopyAb(key->ash, pkey1->ash, pkey1->c * 2);
    CopyAb(key->ash + pkey1->c, pkey2->ash, pkey2->c * 2);
    return key;
}

// Write *pref into the global save variable key.
static inline CRef SetGlobalSaveVarRef(SVKEY *key, CRef *pref, SAVEDATA *saveSet) { return SetGlobalSaveVar(key, *pref, saveSet); }

// Clears the save blob and the rumble variable; when keepSettings is set and the blob matches the schema, the eleven
// settings variables survive the clear.
/* Reset the save blob, keeping the settings variables when the blob matches the schema. */
// Also known as: ResetSlotKeepSettings (upstream readable / Sly 1 name; ResetSlotKeepSettings__FPci).
extern "C" void ResetSaveGs(SAVEDATA *saveSet, int fKeepSettings)
{
    STACKSCOPE stackscope;
    SVKEY *apkey[11] = { (SVKEY *)g_sw.psvpLang, (SVKEY *)g_sw.psvpNoVibration, (SVKEY *)g_sw.psvpAutosaveWarned,
                         (SVKEY *)g_sw.psvpReverseLookAround, (SVKEY *)g_sw.psvpReverseAim, (SVKEY *)g_sw.psvpMixerMode,
                         (SVKEY *)g_sw.psvpMusicVolume, (SVKEY *)g_sw.psvpEffectsVolume, (SVKEY *)g_sw.psvpUseHeadset,
                         (SVKEY *)g_sw.psvpScreenCenterX, (SVKEY *)g_sw.psvpScreenCenterY };
    CRef aref[11];
    unsigned int i;
    if (fKeepSettings && !DoesGsMatchSchema(&g_sw.savevars, saveSet))
        fKeepSettings = 0;
    if (fKeepSettings)
    {
        for (i = 0; i < 11; i++)
            aref[i] = GetGlobalStoreSaveVar(apkey[i], saveSet);
    }
    memset(saveSet, 0, 0x5400);
    ResetRumbleSaveVar();
    if (fKeepSettings)
    {
        for (i = 0; i < 11; i++)
        {
            CRef ref(aref[i]);
            const CRef &refT = SetGlobalSaveVarRef(apkey[i], &ref, saveSet);
        }
    }
}

// Read the global save variable key as an int.
static inline int GetGlobalSaveVarInt(SVKEY *key, SAVEDATA *saveSet) { return GetGlobalSaveVar(key, saveSet).u.n; }
// Read the global save variable key as a float.
static inline float GetGlobalSaveVarFloat(SVKEY *key, SAVEDATA *saveSet) { return GetGlobalSaveVar(key, saveSet).u.g; }

// Applies the settings variables: vibration, sound mode, music and effects volumes, a flag, and the screen offset.
// Also known as: ApplyGameSettings (upstream readable / Sly 1 name; ApplyGameSettings__FP8GAMESAVE).
extern "C" void ApplyGsSettings(SAVEDATA *saveSet)
{
    if ((SVKEY *)g_sw.psvpLang && CountRegionLanguages() >= 2)
        SetLanguage(GetGlobalSaveVarInt((SVKEY *)g_sw.psvpLang, saveSet));
    if ((SVKEY *)g_sw.psvpMixerMode)
        SetSndmgrPlaybackMode(&g_sndmgr, GetGlobalSaveVarInt((SVKEY *)g_sw.psvpMixerMode, saveSet));
    if ((SVKEY *)g_sw.psvpMusicVolume)
        SetSndmgrChannelVolume(&g_sndmgr, 1, 1, 1.0f - GetGlobalSaveVarFloat((SVKEY *)g_sw.psvpMusicVolume, saveSet));
    if ((SVKEY *)g_sw.psvpEffectsVolume)
    {
        SetSndmgrChannelVolume(&g_sndmgr, 0, 1, 1.0f - GetGlobalSaveVarFloat((SVKEY *)g_sw.psvpEffectsVolume, saveSet));
        SetSndmgrChannelVolume(&g_sndmgr, 3, 1, 1.0f - GetGlobalSaveVarFloat((SVKEY *)g_sw.psvpEffectsVolume, saveSet));
    }
    if ((SVKEY *)g_sw.psvpUseHeadset)
        g_sndmgr.isHeadsetEnabled = GetGlobalSaveVarInt((SVKEY *)g_sw.psvpUseHeadset, saveSet);
    if ((SVKEY *)g_sw.psvpScreenCenterX)
    {
        float g = GetGlobalSaveVarFloat((SVKEY *)g_sw.psvpScreenCenterX, saveSet);
        GSREGS *p = &D_002DEB80;
        p->d1.dx = p->d2.dx = fptoui(g + 636.0f);
    }
    if ((SVKEY *)g_sw.psvpScreenCenterY)
    {
        float g = GetGlobalSaveVarFloat((SVKEY *)g_sw.psvpScreenCenterY, saveSet);
        GSREGS *p = &D_002DEB80;
        p->d1.dy = p->d2.dy = fptoui(g + 50.0f) & ~1;
    }
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const char r_D_002A78F0[] __asm__("D_002A78F0") = "fVault"; }
namespace RO_ORDER { extern const char r_D_002A78F8[] __asm__("D_002A78F8") = "from_memcard"; }
// ---- save's .bss (tu-merging.md "Data": .bss gaps): g_gs, the save blob with its schema version and checksum
// (0x3D4A60..0x3D9E68, then screen's .bss at 0x3D9E70). 16-aligned on the definition, as the layout shows (0x3D4A60
// after safehouse's word at 0x3D4A54; SAVEDATA itself is 8-aligned, and its 0x5408 bytes are what the copies in this
// file move). Defined after the functions, so none sees a definition.
SAVEDATA g_gs __attribute__((aligned(16)));
