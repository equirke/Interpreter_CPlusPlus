#include <iostream>
#include <memory>
#include <exception>
#include "parser.h"
#include "tokeniser.h"

int main(void){
	std::istream *ptr = &(std::cin);
	tokeniser* tkner = new tokeniser(ptr);
	parser psr(tkner);
	
	
	
	while(1){
		try
		{
			statement* node = psr.parse();
			std::cout << node->to_string() << std::endl;
			delete node;
		}
		catch (std::exception& e)
		{
		std::cerr << "exception caught: " << e.what() << '\n';
		}
	}
}