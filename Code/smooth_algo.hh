#include <algorithm>
#include <OpenMesh/Core/Utils/Property.hh>

#ifndef DOXY_IGNORE_THIS

template <class Mesh> class SmootherT {
	public:
		// 把Mesh::Point 重命名为cog_t
		typedef typename Mesh::Point cog_t;
		// 类似
		typedef OpenMesh::VPropHandleT<cog_t> Property_cog;
	public:
		// 初始化函数
		// 把参数_mesh赋值给mesh_变量
		// mesh_变量为SmootherT的私有变量，类型为Mesh
		SmootherT(Mesh& _mesh):mesh_(_mesh) {
			// cog_是SmootherT的私有变量，类型为Property_cog;
			// 设置装载
			mesh_.add_property(cog_);
		}
		// 析构函数
		~SmootherT() {
			// 移除装载
			mesh_.remove_property(cog_);
		}
		// 公有光滑函数，参数为迭代的次数N
		void smooth(unsigned int _iterations) {
			for (unsigned int i=0;i<_iterations;++i) {
				// 使用for_each来循环使用Compute和Set函数
				std::for_each(mesh_.vertices_begin(),mesh_.vertices_end(),ComputeCOG(mesh_,cog_));
				std::for_each(mesh_.vertices_begin(),mesh_.vertices_end(),SetCOG(mesh_,cog_));
			}
		}
	private:
		class ComputeCOG {
			public:
				// 把传入的参数赋值给ComputeCOG类的私有变量
				ComputeCOG(Mesh& _mesh,Property_cog& _cog):mesh_(_mesh),cog_(_cog) {

				}
				// 重载函数
				// 参数_vh表示迭代器的指示
				void operator()(const typename Mesh::VertexHandle& _vh) {
					typename Mesh::VertexVertexIter vv_it;
					typename Mesh::Scalar valence(0.0);
					// 归零
					mesh_.property(cog_,_vh) = typename Mesh::Point(0.0,0.0,0.0);
					// 遍历节点环来找重心
					for (vv_it=mesh_.vv_iter(_vh);vv_it.is_valid();++vv_it) {
						mesh_.property(cog_,_vh) += mesh_.point(*vv_it);
						++valence;
					}
					mesh_.property(cog_,_vh) /= valence;
				}
			private:
				Mesh& mesh_;
				Property_cog& cog_;
		};

		class SetCOG {
			public:
				SetCOG(Mesh& _mesh,Property_cog& _cog):mesh_(_mesh),cog_(_cog) {

				}
				void operator()(const typename Mesh::VertexHandle& _vh) {
					// 更新顶点的位置到propert(cog_)中的重心
					if (!mesh_.is_boundary(_vh)) {
						mesh_.set_point(_vh,mesh_.property(cog_,_vh))
					}
				}
			private:
				Mesh& mesh_;
				Property_cog& cog_;
		}

		Mesh& mesh_;
		Property_cog& cog_;
};
#endif
