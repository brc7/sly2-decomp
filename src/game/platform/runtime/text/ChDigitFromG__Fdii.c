extern unsigned char D_002C40C8[];
double pow(double, double);
double GTrunc__Fd(double);
double dpmul(double,double);
double dpsub(double,double);
double litodp(int);
int dptoli(double);
/* Digit of g at decimal position a (precision b): 0 outside [b, b+15). */
/** @sly2-annot addr=0x21E0D8
 * Purpose: digit character of g at decimal position a (precision b)
 * Info: proposed name ChDigitFromG (T1); TU text
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8+verdicts_A2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
unsigned char ChDigitFromG__Fdii(double g, int a, int b)
{
int i; double x;
    if (a < b || a - 15 >= b) i = 0;
    else {
        x = GTrunc__Fd(dpmul(g, pow(10.0, litodp(-b))));
        i = dptoli(dpsub(x, dpmul(GTrunc__Fd(dpmul(x, 0.1)), 10.0)));
    }
    return D_002C40C8[i];
}
