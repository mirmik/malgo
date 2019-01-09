#include <malgo/phasestate.h>
#include <nos/print.h>

int main()
{
//A = np.mat('''	0 1;
//				-1 -1''') # собственная матрица
//B = np.mat('''	0;
//				-1''') # входная матрица
//C = np.mat('''  1 0''') # выходная матрица
//D = np.mat('''0''') # проходная матрица
//x0 = np.mat("0; 0") # начальное состояние


	malgo::matrix<double> A {{0,-1},{1,-1}};
	malgo::matrix<double> B {{0,-1}};
	malgo::matrix<double> C {{1},{0}};
	malgo::matrix<double> D {{0}};

	malgo::transfer_function<double> W(A,B,C,D,0.004);

	nos::println(W);
}