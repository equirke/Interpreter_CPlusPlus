#include <string>

#pragma once

enum token_type{NIL, INT, FLOAT, STRING, IDENT, ADD, SUB, MUL, DIV, MOD, GT, GTE, LT, LTE, ASSIGN, EQ, NEQ, NOT, LPAREN, RPAREN, LBRACK, RBRACK, COLON, IF, WHILE, VAR, FUNCTION, COMMA, RETURN};
const std::string type_names[] = {"NIL", "INT", "FLOAT", "STRING", "IDENT", "ADD", "SUB", "MUL", "DIV", "MOD", "GT", "GTE", "LT", "LTE", "ASSIGN", "EQ", "NEQ", "NOT", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "COLON", "IF", "WHILE", "VAR", "FUNCTION", "COMMA", "RETURN"};