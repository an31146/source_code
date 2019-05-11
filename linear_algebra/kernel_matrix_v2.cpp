
// g++.exe -Wall -O2 -std=c++11 kernel_matrix_v2.cpp -o kernel_matrix_v2.exe

#include <iostream>
#include <cmath>
#include <ctime>
#include <random>
#include <cstring>

using namespace std;

template<typename T> // See section on templates for more
class Matrix {
public: 
	Matrix(unsigned nrows, unsigned ncols); 
	Matrix(void *array, unsigned nrows, unsigned ncols);

	// Throws a BadSize object if either size is zero 
	class BadSize { }; 

	// Based on the Law Of The Big Three: 
	~Matrix(); 
	Matrix(const Matrix<T>& m); 
	Matrix<T>& operator= (const Matrix<T>& m); 

	// Access methods to get the (i,j) element: 
	T& operator() (unsigned i, unsigned j); 

	// Subscript operators often come in pairs 
	const T& operator() (unsigned i, unsigned j) const; 

	// Return dimensions
	const unsigned rows() { return nrows_; }
	const unsigned cols() { return ncols_; }
	
	// These throw a BoundsViolation object if i or j is too big 
	class BoundsViolation { };

private: 
	unsigned nrows_, ncols_; T* data_;
};

template<typename T>
inline T& Matrix<T>::operator() (unsigned row, unsigned col)
{ 
	if (row >= nrows_ || col >= ncols_) 
		throw BoundsViolation(); 
	return data_[row*ncols_ + col];
}
template<typename T>
inline const T& Matrix<T>::operator() (unsigned row, unsigned col) const
{ 
	if (row >= nrows_ || col >= ncols_) 
		throw BoundsViolation(); 
	return data_[row*ncols_ + col];
}
template<typename T>
inline Matrix<T>::Matrix(unsigned nrows, unsigned ncols) : nrows_ (nrows) , ncols_ (ncols)
//, data_ ← initialized below after the if...throw statement
{ 
	if (nrows == 0 || ncols == 0) 
		throw BadSize();
	data_ = new T[nrows * ncols];
	::memset(data_, 0, sizeof(T)*nrows*ncols);
}
template<typename T>
inline Matrix<T>::Matrix(void *array, unsigned nrows, unsigned ncols) : nrows_ (nrows) , ncols_ (ncols)
{
	if (nrows == 0 || ncols == 0) 
		throw BadSize();
	data_ = new T[nrows * ncols];
	::memcpy(data_, array, sizeof(T)*nrows*ncols);
}
template<typename T>
inline Matrix<T>::~Matrix()
{ 
	delete[] data_;
}
template<typename T>
inline Matrix<T>& Matrix<T>::operator= (const Matrix<T>& m)
{
	delete[] data_;
	data_ = new T[m.nrows_ * m.ncols_];
	::memcpy(data_, m.data_, sizeof(T)*m.nrows_*m.ncols_);
	nrows_ = m.nrows_;
	ncols_ = m.ncols_;
}

/*
#define SIZE 6
int A[SIZE][SIZE] = { {1,1,0,0,1,1},
					  {0,1,0,0,1,1},
					  {1,0,1,0,0,0},
					  {0,0,1,0,0,0},
					  {0,0,0,0,0,0},
					  {0,0,0,1,0,1} };
Matrix<int> M = Matrix<int>(A, SIZE, SIZE);
*/

#define DEBUG 0
                            
#define SIZE 100
int A[SIZE][SIZE] = { {1,0,1,0,0,0,0,1,0,0},
					  {0,0,0,0,1,0,0,0,0,1},
					  {1,0,0,0,0,1,1,0,0,0},
					  {0,1,1,0,0,0,0,1,0,0},
					  {0,0,0,0,0,0,0,0,0,0},
					  {1,0,1,1,0,1,0,0,0,0},
					  {0,0,1,0,0,0,1,0,0,0},
					  {0,0,0,1,0,1,0,0,1,0},
					  {1,1,0,0,0,0,0,0,0,0},
					  {1,0,0,1,1,0,0,0,1,0} };
Matrix<int> M = Matrix<int>(A, SIZE, SIZE);


void gauss(Matrix<int>& m)
{
	int swaps = 0, adds = 0;
    for (unsigned p=0; p<m.rows(); p++)
    {
        // find pivot row and swap 
        for (unsigned i=p+1; i<m.rows(); i++)
        {
            if (m(i,p) > m(p,p)) 
            {
#if DEBUG                
                printf("Swap rows: %d and %d\n", p, i);
#endif                
				swaps++;
                for (unsigned j=0; j<m.cols(); j++)
                {
                    int t = m(i,j);
                    m(i,j) = m(p,j);
                    m(p,j) = t;
                }
            }
        
            if (m(i,p))
            {
#if DEBUG                
                printf("Add row: %d to row: %d\r", p, i);
#endif                
                adds++;
                for (unsigned j=0; j<m.cols(); j++)
                {
                    m(i,j) ^= m(p,j);
                }
            }
        } // for i
#if DEBUG                
        printf("\n"); fflush(stdout);
#endif        
    } // for p
    printf("row swaps: %d\nrow adds: %d\n", swaps, adds);
    printf("press Enter: ");
    getchar();
}

void dump(Matrix<int>& m)
{
	cout << "    ";
	for (unsigned j=0; j<m.cols()/10; j++)
	{
		cout << j*10 << "                  ";
	}
	cout << endl;
    for (unsigned i=0; i<m.rows(); i++)
    {
        cout << (i<10?" ":"") << i << ": ";
        for (unsigned j=0; j<m.cols(); j++)
            cout << m(i, j) << " ";
        cout << endl;
    }
    for (unsigned i=0; i<m.cols(); i++)
        cout << "--";
    cout << "---\n";
}

void augment(Matrix<int>& m)
{
	Matrix<int> n = Matrix<int>(m.rows(), m.cols()+m.rows());
	
	for (unsigned i=0; i<m.rows(); i++)
	{
		for (unsigned j=0; j<m.cols(); j++)
			n(i,j) = m(i,j);
			
		for (unsigned j=0; j<m.rows(); j++)
			if (i == j)
				n(i,j+m.cols()) = 1;
	}
	m = n;
}

void initial(Matrix<int>& m)
{
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution d(0.5f);
    
    gen.seed(time(NULL));
	m = Matrix<int>(SIZE*2, SIZE);

	for (unsigned i=0; i<m.rows(); i++)
		for (unsigned j=0; j<m.cols(); j++)
			//m(i,j) = rand() / 3 & 1;
			m(i,j) = d(gen);
}

int main(int argc, char *argv[])
{
	initial(M);
	augment(M);
#if DEBUG                
    dump(M);
    gauss(M);
    dump(M);
#else    
    gauss(M);
#endif    
    return 0;
}
