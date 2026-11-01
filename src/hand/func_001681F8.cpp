// func_001681F8 (Claude agent, T1651): builds one VIF packet per entry of a model's packet list: a DMA CNT tag, an UNPACK of
// 6 qwords to VU address 0x301 (entry word + bounding sphere + model matrix), padding, and a DMA RET tag, then queues it
// in a render-list entry (func_001CD998) with the descriptor's chain. Stamps the game tick at the end.
// From lanef9v1 (aligned 5) plus func_00169198's packet idioms (template UNPACK tag, CopyQws with the empty do-while).
// STAND-IN (same trick as func_00219400 / func_0013E818): `if (fNever)` is dead code (gcse's last cprop folds it, jump
// deletes it; the final code is unchanged). It only supplies the extra basic block the game had between the UNPACK tag and
// the copy loop at gcse time: gcse PRE then inserts the %hi(D_002DEA28) copy (t2) before that block and the outer `i + 1`
// (s4) after it, below the do-while barrier. What the game really had there is unknown.
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;
union U
{
    qword q;
    float f[4];
};
struct M3
{
    U r[3];
};
struct M4
{
    U r[4];
};
struct V3
{
    float f[3];
};
struct SPHERE
{
    V3 center;
    float radius;
};
struct DESC
{
    char pad[8];
    void *chain;
    char padC[8];
};
struct XF
{
    char pad[0x40];
    M4 matrix;
};
struct ENTRY
{
    int unknown;
    XF *transform;
    M4 *parent;
    uint flags;
    char *matrixData;
    int word;
    char pad18[0xC];
    SPHERE *sphere;
};
struct LOCAL_SPHERE
{
    SPHERE sphere;
    char pad[8];
};
struct PACKETS
{
    char pad[0x38];
    int count;
    DESC *descriptors;
    ENTRY *entries;
    LOCAL_SPHERE *spheres;
    int pad48;
    int tick;
};
struct PACKET
{
    int word;
    int reserved[3];
    U sphere;
    M4 matrix;
};
struct DMA
{
    char pad[0xC];
    uchar *pb;
};
struct RPL
{
    int kind, pad4;
    uchar *pb;
    void *chain;
};
struct G
{
    char pad[0x3D5C];
    int tick;
};
extern DMA D_002DEA28;
extern G D_003DD490;
extern "C" {
void func_001AE5A0(void *pvDst, const void *pvSrc, uint cb);
void *func_0023320C(void *pvDst, const void *pvSrc, uint cb);
M4 *func_0011C168(M4 *pmatDst, const M4 *pmatA, const M4 *pmatB);
float func_0011CAB0(const M3 *pmat);
RPL *func_001CD998(void);
}
static inline qword QwTransformPos(M4 *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(pmat->r[0].q), "j"(pmat->r[1].q), "j"(pmat->r[2].q), "j"(pmat->r[3].q), "j"(qwPos));
    return qwResult;
}
static inline void CopyQws(uint *pwDst, const uint *pwSrc, int cw, int fNever)
{
    if (fNever) // STAND-IN for the game's dead code (see the top of the file)
        *pwDst = 0;
    // empty do-while(0) (as left by a statement macro): its loop notes keep later setup below the tag/pb stores
    do
    {
    } while (0);
    for (int k = 0; k < cw; k += 4)
    {
        uint w0 = pwSrc[0];
        uint w1 = pwSrc[1];
        uint w2 = pwSrc[2];
        uint w3 = pwSrc[3];
        pwSrc += 4;
        pwDst[0] = w0;
        pwDst[1] = w1;
        pwDst[2] = w2;
        pwDst[3] = w3;
        pwDst += 4;
    }
}
template <uint TAG> static inline void UnpackQws(const void *pv, int cw, int fNever)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = TAG;
    pdma->pb += 4;
    CopyQws((uint *)pdma->pb, (const uint *)pv, cw, fNever);
    pdma->pb += cw * 4;
}
static inline M4 ModelMatrix(const ENTRY *entry) return model
{
    const M4 *parent = entry->parent;
    func_0023320C(&model, &entry->transform->matrix, sizeof(M4));
    if (parent)
    {
        M4 product;
        func_0011C168(&product, &model, parent);
        func_0023320C(&model, &product, sizeof(M4));
    }
}
/** @sly2-annot addr=0x1681F8
 * Purpose: Queues a kind-0 RPL per model packet: CNT + UNPACK 6 qw (EE dest word, sphere, matrix) to VU
 *   TOPS+0x301 + RET; stamps the model with g_sw+0x3D5C
 * Info: proposed name SubmitGlobsetRelight (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#2-render-list-entries-rpl,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
extern "C" void func_001681F8(PACKETS *p)
{
    int fNever = 0;
    int i;
    for (i = 0; i < p->count; i++)
    {
        DESC *descriptor = &p->descriptors[i];
        ENTRY *entry = &p->entries[i];
        PACKET packet;
        if (!descriptor->chain)
            continue;
        packet.word = entry->word;
        if (entry->flags & 0x1000)
            func_001AE5A0(&packet.matrix, entry->matrixData + 0x14, sizeof(M4));
        else
            packet.matrix = ModelMatrix(entry);
        if (entry->sphere)
        {
            *(V3 *)&packet.sphere = entry->sphere->center;
            packet.sphere.f[3] = entry->sphere->radius;
        }
        else
        {
            // a stride variable (not the constant in the index) is what loop.c hoists into s7
            int sphereStride = sizeof(LOCAL_SPHERE);
            LOCAL_SPHERE *sphere = (LOCAL_SPHERE *)((char *)p->spheres + i * sphereStride);
            U center;
            func_001AE5A0(&center, &sphere->sphere.center, sizeof(V3));
            packet.sphere.q = QwTransformPos(&packet.matrix, center.q);
            packet.sphere.f[3] = func_0011CAB0((const M3 *)&packet.matrix) * sphere->sphere.radius;
        }
        ulong *ptag = (ulong *)D_002DEA28.pb;
        *ptag = 0x10000000;
        D_002DEA28.pb += 0x10;
        UnpackQws<0x6C06C301>(&packet, sizeof(PACKET) / sizeof(uint), fNever);
        {
            DMA *pdma = &D_002DEA28;
            for (uchar *pbPad = pdma->pb; (uint)pbPad & 0xF; pbPad = pdma->pb)
            {
                *(uint *)pbPad = 0;
                pdma->pb += 4;
            }
        }
        *(uint *)ptag |= ((D_002DEA28.pb - (uchar *)ptag) >> 4) - 1;
        *(ulong *)D_002DEA28.pb = 0x60000000;
        D_002DEA28.pb += 0x10;
        RPL *prpl = func_001CD998();
        prpl->pb = (uchar *)ptag;
        prpl->kind = 0;
        prpl->pad4 = 0;
        prpl->chain = descriptor->chain;
    }
    p->tick = D_003DD490.tick;
}
