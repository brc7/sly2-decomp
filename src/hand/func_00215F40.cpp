extern "C" {
struct S { unsigned char *start; unsigned char *cur; void *font; char pad[0x74-0xC]; float width; char pad1[0x88-0x78]; float scale; };
extern unsigned char D_002AA6A1[];
void func_00215CA8(struct S *,unsigned char *);
unsigned char func_00215D40(struct S *);
float func_00216B80(void *,unsigned char);
static inline int NewlineValue(){int value;__asm__("addiu %0,$zero,10":"=r"(value));return value;}
static inline void StoreByte(unsigned char *p,int c){*p=c;}
/** @sly2-annot addr=0x215F40
 * Purpose: count lines, inserting newlines when width exceeded
 * Info: proposed name CRichText2_ClineWrap (T2); class CRichText; TU hud
 * Notes: COLLISION: CRichText_ClineWrap already used at 0x15FD68 (existing symbol); renamed
 *   CRichText_ClineWrap_215F40 | v3: superseded CRichText_ClineWrap_215F40 | top50 finding 8: hud-TU rich
 *   text (colours +0x14/+0x18) is a second class; Reset__9CRichText 0x15F970 keeps CRichText. CRichText2 is a
 *   placeholder class word (T2); resolves the v2 T3 collision | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.55] - evidence: semantic review; sources: r7c_hud+top50_round(CRichText2
 *   class split)
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int func_00215F40(struct S *p) {
    float width=0.0f;
    int lines=1;
    unsigned char c;
    int newline=NewlineValue();
    func_00215CA8(p,0);
    while ((c=func_00215D40(p))!=0) {
        if(c=='\n') { width=0.0f; lines++; }
        else {
            width+=func_00216B80(p->font,c)*p->scale;
        }
        if(p->width!=0.0f && p->width<width) {
                unsigned char *q=p->cur-1;
                while(q>=p->start && *q!='\n') {
                    if((*q==' ' || *q=='\t') && !(D_002AA6A1[q[1]]&0x10)) {
                        p->cur=q+1;
                        StoreByte(q,newline);
                        lines++;
                        width=0.0f;
                        break;
                    }
                    q--;
                }
                if(width!=0.0f && *p->cur) {
                    StoreByte(p->cur,newline); lines++; width=0.0f;
                }
            }

    }
    return lines;
}

}