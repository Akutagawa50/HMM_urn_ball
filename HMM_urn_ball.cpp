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

    /*------------------イメージ--------------------//
                     /
    J   /-----------/
       / / / / / / /
      / / / / / / /
     / / / / / / /
    /-----------/
                T
   /---------------------------------------------- */

    //トレリスを生成（縦 pattern-(urn-1)=y 横 urn=x）
    int T = pattern.size() - urn_num - 1;

    //各マスの確率を格納する配列を動的確保
    prob_buff.assign(T, std::vector<double>(urn_num, 0.0));

    //各マスがどこから遷移したかを格納, 自己ループ：true, 遷移：false
    trans_buff.assign(T, std::vector<bool>(urn_num, false));

    //出てきたボールの数を格納
    ball_count.assign(urn_num, std::vector<int>(ball_num, 0));

    for (int t = 0; t < T; t++)
    {
        //Viterbiアルゴリズムを使う
        for (int j = 0; j < urn_num; j++)
        {
            if (t == 0 && j == 0)
            { //0番目のツボからpattern[0]番のボールが出る確率を代入
                prob_buff[0][0] = urn[0].ball[pattern[0]];
            }
            else if (t == 0)
            {
                prob_buff[0][j] *= prob_buff[0][j - 1];
                //遷移なのでfalse
                //trans_buff[t][j] = false;
            }
            else if (j = 0)
            {
                prob_buff[t][0] *= prob_buff[t - 1][0];
                //自己ループなのでtrue
                trans_buff[t][j] = true;
            }
            else
            {
                //それぞれの方向から来た時の確率を計算
                //自己ループの場合
                double p1 = prob_buff[t - 1][j] * urn[j].ball[pattern[t + j]];
                //遷移の場合
                double p2 = prob_buff[t][j - 1] * urn[j].ball[pattern[t + j]];

                if (p1 > p2)
                {
                    prob_buff[t][j] = p1;
                    trans_buff[t][j] = true;
                }
                else
                {
                    prob_buff[t][j] = p2;
                    //trans_buff[t][j]=false;
                }
            }
        }
    }
}
