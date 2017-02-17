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
	gv_->set_bg_color(CGAL::Color(0, 200, 200));
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

void GeomView::print_polyhedron(Polyhedron &mesh, bool wired, const std::string& name) {

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
		gv << " {appearance { +face }{ OFF BINARY\n";
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
