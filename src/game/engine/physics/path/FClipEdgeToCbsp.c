
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
/** @sly2-annot addr=0x1BF900
 * Purpose: True if the segment leaves the walkable area (no span or span not [0,1])
 * Info: proposed name FClipEdgeToCbsp (T1); TU path
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
int FClipEdgeToCbsp(GRAPH*g,qword from,qword to,int flags){
 ROW spans[2];
 int count=ClsgClipEdgeToCbsp(g,from,to,2,spans,flags|2);
 int result=0;
 if(!count||spans[0].low!=0.0f||spans[0].high!=1.0f)result=1;
 return result;
}
