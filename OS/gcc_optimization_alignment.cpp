#include <iostream>

const int LOOP_BOUND = 1000000000;

__attribute__((noinline))
static int add(const int& x, const int& y) {
    return x + y;
}

__attribute__((noinline))
static int work(int xval, int yval) {
    int sum(0);
    for (int i=0; i<LOOP_BOUND; ++i) {
        int x(xval+sum);
        int y(yval+sum);
        int z = add(x, y);
        sum += z;
    }
    return sum;
}

int main(int argc, char* argv[]) {
    if (argc != 3)
    {
	std::cout << "Usage: " << argv[0] << " <int0> <int1>" << std::endl;
	return -1;
    }
    int result = work(*argv[1], *argv[2]);
    return result;
}
