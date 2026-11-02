// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
typedef void *(*__eh_matcher)(void *, void *, void *);
typedef struct { __eh_matcher match_function; short language; short version; } __eh_info;
void **__get_eh_info(void);
int __eh_rtime_match(void *rtime)
{
    void *info;
    __eh_matcher matcher;
    void *ret;
    info = *(__get_eh_info());
    matcher = ((__eh_info *)info)->match_function;
    if (!matcher)
        goto none;
    ret = (*matcher)(info, rtime, (void *)0);
    return (ret != 0);
none:
    return 0;
}
