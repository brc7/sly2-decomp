#include "gifs.h"
struct RGBA_RAW { uchar r, g, b, a; };
struct RGBA {
    union { RGBA_RAW bytes; uint word; };
    RGBA() {}
    RGBA(uint value) { word = value; }
    RGBA(const RGBA &c) { bytes = c.bytes; }
};
struct CFont { char opaque[0x140]; } __attribute__((aligned(16)));
struct CRichText {
    char *text, *cursor;
    CFont *current, *base;
    CFont font;
    int dirty;
    RGBA color, setColor, baseColor, fontColor;
    CRichText(char *str, CFont *pf);
};
struct TB {
    float x, y, dx, dy;
    RGBA color;
    int jh;
    int jv;
    void SetPos(float x, float y) __asm__("CTextBox_SetPos");
    void SetSize(float dx, float dy) __asm__("CTextBox_SetSize");
    void SetColor(RGBA *prgba) __asm__("CTextBox_SetTextColor");
    void SetHorizontalJust(int j) __asm__("CTextBox_SetHorizontalJust");
    void SetVerticalJust(int j) __asm__("CTextBox_SetVerticalJust");
    TB(float x, float y, float dx, float dy, uint word = 0xFFC0C0C0) {
        SetPos(x, y);
        SetSize(dx, dy);
        RGBA rgba(word);
        SetColor(&rgba);
        SetHorizontalJust(0);
        SetVerticalJust(0);
    }
};
struct S { int x0; int x4; };
struct TEXT_ENTRY { int id; char *text; };
struct TEXT_TABLE { int count; TEXT_ENTRY *entries; };
struct P { char pad[0x104]; S *x104; int pad108[2]; char *x110; int pad114; int x118; int x11C; };
extern "C" {
extern CFont *D_002DE760;
extern TEXT_TABLE D_003E1C20;
void func_0015F078(CFont *pf, float sx, float sy);
char *func_00210CA8(TEXT_TABLE *p, int k);
float func_0015FEF8(CRichText *pfont, float dx);
float func_0015FF40(CRichText *pfont);
void FillScreenRect(RGBA rgba, GIFS *pgifs, float x0, float y0, float x1, float y1);
void func_0015FFD0(CRichText *pfont, TB *ptb, TB *clip, GIFS *pgifs);
void func_0015F0C0(CFont *pf);
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char RGBA_RAW_r_Offset[FIELD_OFFSET(RGBA_RAW,r)==0?1:-1];
typedef char RGBA_RAW_g_Offset[FIELD_OFFSET(RGBA_RAW,g)==1?1:-1];
typedef char RGBA_RAW_b_Offset[FIELD_OFFSET(RGBA_RAW,b)==2?1:-1];
typedef char RGBA_RAW_a_Offset[FIELD_OFFSET(RGBA_RAW,a)==3?1:-1];
typedef char RGBA_bytes_Offset[FIELD_OFFSET(RGBA,bytes)==0?1:-1];
typedef char RGBA_word_Offset[FIELD_OFFSET(RGBA,word)==0?1:-1];
typedef char CRichText_text_Offset[FIELD_OFFSET(CRichText,text)==0?1:-1];
typedef char CRichText_cursor_Offset[FIELD_OFFSET(CRichText,cursor)==4?1:-1];
typedef char CRichText_current_Offset[FIELD_OFFSET(CRichText,current)==8?1:-1];
typedef char CRichText_base_Offset[FIELD_OFFSET(CRichText,base)==12?1:-1];
typedef char CRichText_font_Offset[FIELD_OFFSET(CRichText,font)==16?1:-1];
typedef char CRichText_dirty_Offset[FIELD_OFFSET(CRichText,dirty)==336?1:-1];
typedef char CRichText_color_Offset[FIELD_OFFSET(CRichText,color)==340?1:-1];
typedef char CRichText_setColor_Offset[FIELD_OFFSET(CRichText,setColor)==344?1:-1];
typedef char CRichText_baseColor_Offset[FIELD_OFFSET(CRichText,baseColor)==348?1:-1];
typedef char CRichText_fontColor_Offset[FIELD_OFFSET(CRichText,fontColor)==352?1:-1];
typedef char TB_x_Offset[FIELD_OFFSET(TB,x)==0?1:-1];
typedef char TB_y_Offset[FIELD_OFFSET(TB,y)==4?1:-1];
typedef char TB_dx_Offset[FIELD_OFFSET(TB,dx)==8?1:-1];
typedef char TB_dy_Offset[FIELD_OFFSET(TB,dy)==12?1:-1];
typedef char TB_color_Offset[FIELD_OFFSET(TB,color)==16?1:-1];
typedef char TB_jh_Offset[FIELD_OFFSET(TB,jh)==20?1:-1];
typedef char TB_jv_Offset[FIELD_OFFSET(TB,jv)==24?1:-1];
typedef char S_x0_Offset[FIELD_OFFSET(S,x0)==0?1:-1];
typedef char S_x4_Offset[FIELD_OFFSET(S,x4)==4?1:-1];
typedef char TEXT_ENTRY_id_Offset[FIELD_OFFSET(TEXT_ENTRY,id)==0?1:-1];
typedef char TEXT_ENTRY_text_Offset[FIELD_OFFSET(TEXT_ENTRY,text)==4?1:-1];
typedef char TEXT_TABLE_count_Offset[FIELD_OFFSET(TEXT_TABLE,count)==0?1:-1];
typedef char TEXT_TABLE_entries_Offset[FIELD_OFFSET(TEXT_TABLE,entries)==4?1:-1];
typedef char P_x104_Offset[FIELD_OFFSET(P,x104)==260?1:-1];
typedef char P_x110_Offset[FIELD_OFFSET(P,x110)==272?1:-1];
typedef char P_x118_Offset[FIELD_OFFSET(P,x118)==280?1:-1];
typedef char P_x11C_Offset[FIELD_OFFSET(P,x11C)==284?1:-1];
typedef char RGBA_RAW_Size[sizeof(RGBA_RAW)==4?1:-1];
typedef char RGBA_Size[sizeof(RGBA)==4?1:-1];
typedef char CFont_Size[sizeof(CFont)==320?1:-1];
typedef char CRichText_Size[sizeof(CRichText)==368?1:-1];
typedef char TB_Size[sizeof(TB)==28?1:-1];
typedef char S_Size[sizeof(S)==8?1:-1];
typedef char TEXT_ENTRY_Size[sizeof(TEXT_ENTRY)==8?1:-1];
typedef char TEXT_TABLE_Size[sizeof(TEXT_TABLE)==8?1:-1];
typedef char P_Size[sizeof(P)==288?1:-1];
typedef char GIFS_Size[sizeof(GIFS)==40?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char uint_Size[sizeof(uint)==4?1:-1];
typedef char ulong_Size[sizeof(ulong)==8?1:-1];
typedef char long_Size[sizeof(long)==8?1:-1];
typedef char float_Size[sizeof(float)==4?1:-1];
typedef char CFont_Alignment[__alignof__(CFont)==16?1:-1];
typedef char CRichText_Alignment[__alignof__(CRichText)==16?1:-1];
typedef char RGBA_RAW_Alignment[__alignof__(RGBA_RAW)==1?1:-1];
/** @sly2-annot addr=0x1B3700
 * Purpose: Draw movie subtitle/caption text (CRichText/CFont scaling)
 * Info: proposed name DrawMpegSubtitles (T2); TU mpeg
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#2-fmv-playback-mpegcpp-0x1b2820-0x1b4438,
 *   docs/engine/cutscenes.md#open-questions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
extern "C" void func_001B3700(P *p)
{
    GIFS gifs;
    gifs.AllocStatic(0x200, (QW *)(p->x110 + 0x41BC00));
    gifs.AddDmaCnt();
    gifs.AddPrimPack(6, 1, 0xE);
    gifs.PackAD(6, 0xAA8031800L);
    gifs.PackAD(0x14, 0x61);
    gifs.PackAD(8, 5);
    gifs.PackAD(0x42, 0x44);
    gifs.PackAD(0x47, 0x31001);
    gifs.AddPrimPack(0x116, 4, 0x4343);
    gifs.PackUV(8, 8);
    gifs.PackXYZF(0x7000, 0x7900, 0, 0);
    gifs.PackUV((p->x118 << 4) | 8, (p->x11C << 4) | 8);
    gifs.PackXYZF(0x9000, 0x8700, 0, 0);
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(0x61, 0);
    gifs.AddPrimEnd();
    gifs.AddDmaEnd();
    gifs.Detach(0, 0);
    if (p->x104) {
        gifs.AllocStatic(0x200, (QW *)(p->x110 + 0x41DC00));
        gifs.AddDmaCnt();
        func_0015F078(D_002DE760, 0.8f, 0.8f);
        CRichText font(func_00210CA8(&D_003E1C20, p->x104->x4), D_002DE760);
        float dy = func_0015FEF8(&font, 320.0f);
        float dx = func_0015FF40(&font);
        TB tb(320.0f - dx * 0.5f, 476.8000183f - dy, dx, dy);
        RGBA rgba;
        TB *ptb = &tb;
        ptb->jh = 1;
        rgba.word = 0x80000000;
        FillScreenRect(rgba, &gifs, tb.x, tb.y, tb.x + tb.dx, tb.y + tb.dy);
        func_0015FFD0(&font, ptb, 0, &gifs);
        func_0015F0C0(D_002DE760);
        gifs.AddPrimPack(0, 1, 0xE);
        gifs.PackAD(0x61, 0);
        gifs.AddPrimEnd();
        gifs.AddDmaEnd();
        gifs.Detach(0, 0);
    }
}
