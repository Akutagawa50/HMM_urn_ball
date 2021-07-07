#include "HMM_urn_ball.h"

/*---------------Urn----------------*/
//ツボのコンストラクタ
Urn::Urn()
{
    ball_kind = 2;                           //ballの種類を設定
    ball.assign(ball_kind, 1.0 / ball_kind); //ball_numだけボールを用意して等確率にする
    loop = 0.5;
    //loop = (double)(rand() % 100) / 100.0;
    trans = 1.0 - loop;
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
bool HMM_urn_ball::HMMLearning(std::vector<int> pattern, int calc_max, double error)
{
    double prob = 0.0;     //計算結果を格納
    double pre_prob = 1.0; //前の計算結果を格納

    learn_pattern = pattern; //パターンを保存

    prob = HMMMeasure(learn_pattern);

    //patternが条件に合わなかったらエラーを返す
    if (prob == -1.0)
        return false;

    for (int i = 0; i < calc_max; i++)
    {
        //パラメータ更新
        for (int j = 0; j < urn_num; j++)
        {

            //j番目のツボのボールの個数を合計
            int ball_sum = std::accumulate(ball_count[j].begin(), ball_count[j].end(), 0.0);

            //j番目のツボ遷移回数を合計
            int trans_sum = trans_count[j][0] + trans_count[j][1];

            //各ボールが出る確率を計算
            for (int b = 0; b < ball_num; b++)
            {
                //DEBUG urn[j].ball[b] - (double)ball_count[j][b] / ball_sum ENDL;
                urn[j].ball[b] = (double)ball_count[j][b] / ball_sum;
            }
            //遷移の確率を更新
            urn[j].trans = (double)trans_count[j][0] / trans_sum;

            //自己ループの確率を更新
            urn[j].loop = (double)trans_count[j][1] / trans_sum;
        }

        //評価
        pre_prob = prob;
        prob = HMMMeasure(learn_pattern);

        //収束判定
        double def_prob;
        def_prob = prob - pre_prob;

        if (def_prob < error && def_prob > -error)
        {
            return true;
        }
    }
    return true;
}

double HMM_urn_ball::HMMMeasure(std::vector<int> pattern)
{
    urn[0].ball[0] = 0.5;
    urn[0].ball[1] = 0.5;
    urn[0].trans = 0.5;
    urn[0].loop = 0.5;

    urn[1].ball[0] = 0.0;
    urn[1].ball[1] = 1.0;
    urn[1].trans = 0.5;
    urn[1].loop = 0.5;

    //patternの数値の範囲が 0 ~ ball_num であるかを確認
    if (*std::max_element(pattern.begin(), pattern.end()) >= ball_num ||
        *std::min_element(pattern.begin(), pattern.end()) < 0)
        return -1.0;

    //パターンがツボより多いか確認
    if (pattern.size() < urn_num)
        return -1.0;

    /*------------------イメージ--------------------//
                    　E
                     /
    J   /-----------/
       / / / / / / /
      / / / / / / /
     / / / / / / /
    /-----------/
    0           T
   /---------------------------------------------- */

    //トレリスを生成（縦 pattern-(urn-1)=y 横 urn=x）
    int T = pattern.size() - urn_num + 1;
    int J = urn_num;

    //各マスの確率を格納する配列を動的確保
    prob_buff.assign(T, std::vector<double>(J, 0.0));

    //各マスがどこから遷移したかを格納, 自己ループ：true, 遷移：false
    trans_buff.assign(T, std::vector<bool>(J, false));

    //出てきたボールの数を格納
    ball_count.assign(J, std::vector<int>(ball_num, 0));

    //遷移か自己ループした回数を格納
    //遷移trans_count[j][0],自己ループtrans_count[j][1]
    trans_count.assign(J, std::vector<int>(2, 0));

    //Viterbiアルゴリズムで終端までの確率を求める
    for (int t = 0; t < T; t++)
    {
        for (int j = 0; j < J; j++)
        {
            if (t == 0 && j == 0)
            { //0番目のツボからpattern[0]番のボールが出る確率を代入
                prob_buff[0][0] = urn[0].ball[pattern[0]];
            }
            else if (t == 0)
            {
                prob_buff[0][j] = prob_buff[0][j - 1] * urn[j - 1].trans * urn[j].ball[pattern[j]];
                //遷移なのでfalse
                //trans_buff[t][j] = false;
            }
            else if (j == 0)
            {
                prob_buff[t][0] = prob_buff[t - 1][0] * urn[0].loop * urn[0].ball[pattern[t]];
                //自己ループなのでtrue
                trans_buff[t][0] = true;
            }
            else
            {

                //それぞれの方向から来た時の確率を計算
                //自己ループの場合
                double p1 = prob_buff[t - 1][j] * urn[j].loop * urn[j].ball[pattern[t + j]];
                //遷移の場合
                double p2 = prob_buff[t][j - 1] * urn[j - 1].trans * urn[j].ball[pattern[t + j]];

                if (p1 > p2) //自己ループ優先
                {            //自己ループ
                    prob_buff[t][j] = p1;
                    trans_buff[t][j] = true;
                }
                else
                { //遷移
                    prob_buff[t][j] = p2;
                    //trans_buff[t][j]=false;
                }
            }
        }
    }

    //最終的な確率を計算
    //配列の要素の関係でTとJを１減らす
    T--;
    J--;
    double prob = prob_buff[T][J] * urn[J].trans;

    //出てきたボールの数と遷移回数を計算
    for (int i = pattern.size() - 1; i > 0; i--)
    {
        ball_count[J][pattern[i]]++;
        if (trans_buff[T][J])
        { //自己ループだったらTを減らす
            trans_count[J][1]++;
            T--;
        }
        else
        { //遷移していたらJを減らす
            trans_count[J][0]++;
            J--;
        }
    }
    return prob;
}
