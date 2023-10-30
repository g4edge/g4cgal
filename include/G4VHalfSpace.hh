#pragma once

#include "G4VSolid.hh"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Extended_cartesian.h>

#include <CGAL/Nef_polyhedron_3.h>

typedef CGAL::Exact_rational ER;
typedef CGAL::Extended_cartesian<ER> Kernel_ECER;
typedef CGAL::Nef_polyhedron_3<Kernel_ECER> Nef_polyhedron_3;
typedef Kernel_ECER::Point_3 Point_3;
typedef Kernel_ECER::Vector_3 Vector_3;
typedef Kernel_ECER::Plane_3 Plane_3;
typedef Kernel_ECER::Direction_3 Direction_3;

class G4VHalfSpace {

public:
    G4VHalfSpace();
    ~G4VHalfSpace();
    virtual G4bool   Inside(const G4ThreeVector& p) const = 0;
    virtual G4double Distance(const G4ThreeVector& p) const = 0;
    virtual G4double Distance(const G4ThreeVector& p, const G4ThreeVector& v) const = 0;

    virtual Nef_polyhedron_3 GetNefPolyhedron() const = 0;

    static void QuadraticSolve(G4double a, G4double b, G4double c, G4int &nSoln, G4double &x1, G4double &x2);
};