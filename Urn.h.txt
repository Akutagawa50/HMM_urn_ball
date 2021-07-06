#ifndef URN
#define URN

#include <vector>

class Urn
{
public:
    Urn();
    ~Urn();

    bool SetBallKind(int kind); //ボールの種類を設定する関数

    std::vector<double> ball; //各ボールが出る確率
    double loop;              //自己ループの確率
    double trans;             //次のツボに遷移する確率

private:
    int ball_kind;
};

#endif