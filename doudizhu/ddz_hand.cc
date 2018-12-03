//
// Created by deng on 18-11-28.
//

#include <algorithm>

#include "ddz_hand.h"

namespace doudizhu_endgame {

PatternPool::PatternPool() = default;

PatternPool::~PatternPool()
{
    for (auto it : this->pool_) {
        delete it.second;
    }
}

size_t PatternPool::size()
{
    return this->pool_.size();
}

Pattern *PatternPool::get(int8_t power, Type type, const CardSet &hand)
{
    Pattern* pattern = nullptr;
    auto it = this->pool_.find(hand.to_ullong());
    if (it == this->pool_.end()) {
        pattern = new Pattern;
        *pattern = {power, type, hand};
        pool_.emplace(hand.to_ullong(), pattern);
        return pattern;

    } else {
        return it->second;
    }
}

void DouDiZhuHand::play(const CardSet& hand, Pattern* toplay,CardSet& res)
{
    size_t pos = toplay->hand.find_first();

    res = hand;
    while (pos < 64) {
        size_t i = ((pos >> 2) << 2) + 3;
        while (!res.test(i)) {
            i -= 1;
        }
        res.set(i, false);

        pos = toplay->hand.find_next(pos);
    }
}

void DouDiZhuHand::next_hand(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    //搜索的顺序对剪枝的发生影响很大,
    // 这里把 pass 作为最后考虑的选项
    next_moves.reserve(50);

    get_rocket(hand, next_moves);

    get_pair(hand, last, next_moves);

    get_single(hand, last, next_moves);

    get_triple_single(hand, last, next_moves);

    get_triple_pair(hand, last, next_moves);

    get_straight_single(hand, last, next_moves);

    get_straight_pair(hand, last, next_moves);

    get_triple(hand, last, next_moves);

    get_bomb(hand, last, next_moves);

    get_bomb_single(hand, last, next_moves);

    get_bomb_pair(hand, last, next_moves);

    get_plane(hand, last, next_moves);

    get_plane_single(hand, last, next_moves);

    get_plane_pair(hand, last, next_moves);

    get_pass(hand, last, next_moves);
}

void DouDiZhuHand::get_pass(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type != Pass) {
        CardSet res;
        next_moves.emplace_back(this->pattern_pool_.get(-1, Pass, res));

    } else {
        //can not pass twice
    }
}

void DouDiZhuHand::get_single( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass || last->type == Single) {
        for (int8_t i = 0; i < 15; ++i) {
            if (hand.is_single(i) && i > last->power) {
                CardSet res;
                res.set_single(i);
                next_moves.emplace_back(pattern_pool_.get(i, Single, res));
            }
        }

    } else {
        //can not move
    }
}

void DouDiZhuHand::get_pair( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass || last->type == Pair) {
        for (int8_t i = 0; i < 15; ++i) {
            if (hand.is_pair(i) && i > last->power) {

                CardSet res;
                res.set_pair(i);
                Pattern* tmp = pattern_pool_.get(i, Pair, res);
                next_moves.emplace_back(tmp);
            }
        }

    } else {
        //can not move
    }
}

void DouDiZhuHand::get_triple( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass || last->type == Triple) {
        for (int8_t i = 0; i < 15; ++i) {
            if (hand.is_trio(i) && i > last->power) {
                CardSet res;
                res.set_trio(i);
                Pattern* tmp = pattern_pool_.get(i, Triple, res);
                next_moves.emplace_back(tmp);
            }
        }

    } else {
        //can not move
    }
}

void DouDiZhuHand::get_triple_single(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass || last->type == Triple_single) {
        for (int8_t i = 0; i < 15; ++i) {
            if (hand.is_trio(i) && i > last->power) {

                for (int8_t j = 0; j < 15; ++j) {
                    if (hand.is_single(j) && j != i) {
                        CardSet res;
                        res.set_trio_single(i, j);
                        Pattern* tmp = pattern_pool_.get(i, Triple_single, res);
                        next_moves.emplace_back(tmp);
                    }
                }

            }
        }

    } else {
        //can not move;
    }
}

void DouDiZhuHand::get_triple_pair( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass || last->type == Triple_pair) {

        for (int8_t i = 0; i < 15; ++i) {
            if (hand.is_trio(i) && i > last->power) {

                for (int8_t j = 0; j < 15; ++j) {
                    if (hand.is_pair(j) && j != i) {
                        CardSet res;
                        res.set_trio_pair(i, j);
                        Pattern* tmp = pattern_pool_.get(i, Triple_pair, res);
                        next_moves.emplace_back(tmp);
                    }
                }

            }
        }

    } else {
        //can not move;
    }
}

void DouDiZhuHand::get_bomb( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    //powerx4 !!!!
    if (last->type != Rocket) {
        for (int8_t i = 0; i < 13; ++i) {
            if (hand.is_bomb(i) && (i << 2) > last->power) {

                CardSet res;
                res.set_bomb(i);

                Pattern* tmp = pattern_pool_.get((int8_t) (i << 2), Bomb, res);
                next_moves.emplace_back(tmp);
            }
        }

    } else {
        ////not move
    }
}

void DouDiZhuHand::combination(CardSet hand, CardSet used, size_t comb_len, int8_t size,
                        std::vector<std::vector<int8_t>> &combinations)
{
    hand.remove_taken(used);

    size_t n = 0;
    int8_t _hand[15];

    for (int8_t i = 0; i < 15; ++i) {
        if (hand.has_count(i, size)) {
            _hand[n++] = i;
        }
    }

    if (comb_len <= n) {
        std::vector<bool> v(n);
        std::fill(v.end() - comb_len, v.end(), true);

        do {
            std::vector<int8_t > tmp;
            for (size_t i = 0; i < v.size(); ++i) {
                if (v[i]) {
                    tmp.emplace_back(_hand[i]);
                }
            }
            combinations.emplace_back(tmp);
        } while (std::next_permutation(v.begin(), v.end()));

    } else {
        //too few cards
    }
}

void DouDiZhuHand::get_bomb_single( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass || last->type == Bomb_single) {
        for (int8_t i = 0; i < 13; ++i) {
            if (hand.is_bomb(i) && i > last->power) {
                CardSet res;
                res.set_bomb(i);

                std::vector<std::vector<int8_t > > comb;
                combination(hand, res, 2, 1, comb); //res->uesd

                for (std::vector<int8_t > &j : comb) {
                    res.set_single(j[0]);
                    res.set_single(j[1]);
                    Pattern* tmp = pattern_pool_.get(i, Bomb_single, res);
                    next_moves.emplace_back(tmp);

                    //reset
                    res.set_single(j[0], false);
                    res.set_single(j[1], false);
                }

                //bomb + one pair
                for (int8_t k = 0; k < 13; ++k) {
                    if (hand.is_pair(k) && k != i) {
                        res.set_pair(k);

                        Pattern* tmp = pattern_pool_.get(i, Bomb_single, res);
                        next_moves.emplace_back(tmp);

                        //reset
                        res.set_pair(k, false);
                    }
                }
            }
        }

    } else {
        ////not move
    }
}

void DouDiZhuHand::get_bomb_pair( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass || last->type == Bomb_pair) {
        for (int8_t i = 0; i < 13; ++i) {
            if (hand.is_bomb(i) && i > last->power) {
                Pattern* tmp;

                CardSet res;
                res.set_bomb(i);

                std::vector<std::vector<int8_t > > comb;
                combination(hand, res, 2, 2, comb); //res->used

                for (std::vector<int8_t > &j : comb) {
                    res.set_pair(j[0]);
                    res.set_pair(j[1]);

                    tmp = pattern_pool_.get(i, Bomb_pair, res);
                    next_moves.emplace_back(tmp);

                    //res reset
                    res.set_pair(j[0], false);
                    res.set_pair(j[1], false);
                }

            }
        }

    } else {
        ////not move
    }
}

void DouDiZhuHand::create_straight(const CardSet &hand, int8_t min_len, int8_t size,
                            std::vector<std::vector<int8_t >> &straights)
{
    straights.clear();
    /* 2 小王 大王 不考虑 */
    int8_t count = 0;
    int8_t set[12]{};
    for (int8_t i = 0; i < 12; ++i) {
        if (hand.has_count(i, size)) {
            set[count] = i;
            count += 1;
        }
    }

    if (count < min_len) {
        return;
    }

    for (int8_t start = 0; start < count; ++start) {
        for (int8_t end = start; end < count && set[end] - set[start] == end - start; ++end) {
            if (end - start >= min_len - 1) {
                std::vector<int8_t > tmp;
                for (int8_t i = set[start]; i <= set[end]; ++i) {
                    tmp.emplace_back(i);
                }
                straights.emplace_back(tmp);
            }
        }
    }
}

void DouDiZhuHand::get_straight_single(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass && hand.size() >= 5) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 5, 1, straights);

        for (std::vector<int8_t > &i : straights) {
            CardSet res;
            res.set_straight_s(i[0], i.back());
            Pattern* tmp = pattern_pool_.get(i[0], Straight_single, res);
            next_moves.emplace_back(tmp);
        }

    } else if (last->type == Straight_single && hand.size() >= 5) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 5, 1, straights);
        for (std::vector<int8_t > &i : straights) {
            if (i.size() == last->hand.size() && i[0] > last->power) {
                CardSet res;
                res.set_straight_s(i[0], i.back());
                Pattern* tmp = pattern_pool_.get(i[0], Straight_single, res);
                next_moves.emplace_back(tmp);
            }
        }

    } else {
        //not move
    }
}

void DouDiZhuHand::get_straight_pair(const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass && hand.size() >= 6) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 3, 2, straights);
        for (std::vector<int8_t > &i : straights) {
            CardSet res;
            res.set_straight_p(i[0], i.back());

            Pattern* tmp = pattern_pool_.get(i[0], Straight_pair, res);
            next_moves.emplace_back(tmp);
        }

    } else if (last->type == Straight_pair && hand.size() >= 6) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 3, 2, straights);
        for (std::vector<int8_t > &i : straights) {
            if (i.size() / 2 == last->hand.size() && i[0] > last->power) {
                CardSet res;
                res.set_straight_p(i[0], i.back());

                Pattern* tmp = pattern_pool_.get(i[0], Straight_pair, res);
                next_moves.emplace_back(tmp);
            }
        }

    } else {
        //not move
    }
}

void DouDiZhuHand::get_plane( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    if (last->type == Pass && hand.size() >= 6 ) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 2, 3, straights);

        for (std::vector<int8_t > &i : straights) {
            CardSet res;
            res.set_plane(i[0], i.back());

            Pattern* tmp = pattern_pool_.get(i[0], Plane, res);
            next_moves.emplace_back(tmp);
        }

    } else if (last->type == Plane && hand.size() >= 6) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 2, 3, straights);

        for (std::vector<int8_t > &i : straights) {
            if (i.size() == last->hand.size() / 3 && i[0] > last->power) {
                CardSet res;
                res.set_plane(i[0], i.back());

                Pattern* tmp = pattern_pool_.get(i[0], Plane, res);
                next_moves.emplace_back(tmp);
            }
        }

    } else {
        //not move
    }
}

void DouDiZhuHand::get_plane_single( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    int8_t power;
    if (last->type == Pass && hand.size() >= 8) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 2, 3, straights);

        for (std::vector<int8_t > &i : straights) {
            power = i[0];
            size_t comb_len = i.size();
            CardSet used;
            for (int8_t k : i) {
                used.set_bomb(k);
            }

            std::vector<std::vector<int8_t > > comb;
            combination(hand, used, comb_len, 1, comb);

            for (std::vector<int8_t > &j : comb) {
                CardSet res;
                res.set_plane_single(i[0], i.back(), j);

                Pattern* tmp = pattern_pool_.get(power, Plane_single, res);
                next_moves.emplace_back(tmp);
            }
        }

    } else if (last->type == Plane_single && hand.size() >= 8) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 2, 3, straights);

        size_t plane_len = (last->hand.size() / 4);
        for (std::vector<int8_t > &i : straights) {
            power = i[0];
            if (i.size() == plane_len && power > last->power) {
                size_t comb_len = i.size();
                CardSet used;

                for (int8_t k : i) {
                    used.set_bomb(k);
                }

                std::vector<std::vector<int8_t > > comb;
                combination(hand, used, comb_len, 1, comb);

                for (std::vector<int8_t > &j : comb) {
                    CardSet res;
                    res.set_plane_single(i[0], i.back(), j);

                    Pattern* tmp = pattern_pool_.get(power, Plane_single, res);
                    next_moves.emplace_back(tmp);
                }
            }

        }

    } else {
        //not move
    }
}

void DouDiZhuHand::get_plane_pair( const CardSet& hand, Pattern* last, std::vector<Pattern *> &next_moves)
{
    int8_t power;
    if (last->type == Pass && hand.size() >= 10 ) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 2, 3, straights);

        for (std::vector<int8_t > &i : straights) {
            power = i[0];
            size_t comb_len = i.size();
            CardSet used;
            for (int8_t k : i) {
                used.set_bomb(k);
            }

            std::vector<std::vector<int8_t > > comb;
            combination(hand, used, comb_len, 2, comb);

            for (std::vector<int8_t > &j : comb) {
                CardSet res;
                res.set_plane_pair(i[0], i.back(), j);

                Pattern* tmp = pattern_pool_.get(power, Plane_pair, res);
                next_moves.emplace_back(tmp);
            }
        }

    } else if (last->type == Plane_pair && hand.size() >= 10) {
        std::vector<std::vector<int8_t > > straights;
        create_straight(hand, 2, 3, straights);

        size_t plane_len = (last->hand.size() / 5);
        for (std::vector<int8_t > &i : straights) {
            power = i[0];
            if (i.size() == plane_len && power > last->power) {
                size_t comb_len = i.size();
                CardSet used;
                for (int8_t k : i) {
                    used.set_bomb(k);
                }

                std::vector<std::vector<int8_t > > comb;
                combination(hand, used, comb_len, 1, comb);

                for (std::vector<int8_t > &j : comb) {
                    CardSet res;
                    res.set_plane_pair(i[0], i.back(), j);

                    Pattern* tmp = pattern_pool_.get(power, Plane_single, res);
                    next_moves.emplace_back(tmp);
                }
            }

        }

    } else {
        //not move
    }
}

void DouDiZhuHand::get_rocket(const CardSet& hand, std::vector<Pattern *> &next_moves)
{
    if (hand.is_single(13) && hand.is_single(14)) {
        CardSet res;
        res.set_rocket();
        Pattern* tmp = pattern_pool_.get(99, Rocket, res);
        next_moves.emplace_back(tmp);
    }
}

}  //namespace doudizhu_endgame