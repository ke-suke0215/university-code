#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double ax, ay, b, c, vplate, v, d, Lx, Ly; /* v=v^* */

int N = 200; /* ブロック数 */
double f(double v1);
double g1(double t, double x0, double x2, double x1, double v1, double theta1, double a, double L);
double g2(double t, double x0, double x3, double x2, double x1, double v2, double theta2, double a, double L);
double g3(double t, double x0, double x2, double x1, double v2, double theta2, double a, double L);
double h(double v1, double theta1);

int main()
{
    int s, M; /* M:計算の回数 */
    double i, t, xold[N], thetaold[N], xnew[N], vnew[N], thetanew[N], masatu[N], k[N][4], l[N][4], m[N][4], dt, maxt, Time, T, zero, a[N], L[N];

    // 変数・パラメータ設定

    //摩擦パラメータ
    ax = pow(10.0, -5.0);
    ay = pow(10.0, -1.0);
    b = pow(10.0, -3.0);
    c = pow(10.0, -3.0);

    vplate = pow(10.0, -2.0); //プレートの速度
    v = pow(10.0, -1.0);      //特徴的な速度
    d = pow(10.0, -6.0);      //バネの自然長

    // 無次元化されたばね定数
    Lx = 2000.0;
    Ly = 0.2;

    Time = 0.0;
    T = 100.0;
    maxt = 250.0;
    dt = pow(10.0, -6.0);
    M = 0;
    zero = pow(10.0, -10.0);

    for (s = 0; s < N; s++)
    {
        if (s < N / 2)
        {
            a[s] = ax;
            L[s] = Lx;
        }
        else
        {
            a[s] = ay;
            L[s] = Ly;
        }
    }
    for (int i = 0.0; i < N; i++)
    {
        printf("%f  %f\n", a[i], L[i]);
    }
    printf("\n");
}