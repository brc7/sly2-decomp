// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// (Claude agent) newlib wf_pow.c, -O2: exact except the neg**non-integral block: target loads the 0.0/0.0 NaN double as 'lui v0; ld v1,lo(v0); sd v1' (separate store per arm), ours 'ld v0' so jump.c cross-jumps the two sd's (1 insn shorter). pow.c (double) has the same NaN ld-v1 pattern. x<0.0 must be a double compare.
struct exception {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
    int err;
};
extern const int __fdlib_version;
extern const double __infinity_d;
float func_00248038(float x, float y);
int func_0024A0C0(float x);
int func_0024A098(float x);
int func_00249C80(double x);
int func_00249C60(double x);
double func_00249CE0(double x);
int matherr(struct exception *x);
int *func_002401C8(void);
#define _LIB_VERSION __fdlib_version
#define _IEEE_ -1
#define _SVID_ 0
#define _POSIX_ 2
#define DOMAIN 1
#define OVERFLOW 3
#define UNDERFLOW 4
#define errno (*func_002401C8())
#define EDOM 33
#define ERANGE 34
#define HUGE 3.40282346638528859812e+38
#define HUGE_VAL __infinity_d
#define isnanf func_0024A0C0
#define finitef func_0024A098
#define rint func_00249CE0
float powf(float x, float y)
{
	float z;
	struct exception exc;
	z=func_00248038(x,y);
	if(_LIB_VERSION == _IEEE_|| isnanf(y)) return z;
	if(isnanf(x)) {
	    if(y==(float)0.0) { 
		exc.type = DOMAIN;
		exc.name = "powf";
		exc.err = 0;
		exc.arg1 = (double)x;
		exc.arg2 = (double)y;
		exc.retval = (double)x;
		if (_LIB_VERSION == _IEEE_ ||
		    _LIB_VERSION == _POSIX_) exc.retval = 1.0;
		else if (!matherr(&exc)) {
			errno = EDOM;
		}
	        if (exc.err != 0)
	           errno = exc.err;
                return (float)exc.retval; 
	    } else 
		return z;
	}
	if(x==(float)0.0){ 
	    if(y==(float)0.0) {
		exc.type = DOMAIN;
		exc.name = "powf";
		exc.err = 0;
		exc.arg1 = (double)x;
		exc.arg2 = (double)y;
		exc.retval = 0.0;
		if (_LIB_VERSION != _SVID_) exc.retval = 1.0;
		else if (!matherr(&exc)) {
			errno = EDOM;
		}
	        if (exc.err != 0)
	           errno = exc.err;
                return (float)exc.retval; 
	    }
	    if(finitef(y)&&y<(float)0.0) {
		exc.type = DOMAIN;
		exc.name = "powf";
		exc.err = 0;
		exc.arg1 = (double)x;
		exc.arg2 = (double)y;
		if (_LIB_VERSION == _SVID_)
		  exc.retval = 0.0;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  errno = EDOM;
		}
	        if (exc.err != 0)
	           errno = exc.err;
                return (float)exc.retval; 
	    }
	    return z;
	}
	if(!finitef(z)) {
	    if(finitef(x)&&finitef(y)) {
	        if(isnanf(z)) {
		    exc.type = DOMAIN;
		    exc.name = "powf";
		    exc.err = 0;
		    exc.arg1 = (double)x;
		    exc.arg2 = (double)y;
		    if (_LIB_VERSION == _SVID_) 
		        exc.retval = 0.0;
		    else 
		        exc.retval = 0.0/0.0;
		    if (_LIB_VERSION == _POSIX_) 
		        errno = EDOM;
		    else if (!matherr(&exc)) {
		        errno = EDOM;
		    }
	            if (exc.err != 0)
	                errno = exc.err;
                    return (float)exc.retval; 
	        } else {
		    exc.type = OVERFLOW;
		    exc.name = "powf";
		    exc.err = 0;
		    exc.arg1 = (double)x;
		    exc.arg2 = (double)y;
		    if (_LIB_VERSION == _SVID_) {
		       exc.retval = HUGE;
		       y *= 0.5;
		       if(x<0.0&&rint(y)!=y) exc.retval = -HUGE;
		    } else {
		       exc.retval = HUGE_VAL;
		       y *= 0.5;
		       if(x<0.0&&rint(y)!=y) exc.retval = -HUGE_VAL;
		    }
		    if (_LIB_VERSION == _POSIX_)
		        errno = ERANGE;
		    else if (!matherr(&exc)) {
			errno = ERANGE;
		    }
	            if (exc.err != 0)
	                errno = exc.err;
                    return (float)exc.retval; 
	        }
	    }
	} 
	if(z==(float)0.0&&finitef(x)&&finitef(y)) {
	    exc.type = UNDERFLOW;
	    exc.name = "powf";
	    exc.err = 0;
	    exc.arg1 = (double)x;
	    exc.arg2 = (double)y;
	    exc.retval =  0.0;
	    if (_LIB_VERSION == _POSIX_)
	        errno = ERANGE;
	    else if (!matherr(&exc)) {
	        errno = ERANGE;
	    }
	    if (exc.err != 0)
	        errno = exc.err;
            return (float)exc.retval; 
        }
	return z;
}
