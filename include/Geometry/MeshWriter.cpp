#include "MeshWriter.h"
#include "../Utils/Timer.h"
#include "Mesh.h"

bool PhGUtils::OBJWriter::save(const QuadMesh& mesh, const string& filename)
{
	try {
		Timer t;
		t.tic();

		stringstream ss;

		auto verts = mesh.v;
		auto faces = mesh.f;

		// write the vertices
		for(int i=0;i<mesh.vertCount();i++) {
			ss << "v " << mesh.v[i] << '\n';
		}

		// write the texture coordinates
		for(int i=0;i<mesh.t.size();i++) {
			ss << "vt " << mesh.t[i] << '\n';
		}

		// write the faces
		for(int i=0;i<mesh.f.size();i++) {
			Point4i fv = mesh.f[i] + Point4i(1, 1, 1, 1);
			Point4i ft = (mesh.ft[i]==0)?mesh.ft[i]:mesh.ft[i] + Point4i(1, 1, 1, 1);
			Point4i fn = (mesh.fn[i]==0)?mesh.fn[i]:mesh.fn[i] + Point4i(1, 1, 1, 1);
			ss << "f "
			   << fv.x << '/' << ft.x << '/' << fn.x << ' '
			   << fv.y << '/' << ft.y << '/' << fn.y << ' '
			   << fv.z << '/' << ft.z << '/' << fn.z << ' '
			   << fv.w << '/' << ft.w << '/' << fn.w << '\n';
		}

		ofstream fout(filename);
		fout << ss.str();
		fout.close();

		t.toc();
		cout << "finish saving file " << filename << endl;
		cout << "saving time = " << t.gap() << endl;
		return true;
	}
	catch(exception e) {
		cerr << e.what() << endl;
		return false;
	}
}

bool PhGUtils::OBJWriter::save(const QuadMesh& mesh, const vector<int> &selectedFaces, const string& filename)
{
  try {
    Timer t;
    t.tic();

    stringstream ss;

    auto verts = mesh.v;
    auto faces = mesh.f;

    // write the vertices
    for (int i = 0; i < mesh.vertCount(); i++) {
      ss << "v " << mesh.v[i] << '\n';
    }

    // write the texture coordinates
    for (int i = 0; i < mesh.t.size(); i++) {
      ss << "vt " << mesh.t[i] << '\n';
    }

    // write the faces
    for ( auto i : selectedFaces ) {
      Point4i fv = mesh.f[i] + Point4i(1, 1, 1, 1);
      Point4i ft = (mesh.ft[i] == 0) ? mesh.ft[i] : mesh.ft[i] + Point4i(1, 1, 1, 1);
      Point4i fn = (mesh.fn[i] == 0) ? mesh.fn[i] : mesh.fn[i] + Point4i(1, 1, 1, 1);
      ss << "f "
        << fv.x << '/' << ft.x << '/' << fn.x << ' '
        << fv.y << '/' << ft.y << '/' << fn.y << ' '
        << fv.z << '/' << ft.z << '/' << fn.z << ' '
        << fv.w << '/' << ft.w << '/' << fn.w << '\n';
    }

    ofstream fout(filename);
    fout << ss.str();
    fout.close();

    t.toc();
    cout << "finish saving file " << filename << endl;
    cout << "saving time = " << t.gap() << endl;
    return true;
  }
  catch (exception e) {
    cerr << e.what() << endl;
    return false;
  }
}

bool PhGUtils::OBJWriter::save(const TriMesh& mesh, const string& filename)
{
	try {
		Timer t;
		t.tic();

		stringstream ss;

		auto verts = mesh.v;
		auto faces = mesh.f;

		// write the vertices
		for(int i=0;i<mesh.vertCount();i++) {
			ss << "v " << mesh.v[i] << '\n';
		}

		// write the texture coordinates
		for(int i=0;i<mesh.t.size();i++) {
			ss << "vt " << mesh.t[i] << '\n';
		}

		// write the faces
		for(int i=0;i<mesh.f.size();i++) {
			Point3i fv = mesh.f[i] + Point3i(1, 1, 1);
			Point3i ft = mesh.ft[i]==0?mesh.ft[i]:mesh.ft[i] + Point3i(1, 1, 1);
			Point3i fn = mesh.fn[i]==0?mesh.fn[i]:mesh.fn[i] + Point3i(1, 1, 1);

			ss << "f "
				<< fv.x << '/' << ft.x << '/' << fn.x << ' '
				<< fv.y << '/' << ft.y << '/' << fn.y << ' '
				<< fv.z << '/' << ft.z << '/' << fn.z << '\n';
		}

		ofstream fout(filename);
		fout << ss.str();
		fout.close();

		t.toc();
		cout << "finish saving file " << filename << endl;
		cout << "saving time = " << t.gap() << endl;
		return true;
	}
	catch(exception e) {
		cerr << e.what() << endl;
		return false;
	}
}
