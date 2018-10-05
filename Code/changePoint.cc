// ===========================修改位置======================================
#include <iostream>
#include <typeinfo>
// --------------------
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT>
#include <OpenMesh/Core/Geometry/VectorT.hh>

#ifdef DOXY_IGNORE_THIS

struct MyTraits:OpenMesh::DefaultTraits{
	typedef OpenMesh::Vec3d Point;
	typedef OpenMesh::Vec3d Normal;
};

#endif

typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;

int main(int argc,char **argv) {
	MyMesh mesh;

	if (argc!=2) {
		std::cerr << "Usage: " << argv[0] << " <input>\n";
		return 1;
	}

	// 判断顶点数据是否为double类型
	if (typeid(OpenMesh::vector_traits<MyMesh::Point>::value_type) != typeid(double)) {
		std::cerr << "Ouch! ERROR! Data type is wrong!\n";
		return 1;
	}

	// 判断顶点法线是否为double类型
	if (typeid(OpenMesh::vector_traits<MyMesh::Normal>::value_type) != typeid(double)) {
		std::cerr << "Ouch! ERROR! Data type is wrong!\n";
		return 1;
	}

	// 添加顶点法线到mesh的默认属性
	mesh.request_vertex_normals();
	// 添加面法线到mesh的默认属性
	mesh.request_face_normals();

	OpenMesh::IO::Options opt;
	if (!OpenMesh::IO::read_mesh(mesh,argv[1],opt)) {
		std::cerr << "Error loading mesh from file " << argv[1] << std::endl;
		return 1;
	}

	if (!opt.check(OpenMesh::IO::Options::VertexNormal) && mesh.has_face_normal() && mesh.has_vertex_normals()) {
		mesh.update_normals();
	}

	for (MyMesh::VertexIter v_it = mesh.vertices_begin();v_it!=mesh.vertices_end();++v_it) {
		std::cout << "Vertex #" << *v_it << ": " << mesh.point( *v_it );
	    mesh.set_point( *v_it, mesh.point(*v_it)+mesh.normal(*v_it) );
	    std::cout << " moved to " << mesh.point( *v_it ) << std::endl;
	}

	return 0;
}
