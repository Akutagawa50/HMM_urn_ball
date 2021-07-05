#include "Urn.h"

Urn::Urn()
{
    ball_kind = 2;                           //ballの種類を設定
    ball.assign(ball_kind, 1.0 / ball_kind); //ball_numだけボールを用意して等確率にする
    loop = 0.5;
    trans = 0.5;
}

Urn::~Urn() {}

bool Urn::SetBallKind(int kind) //ボールの種類を返す関数
{
    if (kind <= 1)
        return false;
    ball_kind = kind;
    ball.assign(ball_kind, 1.0 / ball_kind);
    return true;
}
