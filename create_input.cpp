#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    ofstream out_file1 ("sum_of_divisors.txt", ofstream::app);
    
    out_file1 << "1993" << endl;
    for (int i=1; i<500000; i+=251)
        out_file1 << i << endl;
    
    out_file1.close();
    
    return 0;
}
