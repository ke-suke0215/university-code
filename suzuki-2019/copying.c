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
        for (s = 1; s < N - 1; s++)
        {
            m[s][0] = dt * h(vold[s], thetaold[s]);
        }
        m[N - 1][0] = dt * h(vold[N - 1], thetaold[N - 1]);

        // dx②
        k[0][1] = dt * f(vold[0] + 0.5 * l[0][0]);
        for (s = 1; s < N - 1; s++)
        {
            k[s][1] = dt * f(vold[s] + 0.5 * l[s][0]);
        }
        k[N - 1][1] = dt * f(vold[N - 1] + 0.5 * l[N - 1][0]);

        // dv②
        l[0][1] = dt * g1(Time + 0.5 * dt, xold0[0], xold[1] * 0.5 * k[1][0], xold[0] + 0.5 * k[0][0], vold[0] + 0.5 * l[0][0], thetaold[0] + 0.5 * m[0][0], a[0], L[0]);
        for (s = 1; s < N - 1; s++)
        {
            l[s][1] = dt * g2(Time + 0.5 * dt, xold0[s], xold[s + 1] * 0.5 * k[s + 1][0], xold[s] + 0.5 * k[s][0], xold[s - 1] + 0.5 * k[s - 1][0], vold[s] + 0.5 * l[s][0], thetaold[s] + 0.5 * m[s][0], a[s], L[s]);
        }
        l[N - 1][1] = dt * g3(Time + 0.5 * dt, xold0[N - 1], xold[N - 1] * 0.5 * k[N - 1][0], xold[N - 2] + 0.5 * k[N - 2][0], vold[N - 1] + 0.5 * l[N - 1][0], thetaold[N - 1] + 0.5 * m[N - 1][0], a[N - 1], L[N - 1]);

        //dtheta②
        m[0][1] = dt * h(vold[0] + 0.5 * l[0][0], thetaold[0] + 0.5 * m[0][0]);
        for (s = 1; s < N - 1; s++)
        {
            m[s][1] = dt * h(vold[s] + 0.5 * l[s][0], thetaold[s] + 0.5 * m[s][0]);
        }
        m[N - 1][1] = dt * h(vold[N - 1] + 0.5 * l[N - 1][0], thetaold[N - 1] + 0.5 * m[N - 1][0]);

        // dx③
        k[0][2] = dt * f(vold[0] + 0.5 * l[0][1]);
        for (s = 1; s < N - 1; s++)
        {
            k[s][2] = dt * f(vold[s] + 0.5 * l[s][1]);
        }
        k[N - 1][2] = dt * f(vold[N - 1] + 0.5 * l[N - 1][1]);

        // dv③
        l[0][2] = dt * g1(Time + 0.5 * dt, xold0[0], xold[1] * 0.5 * k[1][1], xold[0] + 0.5 * k[0][1], vold[0] + 0.5 * l[0][1], thetaold[0] + 0.5 * m[0][1], a[0], L[0]);
        for (s = 1; s < N - 1; s++)
        {
            l[s][2] = dt * g2(Time + 0.5 * dt, xold0[s], xold[s + 1] * 0.5 * k[s + 1][1], xold[s] + 0.5 * k[s][1], xold[s - 1] + 0.5 * k[s - 1][1], vold[s] + 0.5 * l[s][1], thetaold[s] + 0.5 * m[s][1], a[s], L[s]);
        }
        l[N - 1][2] = dt * g3(Time + 0.5 * dt, xold0[N - 1], xold[N - 1] * 0.5 * k[N - 1][1], xold[N - 2] + 0.5 * k[N - 2][1], vold[N - 1] + 0.5 * l[N - 1][1], thetaold[N - 1] + 0.5 * m[N - 1][1], a[N - 1], L[N - 1]);

        //dtheta③
        m[0][2] = dt * h(vold[0] + 0.5 * l[0][1], thetaold[0] + 0.5 * m[0][1]);
        for (s = 1; s < N - 1; s++)
        {
            m[s][2] = dt * h(vold[s] + 0.5 * l[s][1], thetaold[s] + 0.5 * m[s][1]);
        }
        m[N - 1][2] = dt * h(vold[N - 1] + 0.5 * l[N - 1][1], thetaold[N - 1] + 0.5 * m[N - 1][1]);

        // 左端のブロック（j=1）
        xnew[0] = xold[0] * (1.0 / 6.0) * (k[0][0] + 2.0 * k[0][1] + 2.0 * k[0][2] + k[0][3]);
        vnew[0] = vold[0] * (1.0 / 6.0) * (l[0][0] + 2.0 * l[0][1] + 2.0 * l[0][2] + l[0][3]);
        thetanew[0] = thetaold[0] * (1.0 / 6.0) * (m[0][0] + 2.0 * m[0][1] + 2.0 * m[0][2] + m[0][3]);

        // 逆方向に滑るのを防ぐため
        if (vnew[0] < zero)
        {
            vnew[0] = 0.0;
        }

        // 真ん中のブロック（j=2~4）
        for (s = 1; s < N - 1; s++)
        {
            xnew[s] = xold[s] * (1.0 / 6.0) * (k[s][0] + 2.0 * k[s][1] + 2.0 * k[s][2] + k[s][3]);
            vnew[s] = vold[s] * (1.0 / 6.0) * (l[s][0] + 2.0 * l[s][1] + 2.0 * l[s][2] + l[s][3]);
            thetanew[s] = thetaold[s] * (1.0 / 6.0) * (m[s][0] + 2.0 * m[s][1] + 2.0 * m[s][2] + m[s][3]);

            if (vnew[s] < zero)
            {
                vnew[s] = 0.0;
            }
        }

        // 右端のブロック（j=5）
        xnew[N - 1] = xold[N - 1] * (1.0 / 6.0) * (k[N - 1][0] + 2.0 * k[N - 1][1] + 2.0 * k[N - 1][2] + k[N - 1][3]);
        vnew[N - 1] = vold[N - 1] * (1.0 / 6.0) * (l[N - 1][0] + 2.0 * l[N - 1][1] + 2.0 * l[N - 1][2] + l[N - 1][3]);
        thetanew[N - 1] = thetaold[N - 1] * (1.0 / 6.0) * (m[N - 1][0] + 2.0 * m[N - 1][1] + 2.0 * m[N - 1][2] + m[N - 1][3]);

        if (vnew[N - 1] < zero)
        {
            vnew[N - 1] = 0.0;
        }

        // データを減らすため
        if (M % 100000 == 0)
        {
            // ファイルに出力
            fprintf(OUTPUTFILE3, "%25.22lf %d %25.22lf\n", Time + dt, s + 1, (vnew[99] - vold[99]) * pow(10.0, 6.0));
            for (s = 0; s <= N - 1; s++)
            {
                fprintf(OUTPUTFILE1, "%25.22lf %d %25.22lf\n", Time + dt, s + 1, xnew[s] - xold0[N - 1] * (s + 1) * 0.005);
                fprintf(OUTPUTFILE1, "%25.22lf %d %25.22lf\n", Time + dt, s + 1, vnew[s]);
                fprintf(OUTPUTFILE1, "%25.22lf %d %25.22lf\n", Time + dt, s + 1, thetanew[s]);
                printf("%25.22lf\n", Time); //計算機で計算中にどこまで計算が終わったか見るため
            }
        }

        for (s = 0; s <= N - 1; s++)
        {
            xold[s] = xnew[s];
            vold[s] = vnew[s];
            thetaold[s] = thetanew[s];
        }

        if (maxt < Time)
        {
            break;
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
// 左端のブロック
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

// 真ん中のブロック
double g2(double t, double x0, double x3, double x2, double x1, double v2, double theta2, double a, double L)
{
    if (v2 == 0)
    {
        if (vplate * t - (x2 - x0) + L * (x3 - 2.0 * x2 + x1) - (c + a * log(1.0 + (v2 / v)) + b * log(theta2)) > 0)
        {
            return vplate * t - (x2 - x0) + L * (x3 - 2.0 * x2 + x1) - (c + a * log(1.0 + (v2 / v)) + b * log(theta2));
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return vplate * t - (x2 - x0) + L * (x3 - 2.0 * x2 + x1) - (c + a * log(1.0 + (v2 / v)) + b * log(theta2)) * v2 / fabs(v2);
    }
}

// 右端のブロック
double g3(double t, double x0, double x2, double x1, double v2, double theta2, double a, double L)
{
    if (v2 == 0)
    {
        if (vplate * t - (x2 - x0) + L * (x1 - x2 + d) - (c + a * log(1.0 + (v2 / v)) + b * log(theta2)) > 0)
        {
            return vplate * t - (x2 - x0) + L * (x1 - x2 + d) - (c + a * log(1.0 + (v2 / v)) + b * log(theta2));
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return vplate * t - (x2 - x0) + L * (x1 - x2 + d) - (c + a * log(1.0 + (v2 / v)) + b * log(theta2)) * v2 / fabs(v2);
    }
}

double h(double v1, double theta1)
{
    return 1.0 - (v1 * theta1);
}