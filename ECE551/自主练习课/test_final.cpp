// Example program
#include <iostream>
#include <string>
#include <cstdlib>

class A {
protected:
    int x;
public:
    A():x(0){std::cout << "A()\n";}
    A(int _x): x(_x) {std::cout << "A("<<x<<")\n";}
    virtual ~A() {std::cout << "~A()\n";}
    int mynum() const { return x;}
    virtual void setnum(int n) { x=n;}
};

class B : public A{
	protected:
	int y;
	public:
	B():y(0) {std::cout << "B()\n";}
	B(int _x, int _y): A(_x), y(_y) {std::cout << "B("<<x<<","<<y<<")\n";}
	virtual ~B() {std::cout << "~B()\n";}
	virtual int mynum() const { return y;}
	virtual void setnum(int n) {y = n;}
};

int main()
{
  B* b1 = new B();
  B* b2 = new B(3,8);
  A* a1 = b1;
  A * a2 = b2;
  b1->setnum(99);
  a1->setnum(42);
  std::cout << "a1" << a1->mynum() << "\n";
  std::cout << "a2" << a2->mynum() << "\n";
  std::cout << "b1" << b1->mynum() << "\n";
  std::cout << "b2" << b2->mynum() << "\n";
  delete b1;
  delete a2;
  return 0;
}
