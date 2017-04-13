#include "geomview.hpp"

using namespace uipf;
using namespace uipf::geomview;


GeomView* GeomView::instance_ = nullptr;

GeomView* GeomView::instance()
{
	static GeomView::Guard w;
	if (instance_ == nullptr) {
		instance_ = new GeomView();
	}
	return instance_;
}


GeomView::~GeomView() {
	stop();
}


void GeomView::start()
{
	if (started_) {
		return;
	}
	gv_ = new CGAL::Geomview_stream();
	started_ = true;

	gv_->set_line_width(4);
	// gv.set_trace(true);
	gv_->set_bg_color(CGAL::Color(200, 200, 200));
	gv_->clear();
}

void GeomView::stop()
{
	gv_->clear();
	delete gv_;
	started_ = false;
}

CGAL::Geomview_stream& GeomView::gv() {
	if (!started_) {
		start();
	}
	return *gv_;
}

void GeomView::print_polyhedron(Polyhedron &mesh, bool wired, const std::string& name, const std::string& texture) {

	if (!started_) {
		start();
	}

	typedef Polyhedron::Facet Triangle;
	typedef Kernel K;
	typedef K::Point_3                                  Point;
	typedef K::Less_xyz_3                               Comp;

	// We first copy everything in a vector to only require an InputIterator.
	std::vector<Triangle> triangles(mesh.facets_begin(), mesh.facets_end());
	typedef typename std::vector<Triangle>::const_iterator            Tit;

	// Put the points in a map and a vector.
	// The index of a point in the vector is the value associated
	// to it in the map.
	typedef std::map<Point, int, Comp>  Point_map;
	Point_map           point_map(K().less_xyz_3_object());
	std::vector<Point>  points;
	for (Tit i = triangles.begin(); i != triangles.end(); ++i) {

		auto h = i->facet_begin();
		do {
			if (point_map.insert(typename Point_map::value_type(h->vertex()->point(), points.size())).second) {
				points.push_back(h->vertex()->point());
			}
		} while (++h != i->facet_begin());

	}

	CGAL::Geomview_stream& gv = this->gv();

	bool ascii_bak = gv.get_ascii_mode();
	bool raw_bak = gv.set_raw(true);

	// Header.
	gv.set_binary_mode();
	gv << "(geometry " << gv.get_new_id(name);
	// http://www.geom.uiuc.edu/software/geomview/ooglman.html#SEC30
	if (wired) {
		gv << " {appearance { +edge -face }{ OFF BINARY\n";
	} else {
		if (texture.empty()) {
			gv << " {appearance { +face }{ OFF BINARY\n";
		} else {
			// TODO this does not work, bug in geomview? https://sourceforge.net/p/geomview/mailman/geomview-users/?viewmonth=200707
			gv << " {appearance { +face +texturing  texture{ image { data AUTO {<\"" << texture << "\"} } apply decal } }{ OFF BINARY\n";
		}
	}
	gv << points.size() << triangles.size() << 0;

	// Points coordinates.
	for(auto p = points.begin(); p != points.end(); ++p) {
		gv << CGAL::to_double(p->x()) << CGAL::to_double(p->y()) << CGAL::to_double(p->z());
	}

	// Triangles vertices indices.
	for (Tit tit = triangles.begin(); tit != triangles.end(); ++tit) {

		gv << tit->facet_degree();

		auto h = tit->facet_begin();
		do {
			gv << point_map[h->vertex()->point()];
		} while (++h != tit->facet_begin());
//			for (int j = 0; j < 3; ++j)
//				(*this) << point_map[tit->vertex(j)];
		gv << 0; // without color.
	}
	// Footer.
	gv << "}})";

	gv.set_raw(raw_bak);
	gv.set_ascii_mode(ascii_bak);
}

// TODO different way of printing points without normals http://www.geomview.org/FAQ/answers.shtml#points
void GeomView::print_pointcloud(
		std::vector<Point_3> &points,
		const std::string &name,
        const Color& color
) {
	std::vector<std::tuple<Point_3, Direction_3, Color> > directedPoints;
	for(auto p: points) {
		directedPoints.push_back(std::tuple<Point_3, Direction_3, Color>(p, Direction_3(0, 0, 1), color));
	}
	print_colored_directed_pointcloud(directedPoints, name);
}

// TODO different way of printing points without normals http://www.geomview.org/FAQ/answers.shtml#points
void GeomView::print_colored_pointcloud(
		std::vector<std::tuple<Point_3, Color> > &points,
		const std::string &name
) {
	std::vector<std::tuple<Point_3, Direction_3, Color> > directedPoints;
	for(auto p: points) {
		directedPoints.push_back(std::tuple<Point_3, Direction_3, Color>(std::get<0>(p), Direction_3(0, 0, 1), std::get<1>(p)));
	}
	print_colored_directed_pointcloud(directedPoints, name);
}

void GeomView::print_colored_directed_pointcloud(
		std::vector<std::tuple<Point_3, Direction_3, Color> > &points,
        const std::string &name
) {

	if (!started_) {
		start();
	}

	CGAL::Geomview_stream& gv = this->gv();

	bool ascii_bak = gv.get_ascii_mode();
	bool raw_bak = gv.set_raw(true);

	// Header.
	gv.set_binary_mode();
	gv << "(geometry " << gv.get_new_id(name);
	// http://www.geom.uiuc.edu/software/geomview/ooglman.html#SEC30
	gv << " {appearance { +face +keepcolor shading constant }{ OFF BINARY\n";
	gv << (points.size() * 4) << points.size() << 0;

	// create polyhedron of quads, each quad represents a point
	double psize = .01;
	// write points
	for(size_t i = 0; i < points.size(); ++i) {
		Point_3 p = std::get<0>(points[i]);

		// TODO include direction into the plane
		gv << (p.x() - psize) << (p.y() + psize) << (p.z());
		gv << (p.x() - psize) << (p.y() - psize) << (p.z());
		gv << (p.x() + psize) << (p.y() - psize) << (p.z());
		gv << (p.x() + psize) << (p.y() + psize) << (p.z());
	}
	// write facets with color
	for(size_t i = 0; i < points.size(); ++i) {
		Color c = std::get<2>(points[i]);

		// define a facet
		gv << 4 << (i * 4) << (i * 4 + 1) << (i * 4 + 2) << (i * 4 + 3);
		// define its color
//		std::cout << "col: " << ((int) c.r()) << " " << ((int) c.g()) << " " << ((int) c.b()) << std::endl;
		float r = ((int) c.r()) / 255.0f;
		float g = ((int) c.g()) / 255.0f;
		float b = ((int) c.b()) / 255.0f;
//		std::cout << "trc: " << r << " " << g << " " << b << std::endl;
		gv << 3 << r << g << b;
	}

	gv << "}})";

	gv.set_raw(raw_bak);
	gv.set_ascii_mode(ascii_bak);
}
