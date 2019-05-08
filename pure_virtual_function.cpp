/* Compile options needed: none
*/ 

class A;

void fcn( A* );

class A
{
public:
   virtual void f() = 0;
   A() { fcn( this ); }
};

class B : A
{
   void f() { }
};

void fcn( A* p )
{
   p->f();
}

// The declaration below invokes class B's constructor, which
// first calls class A's constructor, which calls fcn. Then
// fcn calls A::f, which is a pure virtual function, and
// this causes the run-time error. B has not been constructed
// at this point, so the B::f cannot be called. You would not
// want it to be called because it could depend on something
// in B that has not been initialized yet.

B b;

int main(void)
{
    return 0;
}
