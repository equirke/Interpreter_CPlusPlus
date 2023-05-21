#include "parser.h"

#include <iostream>
#include <typeinfo>
#include <stdexcept>


token* parser::peek()
{
	return current.get();
}

token_type parser::match(token_type type)
{
	if(current->get_type() != type) return NIL;
	
	advance();
	
	return type;
}

token_type parser::match(std::vector<token_type> types)
{
	/*if(current->get_type() != type) return NIL;
	
	advance();
	
	return type;*/
	
	for(token_type &type : types)
	{
		if(current->get_type() == type)
		{
			advance();
			return type;
		}
	}
	
	return NIL;
	
}

token_type parser::consume(token_type type)
{
	if(current->get_type() == type)
	{
		advance();
		return current->get_type();
	}
	
	throw 20;
}

token* parser::advance()
{
	current = std::unique_ptr<token>(tkner->get_next_token());
	return current.get();
}
statement* parser::parse()
{
	advance();
	return stmt();
}

statement* parser::stmt()
{
	switch(peek()->get_type())
	{
		case LBRACK:
			return block();
		case IF:
			return if_stmt();
		case WHILE:
			return while_stmt();
		case VAR:
			return var_stmt();
		case FUNCTION:
			return function_stmt();
		case RETURN:
			return return_stmt();
		default:
			return expression_stmt();
	}
}

statement* parser::return_stmt()
{
	consume(RETURN);

	ast_node* expr = expression();

	return new return_statement(expr);
}

statement* parser::function_stmt()
{
	consume(FUNCTION);
	
	if(peek()->get_type() == IDENT)
	{
		std::string name = ((identifier_token*)peek())->get_identifier_value();
		advance();
		consume(LPAREN);
		std::vector<parameter> params;
		while(!match(RPAREN))
		{
			params.push_back(param());
			
			if(!match(COMMA))
				break;
		}
		match(RPAREN);
		consume(COLON);
		token_type type = peek()->get_type();
		advance();
		statement* body = stmt();
		return new function_statement(name, type, params, body);
	}
	
	throw std::runtime_error("Expecting function name after function keyword");
}

parameter parser::param()
{
	if(peek()->get_type() == IDENT)
	{
		std::string name = ((identifier_token*)peek())->get_identifier_value();
		advance();
		consume(COLON);
		token_type type = peek()->get_type();
		advance();
		return parameter(name, type);
	}
	
	throw std::runtime_error("Expecting parameter name after function keyword");
}

statement* parser::var_stmt()
{
	consume(VAR);
	
	if(peek()->get_type() == IDENT)
	{
		std::string name = ((identifier_token*)peek())->get_identifier_value();
		advance();
		consume(COLON);
		token_type type = peek()->get_type();
		advance();
		
		if(match(ASSIGN))
		{
			ast_node* init = expression();
			
			return new var_statement(name, type, init);
		}
		
		return new var_statement(name, type);
		
	}
	
	throw std::runtime_error("Error assigning variable");
}


statement* parser::while_stmt()
{
	consume(WHILE);
	
	ast_node* condition = expression();
	
	statement* st = stmt();
	
	return new while_statement(condition, st);
}

statement* parser::if_stmt()
{
	consume(IF);
	
	ast_node* condition = expression();
	
	statement* st = stmt();
	
	return new if_statement(condition, st);
}

statement* parser::block()
{	
	std::vector<std::unique_ptr<statement>> stmts;
	consume(LBRACK);
	while(peek()->get_type() != RBRACK)
	{
		match(NIL);
		stmts.push_back(std::unique_ptr<statement>(stmt()));
	}
	consume(RBRACK);
	
	return new statement_block(stmts);
}

statement* parser::expression_stmt()
{
	return new expression_statement(expression());
}

ast_node* parser::expression()
{
	return assign();
}


ast_node* parser::assign()
{
	ast_node* left = equality();
	
	token_type operation = NIL;
	
	if((operation = match(ASSIGN)) != NIL)
	{
		if(dynamic_cast<variable*>(left))
		{
			ast_node* right = equality();
			
			return new assignment(left, right);
		}
		
		throw std::runtime_error("Can only assign into a variable");
	}
	
	
	return left;
	
}

ast_node* parser::equality()
{
	
	ast_node* left = comparison();
	
	token_type operation = NIL;
	
	while((operation = match(std::vector<token_type>{EQ, NEQ})) != NIL)
	{	
		ast_node* right = comparison();
		
		left = new binary_op(operation, left, right);
	}
	
	return left;

}


ast_node* parser::comparison()
{
	
	ast_node* left = add_sub();
	
	token_type operation = NIL;
	
	while((operation = match(std::vector<token_type>{LT, LTE, GT, GTE})) != NIL)
	{	
		ast_node* right = add_sub();
		
		left = new binary_op(operation, left, right);
	}
	
	return left;

}



ast_node* parser::add_sub()
{
	
	ast_node* left = mul_div_mod();
	
	token_type operation = NIL;
	
	while((operation = match(std::vector<token_type>{ADD, SUB})) != NIL)
	{	
		ast_node* right = mul_div_mod();
		
		left = new binary_op(operation, left, right);
	}
	
	return left;

}


ast_node* parser::mul_div_mod()
{
	
	ast_node* left = unary();
	
	token_type operation = NIL;
	
	while((operation = match(std::vector<token_type>{MUL, DIV, MOD})) != NIL)
	{	
		ast_node* right = unary();
		
		left = new binary_op(operation, left, right);
	}
	
	return left;

}

ast_node* parser::unary()
{
	token_type operation = NIL;
	
	operation = match(std::vector<token_type>{NOT, SUB});
	
	if(operation != NIL)
	{	
		ast_node* right = term();
		return new unary_op(operation, right);
	}
	
	return term();

}

ast_node* parser::term()
{
	ast_node* node;
	std::string ident;
	switch(peek()->get_type())
	{
		case INT:
			node = new literal(INT, ((int_token*)peek())->get_int_value());
			advance();
			return node;
		case FLOAT:
			node = new literal(FLOAT, ((float_token*)peek())->get_float_value());
			advance();
			return node;
		case STRING:
			node = new literal(STRING, ((string_token*)peek())->get_string_value());
			advance();
			return node;
		case IDENT:
			ident = ((identifier_token*)peek())->get_identifier_value();
			consume(IDENT);
			if(peek()->get_type()== LPAREN)
			{
				consume(LPAREN);
				std::vector<ast_node*> parameters;
				while(!match(RPAREN))
				{
					parameters.push_back(expression());
				}
				advance();
				return new call(ident, parameters);
			}
			node = new variable(ident);
			advance();
			return node;
		case LPAREN:
			advance();
			node = expression();
			consume(RPAREN);
			return node;
			
	}
	
	throw std::runtime_error("Expecting variable, literal, or nested expression");
}
