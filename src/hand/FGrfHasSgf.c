unsigned long GrfFromSgf(int n);
int FGrfHasSgf(unsigned long fl, int n) { return (fl & GrfFromSgf(n)) != 0; }
