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
void func_001AE4F0(void *);
#define free func_001AE4F0
void func_00240328(void);
#define abort func_00240328
typedef int __gthread_mutex_t;
static __gthread_mutex_t object_mutex;
static inline int __gthread_mutex_lock(__gthread_mutex_t *m) { return 0; }
static inline int __gthread_mutex_unlock(__gthread_mutex_t *m) { return 0; }
void *
__deregister_frame_info (void *begin)
{
  struct object **p;

  __gthread_mutex_lock (&object_mutex);

  p = &objects;
  while (*p)
    {
      if ((*p)->fde_begin == begin)
	{
	  struct object *ob = *p;
	  *p = (*p)->next;

	  /* If we've run init_frame_for_object on this object, free it.  */
	  if (ob->pc_begin)
	    free (ob->fde_array);

	  __gthread_mutex_unlock (&object_mutex);
	  return (void *) ob;
	}
      p = &((*p)->next);
    }

  __gthread_mutex_unlock (&object_mutex);
  abort ();
}
