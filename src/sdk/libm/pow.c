// (Claude agent) newlib w_pow.c, -O2: only 6 diffs, all the PRE hi-copy artefact (target 'lui s5,%hi(__fdlib_version)' in the prologue; ours 'lui v1 ... daddu s5,v1' after the call). powf (same source, float) has no copy.
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
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
double func_00246300(double x, double y);
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
#define isnan func_00249C80
#define finite func_00249C60
#define rint func_00249CE0
double pow(double x, double y)
{
	double z;
	struct exception exc;
	z=func_00246300(x,y);
	if(_LIB_VERSION == _IEEE_|| isnan(y)) return z;
	if(isnan(x)) {
	    if(y==0.0) { 
		exc.type = DOMAIN;
		exc.name = "pow";
		exc.err = 0;
		exc.arg1 = x;
		exc.arg2 = y;
		exc.retval = x;
		if (_LIB_VERSION == _IEEE_ ||
		    _LIB_VERSION == _POSIX_) exc.retval = 1.0;
		else if (!matherr(&exc)) {
			errno = EDOM;
		}
	        if (exc.err != 0)
	           errno = exc.err;
                return exc.retval; 
	    } else 
		return z;
	}
	if(x==0.0){ 
	    if(y==0.0) {
		exc.type = DOMAIN;
		exc.name = "pow";
		exc.err = 0;
		exc.arg1 = x;
		exc.arg2 = y;
		exc.retval = 0.0;
		if (_LIB_VERSION != _SVID_) exc.retval = 1.0;
		else if (!matherr(&exc)) {
			errno = EDOM;
		}
	        if (exc.err != 0)
	           errno = exc.err;
                return exc.retval; 
	    }
	    if(finite(y)&&y<0.0) {
		exc.type = DOMAIN;
		exc.name = "pow";
		exc.err = 0;
		exc.arg1 = x;
		exc.arg2 = y;
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
                return exc.retval; 
	    }
	    return z;
	}
	if(!finite(z)) {
	    if(finite(x)&&finite(y)) {
	        if(isnan(z)) {
		    exc.type = DOMAIN;
		    exc.name = "pow";
		    exc.err = 0;
		    exc.arg1 = x;
		    exc.arg2 = y;
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
                    return exc.retval; 
	        } else {
		    exc.type = OVERFLOW;
		    exc.name = "pow";
		    exc.err = 0;
		    exc.arg1 = x;
		    exc.arg2 = y;
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
                    return exc.retval; 
	        }
	    }
	} 
	if(z==0.0&&finite(x)&&finite(y)) {
	    exc.type = UNDERFLOW;
	    exc.name = "pow";
	    exc.err = 0;
	    exc.arg1 = x;
	    exc.arg2 = y;
	    exc.retval =  0.0;
	    if (_LIB_VERSION == _POSIX_)
	        errno = ERANGE;
	    else if (!matherr(&exc)) {
	        errno = ERANGE;
	    }
	    if (exc.err != 0)
	        errno = exc.err;
            return exc.retval; 
        }
	return z;
}
