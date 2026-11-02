// WriteTftUl(TFT*, OSTRM*, ulong): unsigned printf conversion (Claude agent)
struct TFT {int fLeft,fPlus,fSpace,fPrefix,fZero,fPrec0,width,precision,cbits;int matrixRows,vectorColumns;unsigned char conversion;int fUpper;};
union UL {unsigned short us;short s;unsigned int u;int i;unsigned long ul;long l;};
class OSTRM {public:char *buffer;int capacity,used;char unknownC[4];int written,buffered;int CbWrite(void *,int);};
int FNegTftUl(TFT *,UL *);
int CchAchFromUl(char *,int,unsigned long,unsigned int);
int UnRadixFromChConversion(unsigned char) __asm__("UnRadixFromChConversion__Fc");
extern "C" unsigned int func_0021ED28(const char *);
extern "C" void func_0021EE30(char *);
static inline void WritePad(OSTRM *out,char ch,int count){char b=ch;while(count-- >0)if(out->CbWrite(&b,1)!=1)break;}
/** @sly2-annot addr=0x21DD40
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
void WriteTftUl(TFT *f,OSTRM *out,unsigned long n){
    char digits[65];
    char prefix[3];
    int cPrefix,cDigits,cZero;
    if(f->fPrec0 || f->precision)f->fZero=0;
    if(f->fPrec0 && n==0){
        cPrefix=0;
        cZero=0;
        cDigits=0;
    } else {
        int negative=FNegTftUl(f,(UL *)&n);
        cZero=f->precision;
        prefix[0]=0;
        switch(f->conversion){
        case 'd':case 'i':
            if(negative)__builtin_memcpy(prefix,"-",2);
            else if(f->fPlus)__builtin_memcpy(prefix,"+",2);
            else if(f->fSpace)__builtin_memcpy(prefix," ",2);
            cZero-=func_0021ED28(prefix);
            break;
        case 'b':if(f->fPrefix)__builtin_memcpy(prefix,"0b",3);break;
        case 'o':if(f->fPrefix)__builtin_memcpy(prefix,"0",2);break;
        case 'x':if(f->fPrefix)__builtin_memcpy(prefix,"0x",3);break;
        }
        cPrefix=func_0021ED28(prefix);
        cDigits=CchAchFromUl(digits,0x40,n,UnRadixFromChConversion(f->conversion));
        cZero-=cDigits;
        if(cZero<0)cZero=0;
    }
    digits[cDigits]=0;
    if(f->fUpper){func_0021EE30(prefix);func_0021EE30(digits);}
    int total=cPrefix+cZero+cDigits;
    int left=0,right=0;
    if(total<f->width){
        int padding=f->width-total;
        if(f->fLeft)right=padding;
        else if(f->fZero && cDigits)cZero+=padding;
        else left=padding;
    }
    WritePad(out,' ',left);
    out->CbWrite(prefix,cPrefix);
    WritePad(out,'0',cZero);
    out->CbWrite(digits,cDigits);
    WritePad(out,' ',right);
}
