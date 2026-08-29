#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stack>

#include "token.h"
#include "parser.h"

class Interpreter{
private:
    const std::vector<Sequence>& node_sequence_vector;
    const std::vector<int>& node_combined_index;
    const std::vector<InitNode>& init_node_vector;
    const std::vector<UpdateNode>& update_node_vector;
    const std::vector<FlowNode>& flow_node_vector;
    const std::vector<onscNode>& onsc_node_vector;

    std::unordered_map<std::string_view,int> symTable;
    std::stack<Sequence> st_skip;
    std::stack<int> loop_ret;

    int curr_index = 0;
    size_t size = 0;

    bool branch_yes = false;

    void handle_init();
    void handle_update();
    int handle_expression(const Expression &expr);
    Sequence get_nodetp();
    bool handle_condCheck(const Expression &expr);
    bool check_BEGIN(Sequence tp);
    bool check_END(Sequence tp);
    void handle_branch(bool cond_result);
    void handle_else();
    void handle_loop(bool cond_result);
    void handle_flow();
    void handle_print();

public:
    Interpreter(const ParserOutput &p) :
    node_sequence_vector(p.node_sequence_vector),
    node_combined_index(p.node_combined_index),
    init_node_vector(p.init_node_vector),
    update_node_vector(p.update_node_vector),
    flow_node_vector(p.flow_node_vector),
    onsc_node_vector(p.onsc_node_vector)
    {
        this->size = node_sequence_vector.size();
    }
    void interpret();
};
