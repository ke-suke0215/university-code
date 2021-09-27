#include <stdio.h>
#include <math.h>

double exp2(double num);

int main(void)
{
    double fact_x1, fact_x2, fact_v1, fact_v2; //理論値

    double t;                //時間
    double dt;               //時間の刻み
    double x1, x2;           //現在のx
    double x1_next, x2_next; //次のx
    double v1, v2;           //現在のv
    double v1_next, v2_next; //次のv
    double x1_diff, x2_diff;
    double v1_diff, v2_diff;
    double d_err;
    int m = 1, k = 2;

    FILE *output_err_euler;
    output_err_euler = fopen("output_err_euler.txt", "w");

    //計算で使用する定数定義
    double omga = sqrt(k / m);
    double omgb = sqrt(3 * k / m);

    fact_x1 = (cos(omga * 5) - cos(omgb * 5)) / 2;
    fact_x2 = (cos(omga * 5) + cos(omgb * 5)) / 2;
    fact_v1 = -(omga * sin(omga * 5) - omgb * sin(omgb * 5)) / 2;
    fact_v2 = -(omga * sin(omga * 5) + omgb * sin(omgb * 5)) / 2;

    for (dt = pow(10, -7); dt < 1; dt *= 10)
    {
        //初期条件
        x1 = 0.0;
        v1 = 0.0;

        x2 = 1.0;
        v2 = 0.0;

        for (t = 0; t <= 5; t = t + dt)
        {
            //次のx,vを計算
            x1_next = v1 * dt + x1;
            v1_next = v1 - k / m * (2 * x1 - x2) * dt;

            x2_next = v2 * dt + x2;
            v2_next = v2 - k / m * (2 * x2 - x1) * dt;

            //次のx,vをそれぞれ代入
            x1 = x1_next;
            v1 = v1_next;

            x2 = x2_next;
            v2 = v2_next;
        }

        x1_diff = fabs(x1 - fact_x1);
        x2_diff = fabs(x2 - fact_x2);
        v1_diff = fabs(v1 - fact_v1);
        v2_diff = fabs(v2 - fact_v2);

        d_err = sqrt(exp2(x1_diff) + exp2(x2_diff) + exp2(v1_diff) + exp2(v2_diff));

        fprintf(output_err_euler, "%.15f\t%.15f\n", dt, d_err);
    }
    fclose(output_err_euler);

    return 0;
}

double exp2(double num)
{
    return num * num;
}