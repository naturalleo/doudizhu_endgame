//
// Created by deng on 18-11-28.
//

#include "negamax.h"

namespace doudizhu_endgame {

Negamax::Negamax() = default;

Negamax::~Negamax()
{
    destroy_tree(tree_);
}

void Negamax::gen_nodes(TreeNode *node)
{
    if (node->turn == 1) { //farmer turn
        std::vector<Pattern *> selections;
        doudizhu_.next_hand(node->farmer, node->last_move, selections);

        node->child.reserve(selections.size());

        for (Pattern *move : selections) {
            CardSet after_play;
            doudizhu_.play(node->farmer, move, after_play);

            auto child = new TreeNode{0, 0, node->lord, after_play, move};
            if (after_play.empty()) {
                child->score = -1;

                for (TreeNode *i : node->child) {
                    delete i;
                }
                std::vector<TreeNode *>().swap(node->child);
                node->child.emplace_back(child);
                return;
            }
            node->child.emplace_back(child);
        }

    } else {//lord turn
        std::vector<Pattern *> selections;
        doudizhu_.next_hand(node->lord, node->last_move, selections);

        node->child.reserve(selections.size());

        for (Pattern *move : selections) {
            CardSet after_play;
            doudizhu_.play(node->lord, move, after_play);

            auto child = new TreeNode{1, 0, after_play, node->farmer, move};
            if (after_play.empty()) {
                child->score = -1;

                for (TreeNode *i : node->child) {
                    delete i;
                }
                std::vector<TreeNode *>().swap(node->child);
                node->child.emplace_back(child);
                return;
            }
            node->child.emplace_back(child);
        }
    }
}

int8_t Negamax::negamax(TreeNode* node)
{
    nodes_searched_ += 1;
    int8_t search = table_.get(node);

    if (search != 0) {
        hash_hit_ += 1;
        return search;
    }

    gen_nodes(node);
    int8_t score = -1;
    for (TreeNode* &child: node->child) {
        int8_t val{};
        if (child->score != 0) {
            val = -child->score;
            nodes_searched_ += 1;

        } else {
            val = -negamax(child);
        }

        if (val > 0) {
            score = val;
            break;          //发生剪枝
        }
    }

    node->score = score;
    table_.add(node);
    pruning_tree(node);

    return score;
}

TreeNode* Negamax::search(const CardSet& lord, const CardSet& farmer)
{
    CardSet pass;
    Pattern start = {-1, Pass, pass};
    auto root = new TreeNode{0, 0, lord, farmer, &start};

    negamax(root);
    tree_ = root;

    return root;
}

TreeNode *Negamax::search(const CardSet &lord, const CardSet &farmer, Pattern* last)
{
    auto root = new TreeNode{0, 0, lord, farmer, last};

    negamax(root);
    tree_ = root;

    return root;
}

void Negamax::pruning_tree(TreeNode *node)
{
    if (node->turn == 0) {
        while (!node->child.empty() && node->child.back()->score != -1) {
            destroy_tree(node->child.back());
            node->child.pop_back();
        }

        if (!node->child.empty()) {
            std::swap(node->child.front(), node->child.back());
            while (node->child.size() > 1) {
                destroy_tree(node->child.back());
                node->child.pop_back();
            }

        } else {
            //no child's score is -1
        }

    } else {
        //not pruning
    }
}

void Negamax::destroy_tree(TreeNode *node)
{
    if (!node->child.empty()) {
        for (TreeNode* i : node->child) {
            destroy_tree(i);
        }
        std::vector<TreeNode *>().swap(node->child);
    }

    delete node;
}

void TranspositionTable::add(TreeNode *node)
{
    uint64_t key = gen_key(node);
    auto it = table_.find(key);

    if (it == table_.end()) {
        this->table_.emplace(key, node->score);

    } else {
        //not add
    }
}

size_t TranspositionTable::size()
{
    return table_.size();
}

int8_t TranspositionTable::get(TreeNode *node)
{
    uint64_t key = gen_key(node);
    auto it = table_.find(key);

    if (it == table_.end()) {
        return 0;

    } else {
        return it->second;
    }
}

uint64_t TranspositionTable::gen_key(TreeNode *node)
{
    uint64_t val = 0;

    val ^= node->farmer.to_ullong() + 0x9e3779b9 + (val<<6) + (val>>2);
    val ^= node->lord.to_ullong() + 0x9e3779b9 + (val<<6) + (val>>2);
    val ^= node->last_move->hand.to_ullong() + 0x9e3779b9 + (val<<6) + (val>>2);
    val ^= node->turn + 0x9e3779b9 + (val<<6) + (val>>2);

    return val;
}

}//namespace doudizhu_endgame
