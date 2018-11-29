//
// Created by deng on 18-11-28.
//

#include <iostream>
#include <chrono>
#include <cstring>

#include "solution.h"

namespace doudizhu_endgame {

#define CLI
//
//输入不带空格，'10'用'0'（零）代替
//示例：[大王 A Q Q J J 10 9 9 9 4 4]
//输入：zaqqjj0999844
//
void Solution::start()
{

#ifdef CLI
    std::string l = input_stdin("input lord hand:");
    std::string f = input_stdin("input farmer hand:");
#else
    std::string l{"zaqqjj0999844"};
    std::string f{"y22aa0886633"};
#endif //CLI

    CardSet lord, farmer;
    lord.from_string(l);
    farmer.from_string(f);

    using namespace std::chrono;
    steady_clock::time_point start = steady_clock::now();
    TreeNode* root = engine_->search(lord, farmer);
    steady_clock::time_point end = steady_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(end - start);
    std::cout << "nodes searched: " << engine_->nodes_searched() << "\n";
    std::cout << "search time: " << time_span.count() << " seconds.\n";
    std::cout << "transposition table hit rate: " << engine_->hash_hit_rate() << "%\n\n";

    if (root->score == 1 && !root->child.empty()) {
        std::cout <<"first hand:["
                  << root->child[0]->last_move->hand.str() << "]\n";

#ifdef CLI
        process_result(root->child[0]);
#endif //CLI

    } else {
        std::cout << "can not win" << "\n";
    }
}

void Solution::process_result(TreeNode *node)
{
    Pattern* last = nullptr;

    bool search = true;
    while (!node->child.empty() && search) {
        CardSet hand;
        hand.from_string(input_stdin("input farmer play:"));

        for (auto child : node->child) {
            if (child->last_move->hand == hand) {
                last = child->last_move;
                if (!child->child.empty()) {
                    node = child->child[0];
                    std::cout<< "play: ["<< node->last_move->hand.str() <<"]\n";
                    std::cout<<"currt loard hand: ["<<node->lord.str()<<"]\n";
                    std::cout<<"currt farmer hand:["<<node->farmer.str()<<"]\n";

                } else {
                    search = false;
                }

            } else {
                //find next match
            }
        }
    }

    if (!node->lord.empty() && last != nullptr) {
        research(node, last);

    } else {
        //finsh process
    }
}

void Solution::research(TreeNode *node, Pattern *last)
{
    CardSet lord = node->lord;
    CardSet farmer = node->farmer;
    farmer.remove(last->hand);
    Pattern last_{last->power, last->type, last->hand};
    std::cout << "re searching..."<<"\n";

    reset_engine();
    TreeNode* re = engine_->search(lord, farmer, &last_);

    if (!re->child.empty()) {
        std::cout<< "play: ["<< re->child[0]->last_move->hand.str() << "]\n";
        process_result(re->child[0]);

    } else {
        std::cout<<"can not win"<<"\n";
    }
}

std::string Solution::input_stdin(const char *prompt)
{
    printf("%s\n", prompt);

    char *buff = new char[50];

    buff = fgets(buff, 50, stdin);
    size_t len = strlen(buff);
    if (len > 1) {
        buff[len - 1] = '\0';

    } else {
        //input is a 'Pass'
    }

    std::string string(buff);
    delete[] buff;

    return string;
}

void Solution::reset_engine()
{
    delete engine_;
    engine_ = new Negamax;
}

void Solution::set_time_out()
{
    //
}


} //namespace doudizhu_endgame