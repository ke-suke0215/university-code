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
    double i, t, xold[N], vold[N], thetaold[N], xnew[N], vnew[N], thetanew[N], masatu[N], k[N][4], l[N][4], m[N][4], dt, maxt, Time, T, zero, a[N], L[N];

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

    for (s = 1; s < N; s++)
    {
        // ブロックの0~99番目を場合Ⅰ
        if (s < 100)
        {
            a[s] = ax;
            L[s] = Lx;
        }
        // ブロックの100~199番目までを場合Ⅱ
        else
        {
            a[s] = ay;
            L[s] = Ly;
        }
    }

    // 初期配置
    double xold0[N];
    xold0[0] = 0.0;

    for (s = 1; s < N; s++)
    {
        i = (double)rand() / (double)RAND_MAX;

        xold0[s] = i + xold0[s - 1];
    }
    xold[0] = 0.0;

    for (s = 1; s < N; s++)
    {
        xold[s] = xold0[s];
    }

    // 値の代入
    for (s = 0; s < N; s++)
    {
        vold[s] = pow(10.0, -4.0);
        thetaold[s] = pow(10.0, 3.0);
        xnew[s] = 0.0;
        vnew[s] = 0.0;
        thetanew[s] = 0.0;
    }

    FILE *OUTPUTFILE1;
    FILE *OUTPUTFILE2;
    FILE *OUTPUTFILE3;
    FILE *OUTPUTFILE4;

    OUTPUTFILE1 = fopen("region410x.txt", "w");
    OUTPUTFILE2 = fopen("region410y.txt", "w");
    OUTPUTFILE3 = fopen("region410f.txt", "w");
    OUTPUTFILE4 = fopen("region410t.txt", "w");

    for (Time = T; Time < maxt; Time += dt)
    {
        M++;

        // dx①
        k[0][0] = dt * f(vold[0]);
        for (s = 1; s < N - 1; s++)
        {
            k[s][0] = dt * f(vold[s]);
        }
        k[N - 1][0] = dt * f(vold[N - 1]);

        // dv①
        l[0][0] = dt * g1(Time, xold0[0], xold[1], xold[0], vold[0], thetaold[0], a[0], L[0]);
        for (s = 1; s < N - 1; s++)
        {
            l[s][0] = dt * g2(Time, xold0[s], xold[s + 1], xold[s], xold[s - 1], vold[s], thetaold[s], a[s], L[s]);
        }
        l[N - 1][0] = dt * g3(Time, xold0[N - 1], xold[N - 1], xold[N - 2], vold[N - 1], thetaold[N - 1], a[N - 1], L[N - 1]);

        //dtheta①
        m[0][0] = dt * h(vold[0], thetaold[0]);
        for (s = 1; s < N; s++)
        {
        }
    }

    //一旦書くのやめた

    fclose(OUTPUTFILE1);
    fclose(OUTPUTFILE2);
    fclose(OUTPUTFILE3);
    fclose(OUTPUTFILE4);
}

// dx
double f(double v1)
{
    return v1;
}

// dv
double g1(double t, double x0, double x2, double x1, double v1, double theta1, double a, double L)
{
    if (v1 == 0)
    {
        if (vplate * t - (x1 - x0) + L * (x2 - x1 - d) - (c + a * log(1.0 + (v1 / v)) + b * log(theta1)) > 0)
        {
            return vplate * t - (x1 - x0) + L * (x2 - x1 - d) - (c + a * log(1.0 + (v1 / v)) + b * log(theta1));
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return vplate * t - (x1 - x0) + L * (x2 - x1 - d) - (c + a * log(1.0 + (v1 / v)) + b * log(theta1)) * v1 / fabs(v1);
    }
}

double g2(double t, double x0, double x3, double x2, double x1, double v2, double theta2, double a, double L)
{
}

double g3(double t, double x0, double x2, double x1, double v2, double theta2, double a, double L)
{
}

double h(double v1, double theta1)
{
}