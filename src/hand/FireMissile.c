#include "vec.h"
struct LO;
struct CONTROLLER;
struct ASEG;
struct ASEGA;
struct ACTION;
struct P {
 char pad[0x5C]; struct CONTROLLER *controller;
 char pad1[0x210-0x60]; struct LO *source;
 int pad2[3]; struct VECTOR position;
 struct ASEG *aseg; struct ASEGA *asega[12];
 int enabled; struct ACTION *action; float parameter;
};
void func_001305B8(struct ASEG*,struct P*,float,float,int,struct ASEGA**);
struct ACTION *func_0012DCA0(struct P*,struct CONTROLLER*,int,int,float,float);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char P_controller_Offset[FIELD_OFFSET(struct P,controller)==92?1:-1];
typedef char P_source_Offset[FIELD_OFFSET(struct P,source)==528?1:-1];
typedef char P_position_Offset[FIELD_OFFSET(struct P,position)==544?1:-1];
typedef char P_aseg_Offset[FIELD_OFFSET(struct P,aseg)==560?1:-1];
typedef char P_asega_Offset[FIELD_OFFSET(struct P,asega)==564?1:-1];
typedef char P_enabled_Offset[FIELD_OFFSET(struct P,enabled)==612?1:-1];
typedef char P_action_Offset[FIELD_OFFSET(struct P,action)==616?1:-1];
typedef char P_parameter_Offset[FIELD_OFFSET(struct P,parameter)==620?1:-1];
typedef char P_PrefixSize[sizeof(struct P)==0x270?1:-1];
typedef char Vector_Size[sizeof(struct VECTOR)==16?1:-1];
typedef char AsegaArray_Size[sizeof(((struct P*)0)->asega)==48?1:-1];
void FireMissile(struct P *p,struct LO *source,struct VECTOR *point)
{
 struct VECTOR v;
 v=*point;
 p->source=source;
 p->position=v;
 func_001305B8(p->aseg,p,0.0f,1.0f,8,p->asega);
 if(p->enabled)
  p->action=func_0012DCA0(p,p->controller,0,5,p->parameter,0.0f);
}
