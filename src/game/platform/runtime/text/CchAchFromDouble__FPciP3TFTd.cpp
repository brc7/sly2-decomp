struct TFT {int fLeft,fPlus,fSpace,fPrefix,fZero,fPrec0,width,precision,cbits;char pad24[8];unsigned char conversion;char pad2d[3];int fUpper;};
extern "C" {
char *strcpy(char *,const char *);
unsigned int func_0021ED28(const char *);
int ExpFirstFromG__Fd(double);
unsigned char ChDigitFromG__Fdii(double,int,int);
double pow(double,double);
extern char D_002A94F0[],D_002A94F8[];
extern char D_002C40C8[];
}
/** @sly2-annot addr=0x21E1B0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int CchAchFromDouble(char *buffer,int capacity,TFT *f,double value){
    union {double d;unsigned long u;} bits;
    bits.d=value;
    if((bits.u&0x7FF0000000000000UL)==0x7FF0000000000000UL){
        strcpy(buffer,(bits.u&0xFFFFFFFFFFFFFUL)?D_002A94F0:D_002A94F8);
        return func_0021ED28(buffer);
    }
    int exponent=ExpFirstFromG__Fd(value);
    char *p=buffer;
    int scientific=f->conversion=='e';
    if(f->conversion=='g'){
        if(exponent<-4 || exponent>=f->precision){scientific=1;f->precision--;}
    }
    int roundExponent=scientific?exponent:0;
    double rounded=value+0.5*pow(10.0,(double)(roundExponent-f->precision));
    int roundedExponent=ExpFirstFromG__Fd(rounded);
    if(exponent<ExpFirstFromG__Fd(rounded))exponent=roundedExponent;
    int digit,count;
    if(scientific){count=1;digit=exponent;}
    else if(exponent>=0){count=exponent+1;digit=exponent;}
    else {count=1;digit=0;}
    while(count-- >0)*p++=ChDigitFromG__Fdii(rounded,exponent,digit--);
    if(!f->fPrec0){
        *p++='.';
        count=f->precision;
        while(count-- >0)*p++=ChDigitFromG__Fdii(rounded,exponent,digit--);
        if(f->conversion=='g'){
            while(p>buffer && ((unsigned char *)p)[-1]=='0')--p;
            if(p>buffer && ((unsigned char *)p)[-1]=='.')--p;
        }
    }
    if(scientific){
        *p++='e';
        if(exponent<0){*p++='-';exponent=-exponent;}
        else *p++='+';
        int divisor=(int)pow(10.0,2.0);
        for(int i=2;i>=0;--i){
            *p++=D_002C40C8[exponent/divisor];
            exponent=(exponent%divisor)*10;
        }
    }
    return p-buffer;
}
