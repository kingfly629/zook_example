#include "Base.h"



Base::Base()
{
	std::cout << "here contruct Base...\n";
}


Base::~Base()
{
	std::cout << "here destruct Base...\n";
}

void Base::test()
{
	std::cout << "here Base test...\n";
}