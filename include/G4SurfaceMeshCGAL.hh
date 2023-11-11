#pragma once

#include <iostream>

class G4Polyhedron;
class G4TessellatedSolid;
#include "G4ThreeVector.hh"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wgnu-statement-expression"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "CGAL/Aff_transformation_3.h"
#include "CGAL/Exact_predicates_exact_constructions_kernel.h"
#include "CGAL/Polygon_mesh_processing/corefinement.h"
#include "CGAL/Polygon_mesh_processing/orientation.h"
#include "CGAL/Polygon_mesh_processing/repair.h"
#include "CGAL/Polygon_mesh_processing/transform.h"
#include "CGAL/Polygon_mesh_processing/triangulate_faces.h"
#include "CGAL/Surface_mesh.h"

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef CGAL::Surface_mesh<Kernel::Point_3> Surface_mesh;
typedef CGAL::Aff_transformation_3<Kernel> Aff_transformation_3;
typedef Kernel::Vector_3 Vector_3;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Extended_cartesian.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

typedef CGAL::Exact_rational ER;
typedef CGAL::Extended_cartesian<ER> Kernel_ECER;
typedef CGAL::Nef_polyhedron_3<Kernel_ECER> Nef_polyhedron_3_ECER;
typedef CGAL::Polyhedron_3<Kernel_ECER> Polyhedron_3_ECER;
typedef CGAL::Surface_mesh<Kernel_ECER::Point_3> Surface_mesh_3_ECER;
typedef Kernel_ECER::Point_3 Point_3_ECER;
typedef Kernel_ECER::Vector_3 Vector_3_ECER;
typedef Kernel_ECER::Plane_3 Plane_3_ECER;
typedef Kernel_ECER::Aff_transformation_3 Aff_transform_3_ECER;

#include <CGAL/Polyhedron_incremental_builder_3.h>

// A modifier creating a triangle with the incremental builder.
template <class HDS>
class Build_mesh : public CGAL::Modifier_base<HDS> {
public:
    Build_mesh(Surface_mesh *smIn) {sm = smIn;}
    void operator()(HDS& hds) {
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);

        std::cout << sm->num_vertices() << " " << sm->num_faces() << std::endl;
        B.begin_surface( sm->num_vertices(), sm->num_faces());
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;

        for (Surface_mesh::Vertex_index vd : sm->vertices()) {
            auto p = sm->point(vd);
            B.add_vertex(Point(CGAL::to_double(p.x()),
                                  CGAL::to_double(p.y()),
                                  CGAL::to_double(p.z())));
        }

        int iCount = 0;
        for (Surface_mesh::Face_index fd : sm->faces()) {
            std::vector<unsigned int> cell;

            B.begin_facet();
            std::cout << fd << " ";
            for (Surface_mesh::Halfedge_index hd : CGAL::halfedges_around_face(sm->halfedge(fd), *sm)) {
                std::cout << (unsigned int)sm->source(hd) << " ";
                B.add_vertex_to_facet((unsigned int)sm->source(hd));
            }
            std::cout << std::endl;
            B.end_facet();

            ++iCount;
        }
        B.end_surface();

    }
private:
    Surface_mesh *sm;
};


#pragma GCC diagnostic pop

#include "G4VSurfaceMesh.hh"

class G4SurfaceMeshCGAL : public G4VSurfaceMesh
{
  public:
    G4SurfaceMeshCGAL();
    G4SurfaceMeshCGAL(G4SurfaceMeshCGAL& smIn);
    G4SurfaceMeshCGAL(G4SurfaceMeshCGAL* smIn);
    G4SurfaceMeshCGAL(Surface_mesh* smIn);
    G4SurfaceMeshCGAL(Nef_polyhedron_3_ECER* nefIn);
    G4SurfaceMeshCGAL(Nef_polyhedron_3_ECER& nefIn);
    ~G4SurfaceMeshCGAL();
    void fill(G4Polyhedron* polyIn);
    //void fill(G4TessellatedSolid* tessIn);
    void fill(Polyhedron_3_ECER& phECER);
    void fill(Surface_mesh_3_ECER& smECER);

    G4TessellatedSolid* GetG4TessellatedSolid();
    G4Polyhedron* GetG4Polyhedron();
    Surface_mesh GetCGALSurface_mesh();
    Nef_polyhedron_3_ECER GetCGALNef_polyhedron_3_ECER();
    Polyhedron_3_ECER GetCGALPolyhedron_3_ECER();

    G4SurfaceMeshCGAL* Subtraction(G4SurfaceMeshCGAL* surfaceMesh);
    G4SurfaceMeshCGAL* Union(G4SurfaceMeshCGAL* surfaceMesh);
    G4SurfaceMeshCGAL* Intersection(G4SurfaceMeshCGAL* surfaceMesh);

    void Translate(G4double dx, G4double dy, G4double dz);
    void Translate(const G4ThreeVector &t);

    void AddVertex(double x, double y, double z);
    void AddFace(int i1, int i2, int i3);
    void AddFace(int i1, int i2, int i3, int i4);
    std::vector<G4double> GetVertex(G4int iVertex);
    std::vector<G4int> GetFace(G4int iFace);
    virtual int NumberOfVertices();
    virtual int NumberOfFaces();

    int IsValid();
    int IsTriangular();
    int IsOutwardOriented();
    int IsClosed();
    int IsValidHalfEdgeGraph();
    int BoundAVolume();

    double Volume();
    std::size_t KeepLargestConnectedComponents(int iKeep);
    void WriteMesh(std::string fn);
    std::vector<G4SurfaceMeshCGAL*> DecomposeConnected();

  private:
    Surface_mesh sm;
};
