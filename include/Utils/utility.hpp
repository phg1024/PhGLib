#pragma once

#include "../phgutils.h"
#include <typeinfo>
#include <QImage>

namespace PhGUtils {

	struct param_t {
		template <typename T>
		T getValue() {
			T* vaddr = reinterpret_cast<T*>(&val);
			return (*vaddr);
		};

		float getValue() const {
			switch( type ) {
			case BOOL:
				return val.b_val;
			case UNSIGNED:
				return val.uc_val;
			case INT:
				return val.i_val;
			case LONG:
				return val.l_val;
			case FLOAT:
				return val.f_val;
			case DOUBLE:
				return val.d_val;
			}
		}

		enum ParamType {
			BOOL = 0,
			UNSIGNED,
			CHAR,
			INT,
			LONG,
			FLOAT,
			DOUBLE,
			UNKNOWN
		};

		static ParamType string2type(const string& str) {
			string s = str;
			std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun(::tolower));

			if( s == "bool" ) {
				return BOOL;
			}
			else if( s == "unsigned char" ) {
				return UNSIGNED;
			}
			else if( s == "char" ) {
				return CHAR;
			}
			else if( s == "int" ) {
				return INT;
			}
			else if( s == "long" ) {
				return LONG;
			}
			else if( s == "float" ) {
				return FLOAT;
			}
			else if( s == "double" ) {
				return DOUBLE;
			}
			else {
				return UNKNOWN;
			}
		}

		void parseValue(ParamType t, const string& str) {
			stringstream ss;
			ss << str;
			type = t;
			switch( t ) {
			case BOOL:
				ss >> val.b_val;
				break;
			case UNSIGNED:
				ss >> val.uc_val;
				break;
			case INT:
				ss >> val.i_val;
				break;
			case LONG:
				ss >> val.l_val;
				break;
			case FLOAT:
				ss >> val.f_val;
				break;
			case DOUBLE:
				ss >> val.d_val;
				break;
			}
		}

		void setValue(long v) {	val.l_val = v; }
		void setValue(double v) { val.d_val = v; }
		void setValue(float v) { val.f_val = v; }
		void setValue(int v) { val.i_val = v; }
		void setValue(bool v) { val.b_val = v; }
		void setValue(unsigned char v) { val.uc_val = v; }
		void setValue(char v) { val.c_val = v; }

		union{
			bool	b_val;
			unsigned char uc_val;
			char	c_val;
			int		i_val;
			long	l_val;
			float	f_val;
			double	d_val;
		} val;

		ParamType type;
	};

	static vector<unsigned char> fromQImage(const string& filename) {
		vector<unsigned char> pixmap;
		QImage img(filename.c_str());

		int w = img.width(), h = img.height();

		pixmap.resize(w * h * 4);

		for(int i=0, idx=0;i<h;i++) {
			for(int j=0;j<w;j++,idx+=4)
			{
				QRgb pix = img.pixel(j, i);
				unsigned char r = qRed(pix);
				unsigned char g = qGreen(pix);
				unsigned char b = qBlue(pix);
				unsigned char a = qAlpha(pix);
				pixmap[idx] = b;
				pixmap[idx+1] = g;
				pixmap[idx+2] = r;
				pixmap[idx+3] = a;
			}
		}
		return pixmap;
	}

	// assumes BGR format
	static QImage toQImage(const unsigned char* data, int w, int h) {
		QImage qimg(w, h, QImage::Format_ARGB32);
		for(int i=0, idx=0;i<h;i++) {
			for(int j=0;j<w;j++, idx+=4)
			{
				unsigned char r = data[idx+2];
				unsigned char g = data[idx+1];
				unsigned char b = data[idx];
				unsigned char a = 255;
				QRgb qp = qRgba(r, g, b, a);
				qimg.setPixel(j, i, qp);
			}
		}
		return qimg;
	}

	template <typename T>
	T sum(const vector<T>& v) {
		T res = 0;
		for_each(v.begin(), v.end(), [&](T x) {
			res += x;
		});
		return res;
	}

	template <typename T>
	double average(const vector<T>& v) {
		return sum(v)/(double)v.size();
	}

	// misc
	template <typename T> 
	void printVector(T A, ostream& os = cout) {
		for(int i=0;i<A.size();i++)
			os << A[i] << ' ';
		os << endl;
	}

	template <typename T>
	void printArray(T* A, int N, ostream& os = cout)
	{
		for(int i=0;i<N;i++)
			os << A[i] << ' ';
		os << endl;
	}

	template <typename T>
	void print2DArray(T** A, int rows, int cols, ostream& os = cout)
	{
		for(int i=0;i<rows;i++)
		{
			for(int j=0;j<cols;j++)
			{
				os << A[i][j] << ' ';
			}
			os << endl;
		}
	}

	template <typename T>
	void print2DArray(T* A, int rows, int cols, ostream& os = cout)
	{
		for(int i=0, idx=0;i<rows;i++)
		{
			for(int j=0;j<cols;j++)
			{
				os << A[idx++] << ' ';
			}
			os << endl;
		}
	}

	template <typename T>
	void dump2DArray(T* A, int rows, int cols, ostream& os = cout) {
		os.write(reinterpret_cast<const char*>(A), sizeof(T)*rows*cols);
	}

	// debugging related
	// dummy
	static void debug(){}

	template <typename T>
	void debug(const string& name, T value)
	{
#ifdef _DEBUG
		cout << name << " = " << value << endl;
#endif
	}

#if 0
	// with variadic template support
	template <typename T, typename ...Args>
	void debug(const string& name, T value, Args ...args)
	{
#ifdef _DEBUG
		cout << name << " = " << value << "\t";
		debug(args...);
#endif
	}
#else
	// without variadic template support
	template <typename T1, typename T2>
	void debug(const string& name1, T1 value1, const string& name2, T2 value2)
	{
#ifdef _DEBUG
		cout << name1 << " = " << value1 << "\t" 
			 << name2 << " = " << value2 << endl;
#endif
	}

	template <typename T1, typename T2, typename T3>
	void debug(const string& name1, T1 value1, const string& name2, T2 value2, const string& name3, T3 value3)
	{
#ifdef _DEBUG
		cout << name1 << " = " << value1 << "\t" 
			 << name2 << " = " << value2 << "\t"
			 << name3 << " = " << value3
			 << endl;
#endif
	}
#endif

	// general console message
	inline void debugmessage(const string& msg) {
#ifdef _DEBUG
		cout << msg << endl;
#endif
	}

	inline void message(const string& msg) {
		cout << msg << endl;
	}

	inline void error(const string& msg) {
		cerr << "Error:\t" << msg << endl;
	}

	inline void fail(const string& msg) {
		cerr << "CRITICAL ERROR:\t" << msg << endl;
	}

	inline void abort(const string& msg) {
		cerr << "FATAL ERROR:\t" << msg << endl;
		cerr << "Abort." << endl;
		system("pause");
		exit(0);
	}

	// exception related
	template <typename T>
	class ExceptionBase : public exception
	{
	public:
		ExceptionBase(const string& str = ""):exception(){
			if( !str.empty() )
				msg = string(typeid(T).name()) + " :: " + str;
			else
				msg = string(typeid(T).name());
		}

		virtual const char* what() const throw()
		{
			return msg.c_str();
		}

	private:
		string msg;
	};

	struct LazyProgrammerException{};
	typedef ExceptionBase<LazyProgrammerException> lazy_exception;

}