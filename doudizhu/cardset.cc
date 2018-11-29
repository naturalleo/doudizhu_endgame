//
// Created by deng on 18-11-28.
//

#include <map>

#include "cardset.h"

namespace doudizhu_endgame {

//card to val map
static const std::map<char, int8_t> val_map = {
        {'3',  0},
        {'4',  1},
        {'5',  2},
        {'6',  3},
        {'7',  4},
        {'8',  5},
        {'9',  6},
        {'0',  7},
        {'J',  8},
        {'Q',  9},
        {'K',  10},
        {'A',  11},
        {'2',  12},
        {'Y',  13},
        {'Z',  14},
        {'\n', 15},
        {'P',  15}
};

//val to string
static const std::string str_map[16]{"3 ", "4 ", "5 ", "6 ", "7 ",
                                 "8 ", "9 ", "10 ", "J ", "Q ",
                                 "K ", "A ", "2 ", "小王 ", "大王 ",
                                 "Pass "};


void CardSet::set_rocket()
{
    card_mask_.set((size_t) (14 << 2));
    card_mask_.set((size_t) (13 << 2));
}

void CardSet::set_single(int8_t card, bool val)
{
    card_mask_.set((size_t) (card << 2), val);
}

void CardSet::set_pair(int8_t card, bool val)
{
    card <<= 2;
    card_mask_.set((size_t) card, val);
    card_mask_.set((size_t) (card + 1), val);
}

void CardSet::set_trio(int8_t card)
{
    card <<= 2;
    card_mask_.set((size_t) card);
    card_mask_.set((size_t) (card + 1));
    card_mask_.set((size_t) (card + 2));
}

void CardSet::set_trio_single(int8_t trio, int8_t single)
{
    trio <<= 2;
    card_mask_.set((size_t) trio);
    card_mask_.set((size_t) (trio + 1));
    card_mask_.set((size_t) (trio + 2));

    card_mask_.set((size_t) (single << 2));
}

void CardSet::set_trio_pair(int8_t trio, int8_t pair)
{
    trio <<= 2;
    card_mask_.set((size_t) trio);
    card_mask_.set((size_t) (trio + 1));
    card_mask_.set((size_t) (trio + 2));

    pair <<= 2;
    card_mask_.set((size_t) pair);
    card_mask_.set((size_t) (pair + 1));
}

void CardSet::set_bomb(int8_t bomb)
{
    bomb <<= 2;
    card_mask_.set((size_t) bomb);
    card_mask_.set((size_t) (bomb + 1));
    card_mask_.set((size_t) (bomb + 2));
    card_mask_.set((size_t) (bomb + 3));
}

void CardSet::set_straight_s(int8_t start, int8_t end)
{
    for (int8_t i = start; i <= end; ++i) {
        card_mask_.set((size_t) (i << 2));
    }
}

void CardSet::set_straight_p(int8_t start, int8_t end)
{
    for (int8_t i = start; i <= end; ++i) {
        card_mask_.set((size_t) (i << 2));
        card_mask_.set((size_t) ((i << 2) + 1));
    }
}

void CardSet::set_plane(int8_t start, int8_t end)
{
    for (int8_t i = start; i <= end; ++i) {
        card_mask_.set((size_t) (i << 2));
        card_mask_.set((size_t) ((i << 2) + 1));
        card_mask_.set((size_t) ((i << 2) + 2));
    }
}

void CardSet::set_plane_single(int8_t start, int8_t end, std::vector<int8_t> &comb)
{
    for (int8_t i = start; i <= end; ++i) {
        card_mask_.set((size_t) (i << 2));
        card_mask_.set((size_t) ((i << 2) + 1));
        card_mask_.set((size_t) ((i << 2) + 2));
    }

    for (int8_t i : comb) {
        card_mask_.set((size_t) (i << 2));
    }
}

void CardSet::set_plane_pair(int8_t start, int8_t end, std::vector<int8_t> &comb)
{
    for (int8_t i = start; i <= end; ++i) {
        card_mask_.set((size_t) (i << 2));
        card_mask_.set((size_t) ((i << 2) + 1));
        card_mask_.set((size_t) ((i << 2) + 2));
    }

    for (int8_t i : comb) {
        card_mask_.set((size_t) (i << 2));
        card_mask_.set((size_t) ((i << 2) + 1));
    }
}

bool CardSet::has_count(int8_t card, int8_t count) const
{
    switch (count) {
        case 1:
            return card_mask_[card << 2];
        case 2:
            return card_mask_[(card << 2) + 1];
        case 3:
            return card_mask_[(card << 2) + 2];
        case 4:
            return card_mask_[(card << 2) + 3];
        default:
            return false;
    }
}

std::string CardSet::str()
{
    std::string string;
    size_t pos = card_mask_._Find_first();
    while (pos < 64) {
        size_t index = (pos >> 2);
        if (index < 16) {
            //牌面大在前
            string.insert(0, str_map[index]);

        } else {
            //invalid card
        }
        pos = card_mask_._Find_next(pos);
    }

    if (string.length() == 0) {
        string += str_map[15];
    }

    string.pop_back();
    return string;
}

std::string CardSet::bitset_str()
{
    return card_mask_.to_string();
}

void CardSet::from_string(std::string string)
{
    int8_t card;
    int8_t i = 0;
    while (string[i]) {
        auto it = val_map.find((char) toupper(string[i]));
        if (it == val_map.end()) {
            printf("invalid card:%c\n", string[i]);

        } else {
            if (string[i] != '\n' && string[i] != 'P') {

                card = (it->second) << 2;
                while (card_mask_[card]) {
                    card += 1;
                }
                card_mask_.set((size_t) card);
            } else {
                //Pass(empty) not set
            }
        }
        ++i;
    }
}

void CardSet::from_c_string(char *string)
{
    this->from_string(std::string(string));
}

void CardSet::remove(const CardSet &hand)
{
    size_t pos = hand.find_first();


    while (pos < 64) {
        size_t i = ((pos >> 2) << 2) + 3;
        while (!card_mask_.test(i)) {
            i -= 1;
        }
        card_mask_.set(i, false);

        pos = hand.find_next(pos);
    }
}

}  //namespace doudizhu_endgame