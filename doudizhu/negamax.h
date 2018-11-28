//
// Created by deng on 18-11-28.
//

#ifndef DOUDIZHU_ENDGAME_NEGAMAX_H
#define DOUDIZHU_ENDGAME_NEGAMAX_H

#include <unordered_map>
#include <vector>

#include "card_set.h"
#include "ddz_hand.h"

namespace doudizhu_endgame {

struct TreeNode {
    int8_t                   turn;       //0: lord 1:farmer
    int8_t                   score;
    CardSet                  lord;
    CardSet                  farmer;
    Pattern                 *last_move;
    std::vector<TreeNode *>  child;
};

class NodeSet {
public:

    NodeSet() = default;
    ~NodeSet() = default;

    size_t size();
    void add(TreeNode *node);

    //return node from pool, if not in map return 0
    int8_t get(TreeNode *node);

private:

    std::unordered_map<uint64_t, int8_t> pool_;
    uint64_t gen_key(TreeNode* node);
};

class Negamax {

public:

    Negamax();
    ~Negamax();

    TreeNode *search(const CardSet& lord, const CardSet& farmer);
    //for research
    TreeNode *search(const CardSet& lord, const CardSet& farmer, Pattern* last);

    size_t nodes_searched()
    {
        return nodes_searched_;
    }

    double hash_hit_rate()
    {
        return ((double)hash_hit_ / nodes_searched_) * 100;
    }

private:

    TreeNode       *tree_;
    NodeSet         node_pool_;
    DouDiZhuHand    doudizhu_;

    size_t nodes_searched_{};
    size_t hash_hit_{};

    int8_t negamax(TreeNode *node);
    void gen_nodes(TreeNode *node);
    void pruning_tree(TreeNode* node);
    void destroy_tree(TreeNode* node);

};

} //namespace doudizhu_endgame

#endif //DOUDIZHU_ENDGAME_NEGAMAX_H
