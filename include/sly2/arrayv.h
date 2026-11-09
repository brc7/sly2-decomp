/* include/sly2/arrayv.h -- the loadable dynamic array with listeners, its hash index and the string-keyed hash
 * table: the class templates whose instances are the linkonce copies 0x2564E0..0x256AF0 (link/linkonce-rest).
 *
 * The vtables (in dynarraystr's place in the ELF's linkonce data area, emitted in the reverse order of definition):
 *   0x2CA660  CDynArrayV<char *>        Init 0x2564E0, Clear 0x256658, Load 0x2566D8, ReadElement 0x2566B0,
 *                                       Add 0x2565E0, NotifyAdd 0x256588
 *   0x2CA640  CArrayListener<char *>    two pure slots (__pure_virtual 0x23C820)
 *   0x2CA620  CHashIndex<char *>        Clear 0x2568A0, OnAdd 0x256AF0
 *   0x2CA5E0  dynarraystr's array class   CDynArrayV<char *> with Init (0x119BD0: the base Init, then its hash index
 *                                       at +0x34 keyed by 0x119BC8) and ReadElement (0x119C08: a string) overridden
 * The non-virtual members: CHashIndex::Init 0x256528 (called by 0x119BD0), CHash<char *, int>::Clear 0x2567D0,
 * Resize 0x2568C0 and Insert 0x2569F8. ReadElement reads sizeof(T) bytes and Load scratch-allocates and clears
 * sizeof(T): the element type is a 4-byte one; char * (the derived class's elements are strings, the index's key
 * function returns the element) is a HYPOTHESIS, and so are all names here (no symbol names these classes).
 *
 * The members are defined after the classes without `inline` and emitted by explicit instantiation in
 * src/game/core/containers/dynarraystr.cpp: an explicitly instantiated template member comes out weak in
 * `.gnu.linkonce.t.<symbol>`, as the original's copies did (see docs/guides/tu-merging.md "Linkonce copies by
 * family"). The bodies keep the per-function views they matched with (`this` cast to the view). The vtables are not
 * emitted here (no constructor is instantiated; the data/vtables work owns them). */
#ifndef SLY2_ARRAYV_H
#define SLY2_ARRAYV_H

struct CBinaryInputStream;

extern "C" {
void ArrayV_ListAppend(void *pdl, void *pv) __asm__("AppendSlEntry");          /* append pv to the list at pdl */
void ArrayV_DynArrayAppend(void *pary, int cb, int c) __asm__("CDynArray_Append");
unsigned int ArrayV_HashPchz(int pchz) __asm__("HashStringCaseInsensitive");
void *ArrayV_Memset(void *pv, int b, int cb) __asm__("memset");
void *ArrayV_StackScopeCtor(void *pscope) __asm__("STACKSCOPE_ctor");
void ArrayV_StackScopeDtor(void *pscope, int n) __asm__("_$_10STACKSCOPE");
void *ArrayV_ScratchAlloc(void *pdesc) __asm__("AllocFromStack");
int ArrayV_S32Read(void *pbis) __asm__("S32Read__18CBinaryInputStream");
void ArrayV_Read(CBinaryInputStream *pbis, int cb, void *pv) __asm__("Read__18CBinaryInputStreamiPv");
void ArrayV_Reserve(void *pary, int c) __asm__("Reserve__9CDynArrayi");
void ArrayV_Free(void *pary) __asm__("Free__9CDynArray");
}

namespace ArrayV {
/* g++ 2.95 vtable entry: this-adjust delta and the function. */
struct VTE { short d; short pad; void *pfn; };
/* the per-function views the bodies matched with */
struct OWNER1 { int x0; int x4; };
struct ATTACH1 { OWNER1 *p0; char pad[0x20]; int x24; };
struct OWNER2 { int *x0; char pad[0x24]; int x28; };
struct ATTACH2 { int *x0; int x4, x8; OWNER2 *xC; int x10; char pad[0xC]; int x20; };
struct LISTENER { int x0; LISTENER *next; VTE *vt; };
struct ARYV { char pad[0xC]; VTE *vt; int *data; int c; char pad18[0x10]; LISTENER *first; };
struct VTINIT { char pad[0x30]; short d; short p; void (*f)(void *, void *); };   /* entry 6 (NotifyAdd) */
struct ARR { int *data; int c; };
struct ARYV2 { char pad[0xC]; VTINIT *vt; ARR arr; };
// Load-allocation request: byte count and alignment.
struct LOADDESC { int cb; int align; LOADDESC(int c, int a) { cb = c; align = a; } };
struct SCOPEMARK { int a[4]; };
struct ARYV3 { char pad[0xC]; VTE *vt; char x10[4]; };
struct HALLOC { VTE *vt; };
struct HENT { unsigned int key; int k; int v; HENT *next; };
struct HASH { int c; HENT **ap; int count; HALLOC *pa; };
struct IDXBASE { char pad[0x10]; int *base; };
struct HASHINDEX { char pad[0xC]; IDXBASE *pb; int (*pfn)(int *); char h[0x10]; };
}
#define ARRAYV_HFREE(pa, p) ((void (*)(void *, void *))(pa)->vt[3].pfn)((char *)(pa) + (pa)->vt[3].d, p)

/* A chained hash table: bucket count, buckets, entry count, the allocator (its vtable: 1 alloc, 2 realloc, 3 free). */
template <class K, class V> class CHash
{
public:
    int m_c;                  /* +0x00 bucket count (0 until the first Insert) */
    void **m_ap;              /* +0x04 buckets */
    int m_count;              /* +0x08 number of entries */
    void *m_palloc;           /* +0x0C allocator of the entries */

    void Clear();
    void Resize(int c);       /* rehash into c buckets (only the <char *, int> specialization is defined) */
    V *Insert(K *pk, V *pv);
};

/* An object attached to an owner's list (owner +0, link +4); the array's first base. */
class CDynArrayVBase
{
public:
    void *m_powner;           /* +0x00 */
    CDynArrayVBase *m_pnext;  /* +0x04 */
    int m_unk8;               /* +0x08 unknown */
                              /* +0x0C vptr */
    virtual void Init(void *powner) = 0;
    virtual void Clear() = 0;
    virtual void Load(CBinaryInputStream *pbis) = 0;
};

/* A listener on a CDynArrayV: told about every element added (OnAdd) and when the array is cleared. */
template <class T> class CArrayListener
{
public:
    void *m_powner;           /* +0x00 the array's owner */
    CArrayListener *m_pnext;  /* +0x04 the array's listener list */
                              /* +0x08 vptr */
    virtual void Clear() = 0;
    virtual void OnAdd(T *pt) = 0;
};

/* A dynamic array of T (CDynArray at +0x10, heap at +0x24) that tells its listeners (+0x28) about every element. */
template <class T> class CDynArrayV : public CDynArrayVBase
{
public:
    char m_ary[0x18];                 /* +0x10 CDynArray */
    CArrayListener<T> *m_plistener;   /* +0x28 first listener */

    virtual void Init(void *powner);
    virtual void Clear();
    virtual void Load(CBinaryInputStream *pbis);
    virtual void ReadElement(T *pt, CBinaryInputStream *pbis);
    virtual T *Add(T *pt);
    virtual void NotifyAdd(T *pt);
};

/* A listener that indexes an array's elements by key (the key function's result) in a hash of element indices. */
template <class T> class CHashIndex : public CArrayListener<T>
{
public:
    CDynArrayV<T> *m_pary;            /* +0x0C the indexed array */
    int (*m_pfnKey)(T *);             /* +0x10 key function (element -> key) */
    CHash<T, int> m_hash;             /* +0x14 key -> element index */

    void Init(CDynArrayV<T> *pary, void *pfnKey);
    virtual void Clear();
    virtual void OnAdd(T *pt);
};

/* 0x2564E0: attach to an owner (store it, append to its list at +4, take its heap). */
template <class T> void CDynArrayV<T>::Init(void *pvOwner)
{
    ArrayV::ATTACH1 *a = (ArrayV::ATTACH1 *)this;
    ArrayV::OWNER1 *b = (ArrayV::OWNER1 *)pvOwner;
    a->p0 = b;
    ArrayV_ListAppend(&b->x4, a);
    a->x24 = b->x0;
}

/* 0x256528: the same for the index, onto the array's listener list (+0x28), with the key function. */
template <class T> void CHashIndex<T>::Init(CDynArrayV<T> *pary, void *pfnKey)
{
    ArrayV::ATTACH2 *a = (ArrayV::ATTACH2 *)this;
    ArrayV::OWNER2 *b = (ArrayV::OWNER2 *)pary;
    a->x0 = b->x0;
    a->xC = b;
    a->x10 = (int)pfnKey;
    ArrayV_ListAppend(&b->x28, a);
    a->x20 = *a->x0;
}

/* 0x256588: every listener's OnAdd (slot 2). */
template <class T> void CDynArrayV<T>::NotifyAdd(T *pt)
{
    ArrayV::ARYV *p = (ArrayV::ARYV *)this;
    ArrayV::LISTENER *l;
    for (l = p->first; l; l = l->next)
        ((void (*)(void *, void *))l->vt[2].pfn)((char *)l + l->vt[2].d, pt);
}

/* 0x2565E0: append one element, then NotifyAdd (slot 6) with the new element. */
template <class T> T *CDynArrayV<T>::Add(T *pt)
{
    ArrayV::ARYV2 *p = (ArrayV::ARYV2 *)this;
    ArrayV::ARR *a = &p->arr;
    ArrayV_DynArrayAppend(a, (int)pt, 1);
    p->vt->f((char *)p + p->vt->d, &p->arr.data[a->c - 1]);
    return (T *)&p->arr.data[a->c - 1];
}

/* 0x256658: free the elements, then every listener's Clear (slot 1). */
template <class T> void CDynArrayV<T>::Clear()
{
    ArrayV::ARYV *p = (ArrayV::ARYV *)this;
    ArrayV::LISTENER *l;
    ArrayV_Free(&p->data);
    for (l = p->first; l; l = l->next)
        ((void (*)(void *))l->vt[1].pfn)((char *)l + l->vt[1].d);
}

/* 0x2566B0: one element's bytes. */
template <class T> void CDynArrayV<T>::ReadElement(T *pt, CBinaryInputStream *pbis)
{
    ArrayV_Read(pbis, 4, pt);
}

/* 0x2566D8: Clear (slot 2), then a count and per element: clear a scratch element, ReadElement (slot 4), Add (5). */
template <class T> void CDynArrayV<T>::Load(CBinaryInputStream *pbis)
{
    ArrayV::ARYV3 *p = (ArrayV::ARYV3 *)this;
    void *pv = pbis;
    ((void (*)(void *))p->vt[2].pfn)((char *)p + p->vt[2].d);
    ArrayV::SCOPEMARK m;
    ArrayV_StackScopeCtor(&m);
    ArrayV::LOADDESC d(4, 0x10);
    void *buf = ArrayV_ScratchAlloc(&d);
    int n = ArrayV_S32Read(pv);
    ArrayV_Reserve(p->x10, n);
    if (n > 0) do {
        ArrayV_Memset(buf, 0, 4);
        ((void (*)(void *, void *, void *))p->vt[4].pfn)((char *)p + p->vt[4].d, buf, pv);
        ((void (*)(void *, void *))p->vt[5].pfn)((char *)p + p->vt[5].d, buf);
    } while (--n);
    ArrayV_StackScopeDtor(&m, 2);
}

/* 0x2567D0: free every entry and the buckets through the allocator (slot 3). */
template <class K, class V> void CHash<K, V>::Clear()
{
    ArrayV::HASH *h = (ArrayV::HASH *)this;
    int i;
    for (i = 0; i < h->c; i++) {
        ArrayV::HENT *e = h->ap[i];
        while (e) {
            ArrayV::HENT *next = e->next;
            ARRAYV_HFREE(h->pa, e);
            e = next;
        }
    }
    ArrayV_Memset(h->ap, 0, h->c * 4);
    h->count = 0;
    ARRAYV_HFREE(h->pa, h->ap);
    h->c = 0;
    h->ap = 0;
}

/* 0x2568A0: the index's hash. */
template <class T> void CHashIndex<T>::Clear()
{
    ((CHash<T, int> *)((char *)this + 0x14))->Clear();
}

/* 0x2568C0 CHash<K, V>::Resize: defined in dynarraystr.cpp as the explicit specialization for <char *, int> (the
   instantiated template member compiles 2 instructions off; see there). */

/* 0x2569F8: insert (key, value) under the key's caseless string hash (53 buckets on the first insert). */
template <class K, class V> V *CHash<K, V>::Insert(K *pk, V *pvValue)
{
    ArrayV::HASH *h = (ArrayV::HASH *)this;
    unsigned int hash, i;
    ArrayV::HENT *e;
    h->count++;
    if ((unsigned int)h->c == 0)
        Resize(0x35);
    hash = ArrayV_HashPchz(*(int *)pk);
    i = hash % (unsigned int)h->c;
    e = ((ArrayV::HENT *(*)(void *, int, int))h->pa->vt[1].pfn)((char *)h->pa + h->pa->vt[1].d, 0x10, 3);
    ArrayV_Memset(e, 0, 0x10);
    e->key = hash;
    e->k = *(int *)pk;
    e->v = *(int *)pvValue;
    e->next = h->ap[i];
    h->ap[i] = e;
    return (V *)&e->v;
}

/* 0x256AF0: insert (key of the element, its index) into the hash. */
template <class T> void CHashIndex<T>::OnAdd(T *pt)
{
    ArrayV::HASHINDEX *p = (ArrayV::HASHINDEX *)this;
    int *px = (int *)pt;
    int a[2];
    CHash<T, int> *h = (CHash<T, int> *)p->h;
    a[1] = px - p->pb->base;
    a[0] = p->pfn(px);
    h->Insert((T *)&a[0], &a[1]);
}

#endif
