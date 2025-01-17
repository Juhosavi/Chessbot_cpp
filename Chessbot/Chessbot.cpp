#include <iostream>
#include "Asema.h"

int main()
{
	int rivi;
	int linja;

	Asema asema;
	asema.tulosta();
	asema.etsi_kuningas(bR, rivi, linja);
	std::cout << rivi <<linja<< std::endl;

}

