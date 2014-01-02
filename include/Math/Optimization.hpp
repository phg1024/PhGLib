#include "../phgutils.h"
#include "mkl.h"

namespace PhGUtils {
	template <typename T>
	int GaussNewton(
		void (*func)(T *x, T *r, int m, int n, void *adata),
		void (*jacf)(T *x, T *J, int m, int n, void *adata),
		T *x, T *r, T* J, int m, int n, int itmax, 
		T *opts,	/* delta,  r_threshold, diff_threshold */
		void *adata)
	{
		float delta, R_THRES, DIFF_THRES;
		if( opts == NULL ) {
			// use default values
			delta = 1.0;	// step size, default to use standard Newton-Ralphson
			R_THRES = 1e-6;	DIFF_THRES = 1e-6;
		}
		else {
			delta = opts[0]; R_THRES = opts[1]; DIFF_THRES = opts[2];
		}

		bool allocateR = false, allocateJ = false;
		// residue
		if( r == NULL ) {
			// allocate space for residue
			allocateR = true;
			r = new T[n];
		}

		T* x0 = new T[m];
		memset(x0, 0, sizeof(T)*m);

		T* deltaX = new T[m];	// also for Jtr
		cblas_scopy(m, x, 1, deltaX, 1);

		T* JtJ = new T[m * m];

		// Jacobian
		if( J == NULL ) {
			allocateJ = true;
			J = new T[m * n];
		}

		// compute initial residue
		func(x, r, m, n, adata);

		int iters = 0;

		//printArray(x, m);
		//printArray(r, n);

		// do iteration
		while( (cblas_snrm2(m, deltaX, 1) > DIFF_THRES && cblas_snrm2(n, r, 1) > R_THRES && iters < itmax) || iters < 1 ) {
			// compute Jacobian
			jacf(x, J, m, n, adata);

			// store old value
			cblas_scopy(m, x, 1, x0, 1);

			//ofstream fout1("J.txt");
			//print2DArray(J, n, m, fout1);
			//fout1.close();

			// compute JtJ
			cblas_ssyrk (CblasColMajor, CblasUpper, CblasNoTrans, m, n, 1.0, J, m, 0, JtJ, m);

			//ofstream fout("JtJ.txt");
			//print2DArray(JtJ, m, m, fout);
			//fout.close();

			// compute Jtr
			cblas_sgemv (CblasColMajor, CblasNoTrans, m, n, 1.0, J, m, r, 1, 0, deltaX, 1);
			
			// compute deltaX
			LAPACKE_spotrf( LAPACK_COL_MAJOR, 'U', m, JtJ, m );
			LAPACKE_spotrs( LAPACK_COL_MAJOR, 'U', m, 1, JtJ, m, deltaX, m );
			
			// update x
			cblas_saxpy(m, -delta, deltaX, 1, x, 1);

			// update residue
			func(x, r, m, n, adata);

			//printArray(x, m);
			//system("pause");
			iters++;
		}

		// delete workspace
		delete[] x0;
		delete[] deltaX;
		delete[] JtJ;

		if( allocateR ){ delete[] r;}
		if( allocateJ ){ delete[] J;}

		return iters;
	}

}