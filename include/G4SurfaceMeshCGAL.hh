#pragma once

#include <iostream>

class G4Polyhedron;
class G4TessellatedSolid;
#include "G4ThreeVector.hh"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wgnu-statement-expression"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <CGAL/Exact_rational.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Extended_cartesian.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

#include "CGAL/Aff_transformation_3.h"

#include "CGAL/Polygon_mesh_processing/corefinement.h"
#include "CGAL/Polygon_mesh_processing/orientation.h"
#include "CGAL/Polygon_mesh_processing/repair.h"
#include "CGAL/Polygon_mesh_processing/transform.h"
#include "CGAL/Polygon_mesh_processing/triangulate_faces.h"


typedef CGAL::Exact_rational ER;

typedef CGAL::Exact_predicates_exact_constructions_kernel EPECK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel EPICK;
typedef CGAL::Extended_cartesian<ER> ECER;

typedef EPECK::Point_3 Point_3_EPECK;
typedef EPICK::Point_3 Point_3_EPICK;
typedef ECER::Point_3 Point_3_ECER;

typedef EPECK::Vector_3 Vector_3_EPECK;
typedef EPICK::Vector_3 Vector_3_EPICK;
typedef ECER::Vector_3 Vector_3_ECER;

typedef ECER::Plane_3 Plane_3_ECER;

typedef std::vector<std::size_t> Polygon;

typedef CGAL::Surface_mesh<EPECK::Point_3> Surface_mesh_3_EPECK;
typedef CGAL::Surface_mesh<EPICK::Point_3> Surface_mesh_3_EPICK;
typedef CGAL::Surface_mesh<ECER::Point_3> Surface_mesh_3_ECER;

typedef CGAL::Nef_polyhedron_3<ECER> Nef_polyhedron_3_ECER;

typedef CGAL::Polyhedron_3<ECER> Polyhedron_3_ECER;

typedef CGAL::Aff_transformation_3<EPECK> Aff_transformation_3_EPECK;
typedef CGAL::Aff_transformation_3<EPICK> Aff_transformation_3_EPICK;
typedef CGAL::Aff_transformation_3<ECER> Aff_transform_3_ECER;

typedef Surface_mesh_3_EPECK Surface_mesh_3;
typedef Vector_3_EPECK Vector_3;
typedef Aff_transformation_3_EPECK Aff_transformation_3;
typedef Point_3_EPECK Point_3;
typedef boost::graph_traits<Surface_mesh_3>::vertex_descriptor vertex_descriptor;

// A modifier creating a triangle with the incremental builder.
template <class HDS>
class Build_mesh : public CGAL::Modifier_base<HDS> {
public:
    Build_mesh(Surface_mesh_3 *smIn) {sm = smIn;}
    void operator()(HDS& hds) {
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);

        std::cout << sm->num_vertices() << " " << sm->num_faces() << std::endl;
        B.begin_surface( sm->num_vertices(), sm->num_faces());
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;

        for (Surface_mesh_3::Vertex_index vd : sm->vertices()) {
            auto p = sm->point(vd);
            B.add_vertex(Point(CGAL::to_double(p.x()),
                                  CGAL::to_double(p.y()),
                                  CGAL::to_double(p.z())));
        }

        int iCount = 0;
        for (Surface_mesh_3::Face_index fd : sm->faces()) {
            std::vector<unsigned int> cell;

            B.begin_facet();
            std::cout << fd << " ";
            for (Surface_mesh_3::Halfedge_index hd : CGAL::halfedges_around_face(sm->halfedge(fd), *sm)) {
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
    Surface_mesh_3 *sm;
};

#pragma GCC diagnostic pop

#include "G4VSurfaceMesh.hh"

class G4SurfaceMeshCGAL : public G4VSurfaceMesh
{
public:
  G4SurfaceMeshCGAL();
  G4SurfaceMeshCGAL(G4SurfaceMeshCGAL& smIn);
  G4SurfaceMeshCGAL(G4SurfaceMeshCGAL* smIn);
  G4SurfaceMeshCGAL(Surface_mesh_3* smIn);
  G4SurfaceMeshCGAL(Nef_polyhedron_3_ECER* nefIn);
  G4SurfaceMeshCGAL(Nef_polyhedron_3_ECER& nefIn);
  ~G4SurfaceMeshCGAL();
  void Fill(G4Polyhedron* polyIn);
  //void fill(G4TessellatedSolid* tessIn);
  void Fill(Polyhedron_3_ECER& phECER);
  void Fill(Surface_mesh_3_ECER& smECER);

  G4TessellatedSolid* GetG4TessellatedSolid();
  Surface_mesh_3 GetCGALSurface_mesh();
  Nef_polyhedron_3_ECER GetCGALNef_polyhedron_3_ECER();
  Polyhedron_3_ECER GetCGALPolyhedron_3_ECER();

  G4SurfaceMeshCGAL* Subtraction(G4SurfaceMeshCGAL* surfaceMesh, G4bool &valid);
  G4SurfaceMeshCGAL* Union(G4SurfaceMeshCGAL* surfaceMesh, G4bool &valid);
  G4SurfaceMeshCGAL* Intersection(G4SurfaceMeshCGAL* surfaceMesh, G4bool &valid);

  void Translate(G4double dx, G4double dy, G4double dz);
  void Translate(const G4ThreeVector &t);
  void Rotate(const G4ThreeVector &axis, G4double angle);

  G4int AddVertex(double x, double y, double z);
  G4int AddFace(int i1, int i2, int i3);
  G4int AddFace(int i1, int i2, int i3, int i4);
  std::vector<G4double> GetVertex(G4int iVertex);
  std::vector<G4int> GetFace(G4int iFace);
  virtual int NumberOfVertices();
  virtual int NumberOfFaces();
  virtual int NumberOfBorderEdges();
  virtual int NumberOfNonManifoldVertices();

  int IsValid();
  int IsTriangular();
  int IsOutwardOriented();
  int IsClosed();
  int IsValidHalfEdgeGraph();
  int BoundAVolume();
  int DoesSelfIntersect();

  double Area();
  double Volume();

  std::vector<G4SurfaceMeshCGAL*> DecomposeConnected();
  std::size_t KeepLargestConnectedComponents(int iKeep);
  void IsotropicRemesh(G4int nIter, G4double targetEdgeLength);
  void RemoveDuplicates();
  std::size_t RemoveIsolatedVertices();
  void RemoveSelfIntersections();
  void ReverseFaceOrientations();
  std::size_t RemoveConnectedComponentsOfNegligibleSize();
  void StitchBorders();

  void WriteMesh(std::string fn);

  void DebugOutputHeader();
  void DebugOutput();

private:
  Surface_mesh_3 sm;
};
