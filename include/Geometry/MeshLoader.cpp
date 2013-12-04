#include "MeshLoader.h"
#include "utility.hpp"
#include "stringutils.h"
#include "fileutils.h"
#include "../IO/FileMapper.h"
#include "../Utils/Timer.h"

namespace PhGUtils {
void MeshLoader::clear() {
	verts.clear();
	faces.clear();
	texcoords.clear();
	normals.clear();

	verts.reserve(131076);
	normals.reserve(131076);
	texcoords.reserve(131076);
	faces.reserve(131076*2);
}

void MeshLoader::triangulate(){
	cout << "Triangulating the mesh ..." << endl;
	vector<face_t> newFaces;

	for(size_t i=0;i<faces.size();i++)
	{
		const face_t& Fi = faces[i];

		if( Fi.v.size() > 3 )
		{
			// triangulate this face

			for(size_t j=1;j<Fi.v.size()-1;j++)
			{
				face_t f;
				f.v.push_back(Fi.v[0]);
				f.v.push_back(Fi.v[j]);
				f.v.push_back(Fi.v[j+1]);

				f.t.push_back(Fi.t[0]);
				f.t.push_back(Fi.t[j]);
				f.t.push_back(Fi.t[j+1]);

				f.n.push_back(Fi.n[0]);
				f.n.push_back(Fi.n[j]);
				f.n.push_back(Fi.n[j+1]);

				newFaces.push_back(f);
			}
		}
		else
		{
			newFaces.push_back(Fi);
		}
	}

	faces = newFaces;

	triangulated = true;

	hasVertexNormal = false;
	hasVertexTexCoord = false;

	cout << "done.";
}

void MeshLoader::estimateNormals()
{
	if( hasVertexNormal )
	{
		cout << "already has vertex normal ..." << endl;
		// only estimate face normal
		for(size_t i=0;i<faces.size();i++)
		{
			Point3f p0, p1, p2;

			p0 = verts[faces[i].v[0]];
			p1 = verts[faces[i].v[1]];
			p2 = verts[faces[i].v[2]];

			norm_t n = Vector3f(p1, p0).cross(Vector3f(p1, p2));
			n.normalize();

			faces[i].normal = n;
		}
	}
	else
	{
		normals.resize(verts.size());
		for(size_t i=0;i<verts.size();i++)
			normals[i] = norm_t(0, 0, 0);

		// for each face, compute its normal
		// add the contribution to its vertices
		for(size_t i=0;i<faces.size();i++)
		{

			Point3f p0, p1, p2;

			p0 = verts[faces[i].v[0]];
			p1 = verts[faces[i].v[1]];
			p2 = verts[faces[i].v[2]];

			norm_t n = Vector3f(p1, p0).cross(Vector3f(p1, p2));
			n.normalize();

			faces[i].normal = n;

			for(size_t j=0;j<faces[i].v.size();j++)
			{
				int pidx, nidx, idx;
				idx = j;
				pidx = j-1;
				if( pidx < 0 ) pidx += faces[i].v.size();
				nidx = j+1;
				if( nidx > faces[i].v.size() - 1 ) nidx -= faces[i].v.size();

				Point3f vp, vc, vn;
				vp = verts[faces[i].v[pidx]];
				vc = verts[faces[i].v[idx]];
				vn = verts[faces[i].v[nidx]];

				Vector3f e1(vc, vp), e2(vc, vn);

				float theta = e1.dot(e2) / (e1.norm() * e2.norm());

				normals[faces[i].v[idx]] += theta * n;
			}
		}

		for(size_t i=0;i<normals.size();i++)
			normals[i].normalize();
	}
}

bool OBJLoader::load(const string& filename) {
	try{
		Timer t;
		t.tic();

		FileMapper fm(filename);

		if( !fm.map() ){
			cerr << "Could not open file " << filename << ". Abort." << endl;
			return false;
		}

		// create a stringstream with the buffer
		stringstream fin(string(fm.buffer()));

		clear();

		triangulated = true;

		while( fin )
		{
			string line;
			getline(fin, line);

			istringstream sline(line);

			string identifier;
			sline >> identifier;

			//cout << identifier << endl;
			//cout << line << endl;

			if( identifier == "v" )
			{
				//cout << "vertex" << endl;
				vert_t p;
				sline >> p;
				//cout << p << endl;
				verts.push_back( p );
			}
			else if( identifier == "f" )
			{
				//cout << "face" << endl;
				face_t f;
				string vstr;
				int vidx, vtidx, vnidx;
				while( sline >> vstr )
				{
					stringlist vlist;
					/// obj file starts indexing vertices from 1

					vlist = split(vstr, "/");

					auto vit = vlist.begin();

					vidx = atoi((*vit).c_str());
					vit++;
					if( vidx < 0 ) vidx = -vidx;
					f.v.push_back(vidx - 1);

					if( vit != vlist.end() )
					{
						vtidx = atoi((*vit).c_str());
						vit++;
						if( vtidx < 0 ) vtidx = -vtidx;
						f.t.push_back(vtidx - 1);

					}
					if( vit != vlist.end() )
					{
						vnidx = atoi((*vit).c_str());
						if( vnidx < 0 ) vnidx = -vnidx;

						f.n.push_back(vnidx - 1);
					}
					//cout << vidx << ", ";
				}
				//cout << endl;

				triangulated &= (f.v.size() <= 3);

				faces.push_back(f);
			}
			else if( identifier == "vt" )
			{
				hasVertexTexCoord = true;
				//cout << "vertex texture" << endl;
				texcoord_t p;
				sline >> p;
				//cout << p << endl;
				texcoords.push_back( p );
			}
			else if( identifier == "vn" )
			{
				hasVertexNormal = true;
				//cout << "vertex normal" << endl;
				norm_t n;
				sline >> n;
				//cout << p << endl;
				normals.push_back( n );
			}

			if( fin.eof() )
				break;
		}

		fm.unmap();
		t.toc();
		cout << "finish loading file " << filename << endl;
		cout << "loading time = " << t.gap() << endl;
		return true;
	}
	catch( exception e )
	{
		cerr << e.what() << endl;
		return false;
	}
}

bool PLYLoader::load(const string& filename) {
	try{
		throw lazy_exception();

		return true;
	}
	catch( exception e )
	{
		cerr << e.what() << endl;
		return false;
	}
}

}