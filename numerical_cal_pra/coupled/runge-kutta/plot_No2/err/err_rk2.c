#include <stdio.h>
#include <math.h>

//与えられた条件をグローバルスコープで定義
double m = 1.0;
double k = 2.0;

double f(double v), g(double x);

int main(void)
{
    double t;
    double dt = 0.01;
    double tmax = 5.0;
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

    FILE *output_err_rk2;
    output_err_rk2 = fopen("output_err_rk2.txt", "w");

    for (dt = pow(10, -7); dt <= 1; dt *= 10)
    {
        //初期条件
        x1 = 0.0;
        v1 = 0.0;

        x2 = 1.0;
        v2 = 0.0;

        for (t = 0; t < tmax; t = t + dt)
        {
            k_arr[0][0] = f(v1);          //おもり1、k1
            k_arr[1][0] = f(v2);          //おもり2、k1
            l_arr[0][0] = g(2 * x1 - x2); //おもり1、l1
            l_arr[1][0] = g(2 * x2 - x1); //おもり2、l1

            k_arr[0][1] = f(v1 + l_arr[0][0] * dt / 2.0);          //おもり1、k2
            k_arr[1][1] = f(v2 + l_arr[1][0] * dt / 2.0);          //おもり2、k2
            l_arr[0][1] = g(2 * x1 - x2 + k_arr[0][0] * dt / 2.0); //おもり1、k2
            l_arr[1][1] = g(2 * x2 - x1 + k_arr[0][0] * dt / 2.0); //おもり2、l2

            k_arr[0][2] = f(v1 + l_arr[0][1] * dt / 2.0);          //おもり1、k3
            k_arr[1][2] = f(v2 + l_arr[1][1] * dt / 2.0);          //おもり2、k3
            l_arr[0][2] = g(2 * x1 - x2 + k_arr[0][1] * dt / 2.0); //おもり1、l3
            l_arr[1][2] = g(2 * x2 - x1 + k_arr[1][1] * dt / 2.0); //おもり2、l3

            k_arr[0][3] = f(v1 + l_arr[0][2] * dt);          //おもり1、k4
            k_arr[1][3] = f(v2 + l_arr[1][2] * dt);          //おもり2、k4
            l_arr[0][3] = g(2 * x1 - x2 + k_arr[0][2] * dt); //おもり1、l4
            l_arr[1][3] = g(2 * x2 - x1 + k_arr[1][2] * dt); //おもり2、l4

            //次のxを計算
            x1 = x1 + (k_arr[0][0] + 2 * k_arr[0][1] + 2 * k_arr[0][2] + k_arr[0][3]) * dt / 6;
            x2 = x2 + (k_arr[1][0] + 2 * k_arr[1][1] + 2 * k_arr[1][2] + k_arr[1][3]) * dt / 6;

            //次のvを計算
            v1 = v1 + (l_arr[0][0] + 2 * l_arr[0][1] + 2 * l_arr[0][2] + l_arr[0][3]) * dt / 6;
            v2 = v2 + (l_arr[1][0] + 2 * l_arr[1][1] + 2 * l_arr[1][2] + l_arr[1][3]) * dt / 6;
        }

        x1_diff = fabs(x1 - fact_x1);
        x2_diff = fabs(x2 - fact_x2);
        v1_diff = fabs(v1 - fact_v1);
        v2_diff = fabs(v2 - fact_v2);

        d_err = sqrt(pow(x1_diff, 2) + pow(x2_diff, 2) + pow(v1_diff, 2) + pow(v2_diff, 2));

        fprintf(output_err_rk2, "%.15f\t%.15f\n", dt, d_err);
    }
    fclose(output_err_rk2);

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