#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// グローバル変数を定義
double param_a_general, param_a_slow, param_b, param_c, plate_v, special_v, d, l_general, l_slow, time_measure, t_start, t_max, dt, zero;

int main()
{
  int cal_count, s; // sはfor文で使用する変数

  ///////////////////////////////////
  /// 定義したグローバル変数に値を代入 ///
  ///////////////////////////////////

  param_a_general = pow(10.0, -5.0); // 通常の地震の摩擦パラメータa
  param_a_slow = pow(10.0, -1.0);    // ゆっくり地震の摩擦パラメータa
  param_b = pow(10.0, -3.0);         // 摩擦パラメータb
  param_c = pow(10.0, -3.0);         // 摩擦パラメータc

  plate_v = pow(10.0, -1.0);   // プレートの速度
  special_v = pow(10.0, -1.0); // 特徴的な速度
  d = pow(10.0, -6.0);         //バネの自然長

  l_general = 2000.0; // 通常の地震での無次元化されたばね定数
  l_slow = 0.2;       // ゆっくり地震での無次元化されたばね定数

  time_measure = 0.0;      // 時間計測用変数
  t_start = 100.0;         // 計測開始時間
  t_max = 250.0;           // 計測終了時間
  dt = pow(10.0, -6.0);    // 時間の刻み幅
  cal_count = 0;           // 計算回数のカウント
  zero = pow(10.0, -10.0); // プレートが逆に滑るのを防ぐための値

  ////////// 値の代入終了 ////////////
}

//////////////////////////////////////////////
/// ルンゲクッタ法で使用する方程式を返す関数を定義 ///
//////////////////////////////////////////////

// dxの方程式を返す関数を定義
double f(double v)
{
  return v;
}

// dv(左のブロック)の方程式を返す関数を定義
double g_left()
{
}

// dv(左右以外のブロック)の方程式を返す関数を定義
double g_inside()
{
}

// dv(右のブロック)の方程式を返す関数を定義
double g_right()
{
}

// dθの方程式を返す関数を定義
double h()
{
}