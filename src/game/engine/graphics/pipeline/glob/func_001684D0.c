typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
typedef struct M4 { qword r[4]; } M4;
typedef struct TRANSFORM { char pad0[0x40]; M4 world; } TRANSFORM;
typedef struct BONE {
    int value, unknown4, index;
    TRANSFORM *transform;
    M4 local;
} BONE;
typedef struct DESC {
    unsigned char flags;
    signed char count;
    unsigned short channel;
    float distance;
    int unknown8;
    uint base;
} DESC;
typedef struct ENTRY { char pad0[0x18]; BONE *bones; } ENTRY;
typedef struct DMA { uint fields[3]; uint *cur; } DMA;
extern DMA D_002DEA28;

static __inline__ qword ReadCPU(const qword *source)
{
    qword value;
    __asm__("lq %0,%1" : "=r"(value) : "m"(*source));
    return value;
}

static __inline__ void Align(uint align)
{
    DMA *dma = &D_002DEA28;
    qword *output = (qword *)dma->cur;
    while (((uint)output & 15) != align) {
        *(uint *)output = 0;
        dma->cur++;
        output = (qword *)dma->cur;
    }
}

/** @sly2-annot addr=0x1684D0
 * Purpose: Appends VIF UNPACKs of a sub-glob's bone matrices to the instance packet
 * Info: proposed name UnpackSubglobBones (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void func_001684D0(void *context, DESC *descriptor, ENTRY *entry)
{
    uint opcode;
    BONE *bone, *last;
    TRANSFORM *parent;
    qword *output;
    qword p0, p1, p2, p3, n0, n1, n2, n3;
    register qword v1 __asm__("vf1"), v2 __asm__("vf2");
    register qword v3 __asm__("vf3"), v4 __asm__("vf4");
    register qword v5 __asm__("vf5"), v6 __asm__("vf6");
    register qword v7 __asm__("vf7"), v8 __asm__("vf8");

    Align(12);
    opcode = (uint)descriptor->count << 18;
    opcode |= 0x6000C000;
    opcode |= 0x0C000000;
    opcode |= descriptor->base;
    *D_002DEA28.cur = opcode;
    D_002DEA28.cur++;
    output = (qword *)D_002DEA28.cur;
    bone = entry->bones;
    last = bone + descriptor->count - 1;
    parent = bone->transform;
    v5 = bone->local.r[0]; v6 = bone->local.r[1];
    v7 = bone->local.r[2]; v8 = bone->local.r[3];
    v1 = parent->world.r[0]; v2 = parent->world.r[1];
    v3 = parent->world.r[2]; v4 = parent->world.r[3];
    for (;;) {
        __asm__ __volatile__("vcallms 0x60" : :
                "j"(v1), "j"(v2), "j"(v3), "j"(v4),
                "j"(v5), "j"(v6), "j"(v7), "j"(v8));
        if (bone < last) {
            TRANSFORM *next = bone[1].transform;
            p0 = ReadCPU(&next->world.r[0]);
            p1 = ReadCPU(&next->world.r[1]);
            p2 = ReadCPU(&next->world.r[2]);
            p3 = ReadCPU(&next->world.r[3]);
            n0 = ReadCPU(&bone[1].local.r[0]);
            n1 = ReadCPU(&bone[1].local.r[1]);
            n2 = ReadCPU(&bone[1].local.r[2]);
            n3 = ReadCPU(&bone[1].local.r[3]);
        }
        {
            int sync;
            register qword result0 __asm__("vf5");
            register qword result1 __asm__("vf6");
            register qword result2 __asm__("vf7");
            register qword result3 __asm__("vf8");
            /* Publish the completed microprogram outputs after the interlock. */
            __asm__ __volatile__("cfc2.i %0,$vi0"
                    : "=r"(sync), "=j"(result0), "=j"(result1),
                      "=j"(result2), "=j"(result3));
            *output++ = result0;
            *output++ = result1;
            *output++ = result2;
            *output++ = result3;
        }
        if (bone == last)
            break;
        v1 = p0; v2 = p1; v3 = p2; v4 = p3;
        v5 = n0; v6 = n1; v7 = n2; v8 = n3;
        bone++;
    }
    D_002DEA28.cur = (uint *)output;
}

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_M4[(sizeof(M4)==0x40)?1:-1];
typedef char layout_BONE[(sizeof(BONE)==0x50)?1:-1];
typedef char layout_DESC[(sizeof(DESC)==0x10)?1:-1];
typedef char layout_TRANSFORM_world[((unsigned long)&((TRANSFORM*)0)->world==0x40)?1:-1];
typedef char layout_BONE_transform[((unsigned long)&((BONE*)0)->transform==0xc)?1:-1];
typedef char layout_BONE_local[((unsigned long)&((BONE*)0)->local==0x10)?1:-1];
typedef char layout_DESC_count[((unsigned long)&((DESC*)0)->count==0x1)?1:-1];
typedef char layout_DESC_base[((unsigned long)&((DESC*)0)->base==0xc)?1:-1];
typedef char layout_ENTRY_bones[((unsigned long)&((ENTRY*)0)->bones==0x18)?1:-1];
typedef char layout_DMA_cur[((unsigned long)&((DMA*)0)->cur==0xc)?1:-1];
typedef char layout_M4_row0[((unsigned long)&((M4*)0)->r[0]==0x0)?1:-1];
typedef char layout_M4_row1[((unsigned long)&((M4*)0)->r[1]==0x10)?1:-1];
typedef char layout_M4_row2[((unsigned long)&((M4*)0)->r[2]==0x20)?1:-1];
typedef char layout_M4_row3[((unsigned long)&((M4*)0)->r[3]==0x30)?1:-1];
