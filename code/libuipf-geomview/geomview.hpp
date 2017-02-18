#ifndef UIPF_GEOMVIEW_GEOMVIEW_HPP
#define UIPF_GEOMVIEW_GEOMVIEW_HPP

#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/Geomview_stream.h>
#include <CGAL/IO/Color.h>
#include <CGAL/Polyhedron_3.h>

// geomview stream imports
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_geomview_ostream.h>
#include <CGAL/IO/Triangulation_geomview_ostream_2.h>
#include <CGAL/IO/Triangulation_geomview_ostream_3.h>
#include <CGAL/IO/alpha_shape_geomview_ostream_3.h>

#ifndef DEG2RAD
#define DEG2RAD M_PI / 180
#endif
#ifndef RAD2DEG
#define RAD2DEG 180 / M_PI
#endif

namespace uipf {

	namespace geomview {
		typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

		typedef Kernel::Point_3      Point_3;         // http://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Point__3.html
		typedef Kernel::Vector_3     Vector_3;        // http://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Vector__3.html
		typedef Kernel::Plane_3      Plane_3;         // http://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Plane__3.html
		typedef Kernel::Circle_3     Circle_3;
		typedef Kernel::Line_3       Line_3;
		typedef Kernel::Segment_3    Segment_3;
		typedef Kernel::Direction_3  Direction_3;     // http://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Direction__3.html
		typedef Kernel::Triangle_3   Triangle_3;
		typedef Kernel::Ray_3        Ray_3;
		typedef CGAL::Bbox_3         Bbox_3;

		typedef Kernel::Point_2      Point_2;         // http://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Point__2.html
		typedef Kernel::Vector_2     Vector_2;        // http://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Vector__2.html
		typedef Kernel::Line_2       Line_2;          // http://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Line__2.html
		typedef Kernel::Segment_2    Segment_2;
		typedef Kernel::Triangle_2   Triangle_2;
		typedef CGAL::Bbox_2         Bbox_2;

		typedef CGAL::Polyhedron_3<Kernel>  Polyhedron;

		typedef Kernel::Aff_transformation_3 Transformation_3;
		typedef Kernel::Aff_transformation_2 Transformation_2;

		typedef CGAL::Color Color;
	}

	class GeomView
	{
	public:

		static GeomView* instance();

	private:
		// holds the singleton instance
		static GeomView *instance_;

		// must not be copied
		GeomView( const GeomView& );

		// helper class to ensure it gets deleted when the context is gone
		class Guard {
		public:
			~Guard() {
				if( GeomView::instance_ != 0 ) {
					delete GeomView::instance_;
				}
			}
		};
		friend class Guard;

	public:
		GeomView() {};
		~GeomView();

		void start();
		void stop();

		CGAL::Geomview_stream& gv();


//
//		void set_name(const std::string& name) {
//			hasNewname = true;
//			newname = name;
//		}
//
//
//		std::string	get_new_id(const std::string & s)
//		{
//			if (hasNewname) {
//				hasNewname = false;
//				return newname;
//			}
//			return CGAL::Geomview_stream::get_new_id(s);
//		}

		void print_polyhedron(
				geomview::Polyhedron& mesh,
				bool wired = false,
				const std::string& name = std::string("Polyhedron")
		);

		void print_pointcloud(
				std::vector<geomview::Point_3> &points,
				const std::string& name = std::string("PointCloud"),
		        const geomview::Color& color = geomview::Color(50, 50, 50)
		);

		void print_colored_pointcloud(
				std::vector<std::tuple<geomview::Point_3, geomview::Color> > &points,
				const std::string& name = std::string("PointCloud")
		);

		void print_colored_directed_pointcloud(
				std::vector<std::tuple<geomview::Point_3, geomview::Direction_3, geomview::Color> > &points,
				const std::string& name = std::string("PointCloud")
		);


	private:

		bool started_ = false;
		CGAL::Geomview_stream* gv_;

		bool hasNewname = false;
		std::string newname;

	};

}


#endif //UIPF_GEOMVIEW_GEOMVIEW_HPP
