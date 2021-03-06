#ifndef HMM_URN_BALL
#define HMM_URN_BALL

#include <vector>
#include <algorithm> //配列の要素の最大値(max_element)と最小値(min_element)を求めるのに使う
#include <numeric>   //配列の要素の総和を求める(accumulate)のに使う

//壺　各ボールの出る確率と遷移確率を格納するクラス
class Urn
{
public:
    Urn();
    ~Urn();

    bool SetBallKind(int n);  //ボールの種類を設定する関数(n：設定するボールの数(int型))
    int GetBallKind();        //ボールの種類の数を返す関数
    std::vector<double> ball; //各ボールが出る確率
    double loop;              //自己ループの確率
    double trans;             //次の壺に遷移する確率

private:
    int ball_kind;
};

//壺とボールのシミュレーションをHMMで扱うクラス
class HMM_urn_ball
{
public:
    HMM_urn_ball(int urn_n, int ball_n); //urn_n：壺の数，ball_n：ボールの数
    ~HMM_urn_ball();
    std::vector<std::vector<double>> GetBallMatrix();  //ボールの出現確率を配列化して返す
    std::vector<std::vector<double>> GetTransMatrix(); //各壺の遷移確率を配列化して返す
    bool SetUrnNum(int n);                             //壺の数を設定する関数
    bool SetBallNum(int n);                            //ボールの種類を設定する関数
    bool SetUrnBallNum(int u, int b);                  //壺とボールの数を変更する関数
    int GetUrnNum();                                   //壺の数を返す
    int GetBallNum();                                  //ボールの種類を返す
    bool HMMLearning(std::vector<int> data,
                     int cl_max = 100,
                     double err = 0.001); //学習する関数 (data：学習するデータ(int型vector), cl_max：最大計算回数(int型)，error：収束判定の誤差(double型))
    double HMMMeasure(std::vector<int> data);  //データの評価を行う関数(data：評価するデータ(int型vector))

private:
    int factorial(int x);                                   //xの階乗を返す関数
    std::vector<Urn> urn;                                   //壺とボール
    std::vector<std::vector<std::vector<int>>> ball_count;  //出てきたボールの種類を壺ごとに分けて格納
    std::vector<std::vector<std::vector<int>>> trans_count; //遷移回数を保存
    std::vector<int> trans_buff;                            //遷移方向を格納
    std::vector<double> prob_buff;                          //各パスの確率を格納する
    std::vector<int> learn_data;                            //学習するデータを格納

    int urn_num;     //壺の数
    int ball_num;    //ボールの種類
    int path_num;    //パスの総数
    double prob_sum; //各データの
};

#endif
