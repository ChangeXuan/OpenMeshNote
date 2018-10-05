// ===========================如何使用特性扩展实体的行为======================================
#include <iostream>
#include <vector>
// -----
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>

struct MyTraits:public OpenMesh::DefaultTraits {
	VertexTraits {
	private:
		Point cog_;
	public:
		VertexT():cog_(Point(0.0f,0.0f,0.0f)) {}
		const Point& cog() const {return cog_;}
		void set_cog(const Point& _p) {cog_ = _p;}
	}
};

typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh2;

// ----------
#define SIZEOF( entity,b ) \
  std::cout << _prefix << "size of " << #entity << ": " \
            << sizeof( entity ) << std::endl;          \
  b += sizeof( entity )

  template <typename Mesh>
  void print_size(const std::string& _prefix = "") {
  	size_t total = 0;
  	SIZEOF(typename Mesh::Vertex,total);
  	SIZEOF(typename Mesh::Halfedge, total);
  	SIZEOF(typename Mesh::Edge,totoal);
  	SIZEOF(typename Mesh::Face,totoal);
  	std::cout << _prefix << "total:" << total << std::endl;
  }
  #undef SIZEOF
// ----------

  int main(int argc,char **argv) {
  	MyMesh mesh;
  	if (argc < 4 || argc > 5) {
  		std::cerr << "Usage: " << argv[0] <<  " [-s] #iterations infile outfile\n"; 
  		exit(1);
  	}

  	int idx = 2;

  	if (argc == 5) {
  		if (std::string("-s") == argv[idx-1]) {
  			std::cout << "Enhanced mesh size statistics\n";
  			print_size<MyMesh>("   ");

  			std::cout << "Default mesh size statistics\n";
  			print_size<MyMesh2>("   ");
  		}
  		++idx;
  	}

  	std::cout << " Input mesh: " << argv[idex] << std::endl;
  	if (!OpenMesh::IO::read_mesh(mesh,argv[idx])) {
  		std::cerr << "Error:Cannot read mesh form " << argv[idx] << std::endl;
  		return 1;
  	}

  	MyMesh::VertexIter v_it,v_end(mesh.vertices_end());
  	MyMesh::VertexVertexIter 	vv_it;
  	MyMesh::Point 				cog;
  	MyMesh::Scalar				valence;
  	unsigned int 				i,N(atoi(argv[idex-1]))

  	std::cout << "Smooth mesh " << N << " times\n";

  	for (i=0;i<N;i++) {
  		for (v_it=mesh.vertices_begin();v_it!=v_end;++v_it) {
  			cog[0] = cog[1] = cog[2] = valence = 0.0;
  			for (vv_it=mesh.vv_iter(*v_it;vv_it.is_valid();++vv_it)) {
  				cog += mesh.point(*vv_it);
  				++valence;
  			}
  			mesh.data(*v_it).set_cog(cog/valence);
  		}

  		for (v_it=mesh.vertices_begin();v_it!=mesh.vertices_end();++v_it) {
  			if (!mesh.is_boundary(*v_it)) {
  				mesh.set_point(*v_it,mesh.data(*v_it).cog());
  			}
  		}
  	}

  	std::cout << "Output mesh: " << argv[idx+1] <<std::endl;

  	if (!OpenMesh::IO::write_mesh(mesh,argv[idx+1])) {
  		std::cerr << "Error: canot write mesh to " << argv[idx+1] << std::endl;
  		return 1;
  	}
  	return 0;
  }
