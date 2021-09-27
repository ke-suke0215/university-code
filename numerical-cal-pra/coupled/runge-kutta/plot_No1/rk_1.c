#include <stdio.h>
#include <math.h>

//与えられた条件をグローバルスコープで定義
double m = 1.0;
double k = 2.0;

double f(double v), g(double x);

double k1(double v);
double l1(double v);
double k2(double v, double l1, double dt);
double l2(double x, double k1, double dt);
double k3(double v, double l2, double dt);
double l3(double x, double k2, double dt);
double k4(double v, double l3, double dt);
double l4(double x, double k3, double dt);

double next_cul_part(double func1, double func2, double func3, double func4);

int main(void)
{
    double t;
    double dt = 0.01;
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
        //k,l を定義した配列に格納
        k_arr[0][0] = k1(v1);
        k_arr[1][0] = k1(v2);
        l_arr[0][0] = l1(x1);
        l_arr[1][0] = l1(x2);

        k_arr[0][1] = k2(v1, l_arr[0][0], dt);
        k_arr[1][1] = k2(v2, l_arr[1][0], dt);
        l_arr[0][1] = l2(x1, k_arr[0][0], dt);
        l_arr[1][1] = l2(x2, k_arr[1][0], dt);

        k_arr[0][2] = k3(v1, l_arr[0][1], dt);
        k_arr[1][2] = k3(v2, l_arr[1][1], dt);
        l_arr[0][2] = l3(x1, k_arr[0][1], dt);
        l_arr[1][2] = l3(x2, k_arr[1][1], dt);

        k_arr[0][3] = k4(v1, l_arr[0][2], dt);
        k_arr[1][3] = k4(v2, l_arr[1][2], dt);
        l_arr[0][3] = l4(x1, k_arr[0][2], dt);
        l_arr[1][3] = l4(x2, k_arr[1][2], dt);

        //次のx,vを計算
        x1 = x1 + next_cul_part(k_arr[0][0], k_arr[0][1], k_arr[0][2], k_arr[0][3]) * dt;
        x2 = x2 + next_cul_part(k_arr[1][0], k_arr[1][1], k_arr[1][2], k_arr[1][3]) * dt;

        v1 = v1 + (2.0 * next_cul_part(l_arr[0][0], l_arr[0][1], l_arr[0][2], l_arr[0][3]) - next_cul_part(l_arr[1][0], l_arr[1][1], l_arr[1][2], l_arr[1][3])) * dt;
        v2 = v2 + (2.0 * next_cul_part(l_arr[1][0], l_arr[1][1], l_arr[1][2], l_arr[1][3]) - next_cul_part(l_arr[0][0], l_arr[0][1], l_arr[0][2], l_arr[0][3])) * dt;

        //値を出力
        fprintf(output_rk_1x, "%f\t%f\n", t, x1);
        fprintf(output_rk_2x, "%f\t%f\n", t, x2);
        fprintf(output_rk_1v, "%f\t%f\n", t, v1);
        fprintf(output_rk_2v, "%f\t%f\n", t, v2);
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

//k1~k4,l1~l4を表す関数を定義
double k1(double v)
{
    return f(v);
}

double l1(double x)
{
    return g(x);
}

double k2(double v, double l1, double dt)
{
    return f(v + l1 * dt / 2.0);
}

double l2(double x, double k1, double dt)
{
    return g(x + k1 * dt / 2.0);
}

double k3(double v, double l2, double dt)
{
    return f(v + l2 * dt / 2.0);
}

double l3(double x, double k2, double dt)
{
    return g(x + k2 * dt / 2.0);
}

double k4(double v, double l3, double dt)
{
    return f(v + l3 * dt);
}

double l4(double x, double k3, double dt)
{
    return g(x + k3 * dt);
}

//次のx,vを決める関数のパーツ（ (k1 + 2*k2 + 2k3 + k4)/6 の部分）を定義
double next_cul_part(double func1, double func2, double func3, double func4)
{
    return ((func1 + 2.0 * func2 + 2.0 * func3 + func4) / 6);
}