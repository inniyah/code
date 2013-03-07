#include <iostream>
#include "test.h"
using namespace std;

int test::function1 (long l)
{
	cout << "in function 1" << endl;
	return l;
}


int test::function2 (int i)
{
	cout << "in function 2" << endl;
	return test::function1 (i) + 1;
}


int test::function3 (char c)
{
	cout << "in function 3" << endl;
	return test::function2 (c) + 1;

}
