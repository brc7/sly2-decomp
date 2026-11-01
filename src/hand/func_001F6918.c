#include "common.h"
struct S { char pad[0x148]; ulong a:26; int b:1; ulong c:37; };
struct G;
extern struct G D_003DD490;
void RecalcSwOxfFilterForObject(struct G *, struct S *);
void func_001F6918(struct S *p, int f) { p->b = f; RecalcSwOxfFilterForObject(&D_003DD490, p); }
