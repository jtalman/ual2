//
// NumLib.hh
//

#ifndef NUMLIB_H
#define NUMLIB_H

int sign(double a);
int min(int a, int b);
int max(int a, int b);

void invMatrix(double** a, int n);
void solveLinearEquations(double** a, double* b, int n);


#endif
