#include <stdio.h>
#include <math.h>

//与えられた条件をグローバルスコープで定義
double m = 1.0;
double k = 2.0;

double f(double v), g(double x);

int main(void)
{
    double t;
    double dt = 0.2;
    double tmax = 5.0;
    double x1, x2; //現在のx
    double v1, v2; //現在のv
    double omg = k / m;

    // k,lを格納するための配列
    double k_arr[2][4];
    double l_arr[2][4];

    FILE *output_rk_1x;
    output_rk_1x = fopen("output_rk_1x.txt", "w");
    FILE *output_rk_2x;
    output_rk_2x = fopen("output_rk_2x.txt", "w");

    FILE *output_rk_1v;
    output_rk_1v = fopen("output_rk_1v.txt", "w");
    FILE *output_rk_2v;
    output_rk_2v = fopen("output_rk_2v.txt", "w");

    //初期条件
    x1 = 0.0;
    v1 = 0.0;

    x2 = 1.0;
    v2 = 0.0;

    for (t = 0; t <= tmax; t = t + dt)
    {
        //k_1を計算
        k_arr[0][0] = f(v1);
        k_arr[1][0] = f(v2);
        //l_1を計算
        l_arr[0][0] = g(2 * x1 - x2);
        l_arr[1][0] = g(2 * x2 - x1);

        //k_2を計算
        k_arr[0][1] = f(v1 + l_arr[0][0] * dt / 2.0);
        k_arr[1][1] = f(v2 + l_arr[1][0] * dt / 2.0);
        //l_2を計算
        l_arr[0][1] = g(2 * x1 - x2 + k_arr[0][0] * dt / 2.0);
        l_arr[1][1] = g(2 * x2 - x1 + k_arr[0][0] * dt / 2.0);

        //k_3を計算
        k_arr[0][2] = f(v1 + l_arr[0][1] * dt / 2.0);
        k_arr[1][2] = f(v2 + l_arr[1][1] * dt / 2.0);
        //l_3を計算
        l_arr[0][2] = g(2 * x1 - x2 + k_arr[0][1] * dt / 2.0);
        l_arr[1][2] = g(2 * x2 - x1 + k_arr[1][1] * dt / 2.0);

        //k_4を計算
        k_arr[0][3] = f(v1 + l_arr[0][2] * dt);
        k_arr[1][3] = f(v2 + l_arr[1][2] * dt);
        //l_4を計算
        l_arr[0][3] = g(2 * x1 - x2 + k_arr[0][2] * dt);
        l_arr[1][3] = g(2 * x2 - x1 + k_arr[1][2] * dt);

        //次のxを計算
        x1 = x1 + (k_arr[0][0] + 2 * k_arr[0][1] + 2 * k_arr[0][2] + k_arr[0][3]) * dt / 6;
        x2 = x2 + (k_arr[1][0] + 2 * k_arr[1][1] + 2 * k_arr[1][2] + k_arr[1][3]) * dt / 6;

        v1 = v1 + (l_arr[0][0] + 2 * l_arr[0][1] + 2 * l_arr[0][2] + l_arr[0][3]) * dt / 6;
        v2 = v2 + (l_arr[1][0] + 2 * l_arr[1][1] + 2 * l_arr[1][2] + l_arr[1][3]) * dt / 6;

        //値を出力
        fprintf(output_rk_1x, "%f\t%f\n", t, x1);
        fprintf(output_rk_2x, "%f\t%f\n", t, x2);
        fprintf(output_rk_1v, "%f\t%f\n", t, v1);
        fprintf(output_rk_2v, "%f\t%f\n", t, v2);

        printf("%f\t%f\t%f\t%f\n", k_arr[0][0], k_arr[0][1], k_arr[0][2], k_arr[0][3]);
    }

    fclose(output_rk_1x);
    fclose(output_rk_2x);
    fclose(output_rk_1v);
    fclose(output_rk_2v);

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