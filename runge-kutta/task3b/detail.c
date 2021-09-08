#include <stdio.h>
#include <math.h>

double f1(double t, double x, double v);
double f2(double t, double x, double v);

double m = 1.0;
double k = 2.0;

int main()
{
    double t, x, v, dt, tmax;
    double k1[2], k2[2], k3[2], k4[2]; //ルンゲクッタの計算で使用するk,lを表す配列
    double fact_x, fact_v;             //理論値
    double x_diff, v_diff;             //理論値との差
    double d_err;

    FILE *output_detail;
    output_detail = fopen("output_detail.txt", "w");

    //t=5 における理論値
    fact_x = cos(sqrt(k / m) * 5);
    fact_v = -sqrt(k / m) * sin(sqrt(k / m) * 5);

    tmax = 5.0; //秒数の最大値

    for (dt = pow(10, -10); dt < 1; dt *= 1.1)
    {
        //初期条件
        x = 1.0;
        v = 0.0;

        for (t = 0; t < tmax; t += dt)
        {
            k1[0] = dt * f1(t, x, v);
            k1[1] = dt * f2(t, x, v);

            k2[0] = dt * f1(t + dt / 2.0, x + k1[0] / 2.0, v + k1[1] / 2.0);
            k2[1] = dt * f2(t + dt / 2.0, x + k1[0] / 2.0, v + k1[1] / 2.0);

            k3[0] = dt * f1(t + dt / 2.0, x + k2[0] / 2.0, v + k2[1] / 2.0);
            k3[1] = dt * f2(t + dt / 2.0, x + k2[0] / 2.0, v + k2[1] / 2.0);

            k4[0] = dt * f1(t + dt, x + k3[0], v + k3[1]);
            k4[1] = dt * f2(t + dt, x + k3[0], v + k3[1]);

            x = x + (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]) / 6.0;
            v = v + (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]) / 6.0;
        }

        x_diff = fabs(x - fact_x);
        v_diff = fabs(v - fact_v);

        d_err = sqrt(x_diff * x_diff + v_diff * v_diff);

        fprintf(output_detail, "%.15f\t%.15f\n", dt, d_err);
    }

    fclose(output_detail);

    return 0;
}

double f1(double t, double x, double v)
{
    return v;
}

double f2(double t, double x, double v)
{
    return (-k / m * x);
}