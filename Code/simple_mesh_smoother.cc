// ===========================简单的平滑======================================
#include <iostream>
#include <vector>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;
int main(int argc, char **argv)
{
  MyMesh  mesh;
  // check command line options
  if (argc != 4) 
  {
  	// 命令行得输入3个参数，第一个是平滑迭代的次数，第二个是需要平滑的模型文件，第三个是平滑后的输出模型文件
    std::cerr << "Usage:  " << argv[0] << " #iterations infile outfile\n";
    return 1;
  }
  // read mesh from stdin
  // 读取模型文件的数据到mesh变量中
  if ( ! OpenMesh::IO::read_mesh(mesh, argv[2]) )
  {
    std::cerr << "Error: Cannot read mesh from " << argv[2] << std::endl;
    return 1;
  }
  // this vector stores the computed centers of gravity
  std::vector<MyMesh::Point>  cogs;
  std::vector<MyMesh::Point>::iterator cog_it;
  // mesh.n_vertices为去的模型的顶点
  // 将模型的顶点数量定义为cogs的容器预留空间
  // reserve是容器预留空间，但在空间内不真正创建元素对象，所以在没有添加新的对象之前，不能引用容器内的元素。
  // 添加需要使用push_back()或者insert()
  cogs.reserve(mesh.n_vertices());
  // smoothing mesh argv[1] times
  MyMesh::VertexIter          v_it, v_end(mesh.vertices_end());
  MyMesh::VertexVertexIter    vv_it;
  MyMesh::Point               cog;
  MyMesh::Scalar              valence;
  unsigned int                i, N(atoi(argv[1]));
  // N越大，平滑程度越高
  for (i=0; i < N; ++i)
  {
    cogs.clear();
    // 便利店每一个mesh顶点
    for (v_it=mesh.vertices_begin(); v_it!=v_end; ++v_it)
    {
      cog[0] = cog[1] = cog[2] = valence = 0.0;
      // 对当前顶点环绕一圈来找寻重心
      for (vv_it=mesh.vv_iter( *v_it ); vv_it.is_valid(); ++vv_it)
      {
        cog += mesh.point( *vv_it );
        ++valence;
      }
      // 将重心数据放入cogs中
      // /calence 来取平均
      cogs.push_back(cog / valence);
    }
    
    for (v_it=mesh.vertices_begin(), cog_it=cogs.begin(); 
         v_it!=v_end; ++v_it, ++cog_it)
      if ( !mesh.is_boundary( *v_it ) )
      	// 更新顶点位置到重心
        mesh.set_point( *v_it, *cog_it );
  }
  // write mesh to stdout
  if ( ! OpenMesh::IO::write_mesh(mesh, argv[3]) )
  {
    std::cerr << "Error: cannot write mesh to " << argv[3] << std::endl;
    return 1;
  }
  return 0;
}
