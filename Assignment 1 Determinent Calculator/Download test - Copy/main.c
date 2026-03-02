#include <stdio.h>
int multiplier=1;
void print_matrix(int N, double matrix[N][N])
{
printf("\n \n");
for (int i=0; i<N; i++)
{
for(int j=0; j<N; j++)
{
printf("%0.1lf ",matrix[i][j]);
}
printf("\n");
}
printf("\n \n");
}
void Load_matrix(int N, double matrix[N][N])
{
for (int i=0; i<N; i++)
{
for (int j=0; j<N; j++)
{
scanf ("%lf", &matrix[i][j] );
}
}
}
void Row_Swap (int N, double matrix[N][N], int R1, int R2)
{ double temp;
for(int i=0;i<N;i++)
{
temp= matrix[R1][i];
matrix[R1][i]=matrix[R2][i];
matrix[R2][i]=temp;
}
multiplier=multiplier * -1;
}
int row_search(int N, double matrix[N][N], int i )
{
for(int j= i+1; j<N; j++)
{
if (matrix[j][i] != 0)
{return j;
}
}
return -1;
}
int main() {
double Determinent=0.0;
int N=0;
scanf("%d",&N);
double matrix[N][N];
Load_matrix(N,matrix);
int row_found=0;
for(int r=0; r<N; r++)
{
if (matrix[r][r]==0)
{
row_found= row_search(N, matrix, r);
if(row_found==-1)
{
printf("%lf", Determinent);
return 0;
}
Row_Swap(N, matrix, r, row_found);
}
double pivot_point= matrix[r][r];
for (int r2=r+1; r2<N; r2++)
{
if(matrix[r2][r]==0){continue;}
double factor = matrix[r2][r];
for(int k=r; k<N; k++)
{
matrix[r2][k]=matrix[r2][k]-matrix[r][k]*factor/pivot_point;
}
}
}Determinent=(double)multiplier;
for(int r=0; r<N; r++)
{
Determinent=Determinent*matrix[r][r];
}
printf("%lf", Determinent);
return 0;
}