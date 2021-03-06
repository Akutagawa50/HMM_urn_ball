#include "HMM_urn_ball.h"

/*---------------Urn----------------*/
//壺のコンストラクタ
Urn::Urn()
{
    ball_kind = 2;                           //ballの種類を設定
    ball.assign(ball_kind, 1.0 / ball_kind); //ball_numだけボールを用意して等確率にする
    loop = 0.5;
    //loop = (double)(rand() % 100) / 100.0;
    trans = 1.0 - loop;
}

//壺のデストラクタ
Urn::~Urn() {}

//壺の中のボールを設定する関数
bool Urn::SetBallKind(int n)
{
    if (n <= 1)
        return false;
    ball_kind = n;
    ball.assign(ball_kind, 1.0 / ball_kind);
    return true;
}

//ボールの種類を返す関数
int Urn::GetBallKind()
{
    return ball_kind;
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

std::vector<std::vector<double>> HMM_urn_ball::GetTransMatrix()
{
    std::vector<std::vector<double>> trans_matrix(urn_num, std::vector<double>(2, 0.0));
    for (int i = 0; i < urn_num; i++)
    {
        trans_matrix[i][0] = urn[i].loop;
        trans_matrix[i][1] = urn[i].trans;
    }
    return trans_matrix;
}

//壺の数を設定する関数
bool HMM_urn_ball::SetUrnNum(int n)
{
    //nが1以下だったらエラー
    if (n <= 1)
        return false;

    //ball_numも初期化されるので格納
    int ball_kind = ball_num;

    //壺の数を変更
    urn = std::vector<Urn>(n);

    //ボールの種類をball_numに戻す
    for (int i = 0; i < n; i++)
        urn[i].SetBallKind(ball_kind);

    //壺とボールの数を更新
    urn_num = n;
    ball_num = ball_kind;
    return true;
}

//ボールの数を設定する関数
bool HMM_urn_ball::SetBallNum(int n)
{
    //ボールの数が1以下だとエラー
    if (n <= 1)
        return false;

    //ボールの数を変更，遷移確率を初期化
    for (int i = 0; i < urn_num; i++)
    {
        urn[i].SetBallKind(n);
        urn[i].loop = 0.5;
        urn[i].trans = 0.5;
    }

    //ボールの数を更新
    ball_num = n;
    return true;
}

//壺とボールの数を設定する関数
bool HMM_urn_ball::SetUrnBallNum(int u, int b)
{
    //uかbが1以下だったらエラー
    if (u <= 1 || b <= 1)
        return false;

    //ball_numも初期化されるので格納
    int ball_kind = ball_num;

    //壺の数を変更
    urn = std::vector<Urn>(u);

    //ボールの種類をball_numに戻す
    for (int i = 0; i < u; i++)
        urn[i].SetBallKind(b);

    //壺とボールの数を更新
    urn_num = u;
    ball_num = b;
    return true;
}

//壺の数を返す関数
int HMM_urn_ball::GetUrnNum()
{
    return urn_num;
}

//ボールの数を返す関数
int HMM_urn_ball::GetBallNum()
{
    return ball_num;
}

//学習させるデータを設定する関数
bool HMM_urn_ball::HMMLearning(std::vector<int> data, int calc_max, double error)
{
    double prob = 0.0;     //計算結果を格納
    double pre_prob = 1.0; //前の計算結果を格納

    prob = HMMMeasure(data);

    //dataが条件に合わなかったらエラーを返す
    if (prob <= 0.0)
        return false;

    //パラメータ更新
    //各パスのボールが出る確立を格納
    std::vector<std::vector<std::vector<double>>> ball_prob =
        std::vector<std::vector<std::vector<double>>>(path_num, std::vector<std::vector<double>>(urn_num, std::vector<double>(ball_num, 0.0)));

    //各パスの遷移確率を格納
    std::vector<std::vector<std::vector<double>>> trans_prob =
        std::vector<std::vector<std::vector<double>>>(path_num, std::vector<std::vector<double>>(urn_num, std::vector<double>(2, 0)));

    for (int calc = 0; calc < calc_max; calc++)
    {
        for (int path = 0; path < path_num; path++)
        {
            //壺ごとのパラメータを更新
            for (int j = 0; j < urn_num; j++)
            {
                //ボールの出る確率を計算

                //ボールの総数を計算
                int ball_sum = std::accumulate(ball_count[path][j].begin(), ball_count[path][j].end(), 0);
                ball_sum = ball_sum == 0 ? 1 : ball_sum;
                for (int b = 0; b < ball_num; b++)
                {
                    ball_prob[path][j][b] = (double)ball_count[path][j][b] / ball_sum;
                }

                //遷移確率を計算
                int trans_sum = trans_count[path][j][0] + trans_count[path][j][1];
                trans_prob[path][j][0] = (double)trans_count[path][j][0] / trans_sum;
                trans_prob[path][j][1] = (double)trans_count[path][j][1] / trans_sum;
            }
        }

        //壺ごとのパラメータを統合
        for (int j = 0; j < urn_num; j++)
        {
            for (int b = 0; b < ball_num; b++)
            {
                //最終的なボールが出る確率を格納
                double ball_para = 0.0;

                for (int path = 0; path < path_num; path++)
                {
                    ball_para += ball_prob[path][j][b] * prob_buff[path];
                    //std:: << "ball para is " << ball_para << std::endl;
                }
                //ボールのパラメータを更新
                urn[j].ball[b] = ball_para;
            }

            //最終的なループ確率を格納
            double loop_para = 0.0;
            //最終的な遷移確率を格納
            double trans_para = 0.0;

            for (int path = 0; path < path_num; path++)
            { //遷移確率を計算

                loop_para += trans_prob[path][j][0] * prob_buff[path];
                trans_para += trans_prob[path][j][1] * prob_buff[path];
            }
            //パラメータを更新
            urn[j].loop = loop_para;
            urn[j].trans = trans_para;
        }

        //確率を正規化
        for (int j = 0; j < urn_num; j++)
        {
            //ボールが出る確率を正規化
            double ball_prob_sum = std::accumulate(urn[j].ball.begin(), urn[j].ball.end(), 0.0);
            ball_prob_sum = ball_prob_sum == 0.0 ? 1.0 : ball_prob_sum;
            for (int b = 0; b < ball_num; b++)
            {
                urn[j].ball[b] /= ball_prob_sum;
            }

            //遷移確率を正規化
            double trans_prob_sum = urn[j].loop + urn[j].trans;
            urn[j].loop /= trans_prob_sum;
            urn[j].trans /= trans_prob_sum;
        }

        //評価
        pre_prob = prob;
        prob = HMMMeasure(data);

        //収束判定
        double def_prob = prob - pre_prob;
        if (def_prob < error && def_prob > -error)
        {
            return true;
        }
    }
    return true;
}

double HMM_urn_ball::HMMMeasure(std::vector<int> data)
{
    //dataの数値の範囲が 0 ~ ball_num であるかを確認
    if (*std::max_element(data.begin(), data.end()) >= ball_num ||
        *std::min_element(data.begin(), data.end()) < 0)
        return -1.0;

    //データが壺より多いか確認
    if (data.size() < urn_num)
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

    //トレリスを生成（縦 data-(urn-1)=y 横 urn=x）
    int T = data.size() - urn_num + 1;
    int J = urn_num;

    //パスを生成
    //パスの総数を計算 (T+J-2)!/((T-1)!(J-1)!)
    path_num = factorial(T + J - 2) / (factorial(T - 1) * factorial(J - 1));
    trans_buff.assign(path_num, 0);

    //各パスの確率を格納する配列を動的確保 prob_buf[パスの総数]
    prob_buff.assign(path_num, 0.0);

    //各壺での遷移回数とループ回数を保存
    //prob_count[path_num][J][0] : 自己ループの回数
    //prob_count[path_num][J][1] : 次の壺へ遷移の回数
    trans_count = std::vector<std::vector<std::vector<int>>>(path_num, std::vector<std::vector<int>>(urn_num, std::vector<int>(2, 0)));

    //各パスで出てきたボールの数を格納 ball_count[パスの総数][壺の総数][ボールの種類]
    ball_count = std::vector<std::vector<std::vector<int>>>(path_num, std::vector<std::vector<int>>(urn_num, std::vector<int>(ball_num, 0)));

    int path = 0;
    int x = 1 << data.size(); //Endに遷移するので最上位は1
    //確率の合計を初期化
    prob_sum = 0;

    //パスが全部出るまでループ
    while (path < path_num)
    {
        int count = 0;

        //(data.size - 1) bit 分見る
        for (int b = 0; b < data.size() - 1; b++)
        {
            //b+1 bit目が1だったらcount++(これを遷移とする)
            if (x & (1 << b))
                count++;
        }
        if (count == J - 1)
        { //xのbitの1の数が(J-1)になったらパスを保存してpath++
            trans_buff[path] = x;

            int t = 0;
            int j = 0;
            double p;
            //最初のボールの確率を格納
            prob_buff[path] = urn[0].ball[data[0]];
            //出てきたボールをカウント
            ball_count[path][0][data[0]]++;

            for (int b = 1; b < data.size(); b++)
            {
                //遷移かループかを確認
                if (x & (1 << (b - 1)))
                { //次の壺に遷移する場合，ついでにjをプラスする
                    trans_count[path][j][1]++;
                    p = urn[j].trans * urn[++j].ball[data[b]];
                    prob_buff[path] *= p;
                }
                else
                { //自己ループの場合
                    trans_count[path][j][0]++;
                    p = urn[j].loop * urn[j].ball[data[b]];
                    prob_buff[path] *= p;
                }
                //ボールをカウント
                ball_count[path][j][data[b]]++;
            }

            //最終状態に遷移
            prob_buff[path] *= urn[J - 1].trans;
            trans_count[path][J - 1][1]++;

            //計算した確率を総和に足す
            prob_sum += prob_buff[path];
            path++;
        }
        x++;
    }
    return prob_sum;
}

//xの階乗を返す関数
int HMM_urn_ball::factorial(int x)
{
    return x <= 1 ? 1 : x * factorial(x - 1);
}