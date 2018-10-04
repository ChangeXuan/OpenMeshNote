// ===========================简单的平滑(使用for_each)======================================
#include <iostream>
#include <vector>
// ------OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
// ------self.hh
#include "smooth_algo.hh"

// ------------------------------------------------------
#ifdef DOXY_IGNORE_THIS
struct MyTraits:public OpenMesh::DefaultTraits {
	HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
};
#endif

typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;
// -------------------------------------------------------

int main(int argc,char **argv) {
	MyMesh mesh;

	if (argc != 4) {
		std::cerr << "Usage:  " << argv[0] << " #iterations  infile  outfile\n";
		return 1;
	}

	if (!OpenMesh::IO::read_mesh(mesh,argv[2])) {
		std::cerr << "Error: Cannot read mesh from " << argv[2] << std::endl;
		return 1;
	}

	// 模型光滑
	SmootherT<MyMesh> smoother(mesh);
	smoother.smooth(atoi(argv[1]));

	if (!OpenMesh::IO::write_mesh(mesh,argv[3])) {
		std::cerr << "Error: cannot write mesh to " << argv[3] << std::endl;
		return 1;
	}
	return 0;
}
