#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stack>

#include "token.h"
#include "parser.h"
#include "interpreter.h"

Sequence Interpreter::get_nodetp(){
    return node_sequence_vector[curr_index];
}

void Interpreter::handle_init(){
    int actual_index = node_combined_index[curr_index];
    const InitNode &n = init_node_vector[actual_index];
    symTable[n.name] = n.value;
}

int Interpreter::handle_expression(const Expression &expr){
    int id1 = 0;
    int id2 = 0;

    switch(expr.id1.index()){
        case 0:
            id1 = std::get<0>(expr.id1);
            break;
        case 1:
            id1 = symTable.at(std::get<1>(expr.id1));
            break;
    }

    switch(expr.id2.index()){
        case 0:
            id2 = std::get<0>(expr.id2);
            break;
        case 1:
            id2 = symTable.at(std::get<1>(expr.id2));
            break;
        default:
            return id1;
            break;
    }

    switch(expr.optype){
        case TokenType::PLUS:
            return id1 + id2;
            break;
        case TokenType::MUL:
            return id1 * id2;
            break;
    }
    return 0;

}

void Interpreter::handle_update(){
    int actual_index = node_combined_index[curr_index];
    const UpdateNode &n = update_node_vector[actual_index];
    const Expression &expr = n.expr;
    const std::string_view &target = n.target;

    int result = handle_expression(expr);

    if(symTable.count(target) > 0) symTable[target] = result;
}

bool Interpreter::handle_condCheck(const Expression &expr){
    int id1 = 0;
    int id2 = 0;

    switch(expr.id1.index()){
        case 0:
            id1 = std::get<0>(expr.id1);
            break;
        case 1:
            id1 = symTable.at(std::get<1>(expr.id1));
            break;
    }

    switch(expr.id2.index()){
        case 0:
            id2 = std::get<0>(expr.id2);
            break;
        case 1:
            id2 = symTable.at(std::get<1>(expr.id2));
            break;
        default:
            return static_cast<bool>(id1);
            break;
    }

    switch(expr.optype){
        case TokenType::EQ:
            return id1 == id2;
        case TokenType::EQ_L:
            return id1 <= id2;
        case TokenType::EQ_G:
            return id1 >= id2;
        case TokenType::L_AB:
            return id1 < id2;
        case TokenType::R_AB:
            return id1 > id2;
    }
    return false;
}

bool Interpreter::check_BEGIN(Sequence tp){
    return (tp == Sequence::BEGIN_IF || tp == Sequence::BEGIN_ELSE ||
            tp == Sequence::BEGIN_LOOP || tp == Sequence::BEGIN_MAIN);
}

bool Interpreter::check_END(Sequence tp){
    return (tp == Sequence::END_IF || tp == Sequence::END_ELSE ||
            tp == Sequence::END_LOOP || tp == Sequence::END_MAIN);
}

void Interpreter::handle_branch(bool cond_result){
    curr_index++;
    switch(cond_result){
        case false:
            branch_yes = true;
            st_skip.push(Sequence::BEGIN_IF);
            while(!st_skip.empty()){
                curr_index++;
                Sequence tp = get_nodetp();

                if(check_BEGIN(tp)){
                    st_skip.push(tp);
                }
                else if(check_END(tp)){
                    st_skip.pop();
                }
            }
            break;
        case true:
            branch_yes = false;
            return;
            break;
    }
}

void Interpreter::handle_else(){
    if(!branch_yes){
        curr_index++;
        st_skip.push(Sequence::BEGIN_ELSE);
        while(!st_skip.empty()){
            curr_index++;
            Sequence tp = get_nodetp();

            if(check_BEGIN(tp)){
                st_skip.push(tp);
            }
            else if(check_END(tp)){
                st_skip.pop();
            }
        }
    }
}

void Interpreter::handle_loop(bool cond_result){
    switch(cond_result){
        case false:
            curr_index++;
            st_skip.push(Sequence::BEGIN_LOOP);
            while(!st_skip.empty()){
                curr_index++;
                Sequence tp = get_nodetp();

                if(check_BEGIN(tp)){
                    st_skip.push(tp);
                }
                else if(check_END(tp)){
                    st_skip.pop();
                }
            }
            break;
        case true:
            loop_ret.push(curr_index);
            break;
    }
}

void Interpreter::handle_loop_node(){
    int actual_index = node_combined_index[curr_index];
    const LoopNode &n = loop_node_vector[actual_index];

    const Expression &expr = n.expr;

    bool cond_result = handle_condCheck(expr);
    handle_loop(cond_result);
}

void Interpreter::handle_if_node(){
    int actual_index = node_combined_index[curr_index];
    const IfNode &n = if_node_vector[actual_index];

    const Expression &expr = n.expr;

    bool cond_result = handle_condCheck(expr);
    handle_branch(cond_result);
}

void Interpreter::handle_print(){
    int actual_index = node_combined_index[curr_index];
    const onscNode &n = onsc_node_vector[actual_index];
    int arg_vec_index = 0;

    for (size_t i = 0; i < n.str.size(); ++i) {
        if (n.str[i] == '%') {
            if (n.arg_vec[arg_vec_index].value.index() == 0) {
                std::cout << std::get<0>(n.arg_vec[arg_vec_index].value);
            } else {
                std::cout << symTable[std::get<1>(n.arg_vec[arg_vec_index].value)];
            }
            arg_vec_index++;
        } else {
            std::cout << n.str[i];
        }
    }
    std::cout << '\n';
}

void Interpreter::interpret(){
    while(curr_index < size){
        switch(get_nodetp()){
            case Sequence::BEGIN_MAIN:
                break;
            case Sequence::BEGIN_IF:
                break;
            case Sequence::BEGIN_ELSE:
                break;
            case Sequence::BEGIN_LOOP:
                break;
            case Sequence::INIT_NODE:
                handle_init();
                break;
            case Sequence::UPDATE_NODE:
                handle_update();
                break;
            case Sequence::IF_NODE:
                handle_if_node();
                break;
            case Sequence::LOOP_NODE:
                handle_loop_node();
                break;
            case Sequence::ELSE_NODE:
                handle_else();
                break;
            case Sequence::PRINT_NODE:
                handle_print();
                break;
            case Sequence::END_MAIN:
                break;
            case Sequence::END_IF:
                break;
            case Sequence::END_ELSE:
                break;
            case Sequence::END_LOOP:
                if(!loop_ret.empty()){
                    curr_index = loop_ret.top() - 1;
                    loop_ret.pop();
                }
                break;
        }
        curr_index++;
    }
}
