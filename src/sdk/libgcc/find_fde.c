// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef unsigned int size_t;
typedef unsigned int uword;
typedef int sword;
typedef struct dwarf_fde { uword length; sword CIE_delta; void *pc_begin; uword pc_range; } fde;
struct object { void *pc_begin; void *pc_end; fde *fde_begin; fde **fde_array; size_t count; struct object *next; };
extern struct object *D_00450278;
#define objects D_00450278
void func_0023F710(struct object *ob);
#define frame_init func_0023F710
typedef int __gthread_mutex_t;
static __gthread_mutex_t object_mutex;
static inline int __gthread_mutex_lock(__gthread_mutex_t *m) { return 0; }
static inline int __gthread_mutex_unlock(__gthread_mutex_t *m) { return 0; }
fde *find_fde(void *pc)
{
  struct object *ob;
  size_t lo, hi;

  __gthread_mutex_lock (&object_mutex);

  for (ob = objects; ob; ob = ob->next)
    {
      if (ob->pc_begin == 0)
	frame_init (ob);
      if (pc >= ob->pc_begin && pc < ob->pc_end)
	break;
    }

  __gthread_mutex_unlock (&object_mutex);

  if (ob == 0)
    return 0;

  for (lo = 0, hi = ob->count; lo < hi; )
    {
      size_t i = (lo + hi) / 2;
      fde *f = ob->fde_array[i];

      if (pc < f->pc_begin)
	hi = i;
      else if (pc >= f->pc_begin + f->pc_range)
	lo = i + 1;
      else
	return f;
    }

  return 0;
}
