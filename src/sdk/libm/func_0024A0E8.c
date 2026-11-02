// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/*
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this software is freely
 * granted, provided that this notice is preserved.
 */
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef union {float value;__uint32_t word;} ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do {ieee_float_shape_type u;u.value=(d);(i)=u.word;}while(0)
#define SET_FLOAT_WORD(d,i) do {ieee_float_shape_type u;u.word=(i);(d)=u.value;}while(0)
float func_0024A780(float,float);
static const float two25=3.355443200e+07,twom25=2.9802322388e-08,huge=1.0e+30,tiny=1.0e-30;
float func_0024A0E8(float x,int n)
{
    __int32_t k,ix;
    GET_FLOAT_WORD(ix,x);
    k=(ix&0x7f800000)>>23;
    if(k==0){
        if((ix&0x7fffffff)==0)return x;
        x*=two25;
        GET_FLOAT_WORD(ix,x);
        k=((ix&0x7f800000)>>23)-25;
        if(n < -50000)return tiny*x;
    }
    if(k==0xff)return x+x;
    k=k+n;
    if(k>0xfe)return huge*func_0024A780(huge,x);
    if(k>0){SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23));return x;}
    if(k<=-25){
        if(n>50000)return huge*func_0024A780(huge,x);
        else return tiny*func_0024A780(tiny,x);
    }
    k+=25;
    SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23));
    return x*twom25;
}
