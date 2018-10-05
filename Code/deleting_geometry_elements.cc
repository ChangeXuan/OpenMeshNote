// ===========================删除点，面，边======================================
#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Mesh/Status.hh>
// ----------------------------------------------------------------------------
struct MyTraits : public OpenMesh::DefaultTraits
{
};
typedef OpenMesh::PolyMesh_ArrayKernelT<MyTraits>  MyMesh;
// ----------------------------------------------------------------------------
// Build a simple cube and delete it except one face
  
int main()
{
  MyMesh mesh;
  // 在删除顶点/面/边之前，必须调用该请求。它授予对状态属性的访问权限。
  mesh.request_face_status();
  mesh.request_edge_status();
  mesh.request_vertex_status();
  // 顶点数组
  MyMesh::VertexHandle vhandle[8];
  // 面熟组
  MyMesh::FaceHandle   fhandle[6];

  vhandle[0] = mesh.add_vertex(MyMesh::Point(-1, -1,  1));
  vhandle[1] = mesh.add_vertex(MyMesh::Point( 1, -1,  1));
  vhandle[2] = mesh.add_vertex(MyMesh::Point( 1,  1,  1));
  vhandle[3] = mesh.add_vertex(MyMesh::Point(-1,  1,  1));
  vhandle[4] = mesh.add_vertex(MyMesh::Point(-1, -1, -1));
  vhandle[5] = mesh.add_vertex(MyMesh::Point( 1, -1, -1));
  vhandle[6] = mesh.add_vertex(MyMesh::Point( 1,  1, -1));
  vhandle[7] = mesh.add_vertex(MyMesh::Point(-1,  1, -1));
  // 点围城面时的临时数组
  std::vector<MyMesh::VertexHandle>  tmp_face_vhandles;
  tmp_face_vhandles.clear();
  tmp_face_vhandles.push_back(vhandle[0]);
  tmp_face_vhandles.push_back(vhandle[1]);
  tmp_face_vhandles.push_back(vhandle[2]);
  tmp_face_vhandles.push_back(vhandle[3]);
  fhandle[0] = mesh.add_face(tmp_face_vhandles);
 
  tmp_face_vhandles.clear();
  tmp_face_vhandles.push_back(vhandle[7]);
  tmp_face_vhandles.push_back(vhandle[6]);
  tmp_face_vhandles.push_back(vhandle[5]);
  tmp_face_vhandles.push_back(vhandle[4]);
  fhandle[1] = mesh.add_face(tmp_face_vhandles);
 
  tmp_face_vhandles.clear();
  tmp_face_vhandles.push_back(vhandle[1]);
  tmp_face_vhandles.push_back(vhandle[0]);
  tmp_face_vhandles.push_back(vhandle[4]);
  tmp_face_vhandles.push_back(vhandle[5]);
  fhandle[2] = mesh.add_face(tmp_face_vhandles);
 
  tmp_face_vhandles.clear();
  tmp_face_vhandles.push_back(vhandle[2]);
  tmp_face_vhandles.push_back(vhandle[1]);
  tmp_face_vhandles.push_back(vhandle[5]);
  tmp_face_vhandles.push_back(vhandle[6]);
  fhandle[3] = mesh.add_face(tmp_face_vhandles);
  tmp_face_vhandles.clear();
  tmp_face_vhandles.push_back(vhandle[3]);
  tmp_face_vhandles.push_back(vhandle[2]);
  tmp_face_vhandles.push_back(vhandle[6]);
  tmp_face_vhandles.push_back(vhandle[7]);
  fhandle[4] = mesh.add_face(tmp_face_vhandles);
 
  tmp_face_vhandles.clear();
  tmp_face_vhandles.push_back(vhandle[0]);
  tmp_face_vhandles.push_back(vhandle[3]);
  tmp_face_vhandles.push_back(vhandle[7]);
  tmp_face_vhandles.push_back(vhandle[4]);
  fhandle[5] = mesh.add_face(tmp_face_vhandles);
  // And now delete all faces and vertices
  // except face (vh[7], vh[6], vh[5], vh[4])
  // whose handle resides in fhandle[1]
   
  // 依次删除编号0，2，3，4，5的面
  mesh.delete_face(fhandle[0], false);
  mesh.delete_face(fhandle[2], false);
  mesh.delete_face(fhandle[3], false);
  mesh.delete_face(fhandle[4], false);
  mesh.delete_face(fhandle[5], false);
  
  // If isolated vertices result in a face deletion
  // they have to be deleted manually. If you want this
  // to happen automatically, change the second parameter
  // to true.
  // Now delete the isolated vertices 0, 1, 2 and 3
  // 删除刚刚因为删除面后留下的孤立的点
  mesh.delete_vertex(vhandle[0], false);
  mesh.delete_vertex(vhandle[1], false);
  mesh.delete_vertex(vhandle[2], false);
  mesh.delete_vertex(vhandle[3], false);
  // Delete all elements that are marked as deleted
  // from memory.
  mesh.garbage_collection();
  // write mesh to output.obj
  try {
        if ( !OpenMesh::IO::write_mesh(mesh, "output.off") ) {
          std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
          return 1;
        }
  }
  catch( std::exception& x )
  {
    std::cerr << x.what() << std::endl;
    return 1;
  }
 
  return 0;
}
