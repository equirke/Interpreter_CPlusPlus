#include "ast.h"

class visiter
{
    virtual literal* visit_literal(literal*) = 0;
    virtual literal* visit_unary_op(unary_op*) = 0;
    virtual literal* visit_binary_op(binary_op*) = 0;
    virtual literal* visit_assignment(assignment*) = 0;
    virtual literal* visit_call(call*) = 0 ;

    virtual void visit_statement_block(statement_block*) = 0 ;
    virtual void visit_expression_statement(expression_statement*) = 0;
    virtual void visit_if_statement(if_statement*) = 0;
    virtual void visit_while_statement(while_statement*) = 0;
    virtual void visit_var_statement(var_statement*) = 0;
    virtual void visit_return_statement(return_statement*) = 0;
    virtual void visit_function_statement(function_statement*) = 0;
};