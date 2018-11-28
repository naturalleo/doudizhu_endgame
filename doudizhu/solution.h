//
// Created by deng on 18-11-28.
//

#ifndef DOUDIZHU_ENDGAME_SOLUTION_H
#define DOUDIZHU_ENDGAME_SOLUTION_H

#include "negamax.h"

namespace doudizhu_endgame {

class Solution {
public:
    Solution() : engine_(new Negamax) {}
    ~Solution() { delete engine_; }

    void set_time_out();
    void start();

private:
    Negamax* engine_;

    void process_result(TreeNode* node);

    void research(TreeNode* node, Pattern* last);

    void reset_engine();

    std::string input_stdin(const char *prompt);
};

}  //namespace doudizhu_endgame

#endif //DOUDIZHU_ENDGAME_SOLUTION_H
