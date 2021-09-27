#include <stdio.h>
#include <math.h>

//与えられた条件をグローバルスコープで定義
double m = 1.0;
double k = 2.0;

//f,gを表す関数を宣言
double f(double v), g(double x);

//k,lを表す関数を宣言
double k1(double v);
double l1(double v);
double k2(double v, double l1, double dt);
double l2(double x, double k1, double dt);
double k3(double v, double l2, double dt);
double l3(double x, double k2, double dt);
double k4(double v, double l3, double dt);
double l4(double x, double k3, double dt);

//次のx,vを計算するのに使用する式の一部を関数として宣言
double next_cul_part(double func1, double func2, double func3, double func4);

int main(void)
{
    double t;
    double tmax = 5.0;
    double dt;     //時間の刻み
    double x1, x2; //現在のx
    double v1, v2; //現在のv
    double omg = k / m;

    double fact_x1, fact_x2, fact_v1, fact_v2; //理論値

    //それぞれの差の絶対値を入れるための変数
    double x1_diff, x2_diff;
    double v1_diff, v2_diff;

    //最終的に出力する値
    double d_err;

    //計算で使用する定数定義
    double omga = sqrt(k / m);
    double omgb = sqrt(3 * k / m);

    //理論値の計算
    fact_x1 = (cos(omga * 5) - cos(omgb * 5)) / 2;
    fact_x2 = (cos(omga * 5) + cos(omgb * 5)) / 2;
    fact_v1 = -(omga * sin(omga * 5) - omgb * sin(omgb * 5)) / 2;
    fact_v2 = -(omga * sin(omga * 5) + omgb * sin(omgb * 5)) / 2;

    // k,lを格納するための配列
    double k_arr[2][4];
    double l_arr[2][4];

    FILE *output_err_rk;
    output_err_rk = fopen("output_err_rk.txt", "w");

    for (dt = pow(10, -7); dt <= 1; dt *= 10)
    {
        //初期条件
        x1 = 0.0;
        v1 = 0.0;

        x2 = 1.0;
        v2 = 0.0;

        int count = 1;

        for (t = 0; t < tmax; t = dt * count)
        {
            count += 1;

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
        }

        x1_diff = fabs(x1 - fact_x1);
        x2_diff = fabs(x2 - fact_x2);
        v1_diff = fabs(v1 - fact_v1);
        v2_diff = fabs(v2 - fact_v2);

        d_err = sqrt(pow(x1_diff, 2) + pow(x2_diff, 2) + pow(v1_diff, 2) + pow(v2_diff, 2));

        fprintf(output_err_rk, "%.15f\t%.15f\n", dt, d_err);
    }
    fclose(output_err_rk);

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