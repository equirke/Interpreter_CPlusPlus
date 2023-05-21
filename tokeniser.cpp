#include <istream>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include "tokeniser.h"


tokeniser::tokeniser(std::istream* in)
{
	this->in = in;
	keywords["if"] = IF;
	keywords["var"] = VAR;
	keywords["int"] = INT;
	keywords["float"] = FLOAT;
	keywords["string"] = STRING;
	keywords["while"] = WHILE;
	keywords["function"] = FUNCTION;
	keywords["return"] = RETURN;
}

int tokeniser::match(int c)
{
	if(in->peek() != c) return 0;
	in->get();
	return 1;
}

token* tokeniser::tokenise_num()
{
	std::string num;
	while(std::isdigit(in->peek()))
	{
		num.append(1, in->get());
	}
	
	if(in->peek() == '.')
	{
		num.append(1, in->get());
		while(std::isdigit(in->peek()))
		{
			num.append(1, in->get());
		}
		return new float_token(std::atof(num.c_str()), FLOAT);
	}
	
	return new int_token(std::atoi(num.c_str()), INT);
}

token* tokeniser::tokenise_string()
{
	std::string s;
	in->get();
	while(in->peek() != '"') s.append(1, in->get());
	in->get();
	return new string_token(s, STRING);
}

token_type tokeniser::check_keyword(std::string word)
{
	std::map<std::string, token_type>::iterator it = keywords.find(word);
	
	if(it != keywords.end()){
		it->second;
		return it->second;
	}
	
	return NIL;
}

token* tokeniser::tokenise_identifier()
{
	std::string s;
	token_type keyword;
	
	while(std::isalnum(in->peek())) s.append(1, in->get());
	keyword = check_keyword(s);
	if(keyword != NIL)
		return new token(keyword);
	return new identifier_token(s, IDENT);
}


token* tokeniser::get_next_token()
{
	while(std::isspace(in->peek())) {
		if(in->peek() == '\n'){
			in->get();
			return new token(NIL);
		}
		in->get();
	}
	
	
	if(std::isdigit(in->peek())) return tokenise_num();
	if(in->peek() == '"') return tokenise_string();
	if(std::isalpha(in->peek())) return tokenise_identifier();
	
	if(match('+')) return new token(ADD);
	if(match('-')) return new token(SUB);
	if(match('*')) return new token(MUL);
	if(match('/')) return new token(DIV);
	if(match('%')) return new token(MOD);
	if(match('(')) return new token(LPAREN);
	if(match(')')) return new token(RPAREN);
	if(match('{')) return new token(LBRACK);
	if(match('}')) return new token(RBRACK);
	if(match(':')) return new token(COLON);
	if(match(',')) return new token(COMMA);
	if(match('<')){if(match('=')) return new token(LTE); return new token(LT);}
	if(match('>')){if(match('=')) return new token(GTE); return new token(GT);}
	if(match('=')){if(match('=')) return new token(EQ); return new token(ASSIGN);}
	if(match('!')){if(match('=')) return new token(NEQ); return new token(NOT);}
	return new token(NIL);
}

