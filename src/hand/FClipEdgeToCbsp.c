
typedef int qword __attribute__((mode(TI)));
typedef union U{qword q;float f[4];}U;
typedef struct GRAPH GRAPH;
typedef struct TRI TRI;
typedef struct ROW{U begin,end,beginPlane,endPlane;float low,high;int flags;TRI*leaf;char tail[16];}ROW;
extern int ClsgClipEdgeToCbsp(GRAPH*,qword,qword,int,ROW*,int);
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char ROW_Size[sizeof(ROW)==96?1:-1];
typedef char ROW_Low_Offset[(unsigned int)&((ROW*)0)->low==0x40?1:-1];
typedef char ROW_High_Offset[(unsigned int)&((ROW*)0)->high==0x44?1:-1];
int FClipEdgeToCbsp(GRAPH*g,qword from,qword to,int flags){
 ROW spans[2];
 int count=ClsgClipEdgeToCbsp(g,from,to,2,spans,flags|2);
 int result=0;
 if(!count||spans[0].low!=0.0f||spans[0].high!=1.0f)result=1;
 return result;
}
