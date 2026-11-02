struct TFT {int fLeft,fPlus,fSpace,fPrefix,fZero,fPrec0,width,precision,cbits;char pad24[8];unsigned char conversion;char pad2d[3];int fUpper;};
class OSTRM {public:int CbWrite(void *,int);};
int CchAchFromDouble(char *,int,TFT *,double);
extern "C" unsigned int func_0021ED28(const char *);
extern "C" void UpperizePchz__FPc(char *);
static inline void WritePad(OSTRM *out,char ch,int count){char b=ch;while(count-- >0)if(out->CbWrite(&b,1)!=1)break;}
/** @sly2-annot addr=0x21E4B8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
void WriteTftDouble(TFT *f,OSTRM *out,double value){
    char digits[0x1B6];
    char prefix[0x10];
    int zeroes=0;
    if(!f->precision && !f->fPrec0)f->precision=6;
    else if(f->fPrec0 && f->conversion=='g')f->precision=1;
    int negative=value<0.0;
    if(negative)value=0.0-value;
    prefix[0]=0;
    if(negative)__builtin_memcpy(prefix,"-",2);
    else if(f->fPlus)__builtin_memcpy(prefix,"+",2);
    else if(f->fSpace)__builtin_memcpy(prefix," ",2);
    int cPrefix=func_0021ED28(prefix);
    int cDigits=CchAchFromDouble(digits,0x1B5,f,value);
    digits[cDigits]=0;
    if(f->fUpper)UpperizePchz__FPc(digits);
    int left=0,right=0;
    int total=cPrefix+cDigits;
    if(total<f->width){
        int padding=f->width-total;
        if(f->fLeft)right=padding;
        else if(f->fZero && cDigits)zeroes=padding;
        else left=padding;
    }
    WritePad(out,' ',left);
    out->CbWrite(prefix,cPrefix);
    WritePad(out,'0',zeroes);
    out->CbWrite(digits,cDigits);
    WritePad(out,' ',right);
}
