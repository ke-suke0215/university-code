#include <stdio.h>
#include <math.h>

double f(double v);
double g(double x);

double m = 1.0;
double k = 2.0;

int main()
{
    double t, x, v, dt, tmax;
    double k1, k2, k3, k4, l1, l2, l3, l4; //ルンゲクッタ法で使用するf,gの式を表す変数
    double fact_x, fact_v;                 //理論値
    double x_diff, v_diff;                 //理論値との差
    double d_err;                          //△E

    FILE *
        output_test;
    output_test = fopen("output_test.txt", "w");

    //t=5 における理論値
    fact_x = cos(sqrt(k / m) * 5);
    fact_v = -sqrt(k / m) * sin(sqrt(k / m) * 5);

    tmax = 5.0; //秒数の最大値

    for (dt = pow(10, -7); dt <= 1; dt *= 10)
    {
        //初期条件
        x = 1.0;
        v = 0.0;

        int count = 1;

        for (t = 0; t <= tmax; t = dt * count)
        {
            count += 1;

            k1 = f(v);
            l1 = g(x);

            k2 = f(v + l1 * dt / 2.0);
            l2 = g(x + k1 * dt / 2.0);

            k3 = f(v + l2 * dt / 2.0);
            l3 = g(x + k2 * dt / 2.0);

            k4 = f(v + l3 * dt);
            l4 = g(x + k3 * dt);

            x = x + (k1 + 2.0 * k2 + 2.0 * k3 + k4) * dt / 6.0;
            v = v + (l1 + 2.0 * l2 + 2.0 * l3 + l4) * dt / 6.0;
        }

        printf("%d\t", count);

        x_diff = fabs(x - fact_x);
        v_diff = fabs(v - fact_v);

        d_err = sqrt(x_diff * x_diff + v_diff * v_diff);

        fprintf(output_test, "%.15f\t%.15f\t%.15f\n", dt, d_err, t);
    }

    fclose(output_test);

    return 0;
}

//fの関数を定義
double f(double v)
{
    return v;
}

//gの関数を定義
double g(double x)
{
    return (-k / m * x);
}