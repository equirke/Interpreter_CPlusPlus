#pragma once

#include <memory>
#include <string>
#include <vector>
#include <algorithm>    // std::move (ranges)
#include <utility>		// std::move (objects)

#include "token_types.h"

class ast_node
{
	public:
		ast_node(){}
		virtual std::string to_string() = 0;
		
};



class literal : public ast_node
{
	private:
		token_type type;
		union{
			int int_value;
			float float_value;
		};
		std::string string_value;
		
	public:
		literal(token_type type, int int_value){this->type = type; this->int_value = int_value;}
		literal(token_type type, float float_value){this->type = type; this->float_value = float_value;}
		literal(token_type type, std::string string_value){this->type = type; this->string_value = string_value;}
		
		token_type get_token_type(){ return this->type;}
		
		operator int() {return int_value;}
		operator float() {return float_value;}
		operator std::string(){return string_value;}
		
		std::string to_string()
		{
			std::string value;
			switch(type){
				case INT:
					value = std::to_string(int_value);
					break;
				case FLOAT:
					value = std::to_string(float_value);
					break;
				case STRING:
					value = string_value;
			}
			return std::string(value + ":" + type_names[type]);
		}
		
		
		
};

class variable: public ast_node
{
	private:
		std::string identifier;
	public:
		variable(std::string identifier){this->identifier = identifier;}
		
		std::string to_string()
		{
			return std::string(identifier);
		}
};

class unary_op : public ast_node
{
	private:
		token_type type;
		std::unique_ptr<ast_node> right;
		
	public:
		unary_op(token_type type, ast_node* right){this->type = type; this->right = std::unique_ptr<ast_node>(right);}
		
		std::string to_string()
		{
			return std::string("(" + type_names[type] + " " + right->to_string() + ")");
		}
};

class binary_op : public ast_node
{
	private:
		token_type type;
		std::unique_ptr<ast_node> left;
		std::unique_ptr<ast_node> right;
		
		
	public:
		
		binary_op(token_type type, ast_node* left, ast_node* right){this->type = type; this->left = std::unique_ptr<ast_node>(left); this->right = std::unique_ptr<ast_node>(right);}
		
		std::string to_string(){
			std::string left_string = left->to_string();
			std::string right_string = right->to_string();
			return std::string("(" + type_names[type] + " " + left_string  + " " + right_string + ")");
		}
};

class assignment : public ast_node
{
	private:
		std::unique_ptr<ast_node> variable;
		std::unique_ptr<ast_node> right;
		
	public:
		
		assignment(ast_node* variable, ast_node* right){this->variable = std::unique_ptr<ast_node>(variable); this->right = std::unique_ptr<ast_node>(right);}
		
		std::string to_string()
		{
			return std::string("(ASSIGN " + variable->to_string() + " " +right->to_string() + ")");
		}
		
};

class call : public ast_node
{
	private:
		std::string ident;
		std::vector<std::unique_ptr<ast_node>> parameters;
		
	public:
	
		call(std::string ident, std::vector<ast_node*> parms)
		{
			this->ident = ident;
			
			for(ast_node* parm : parms)
				this->parameters.push_back(std::unique_ptr<ast_node>(parm));
		}
		
		std::string to_string()
		{
			std::string str;
			str += ident + "(";
			
			for(std::unique_ptr<ast_node>& parm: parameters)
				str += parm->to_string() + ",";
			str += ")";
			
			return str;
		}
};

class statement
{
	public:
		statement(){};
		virtual std::string to_string() = 0;
};

class statement_block : public statement
{
	private:
		std::vector<std::unique_ptr<statement>> stmts;
		
	public:
	
		statement_block(std::vector<std::unique_ptr<statement>>& stmts)
		{
			for(std::unique_ptr<statement>& stmt: stmts)
				this->stmts.push_back(std::unique_ptr<statement>(stmt.release()));
		}
		
		std::string to_string()
		{
			std::string block;
			block += "{";
			for(std::unique_ptr<statement>& stmt: stmts)
				block += stmt->to_string();
			block += "}";
			return block;
		}
		
};

class expression_statement : public statement
{
	private:	
		std::unique_ptr<ast_node> expression;
	
	public:
		expression_statement(ast_node* expression){this->expression = std::unique_ptr<ast_node>(expression);}
		std::string to_string(){return expression->to_string() + "\n";}
};

class if_statement : public statement
{
	private:	
		std::unique_ptr<ast_node> condition;
		std::unique_ptr<statement> stmt;
	
	public:
		if_statement(ast_node* condition, statement* stmt){this->condition = std::unique_ptr<ast_node>(condition); this->stmt = std::unique_ptr<statement>(stmt);}
		std::string to_string(){return "if" + condition->to_string() + "\n" +  stmt->to_string() + "\n";}
};

class while_statement : public statement
{
	private:	
		std::unique_ptr<ast_node> condition;
		std::unique_ptr<statement> stmt;
	
	public:
		while_statement(ast_node* condition, statement* stmt){this->condition = std::unique_ptr<ast_node>(condition); this->stmt = std::unique_ptr<statement>(stmt);}
		std::string to_string(){return "if" + condition->to_string() + "\n" +  stmt->to_string() + "\n";}
};

class var_statement : public statement
{
	private:	
		std::string name;
		token_type type;
		std::unique_ptr<ast_node> init;
	
	public:
		var_statement(std::string name, token_type type){this->name = name; this->type = type; this->init = NULL;}
		var_statement(std::string name, token_type type, ast_node* init){this->name = name; this->type = type; this->init = std::unique_ptr<ast_node>(init);}
		std::string to_string(){return name + ":" + type_names[type] + " " +  init->to_string() + "\n";}
};

class parameter
{
	private:
		std::string name;
		token_type type;
	public:
		parameter(std::string name, token_type type){this->name = name; this->type = type;}
		std::string to_string(){return name + ":" + type_names[type];}
};

class function_statement : public statement
{
	private:
		std::string name;
		token_type type;
		std::vector<parameter> parameters;
		std::unique_ptr<statement> stmt;
	public:
		function_statement(std::string name, token_type type, statement* stmt){this->name = name; this->type = type; this->stmt = std::unique_ptr<statement>(stmt);}
		function_statement(std::string name, token_type type, std::vector<parameter> params, statement* stmt){this->name = name; this->type = type;this->parameters = params; this->stmt = std::unique_ptr<statement>(stmt);}
		
		std::string to_string()
		{
			std::string func;
			func += name + "(";
			
			for(parameter& parm: parameters)
				func += parm.to_string() + " ";
			func+= "):" + type_names[type];
			
			return func + " " +  stmt->to_string() + "\n";
		}
		
};

class return_statement : public statement
{
	private:	
		std::unique_ptr<ast_node> expression;
	
	public:
		return_statement(ast_node* expression){this->expression = std::unique_ptr<ast_node>(expression);}
		std::string to_string(){return "(RETURN " + expression->to_string() + ")\n";}
};