#include <stdio.h>
#include <math.h>
//下で定義する関数の宣言
double f(double v);
double g(double x);
//与えられた条件をグローバルスコープで定義
double m = 1.0;
double k = 2.0;
int main()
{
    double t, x, v, dt, tmax;              //計算で使用する変数の定義
    double k1, k2, k3, k4, l1, l2, l3, l4; //ルンゲクッタ法で使用するf,gの式を表す変数
    double fact_x, fact_v;                 //理論値
    double x_diff, v_diff;                 //理論値との差
    double d_err;                          //出力する誤差の値
    tmax = 5.0;                            //秒数の最大値
    FILE *output_message;
    output_message = fopen("output_message.txt", "w");
    //t=5 における理論値
    double sq = sqrt(k / m);
    fact_x = cos(sq * 5);
    fact_v = -sq * sin(sq * 5);

    for (dt = pow(10, -2); dt < 1; dt *= pow(10, 1))
    {
        //初期条件
        x = 1.0;
        v = 0.0;
        for (t = 0; t < tmax; t += dt)
        {
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
        //x,vそれぞれ理論値と実効値の差を計算
        x_diff = x - fact_x;
        v_diff = v - fact_v;
        //出力する誤差を計算
        d_err = sqrt(x_diff * x_diff + v_diff * v_diff);
        fprintf(output_message, "%.15f\t%.15f\t%.15f\n", dt, d_err, t);
    }
    fclose(output_message);
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