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

    void play(const CardSet& hand, Pattern* toplay, CardSet &res);

    void next_hand(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

private:

    PatternPool pattern_pool_;

    void combination(CardSet hand, CardSet used, size_t comb_len, int8_t size,
                     std::vector<std::vector<int8_t>> &combinations);

    void create_straight(const CardSet &hand, int8_t min_len, int8_t size,
                         std::vector<std::vector<int8_t >> &straights);

    void get_rocket(const CardSet& hand, std::vector<Pattern *> &next_moves);

    void get_pass(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_single(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_pair(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_triple(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_triple_single(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_triple_pair(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_bomb(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_bomb_single(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_bomb_pair(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_straight_single(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_straight_pair(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_plane(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_plane_single(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);

    void get_plane_pair(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves);
};

}   //namespace doudizhu_endgame

#endif //DOUDIZHU_ENDGAME_DDZ_HAND_H
