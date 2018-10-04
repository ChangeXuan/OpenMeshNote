// ===========================每个顶点朝着法线方向移动======================================
#include <iostream>
// ----------------
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

int main(int argc, char **argv) {
	MyMesh mesh;
	if (argc!=2) {
		std::cerr << "Usage: " << argv[0] << " <input>\n ";
		return 1;
	}

	mesh.request_vertex_normals();

	if (!mesh.has_vertex_normals()) {
		std::cerr << "ERROR: Standard vertex property 'Normals' not available!\n";
		return 1;
	}

	OpenMesh::IO::Options opt;
	if (!OpenMesh::IO::read_mesh(mesh,argv[1],opt)) {
		std::cerr << "Error loading mesh from file " << argv[1] << std::endl;
		return 1;
	}

	if (!opt.check(OpenMesh::IO::Options::VertexNormal)) {
		mesh.request_face_normals();
		mesh.update_normals();
		mesh.release_face_normals();
	}

	// 遍历每个顶点
	for (MyMesh::VertexIter v_it = mesh.vertices_begin();v_it != mesh.vertices_end();++v_it) {
		std::cout << "Vertex #" << *v_it << ": " << mesh.point(*v_it);
		mesh.set_point(*v_it,mesh.point(*v_it)+mesh.normal(*v_it));
		std::cout << " moved to " << mesh.point(*v_it) << std::endl;
	}

	mesh.release_vertex_normals();

	if (mesh.has_vertex_normals()) {
		std::cerr << "Ouch! ERROR! Shouldn't have any vertex normals anymore!\n";
		return 1;
	}
	return 0;
}
