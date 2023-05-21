#pragma once

#include <memory>
#include <vector>

#include "tokeniser.h"
#include "ast.h"
#include "token_types.h"

class parser
{
	private:
		std::unique_ptr<tokeniser> tkner;
		std::unique_ptr<token> current;
		
		token* peek();
		token* advance();
		token_type consume(token_type type);
		token_type match(std::vector<token_type> type);
		token_type match(token_type type);
		
		statement* stmt();
		statement* block();
		statement* expression_stmt();
		statement* if_stmt();
		statement* while_stmt();
		statement* var_stmt();
		statement* function_stmt();
		statement* return_stmt();
		parameter param();
		
		
		ast_node* expression();
		ast_node* assign();
		ast_node* equality();
		ast_node* comparison();
		ast_node* add_sub();
		ast_node* mul_div_mod();
		ast_node* unary();
		ast_node* term();
		
		
	public:
		parser(tokeniser *tkner){this->tkner = std::unique_ptr<tokeniser>(tkner);}
		statement* parse();
		
			
};