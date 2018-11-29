//
// Created by deng on 18-11-28.
//

#ifndef DOUDIZHU_ENDGAME_DDZ_HAND_H
#define DOUDIZHU_ENDGAME_DDZ_HAND_H

#include <unordered_map>

#include "cardset.h"

namespace doudizhu_endgame {

enum Type {
    Pass,               //不出
    Single,             //单牌
    Pair,               //对子
    Triple,             //三个不带
    Bomb,               //炸弹
    Triple_single,      //三带一
    Triple_pair,        //三带二
    Bomb_single,        //四带两个单
    Bomb_pair,          //四带两对
    Straight_single,    //顺子
    Straight_pair,      //连对
    Plane,              //飞机（不带）
    Plane_single,       //飞机带单牌
    Plane_pair,         //飞机带对子
    Rocket              //王炸（火箭）
};

struct Pattern {
    int8_t  power{};
    Type    type{};
    CardSet hand;
};

class PatternPool {
public:

    PatternPool();
    ~PatternPool();

    size_t size();
    Pattern *get(int8_t power, Type type, const CardSet &hand);

private:
    std::unordered_map<uint64_t , Pattern *> pool_;
};


class DouDiZhuHand {
public:

    DouDiZhuHand() = default;

    ~DouDiZhuHand() = default;

    void set_hand(const CardSet& hand)
    {
        hand_ = hand;
    }

    void set_last(const Pattern* last)
    {
        last_ = last;
    }

    void set_play_hand(const CardSet& toplay)
    {
        play_hand_ = toplay;
    }

    //
    void play(CardSet &res);

    void next_hand(std::vector<Pattern *> &next_moves);

private:

    CardSet             hand_;
    CardSet             play_hand_;
    const Pattern*      last_{};
    PatternPool         pattern_pool_;

    void combination(CardSet hand, CardSet used, size_t comb_len, int8_t size,
                     std::vector<std::vector<int8_t>> &combinations);

    void create_straight(const CardSet &hand, int8_t min_len, int8_t size,
                         std::vector<std::vector<int8_t >> &straights);

    void get_rocket(std::vector<Pattern *> &next_moves);

    void get_pass(std::vector<Pattern *> &next_moves);

    void get_single(std::vector<Pattern *> &next_moves);

    void get_pair(std::vector<Pattern *> &next_moves);

    void get_triple(std::vector<Pattern *> &next_moves);

    void get_triple_single(std::vector<Pattern *> &next_moves);

    void get_triple_pair(std::vector<Pattern *> &next_moves);

    void get_bomb(std::vector<Pattern *> &next_moves);

    void get_bomb_single(std::vector<Pattern *> &next_moves);

    void get_bomb_pair(std::vector<Pattern *> &next_moves);

    void get_straight_single(std::vector<Pattern *> &next_moves);

    void get_straight_pair(std::vector<Pattern *> &next_moves);

    void get_plane(std::vector<Pattern *> &next_moves);

    void get_plane_single(std::vector<Pattern *> &next_moves);

    void get_plane_pair(std::vector<Pattern *> &next_moves);
};

}   //namespace doudizhu_endgame

#endif //DOUDIZHU_ENDGAME_DDZ_HAND_H
