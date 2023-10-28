#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Extended_cartesian.h>

#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Polyhedron_3.h>

#include <CGAL/IO/Nef_polyhedron_iostream_3.h>


typedef CGAL::Exact_rational ER;
typedef CGAL::Extended_cartesian<ER> Kernel_ECER;
typedef CGAL::Nef_polyhedron_3<Kernel_ECER> Nef_polyhedron_3;
typedef CGAL::Polyhedron_3<Kernel_ECER> Polyhedron_3;
typedef Kernel_ECER::Point_3 Point_3;
typedef Kernel_ECER::Vector_3 Vector_3;
typedef Kernel_ECER::Direction_3 Direction_3;
typedef Kernel_ECER::Plane_3 Plane_3;

int main() {
    Plane_3 p1 = Plane_3(Point_3(1,0,0), Direction_3(1,0,0));
    Plane_3 p2 = Plane_3(Point_3(-1,0,0), Direction_3(-1,0,0));
    Plane_3 p3 = Plane_3(Point_3(0,1,0), Direction_3(0,1,0));
    Plane_3 p4 = Plane_3(Point_3(0,-1,0), Direction_3(0,-1,0));
    Plane_3 p5 = Plane_3(Point_3(0,0,1), Direction_3(0,0,1));
    Plane_3 p6 = Plane_3(Point_3(0,0,-1), Direction_3(0,0,-1));

    Nef_polyhedron_3 n1 = Nef_polyhedron_3 (p1);
    Nef_polyhedron_3 n2 = Nef_polyhedron_3 (p2);
    Nef_polyhedron_3 n3 = Nef_polyhedron_3 (p3);
    Nef_polyhedron_3 n4 = Nef_polyhedron_3 (p4);
    Nef_polyhedron_3 n5 = Nef_polyhedron_3 (p5);
    Nef_polyhedron_3 n6 = Nef_polyhedron_3 (p6);

    auto n = n1*n2*n3*n4*n5*n6;

    auto nn = Nef_polyhedron_3();
    nn += n;

    std::cout << nn;

    Polyhedron_3 ph;
    nn.convert_to_polyhedron(ph);

    std::cout << ph;
}