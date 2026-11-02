// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fexceptions
// SLY2_ASM: gnu-reorder-db
typedef unsigned int size_t;
typedef unsigned int uword;
typedef int sword;
typedef struct dwarf_fde { uword length; sword CIE_delta; void *pc_begin; uword pc_range; } fde;
typedef struct fde_vector { fde **array; size_t count; } fde_vector;
static inline int
fde_compare (fde *x, fde *y)
{
  return (int)(x->pc_begin - y->pc_begin);
}
void
fde_split (fde_vector *linear, fde_vector *erratic)
{
  size_t count = linear->count;
  size_t linear_max = (size_t) -1;
  size_t previous_max[count];
  size_t i, j;

  for (i = 0; i < count; i++)
    {
      for (j = linear_max;
           j != (size_t) -1
           && fde_compare (linear->array[i], linear->array[j]) < 0;
           j = previous_max[j])
        {
          erratic->array[erratic->count++] = linear->array[j];
          linear->array[j] = (fde *) 0;
        }
      previous_max[i] = j;
      linear_max = i;
    }

  for (i = 0, j = 0; i < count; i++)
    if (linear->array[i] != (fde *) 0)
      linear->array[j++] = linear->array[i];
  linear->count = j;
}
