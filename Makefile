CC=g++
Objects= tokeniser.o
Flags= -std=c++11 -g



token_test: tokeniser.o token_test.cpp
	$(CC) $(Flags) tokeniser.o token_test.cpp -o token_test
	
parser_test: parser.o tokeniser.o parser.h ast.h parser_test.cpp
	$(CC) $(Flags) parser.o tokeniser.o parser_test.cpp -o parser_test

tokeniser.o: tokeniser.cpp tokeniser.h
	$(CC) $(Flags) -c tokeniser.cpp
	
parser.o: parser.cpp parser.h ast.h
	$(CC) $(Flags) -c parser.cpp




