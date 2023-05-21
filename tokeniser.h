#pragma once

#include <istream>
#include <string>
#include <memory>
#include <map>

#include "token_types.h"


class token
{
  private:
    token_type type;
  public:
    token(token_type type){this->type = type;}
    token_type get_type(){return type;}
};

class string_token : public token
{
  private:
    std::string value;
  public:
    string_token(std::string value, token_type type):token(type){this->value = value;}
    std::string get_string_value(){return value;};
};

class int_token : public token
{
  private:
    int value;
  public:
    int_token(int value, token_type type):token(type){this->value = value;}
    int get_int_value(){return value;};
};

class float_token: public token
{
  private:
    float value;
  public:
    float_token(float value, token_type type):token(type){this->value = value;}
    float get_float_value(){return value;};
	
};

class identifier_token : public token
{
  private:
    std::string value;
  public:
    identifier_token(std::string value, token_type type):token(type){this->value = value;}
    std::string get_identifier_value(){return value;};
};


class tokeniser
{
  private:
    std::istream *in;
    int match(int);
    int peek();
	token* tokenise_num();
	token* tokenise_string();
	token* tokenise_identifier();
	token_type check_keyword(std::string word);
	std::map<std::string, token_type> keywords;
	
  public:
    token* get_next_token();
    tokeniser(std::istream* in);

};
