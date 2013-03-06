#include <iostream>
#include "test.h"

using namespace std;

int main ()
{
	class test *ptest;
	ptest = new test();
	return ptest->function3 (1);
}
