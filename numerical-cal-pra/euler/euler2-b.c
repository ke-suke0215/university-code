#include <stdio.h>
#include <math.h>

int main(void)
{
    double fact_x, fact_v; //理論値

    double t;
    double dt;
    double x;
    double x_next;
    double v;
    double v_next;
    double x_diff;
    double v_diff;
    double d_err;
    int m = 1, k = 2;

    //初期条件
    x = 1.0;
    v = 0.0;

    fact_x = cos(sqrt(k / m) * 5);
    fact_v = -sqrt(k / m) * sin(sqrt(k / m) * 5);

    for (dt = 0.000001; dt < 1; dt *= 10)
    {
        //初期条件
        x = 1.0;
        v = 0.0;

        for (t = 0; t <= 5; t = t + dt)
        {
            //次のx,vを計算
            x_next = v * dt + x;
            v_next = v - k / m * x * dt;

            //次のx,vをそれぞれ代入
            x = x_next;
            v = v_next;
        }

        x_diff = fabs(x - fact_x);
        v_diff = fabs(v - fact_v);

        d_err = sqrt(x_diff * x_diff + v_diff * v_diff);

        printf("%f\t%f\n", dt, d_err);
    }

    return 0;
}