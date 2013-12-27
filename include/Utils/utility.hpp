#pragma once

#include "../phgutils.h"
#include <typeinfo>
#include <QImage>

namespace PhGUtils {
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

	// debugging related
	// dummy
	static void debug(){}

	template <typename T>
	void debug(const string& name, T value)
	{
		cout << name << " = " << value << endl;
	}

#if 0
	// with variadic template support
	template <typename T, typename ...Args>
	void debug(const string& name, T value, Args ...args)
	{
		cout << name << " = " << value << "\t";
		debug(args...);
	}
#else
	// without variadic template support
	template <typename T1, typename T2>
	void debug(const string& name1, T1 value1, const string& name2, T2 value2)
	{
		cout << name1 << " = " << value1 << "\t" 
			 << name2 << " = " << value2 << endl;
	}

	template <typename T1, typename T2>
	void debug(const string& name1, T1 value1, const string& name2, T2 value2, const string& name3, T2 value3)
	{
		cout << name1 << " = " << value1 << "\t" 
			 << name2 << " = " << value2 << "\t"
			 << name3 << " = " << value3
			 << endl;
	}
#endif

	// general console message
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