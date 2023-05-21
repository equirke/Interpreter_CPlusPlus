#include <iostream>
#include "tokeniser.h"


int main(void){
	std::istream *ptr = &(std::cin);
	tokeniser tkner(ptr);
	while(1){
		token* t = tkner.get_next_token();
		std::cout << type_names[t->get_type()] << std::endl;
		delete t;
	}
}