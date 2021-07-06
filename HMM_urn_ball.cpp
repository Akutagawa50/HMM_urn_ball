#include "HMM_urn_ball.h"

/*---------------Urn----------------*/
//ツボのコンストラクタ
Urn::Urn()
{
    ball_kind = 2;                           //ballの種類を設定
    ball.assign(ball_kind, 1.0 / ball_kind); //ball_numだけボールを用意して等確率にする
    loop = 0.5;
    trans = 0.5;
}

//ツボのデストラクタ
Urn::~Urn() {}

//ツボの中のボールを設定する関数
bool Urn::SetBallKind(int kind) //ボールの種類を返す関数
{
    if (kind <= 1)
        return false;
    ball_kind = kind;
    ball.assign(ball_kind, 1.0 / ball_kind);
    return true;
}

/*---------------HMM_urn_ball----------------*/
//HMMのコンストラクタ
HMM_urn_ball::HMM_urn_ball(int urn_n, int ball_n) : urn(urn_n)
{
    urn_num = urn_n;
    ball_num = ball_n;
    //urn_num = 3;
    //ball_num = 2;
    //ball.assign(ball_kind, 1.0 / ball_kind);
    for (int i = 0; i < urn_num; i++)
        urn[i].SetBallKind(ball_num);
}
//HMMのデストラクタ
HMM_urn_ball::~HMM_urn_ball() {}

//各ボールの確率を2次元配列で返す関数
std::vector<std::vector<double>> HMM_urn_ball::GetBallMatrix()
{
    std::vector<std::vector<double>> ball_matrix(urn_num, std::vector<double>(ball_num, 0.0));
    for (int i = 0; i < urn_num; i++)
    {
        for (int j = 0; j < ball_num; j++)
        {
            ball_matrix[i][j] = urn[i].ball[j];
        }
    }
    return ball_matrix;
}

//ツボの数を返す関数
int HMM_urn_ball::GetUrnNum()
{
    return urn_num;
}

//ボールの数を返す関数
int HMM_urn_ball::GetBallNum()
{
    return ball_num;
}

//学習させるパターンを設定する関数
bool HMM_urn_ball::SetLearnPattern(std::vector<int> pattern, int cl_max, double err = 0.00001)
{

    error = err;       //誤差を更新
    calc_max = cl_max; //計算回数を更新

    //patternの数値の範囲が 0 ~ ball_num であるかを確認
    if (*std::max_element(pattern.begin(), pattern.end()) >= ball_num ||
        *std::min_element(pattern.begin(), pattern.end()) < 0)
        return false;

    learn_pattern = pattern;

    return true;
}

bool HMM_urn_ball::StartLearning()
{
}

float HMM_urn_ball::GetProbability(std::vector<int> pattern)
{
    //patternの数値の範囲が 0 ~ ball_num であるかを確認
    if (*std::max_element(pattern.begin(), pattern.end()) >= ball_num ||
        *std::min_element(pattern.begin(), pattern.end()) < 0)
        return -1.0;

    // 7/5 評価のアルゴリズムを考える
    for (int i = 0; i < pattern.size(); i++)
    {
        //トレリス　縦 pattern-(urn-1)=y 横 urn=x
        //通り方の総数　(x+y-2)! / {(x-1)!(y-1)!}
        for (int j = 0; j < ball_num; j++)
        {
            /* code */
        }
    }
}
