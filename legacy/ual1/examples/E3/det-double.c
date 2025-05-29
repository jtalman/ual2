#include <stdio.h>
#include <stdlib.h>

int m,n; // original matrix dimensions

double det(double B[m][n]);

int main() {
    double determinant;
    int row,column;

    printf("Enter rows and columns\n");
    scanf("%d%d",&m,&n);
    printf("m %d\n",m);
    printf("n %d\n",n);
    
    double A[m][n];

    printf("Enter matrix elements\n");

    for(row = 0; row < m; row++){
        for(column = 0; column < n; column++){
            scanf("%lf",&A[row][column]);
        }
    }

    for(row = 0; row < m; row++){
        for(column = 0; column < n; column++){
            printf("%e  ",A[row][column]);
        }
        printf("\n");
    }
            
    determinant = det(A);

    printf("determinant = %e \n",determinant);

    return 0;
}

double det(double B[m][n]) {
    int row_size = m;
    int column_size = n;

    if (row_size != column_size) {
        printf("DimensionError: Operation Not Permitted \n");
        exit(1);
    }

    else if (row_size == 1)
        return (B[0][0]);

    else if (row_size == 2)
        return (B[0][0]*B[1][1] - B[1][0]*B[0][1]);

    else {
        double minor[row_size-1][column_size-1];
        int row_minor, column_minor;
        int firstrow_columnindex;
        double sum = 0;

        register int row,column;

        // exclude first row and current column
        for(firstrow_columnindex = 0; firstrow_columnindex < row_size;
                firstrow_columnindex++) {

            row_minor = 0;

            for(row = 1; row < row_size; row++) {

                column_minor = 0;

                for(column = 0; column < column_size; column++) {
                    if (column == firstrow_columnindex)
                        continue;
                    else
                        minor[row_minor][column_minor] = B[row][column];

                    column_minor++;
                }

                row_minor++;
            }

            m = row_minor;
            n = column_minor;

            if (firstrow_columnindex % 2 == 0)
                sum += B[0][firstrow_columnindex] * det(minor);
            else
                sum -= B[0][firstrow_columnindex] * det(minor);

        }

        return sum;

    }
}
