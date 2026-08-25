#include <vector>
#include <string>
#include <unordered_map>
#include <stack>

#include "token.h"
#include "parser.h"

class Interpreter{
private:
    std::vector<Sequence> node_sequence_vector;
    std::vector<int> node_combined_index;
    std::vector<InitNode> init_node_vector;
    std::vector<UpdateNode> update_node_vector;
    std::vector<FlowNode> flow_node_vector;
    std::vector<onscNode> onsc_node_vector;

    std::unordered_map<std::string,int> symTable;
    std::stack<Sequence> st_skip;
    std::stack<int> loop_ret;

    int curr_index = 0;
    size_t size = 0;

    bool branch_yes = false;

    void handle_init();
    void handle_update();
    int handle_expression(Expression &expr);
    Sequence get_nodetp();
    bool handle_condCheck(Expression expr);
    bool check_BEGIN(Sequence tp);
    bool check_END(Sequence tp);
    void handle_branch(bool cond_result);
    void handle_else();
    void handle_loop(bool cond_result);
    void handle_flow();
    void handle_print();

public:
    Interpreter(ParserOutput &p){
        this->node_sequence_vector = p.node_sequence_vector;
        this->node_combined_index = p.node_combined_index;
        this->init_node_vector = p.init_node_vector;
        this->update_node_vector = p.update_node_vector;
        this->flow_node_vector = p.flow_node_vector;
        this->onsc_node_vector = p.onsc_node_vector;

        this->size = node_sequence_vector.size();
    }
    void interpret();
};
