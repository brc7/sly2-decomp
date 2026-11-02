// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef unsigned int uword;
typedef int sword;
typedef struct dwarf_fde {
    uword length;
    sword CIE_delta;
    void *pc_begin;
    uword pc_range;
} fde;
typedef struct fde_vector {
    fde **array;
    unsigned int count;
} fde_vector;
typedef struct fde_accumulator {
    fde_vector linear;
    fde_vector erratic;
} fde_accumulator;

static __inline__ fde *next_fde(fde *p)
{
    return (fde *)((char *)p + p->length + sizeof(p->length));
}

static __inline__ void fde_insert(fde_accumulator *accu, fde *this_fde)
{
    accu->linear.array[accu->linear.count++] = this_fde;
}

void add_fdes(fde *this_fde, fde_accumulator *accu, void **beg_ptr, void **end_ptr)
{
    void *pc_begin = *beg_ptr;
    void *pc_end = *end_ptr;

    for (; this_fde->length != 0; this_fde = next_fde(this_fde))
    {
        if (this_fde->CIE_delta == 0 || this_fde->pc_begin == 0)
            continue;

        fde_insert(accu, this_fde);

        if (this_fde->pc_begin < pc_begin)
            pc_begin = this_fde->pc_begin;
        if ((char *)this_fde->pc_begin + this_fde->pc_range > (char *)pc_end)
            pc_end = (char *)this_fde->pc_begin + this_fde->pc_range;
    }

    *beg_ptr = pc_begin;
    *end_ptr = pc_end;
}
