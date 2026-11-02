// Parse one TFT specifier; all length modifiers advance to conversion.
struct TFT {int fLeft,fPlus,fSpace,fPrefix,fZero,fPrec0,width,precision,cbits,matrixRows,vectorColumns;unsigned char conversion;int fUpper;};
extern "C" {void *func_002333B4(void *,int,int);extern unsigned char D_002AA6A1[];}
int CchParsePchzInt(char *,int *);
static inline int Abs(int n){return n<0?-n:n;}
static inline int Upper(unsigned char c){return D_002AA6A1[c]&1;}
static inline int Lower(unsigned char c){int r=c+0x20;if(!Upper(c))r=c;return r;}
/** @sly2-annot addr=0x21D6B0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int CchParsePchzTft(char *format,TFT *f,char **args){
    int state=0;
    func_002333B4(f,0,0x34);
    unsigned char *p=(unsigned char *)format;
    while(*p && state!=6){
        int consumed=0;
        int next=state;
        switch(state){
        case 0:
            if(*p=='%'){
                if(p[1]==*p){f->conversion=p[1];next=6;consumed=2;}
                else {next=1;consumed=1;}
            }else next=6;
            break;
        case 1:
            switch(*p){
            case '-':f->fLeft=1;consumed=1;break;
            case '+':f->fPlus=1;consumed=1;break;
            case ' ':f->fSpace=1;consumed=1;break;
            case '#':f->fPrefix=1;consumed=1;break;
            case '0':f->fZero=1;consumed=1;break;
            default:next=2;break;
            }
            break;
        case 2:
            if(*p=='*'){
                char *a=*args;*args=a+8;
                f->width=*(int *)a;
                if(f->width<0){int width=Abs(f->width);f->fLeft=1;f->width=width;}
                consumed=1;
            }else consumed=CchParsePchzInt((char *)p,&f->width);
            if(p[consumed]=='.'){next=3;consumed++;}else next=4;
            break;
        case 3:
            if(*p=='*'){
                char *a=*args;*args=a+8;
                f->precision=Abs(*(int *)a);consumed=1;
            }else consumed=CchParsePchzInt((char *)p,&f->precision);
            f->fPrec0=f->precision==0;
            next=4;
            break;
        case 4:
            switch(*p){
            case 'h':f->cbits=1;consumed=1;break;
            case 'l':consumed=1;if(p[1]!='l'){f->cbits=2;break;}
            case 'L':case 'q':f->cbits=2;consumed++;break;
            case 'M':case 'm':case 'V':case 'v':
                f->matrixRows=Upper(*p)?4:3;
                f->vectorColumns=Lower(*p)=='v'?1:f->matrixRows;
                consumed=1;break;
            default:break;
            }
            next=5;
            break;
        case 5:
            if(*p!='%'){f->fUpper=Upper(*p);f->conversion=Lower(*p);}
            consumed=1;next=6;
            break;
        default:goto done;
        }
        p+=consumed;state=next;
    }
 done:
    return (char *)p-format;
}
