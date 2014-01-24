#pragma once

#include "../phgutils.h"

#define USE_ARMADILLO 0

#if USE_ARMADILLO
#include <armadillo>
#endif
#include <mkl.h>

template <typename T>
class Tensor1
{
public:
	Tensor1(void):n(0),data(nullptr){}
	Tensor1(int n):n(n) { 
		data = new T[n];
		memset(data, 0, sizeof(T)*n);
	}
	Tensor1(const Tensor1& other):
		n(other.n)
	{
		data = new T[n];
		memcpy(data, other.data, sizeof(T)*n);
	}
	Tensor1(Tensor1&& other):
		n(other.n)
	{
		data = other.data;
		other.data = nullptr;
	}
	~Tensor1(){
		if( data != nullptr ) delete[] data;
	}

	Tensor1<T>& operator=(const Tensor1<T>& other) {
		if( this != &other ) {
			n = other.n;
			if( data ) delete[] data;
			data = new T[n];
			memcpy(data, other.data, sizeof(T)*n);
		}
		return (*this);
	}

	Tensor1<T>& operator=(Tensor1<T>&& other) {
		if( this != &other ) {
			n = other.n;
			if( data ) delete[] data;
			data = other.data;
			other.data = nullptr;
		}
		return (*this);
	}

	const T& operator()(int i) const{
		return data[i];
	}
	T& operator()(int i) {
		return data[i];
	}

	int length() const {
		return n;
	}
	void resize(int size){
		n = size;
		if( data ) delete[] data;
		data = new T[n];
		memset(data, 0, sizeof(T)*n);
	}

	T* rawptr() { return data; }
	const T* rawptr() const { return data; }

	void print() const{
		cout << (*this);
	}

	vector<T> toStdVector() const {
		return vector<T>(data, data+n);
	}
private:
	int n;
	T* data;
};

template <typename T>
ostream& operator<<(ostream& os, const Tensor1<T>& t) {
	for(int i=0;i<t.length();i++) {
		os << t(i) << ((i==t.length()-1)?'\n':' ');
	}
	return os;
}

// Order 2 tensor, basically a matrix
template <typename T>
class Tensor2
{
public:
	Tensor2(void):data(nullptr){ d[0] = d[1] = 0; }
	Tensor2(int m, int n){
		d[0] = m; d[1] = n;
		data = new T[m * n];
		memset(data, 0, sizeof(T)*m*n);
	}
	Tensor2(const Tensor2& other){
		d[0] = other.d[0]; d[1] = other.d[1];
		data = new T[d[0] * d[1]];
		memcpy(data, other.data, sizeof(T) * d[0] * d[1]);
	}
	Tensor2(Tensor2&& other){
		d[0] = other.d[0]; d[1] = other.d[1];
		data = other.data;
		other.data = nullptr;
	}
	~Tensor2(){
		if( data ) {
			delete[] data;
		}
	}

	Tensor2<T>& operator=(const Tensor2<T>& other){

		if( this != &other ) {
			d[0] = other.d[0];
			d[1] = other.d[1];
			if( data ) delete[] data;
			data = new T[d[0] * d[1]];
			memcpy(data, other.data, sizeof(T) * d[0] * d[1]);
		}

		return (*this);
	}

	Tensor2<T>& operator=(Tensor2<T>&& other) {
		if( this != &other ) {
			d[0] = other.d[0];
			d[1] = other.d[1];
			if( data ) delete[] data;
			data = other.data;
			other.data = nullptr;
		}
		return (*this);
	}

	const T* operator()(int i) const {
		return data + i * d[1];
	}

	T* operator()(int i) {
		return data + i * d[1];
	}

	const T& operator()(int i, int j) const {
		return data[i * d[1] + j];
	}
	T& operator()(int i, int j) {
		return data[i * d[1] + j];
	}

	int dim(int mid) const{
		return d[mid];
	}

	int rows() { return d[0]; }
	int cols() { return d[1]; }

	void resize(int r, int c){
		d[0] = r;
		d[1] = c;
		if( data ) delete[] data;
		data = new T[r * c];
	}

	int modeProduct(const Tensor1<T>& v, int mid, Tensor1<T>& t1) {
		switch( mid ) {
		case 0:
			{
				// should use mkl
				assert(v.length() == d[0]);
				
				// view the tensor as a matrix stored in column major order
				cblas_sgemv(CblasColMajor, CblasNoTrans, d[1], d[0], 1.0,
					data, d[1], v.rawptr(), 1, 0.0, t1.rawptr(), 1);
				return 0;
			}
		case 1:
			{
				// should use mkl
				assert(v.length() == d[1]);
				
				cblas_sgemv(CblasRowMajor, CblasNoTrans, d[0], d[1], 1.0,
					data, d[1], v.rawptr(), 1, 0.0, t1.rawptr(), 1);
				return 0;
			}
		default:
			{
				throw "invalid mode!";
			}
		}
	}

	Tensor1<T> modeProduct(const Tensor1<T>& v, int mid) {
		switch( mid ) {
		case 0:
			{
				// should use mkl
				assert(v.length() == d[0]);
				Tensor1<T> t1(d[1]);

				/*
				T* t1ptr = t1.rawptr();
				for(int i=0, idx=0;i<d[0];i++) {
					for(int j=0;j<d[1];j++) {
						t1ptr[j] += data[idx++] * v(i);
					}
				}
				*/
				
				// view the tensor as a matrix stored in column major order
				cblas_sgemv(CblasColMajor, CblasNoTrans, d[1], d[0], 1.0,
					data, d[1], v.rawptr(), 1, 0.0, t1.rawptr(), 1); 
					

				return t1;
			}
		case 1:
			{
				// should use mkl
				assert(v.length() == d[1]);
				Tensor1<T> t1(d[0]);

				// naive matrix-vector product
				/*
				for(int i=0, idx=0;i<d[0];i++) {
					for(int j=0;j<d[1];j++) {
						t1(i) += data[idx++] * v(j);
					}
				}
				*/
				
				cblas_sgemv(CblasRowMajor, CblasNoTrans, d[0], d[1], 1.0,
					data, d[1], v.rawptr(), 1, 0.0, t1.rawptr(), 1); 
					
				return t1;
			}
		default:
			{
				throw "invalid mode!";
			}
		}
	}

	Tensor1<T> unfold() const{
		Tensor1<T> t(d[0] * d[1]);

		memcpy(t.rawptr(), data, sizeof(T)*d[0]*d[1]);

		/*
		for(int i=0, idx=0;i<d[0];i++) {
			for(int j=0;j<d[1];j++, idx++) {
				t(idx) = data[idx];
			}
		}
		*/

		return t;
	}

#if USE_ARMADILLO
	// convert the tensor to an armadillo format matrix
	arma::fmat toMat() const {
		arma::fmat m(d[0], d[1]);
		for(int i=0;i<d[0];i++) {
			for(int j=0;j<d[1];j++) {
				m(i, j) = data(i * d[1] + j);
			}
		}
		return m;
	}

	static Tensor2<T> fromMat(const arma::fmat& m) {
		Tensor2<T> t(m.n_rows, m.n_cols);
		for(int i=0;i<t.dim(0);i++) {
			for(int j=0;j<t.dim(1);j++) {
				t(i, j) = m(i, j);
			}
		}
		return t;
	}
#endif

	void print(const string& title = "") const{
		if( !title.empty() ) cout << title << " = " << endl;;
		for(int i=0;i<d[0];i++) {
			int offset = i * d[1];
			for(int j=0;j<d[1];j++) {
				cout << data[offset + j] << ((j==d[1]-1)?'\n':' ');
			}
		}
	}

	T* rawptr() const { return data; }
	template <typename TT>
	friend ostream& operator<<(ostream& os, const Tensor2<TT>& t);

private:
	int d[2];
	
	// row major storage
	T* data;
};

template <typename T>
ostream& operator<<(ostream& os, const Tensor2<T>& t) {
	for(int i=0;i<t.dim(0);i++) {
		os << t.data[i] << endl;
	}
	return os;
}

// Order 3 tensor
template <typename T>
class Tensor3
{
public:
	Tensor3(void){}
	Tensor3(int l, int m, int n){
		d[0] = l; d[1] = m; d[2] = n;
		data.resize(l);
		for_each(data.begin(), data.end(), [=](Tensor2<T>& t){
			t.resize(m, n);
		});
	}
	Tensor3(const Tensor3& other) {
		d[0] = other.d[0]; d[1] = other.d[1]; d[2] = other.d[2];
		data = other.data;
	}
	~Tensor3(void){}

	const Tensor2<T>& operator()(int i) const {
		return data[i];
	}
	Tensor2<T>& operator()(int i) {
		return data[i];
	}

	const Tensor1<T>& operator()(int i, int j) const {
		return data[i](j);
	}
	Tensor1<T>& operator()(int i, int j) {
		return data[i](j);
	}

	const T& operator()(int i, int j, int k) const{
		return data[i](j, k);
	}
	T& operator()(int i, int j, int k) {
		return data[i](j, k);
	}

	void resize(int l, int m, int n) {
		d[0] = l; d[1] = m; d[2] = n;
		data.resize(l);
		for_each(data.begin(), data.end(), [=](Tensor2<T>& t){
			t.resize(m, n);
		});
	}

	int dim(int mid) const{
		return d[mid];
	}

	Tensor2<T> unfold(int mid) const {
		cout << "unfolding tensor in mode " << mid << endl;
		switch( mid ) {
		case 0:{
					Tensor2<T> t(d[0], d[1] * d[2]);
					// fill in the values
					for(int i=0;i<d[0];i++) {
						const Tensor2<T>& ti = data[i];
						Tensor1<T> ti_unfold = ti.unfold();

						// copy every thing into t
						for(int j=0;j<d[1] * d[2];j++) {
							t(i, j) = ti_unfold(j);
						}
					}
					return t;
			   }		
		case 1:{			
					Tensor2<T> t(d[1], d[0] * d[2]);
					// fill in the values
					for(int i=0;i<d[0];i++) {
						// copy everything into t
						for(int j=0;j<d[1];j++) {
							for(int k=0, offset=0;k<d[2];k++, offset+=d[0]) {
								t(j, offset+i) = (*this)(i, j, k);
							}
						}
					}
					return t;
			   }
		case 2: {
					Tensor2<T> t(d[2], d[0] * d[1]);
					// fill in the values
					for(int i=0, offset=0;i<d[0];i++, offset+=d[1]) {
						const Tensor2<T>& ti = data[i];
						for(int j=0;j<d[1];j++) {
							for(int k=0;k<d[2];k++) {
								t(k, j+offset) = ti(j, k);
							}
						}
					}
					return t;
				}		
		default: {
					throw "Invalid mode!";
				 }
		}
		//cout << "done." << endl;
	}

	// fold a order 2 tensor to a order 3 tensor
	static Tensor3<T> fold(const Tensor2<T>& t, int mid, int d0, int d1, int d2) {
		cout << "folding tensor in mode " << mid << endl;
		switch( mid ) {
		case 0:
			{
				// the tensor2 must be of the shape d0 x (d2 x d1)
				Tensor3<T> t3(d0, d1, d2);
				assert(t.dim(0) == d0 && t.dim(1) == d1 * d2);
				for(int i=0;i<d0;i++) {
					const Tensor1<T> ti = t(i);
					for(int j=0, offset=0;j<d1;j++,offset+=d2) {
						for(int k=0;k<d2;k++) {
							t3(i, j, k) = ti(k+offset);
						}
					}
				}
				return t3;
			}
		case 1:
			{
				// the tensor2 must be of the shape d1 x (d0 x d3)
				Tensor3<T> t3(d0, d1, d2);
				assert(t.dim(0) == d1 && t.dim(1) == d0 * d2);
				for(int i=0;i<d0;i++) {
					for(int j=0;j<d1;j++) {
						for(int k=0, offset=0;k<d2;k++, offset+=d0) {
							t3(i, j, k) = t(j, i + offset);
						}
					}
				}
				return t3;
			}
		case 2:
			{
				// the tensor must be of the shape d2 x (d1 x d0)
				Tensor3<T> t3(d0, d1, d2);
				assert(t.dim(0) == d2 && t.dim(1) == d1*d0);
				for(int i=0, offset=0;i<d0;i++, offset+=d1) {
					for(int j=0;j<d1;j++) {
						for(int k=0;k<d2;k++) {
							t3(i, j, k) = t(k, j + offset);
						}
					}
				}
				return t3;
			}
		default:
			{
				throw "Invalid mode!";
			}
		}
	}

	// mode product with a vector
	int modeProduct(const Tensor1<T>& v, int mid, Tensor2<T>& t2) const {
		switch( mid ) {
		case 0:
			{
				assert(v.length() == d[0]);

				for(int i=0;i<d[1];i++) {
					for(int j=0;j<d[2];j++) {
						T val = 0;
						for(int k=0;k<d[0];k++)
							val += data[k](i, j) * v(k);
						t2(i, j) = val;
					}
				}
				return 0;
			}
		case 1:
			{
				assert(v.length() == d[1]);
				for(int i=0;i<d[0];i++) {
					for(int j=0;j<d[2];j++) {
						T val = 0;
						for(int k=0;k<d[1];k++)
							val += data[i](k, j) * v(k);
						t2(i, j) = val;
					}
				}
				return 0;
			}
		case 2:
			{
				assert(v.length() == d[2]);
				for(int i=0;i<d[0];i++) {
					for(int j=0;j<d[1];j++) {
						T val = 0;
						for(int k=0;k<d[2];k++)
							val += data[i](j, k) * v(k);
						t2(i, j) = val;
					}
				}
				return 0;
			}
		default:
			throw "Invalid mode!";
		}
	}

	// mode product with a vector
	Tensor2<T> modeProduct(const Tensor1<T>& v, int mid) const {
		switch( mid ) {
		case 0:
			{
				assert(v.length() == d[0]);
				Tensor2<T> t2(d[1], d[2]);

				#pragma omp parallel for shared(t2)
				for(int i=0;i<d[1];i++) {
					for(int j=0;j<d[2];j++) {
						T val = 0;
						for(int k=0;k<d[0];k++)
							val += data[k](i, j) * v(k);
						t2(i, j) = val;
					}
				}
				return t2;
			}
		case 1:
			{
				assert(v.length() == d[1]);
				Tensor2<T> t2(d[0], d[2]);

				#pragma omp parallel for shared(t2)
				for(int i=0;i<d[0];i++) {
					for(int j=0;j<d[2];j++) {
						T val = 0;
						for(int k=0;k<d[1];k++)
							val += data[i](k, j) * v(k);
						t2(i, j) = val;
					}
				}
				return t2;
			}
		case 2:
			{
				assert(v.length() == d[2]);
				Tensor2<T> t2(d[0], d[1]);

				#pragma omp parallel for shared(t2)
				for(int i=0;i<d[0];i++) {
					for(int j=0;j<d[1];j++) {
						T val = 0;
						for(int k=0;k<d[2];k++)
							val += data[i](j, k) * v(k);
						t2(i, j) = val;
					}
				}
				return t2;
			}
		default:
			throw "Invalid mode!";
		}
	}

	// mode product with a matrix
	Tensor3<T> modeProduct(const Tensor2<T>& M, int mid) const {
		switch( mid ) {
		case 0:
			{
				assert(M.dim(1) == d[0]);
				// unfold the tensor in mode 0
				Tensor2<T> tunfold = this->unfold(0);

				Tensor2<T> t2(M.dim(0), d[1] * d[2]);

				// do the multiplication to each column vector
				Tensor1<T> col(M.dim(0));	// store the result for current column
				Tensor1<T> tcol(d[0]);		// column in the unfolded tensor
				for(int i=0;i<tunfold.dim(1);i++) {
					for(int j=0;j<tunfold.dim(0);j++) {
						tcol(j) = tunfold(j, i);
					}
					for(int r=0;r<M.dim(0);r++) {
						T sum = 0;
						const Tensor1<T>& Mr = M(r);
						for(int c=0;c<M.dim(1);c++) {
							sum += Mr(c) * tcol(c);
						}
						col(r) = sum;
					}
					// copy back the result to tunfold
					for(int j=0;j<M.dim(0);j++) {
						t2(j, i) = col(j);
					}
				}

				// fold the tensor again
				return Tensor3<T>::fold(t2, 0, M.dim(0), d[1], d[2]);
			}
		case 1:
			{
				assert(M.dim(1) == d[1]);
				// unfold the tensor in mode 1
				Tensor2<T> tunfold = this->unfold(1);

				Tensor2<T> t2(M.dim(0), d[0] * d[2]);

				// do the multiplication to each column vector
				Tensor1<T> col(M.dim(0));	// store the result for current column
				Tensor1<T> tcol(d[1]);		// column in the unfolded tensor
				for(int i=0;i<tunfold.dim(1);i++) {
					for(int j=0;j<tunfold.dim(0);j++) {
						tcol(j) = tunfold(j, i);
					}
					for(int r=0;r<M.dim(0);r++) {
						T sum = 0;
						const Tensor1<T>& Mr = M(r);
						for(int c=0;c<M.dim(1);c++) {
							sum += Mr(c) * tcol(c);
						}
						col(r) = sum;
					}
					// copy back the result to tunfold
					for(int j=0;j<M.dim(0);j++) {
						t2(j, i) = col(j);
					}
				}

				return Tensor3<T>::fold(t2, 1, d[0], M.dim(0), d[2]);
			}
		case 2:
			{
				assert(M.dim(1) == d[2]);
				// unfold the tensor in mode 2
				Tensor2<T> tunfold = this->unfold(2);

				Tensor2<T> t2(M.dim(0), d[1] * d[0]);

				// do the multiplication to each column vector
				Tensor1<T> col(M.dim(0));	// store the result for current column
				Tensor1<T> tcol(d[2]);		// column in the unfolded tensor
				for(int i=0;i<tunfold.dim(1);i++) {
					for(int j=0;j<tunfold.dim(0);j++) {
						tcol(j) = tunfold(j, i);
					}
					for(int r=0;r<M.dim(0);r++) {
						T sum = 0;
						const Tensor1<T>& Mr = M(r);
						for(int c=0;c<M.dim(1);c++) {
							sum += Mr(c) * tcol(c);
						}
						col(r) = sum;
					}
					// copy back the result to tunfold
					for(int j=0;j<M.dim(0);j++) {
						t2(j, i) = col(j);
					}
				}

				return Tensor3<T>::fold(t2, 2, d[0], d[1], M.dim(0));
			}
		default:
			{
				throw "Invalid mode!";
			}
		}
	}

#if USE_ARMADILLO
	// svd on certain modes, with truncation
	tuple<Tensor3<T>, vector<Tensor2<T> > > svd(
			const vector<int>& modes,	// modes to perform svd
			const vector<int>& dims		// truncated dimensions
		) const
	{
		vector<arma::fmat> U, V;
		vector<arma::fvec> s;

		for(int i=0;i<modes.size();i++) {
			int mid = modes[i];
			cout << "svd on mode " << mid << endl;
			// unfold in mode mid
			Tensor2<T> t2 = this->unfold(mid);
			// convert to armadillo matrix
			arma::fmat m2 = t2.toMat();

			// compute svd
			arma::fmat ui, vi;
			arma::fvec si;
			arma::svd_econ(ui, si, vi, m2, 'l');
			cout << "done." << endl;
			// store the svd results
			U.push_back(ui); V.push_back(vi); s.push_back(si);
		}

		// decompose the tensor, with truncation
		Tensor3<T> core = (*this);
		vector<Tensor2<T>> tu;
		for(int i=0;i<modes.size();i++) {
			int mid = modes[i];
			arma::fmat u_truncated = U[i].submat(arma::span::all, arma::span(0, dims[i]-1));
			Tensor2<T> tui = Tensor2<T>::fromMat( u_truncated );
			Tensor2<T> tuit = Tensor2<T>::fromMat( arma::trans(u_truncated) );

			core = core.modeProduct(tuit, mid);
			tu.push_back(tui);
		}

		return make_tuple(core, tu);
	}

	tuple<Tensor3<T>, Tensor2<T>, Tensor2<T>, Tensor2<T>> svd() const {
		// unfold the tensor in mode 0 and compute the svd of the unfolded matrix
		Tensor2<T> t20 = this->unfold(0);
		arma::fmat m20 = t20.toMat();
		// compute svd
		arma::fmat U0, V0;
		arma::fvec s0;
		arma::svd_econ(U0, s0, V0, m20, 'l');

		// unfold the tensor in mode 1 and compute the svd of the unfolded matrix
		Tensor2<T> t21 = this->unfold(1);
		arma::fmat m21 = t21.toMat(); 
		// compute svd
		arma::fmat U1, V1;
		arma::fvec s1;
		arma::svd_econ(U1, s1, V1, m21, 'l');

		// unfold the tensor in mode 2 and compute the svd of the unfolded matrix
		Tensor2<T> t22 = this->unfold(2);
		arma::fmat m22 = t22.toMat(); 
		// compute svd
		arma::fmat U2, V2;
		arma::fvec s2;
		arma::svd_econ(U2, s2, V2, m22, 'l');

		Tensor2<T> tu0 = Tensor2<T>::fromMat( (U0) );
		Tensor2<T> tu0t = Tensor2<T>::fromMat( arma::trans(U0) );
		Tensor2<T> tu1 = Tensor2<T>::fromMat( (U1) );
		Tensor2<T> tu1t = Tensor2<T>::fromMat( arma::trans(U1) );
		Tensor2<T> tu2 = Tensor2<T>::fromMat( (U2) );
		Tensor2<T> tu2t = Tensor2<T>::fromMat( arma::trans(U2) );

		// core tensor is then defined as T x0 U(0)' x0 U(1)'
		Tensor3<T> core(d[0], d[1], d[2]);
		core = this->modeProduct(tu0t, 0).modeProduct(tu1t, 1).modeProduct(tu2t, 2);

		// return the core, U(0) and U(1)
		return make_tuple(core, tu0, tu1, tu2);
	}
#endif


	void print(const string& title="") {
		if( !title.empty() )
			cout << title << " = " << endl;
		for(int i=0;i<d[0];i++) {
			data[i].print();
		}
	}

	bool read(const string& filename) {
		try {
			cout << "Reading tensor file " << filename << endl;
			fstream fin;
			fin.open(filename, ios::in | ios::binary);

			fin.read(reinterpret_cast<char*>(&(d[0])), sizeof(int)*3);

			this->resize(d[0], d[1], d[2]);

			for(int i=0;i<d[0];i++) {
				Tensor2<T>& ti = data[i];
				for(int j=0;j<d[1];j++) {
					fin.read(reinterpret_cast<char*>(ti(j)), sizeof(T)*d[2]);
				}				
			}

			fin.close();

			cout << "done." << endl;

			return true;
		}
		catch( ... ) {
			cerr << "Failed to read tensor from file " << filename << endl;
			return false;
		}
	}

	bool write(const string& filename) {
		try {
			cout << "writing tensor to file " << filename << endl;
			fstream fout;
			fout.open(filename, ios::out | ios::binary);

			fout.write(reinterpret_cast<char*>(&(d[0])), sizeof(int)*3);

			for(int i=0;i<d[0];i++) {
				const Tensor2<T>& ti = data[i];
				for(int j=0;j<d[1];j++) {
					fout.write(reinterpret_cast<const char*>(ti(j)), sizeof(T)*d[2]);
				}				
			}

			fout.close();

			cout << "done." << endl;
			return true;
		}
		catch(...) {
			cerr << "Failed to write tensor to file " << filename << endl;
			return false;
		}
	}

private:
	// mode 0, mode 1, mode 2
	int d[3];
	vector<Tensor2<T>> data;
};