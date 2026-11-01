typedef int qword __attribute__((mode(TI)));
float UFromShapeS(void *p, float seed);
void FindShapeClosestPointFromU(void *p, qword q, void *a, qword *b, qword *c, float *d, float *e, float g);
void FindShapeClosestPointFromS(void *p, qword q, void *a, qword *b, qword *c, float *d, float *e, float seed)
{
    FindShapeClosestPointFromU(p, q, a, b, c, d, e, UFromShapeS(p, seed));
}
