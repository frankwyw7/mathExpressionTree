#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "declaration.h"

using namespace expressionTree;

int main()
{
	std::fstream sfm("expresions.txt");
	std::string tmp;
	std::vector < std::string > strvec;

	while (!sfm.eof())
	{
		getline(sfm, tmp);
		strvec.push_back(tmp);
	}
	sfm.close();

	Calculator test(strvec);

	try
	{
		test.run();
	}
	catch (exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	
}