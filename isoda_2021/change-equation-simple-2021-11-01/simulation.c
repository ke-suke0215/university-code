#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// グローバル変数を定義
int N = 30; // ブロック数
double param_a_general, param_a_slow, param_b_general, param_b_slow, param_c, plate_v, special_v, d, l_general, l_slow, Time, t_start, t_max, dt, zero, random_num;

// main外に記述する関数を定義
double f(double v);
double determine_friction_direction(double others, double friction, double v);
double g_left(double t, double x_i, double x_ip1, double v, double theta, double param_a, double param_b, double l);
double g_inside(double t, double x_im1, double x_i, double x_ip1, double v, double theta, double param_a, double param_b, double l);
double g_right(double t, double x_im1, double x_i, double v, double theta, double param_a, double param_b, double l);
double h(double v, double theta);

int main()
{
  // cal_count:ループの回数を格納する変数、s:for文で使用する変数
  int cal_count, s;

  //ブロックの初期位置、位置、速度、状態変数の長さNの配列
  double x[N], v[N], theta[N];

  // ルンゲクッタ法で使用するk,l,mの値を格納する N×4 の配列
  double rk_k[N][4], rk_l[N][4], rk_m[N][4];

  // 通常の地震とゆっくり地震を判断するためのパラメータを格納する配列
  double param_a[N], param_b[N], l[N];

  // 摩擦出力に使用する配列
  double friction_array[N];

  ///////////////////////////
  /// 定義した変数に値を代入 ///
  ///////////////////////////

  param_a_general = pow(10.0, -5.0); // 通常の地震の摩擦パラメータa
  param_a_slow = pow(10.0, -1.0);    // ゆっくり地震の摩擦パラメータa
  param_b_general = pow(10.0, -3.0); // 通常の地震の摩擦パラメータb
  param_b_slow = pow(10.0, -3.0);    // ゆっくり地震の摩擦パラメータb
  param_c = pow(10.0, -3.0);         // 摩擦パラメータc

  plate_v = pow(10.0, -2.0);   // プレートの速度
  special_v = pow(10.0, -1.0); // 特徴的な速度
  d = 0.5;                     //バネの自然長

  l_general = 2000.0; // 通常の地震での無次元化されたばね定数
  l_slow = 0.2;       // ゆっくり地震での無次元化されたばね定数

  Time = 0.0;              // 時間計測用変数
  t_start = 100.0;         // 計測開始時間
  t_max = 105.0;           // 計測終了時間
  dt = pow(10.0, -5);      // 時間の刻み幅
  cal_count = 0;           // 計算回数のカウント
  zero = pow(10.0, -10.0); // プレートが逆に滑るのを防ぐための値

  // 各ブロックに対応する値をN回ループを回して代入
  for (s = 0; s < N; s++)
  {
    /////// 通常の地震とゆっくり地震が起こるパラメータを代入 ///////

    // if (s > 10 && s < 19)
    // {
    //   param_a[s] = param_a_slow;
    //   l[s] = l_slow;
    // }
    // else
    // {
    //   param_a[s] = param_a_general;
    //   l[s] = l_general;
    // }

    // 通常の地震とゆっくり地震半分ずつ
    if (s < N / 2)
    {
      param_a[s] = param_a_general;
      param_b[s] = param_b_general;
      l[s] = l_general;
    }
    else if (s >= N / 2)
    {
      param_a[s] = param_a_slow;
      param_b[s] = param_b_slow;
      l[s] = l_slow;
    }
    else
    {
      printf("不適切な値で計算が行われています。");
      return 0;
    }

    // 全てゆっくり地震のパターン
    // param_a[s] = param_a_slow;
    // param_b[s] = param_b_slow;
    // l[s] = l_slow;

    //全て通常の地震のパターン
    // param_a[s] = param_a_general;
    // param_b[s] = param_a_general;
    // l[s] = l_general;

    //////// x_initial, x, v, θ に値を代入 ////////

    v[s] = pow(10.0, -4.0);
    theta[s] = pow(10.0, 3.0);

    // x_initial と x は1つ目のブロックは0、それ以降は0~1の乱数を用いて値を代入
    random_num = (double)rand() / (double)RAND_MAX;
    x[s] = random_num;
  }

  ////////// 値の代入終了 ////////////

  // データを記述するファイルの作成

  FILE *OUTPUTFILE1;
  FILE *OUTPUTFILE2;
  FILE *OUTPUTFILE3;
  FILE *OUTPUTFILE4;

  OUTPUTFILE1 = fopen("output/time_100-105/v.txt", "w");
  OUTPUTFILE2 = fopen("output/time_100-105/x.txt", "w");
  OUTPUTFILE3 = fopen("output/time_100-105/theta.txt", "w");
  OUTPUTFILE4 = fopen("output/time_100-105/friction.txt", "w");

  //////////////////////////////////
  ///// ブロックを動かすループ開始 /////
  //////////////////////////////////
  for (Time = t_start; Time < t_max; Time += dt)
  {
    cal_count++; // ループの回数を記録
    // printf("%f\t", Time);

    ////////////////////////////////////////////////////////////////////
    ////// 各ブロックのルンゲクッタ法の計算で使用する k,l,m の1~4に値を代入 /////
    ////////////////////////////////////////////////////////////////////

    /////////////////////////////
    ///// k1, l1, m1 を代入 //////
    /////////////////////////////

    // k -> dx, l -> dv, m -> dθ にそれぞれ対応している
    for (s = 0; s < N; s++) // k1を代入
    {
      rk_k[s][0] = dt * f(v[s]);
    }
    for (s = 0; s < N; s++) // l1を代入
    {
      if (s == 0) // 左のブロック
      {
        rk_l[s][0] = dt * g_left(Time, x[s], x[s + 1], v[s], theta[s], param_a[s], param_b[s], l[s]);
      }
      else if (s == N - 1) // 右のブロック
      {
        rk_l[s][0] = dt * g_right(Time, x[s - 1], x[s], v[s], theta[s], param_a[s], param_b[s], l[s]);
      }
      else // 左右以外のブロック
      {
        rk_l[s][0] = dt * g_inside(Time, x[s - 1], x[s], x[s + 1], v[s], theta[s], param_a[s], param_b[s], l[s]);
      }
    }
    for (s = 0; s < N; s++) // m1を代入
    {
      rk_m[s][0] = dt * h(v[s], theta[s]);
    }

    ///// k1, l1, m1 代入終了 //////

    /////////////////////////////
    ///// k2, l2, m2 を代入 //////
    /////////////////////////////

    for (s = 0; s < N; s++) // k2を代入
    {
      rk_k[s][1] = dt * f(v[s] + 0.5 * rk_l[s][0]);
    }
    for (s = 0; s < N; s++) // l2を代入
    {
      if (s == 0) // 左のブロック
      {
        rk_l[s][1] = dt * g_left(Time + 0.5 * dt, x[s] + 0.5 * rk_k[s][0], x[s + 1] + 0.5 * rk_k[s + 1][0], v[s] + 0.5 * rk_l[s][0], theta[s] + 0.5 * rk_m[s][0], param_a[s], param_b[s], l[s]);
      }
      else if (s == N - 1) // 右のブロック
      {
        rk_l[s][1] = dt * g_right(Time + 0.5 * dt, x[s - 1] + 0.5 * rk_k[s - 1][0], x[s] + 0.5 * rk_k[s][0], v[s] + 0.5 * rk_l[s][0], theta[s] + 0.5 * rk_m[s][0], param_a[s], param_b[s], l[s]);
      }
      else // 左右以外のブロック
      {
        rk_l[s][1] = dt * g_inside(Time + 0.5 * dt, x[s - 1] + 0.5 * rk_k[s - 1][0], x[s] + 0.5 * rk_k[s][0], x[s + 1] + 0.5 * rk_k[s + 1][0], v[s] + 0.5 * rk_l[s][0], theta[s] + 0.5 * rk_m[s][0], param_a[s], param_b[s], l[s]);
      }
    }
    for (s = 0; s < N; s++) // m2を代入
    {
      rk_m[s][1] = dt * h(v[s] + 0.5 * rk_l[s][0], theta[s] + 0.5 * rk_m[s][0]);
    }

    ///// k2, l2, m2 代入終了 //////

    /////////////////////////////
    ///// k3, l3, m3 を代入 //////
    /////////////////////////////

    for (s = 0; s < N; s++) // k3を代入
    {
      rk_k[s][2] = dt * f(v[s] + 0.5 * rk_l[s][1]);
    }
    for (s = 0; s < N; s++) // l3を代入
    {
      if (s == 0) // 左のブロック
      {
        rk_l[s][2] = dt * g_left(Time + 0.5 * dt, x[s] + 0.5 * rk_k[s][1], x[s + 1] + 0.5 * rk_k[s + 1][1], v[s] + 0.5 * rk_l[s][1], theta[s] + 0.5 * rk_m[s][1], param_a[s], param_b[s], l[s]);
      }
      else if (s == N - 1) // 右のブロック
      {
        rk_l[s][2] = dt * g_right(Time + 0.5 * dt, x[s - 1] + 0.5 * rk_k[s - 1][1], x[s] + 0.5 * rk_k[s][1], v[s] + 0.5 * rk_l[s][1], theta[s] + 0.5 * rk_m[s][1], param_a[s], param_b[s], l[s]);
      }
      else // 左右以外のブロック
      {
        rk_l[s][2] = dt * g_inside(Time + 0.5 * dt, x[s - 1] + 0.5 * rk_k[s - 1][1], x[s] + 0.5 * rk_k[s][1], x[s + 1] + 0.5 * rk_k[s + 1][1], v[s] + 0.5 * rk_l[s][1], theta[s] + 0.5 * rk_m[s][1], param_a[s], param_b[s], l[s]);
      }
    }
    for (s = 0; s < N; s++) // m3を代入
    {
      rk_m[s][2] = dt * h(v[s] + 0.5 * rk_l[s][1], theta[s] + 0.5 * rk_m[s][1]);
    }

    ///// k3, l3, m3 代入終了 //////

    /////////////////////////////
    ///// k4, l4, m4 を代入 //////
    /////////////////////////////

    for (s = 0; s < N; s++) // k4を代入
    {
      rk_k[s][3] = dt * f(v[s] + rk_l[s][2]);
    }
    for (s = 0; s < N; s++) // l4を代入
    {
      if (s == 0) // 左のブロック
      {
        rk_l[s][3] = dt * g_left(Time + dt, x[s] + rk_k[s][2], x[s + 1] + rk_k[s + 1][2], v[s] + rk_l[s][2], theta[s] + rk_m[s][2], param_a[s], param_b[s], l[s]);
      }
      else if (s == N - 1) // 右のブロック
      {
        rk_l[s][3] = dt * g_right(Time + dt, x[s - 1] + rk_k[s - 1][2], x[s] + rk_k[s][2], v[s] + rk_l[s][2], theta[s] + rk_m[s][2], param_a[s], param_b[s], l[s]);
      }
      else // 左右以外のブロック
      {
        rk_l[s][3] = dt * g_inside(Time + dt, x[s - 1] + rk_k[s - 1][2], x[s] + rk_k[s][2], x[s + 1] + rk_k[s + 1][2], v[s] + rk_l[s][2], theta[s] + rk_m[s][2], param_a[s], param_b[s], l[s]);
      }
    }
    for (s = 0; s < N; s++) // m4を代入
    {
      rk_m[s][3] = dt * h(v[s] + 0.5 * rk_l[s][1], theta[s] + 0.5 * rk_m[s][1]);
    }

    ///// k4, l4, m4 代入終了 //////

    ///////////////////////////////////////
    ///// k,l,m を用いて次の x,v,θ を計算/////
    ///////////////////////////////////////

    for (s = 0; s < N; s++)
    {
      friction_array[s] = param_c + param_a[s] * log(1 + v[s] / special_v) + param_b[s] * log(theta[s]);

      x[s] = x[s] + (1.0 / 6.0) * (rk_k[s][0] + 2.0 * rk_k[s][1] + 2.0 * rk_k[s][2] + rk_k[s][3]);
      v[s] = v[s] + (1.0 / 6.0) * (rk_l[s][0] + 2.0 * rk_l[s][1] + 2.0 * rk_l[s][2] + rk_l[s][3]);
      theta[s] = theta[s] + (1.0 / 6.0) * (rk_m[s][0] + 2.0 * rk_m[s][1] + 2.0 * rk_m[s][2] + rk_m[s][3]);

      if (cal_count % 1000 == 0)
      {
        fprintf(OUTPUTFILE1, "%d\t%25.22lf\t%25.22lf\n", s, Time + dt, v[s]);
        fprintf(OUTPUTFILE2, "%d\t%25.22lf\t%25.22lf\n", s, Time + dt, x[s]);
        fprintf(OUTPUTFILE3, "%d\t%25.22lf\t%25.22lf\n", s, Time + dt, theta[s]);
        fprintf(OUTPUTFILE4, "%d\t%25.22lf\t%25.22lf\n", s, Time + dt, friction_array[s]);
      }

      // 逆方向へ滑るのを防ぐため
      if (v[s] < zero)
      {
        v[s] = 0.0;
      }
    }
    if (cal_count % 100 == 0)
    {
      // fprintf(OUTPUTFILE4, "%25.22lf\t%25.22lf\t%25.22lf\n", v[10], Time + dt, friction_array[10]);
    }

    ///// 計算終了 /////

    if (t_max < Time)
    {
      break;
    }
  }

  // ファイルへの記述を終了
  fclose(OUTPUTFILE1);
  fclose(OUTPUTFILE2);
  fclose(OUTPUTFILE3);
  fclose(OUTPUTFILE4);
}

//////////////////////////////////////////////
/// ルンゲクッタ法で使用する方程式を返す関数を記述 ///
//////////////////////////////////////////////

// dxの方程式を返す関数
double
f(double v)
{
  return v;
}

// 3つのdvの方程式の関数で使用する、摩擦の進行方向に対して摩擦の有無と向きを決める関数
// 引数は運動方程式の摩擦以外の部分（others）、運動方程式の摩擦部分（friction）、ブロックの速度(v)
double determine_friction_direction(double others, double friction, double v)
{
  if (v == 0)
  {
    if (others - friction > 0)
    {
      return (others - friction);
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return others - friction * v / fabs(v);
  }
}

// dv(左のブロック)の方程式を返す関数
double g_left(double t, double x_i, double x_ip1, double v, double theta, double param_a, double param_b, double l)
{
  // 運動方程式の摩擦以外の部分を定義 （vによって摩擦の値が変更される場合があるため）
  double other_than_friction = plate_v * t - x_i + l * (x_ip1 - x_i - d);
  // 運動方程式の摩擦力の部分を定義
  double friction = param_c + param_a * log(1 + v / special_v) + param_b * log(theta);
  return determine_friction_direction(other_than_friction, friction, v);
}

// dv(左右以外のブロック)の方程式を返す関数
double g_inside(double t, double x_im1, double x_i, double x_ip1, double v, double theta, double param_a, double param_b, double l)
{
  double other_than_friction = plate_v * t - x_i + l * (x_ip1 - 2.0 * x_i + x_im1);
  double friction = param_c + param_a * log(1 + v / special_v) + param_b * log(theta);
  return determine_friction_direction(other_than_friction, friction, v);
}

// dv(右のブロック)の方程式を返す関数
double g_right(double t, double x_im1, double x_i, double v, double theta, double param_a, double param_b, double l)
{
  double other_than_friction = plate_v * t - x_i + l * (x_im1 - x_i + d);
  double friction = param_c + param_a * log(1 + v / special_v) + param_b * log(theta);
  return determine_friction_direction(other_than_friction, friction, v);
}

// dθの方程式を返す関数
double h(double v, double theta)
{
  return 1.0 - (v * theta);
}