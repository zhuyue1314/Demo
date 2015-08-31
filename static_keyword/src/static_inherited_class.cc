/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference:
 */

/*
 * Description: This file shows that inherited class can inherit
 * the static method of base class.
 */

#include <iostream>

using std::cout;
using std::endl;

class BaseClass {
public:
	static void print()
	{
		cout << "BaseClass::print()" << endl;
	}
};

class InheritedClass : public BaseClass {
};

int main()
{
	cout << "===================================" << endl;
	cout << "\tThis is a demo project" << endl;
	cout << "===================================" << endl;

	BaseClass::print();

	cout << endl;
	cout << "****: Inherited class can inherit the static method of base class" << endl;
	cout << endl;

	InheritedClass::print();

	return 0;
}
