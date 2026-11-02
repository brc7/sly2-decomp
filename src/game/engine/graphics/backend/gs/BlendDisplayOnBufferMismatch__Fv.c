typedef struct { unsigned long EN1:1, EN2:1, CRTMD:3, MMOD:1, AMOD:1, SLBG:1, ALP:8, reserved:48; } __attribute__((packed)) PMODE_FIELDS;
typedef union { PMODE_FIELDS field; unsigned long word; } __attribute__((packed)) PMODE;
typedef struct { unsigned int FBP:9, FBW:6, PSM:5, reserved0:12, DBX:11, DBY:11, reserved1:10; } __attribute__((packed)) DISPFB;
typedef struct { unsigned int DX:12, DY:11, MAGH:4, MAGV:2, reserved0:3, DW:12, DH:11, reserved1:9; } __attribute__((packed)) DISPLAY;
typedef struct { PMODE pmode; unsigned long smode2; DISPFB dispfb; DISPLAY display; unsigned long bgcolor; } DISPENV;
extern DISPENV D_002DEB80[2];
extern int D_002DEDB0,D_002DEDB8;
static inline int FField(void)
{
    unsigned int bits;
    __asm__ __volatile__("lw %0,0x12001000":"=r"(bits):"X"(*(volatile unsigned int*)0x12001000));
    return (bits>>13)&1;
}
static inline void PublishDisplay(const DISPLAY*value)
{
    register unsigned long word __asm__("$8");
    __asm__("ldl %0,%1\n\tldr %0,%2":"=&r"(word):"m"(((const char*)value)[7]),"m"(((const char*)value)[0]),"X"(*value));
    __asm__ __volatile__("sdl %1,0x12000087\n\tsdr %1,0x12000080":"=X"(*(volatile DISPLAY*)0x12000080):"r"(word):"$1");
}
static inline void PublishBuffer(const DISPFB*value)
{
    register unsigned long word __asm__("$4");
    __asm__("ldl %0,%1\n\tldr %0,%2":"=&r"(word):"m"(((const char*)value)[7]),"m"(((const char*)value)[0]),"X"(*value));
    __asm__ __volatile__("sdl %1,0x12000077\n\tsdr %1,0x12000070":"=X"(*(volatile DISPFB*)0x12000070):"r"(word):"$1");
}
static inline void PublishMode(const PMODE*value)
{
    register unsigned long word __asm__("$9");
    __asm__("ldl %0,%1\n\tldr %0,%2":"=&r"(word):"m"(((const char*)value)[7]),"m"(((const char*)value)[0]),"X"(*value));
    __asm__ __volatile__("sdl %1,0x12000007\n\tsdr %1,0x12000000":"=X"(*(volatile PMODE*)0x12000000):"r"(word):"$1");
}
static inline void EnablePrimary(PMODE*mode)
{
    const unsigned long primaryMask=1;
    register unsigned long word __asm__("$2"),mask __asm__("$3");
    __asm__("li %1,%2\n\tld %0,%3":"=r"(word),"=r"(mask):"i"(primaryMask),"m"(*mode));
    mode->word=word|mask;
}
/** @sly2-annot addr=0x16F2D0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/formats/textures.md#6-runtime-notes
 * @end-sly2-annot */
void BlendDisplayOnBufferMismatch__Fv(void)
{
    int field=D_002DEDB8;
    int i=D_002DEDB0&1;
    PMODE pmode=D_002DEB80[i].pmode;
    if(field!=FField()){
        DISPLAY display=*(DISPLAY*)((char*)&D_002DEB80[0].display+i*sizeof(DISPENV));
        DISPFB dispfb=*(DISPFB*)((char*)&D_002DEB80[0].dispfb+i*sizeof(DISPENV));
        display.DH-=1;
        if(field)display.DY+=1;else dispfb.DBY+=1;
        PublishDisplay(&display);
        PublishBuffer(&dispfb);
        EnablePrimary(&pmode);
    }
    PublishMode(&pmode);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_ulong[(sizeof(unsigned long)==8)?1:-1];
typedef char layout_uint[(sizeof(unsigned int)==4)?1:-1];
typedef char layout_PMODE[(sizeof(PMODE)==8)?1:-1];
typedef char layout_DISPFB[(sizeof(DISPFB)==8)?1:-1];
typedef char layout_DISPLAY[(sizeof(DISPLAY)==8)?1:-1];
typedef char layout_DISPENV[(sizeof(DISPENV)==0x28)?1:-1];
typedef char layout_env2[(sizeof(D_002DEB80)==0x50)?1:-1];
typedef char layout_env_pmode[((unsigned)&((DISPENV*)0)->pmode==0x0)?1:-1];
typedef char layout_env_smode2[((unsigned)&((DISPENV*)0)->smode2==0x8)?1:-1];
typedef char layout_env_dispfb[((unsigned)&((DISPENV*)0)->dispfb==0x10)?1:-1];
typedef char layout_env_display[((unsigned)&((DISPENV*)0)->display==0x18)?1:-1];
typedef char layout_env_bgcolor[((unsigned)&((DISPENV*)0)->bgcolor==0x20)?1:-1];

typedef char layout_PMODE_FIELDS[(sizeof(PMODE_FIELDS)==8)?1:-1];
typedef char layout_PMODE_align[(__alignof__(PMODE)==1)?1:-1];
typedef char layout_PMODE_field[((unsigned)&((PMODE*)0)->field==0)?1:-1];
typedef char layout_PMODE_word[((unsigned)&((PMODE*)0)->word==0)?1:-1];
