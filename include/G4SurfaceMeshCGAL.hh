#pragma once

class G4Polyhedron;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wgnu-statement-expression"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "CGAL/Exact_predicates_exact_constructions_kernel.h"
#include "CGAL/Surface_mesh.h"
#include "CGAL/Polygon_mesh_processing/corefinement.h"
#include "CGAL/Polygon_mesh_processing/orientation.h"
#include "CGAL/Polygon_mesh_processing/repair.h"
#include "CGAL/Polygon_mesh_processing/triangulate_faces.h"

#include "CGAL/Aff_transformation_3.h"
#include "CGAL/Polygon_mesh_processing/transform.h"

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_3                                   Point;
typedef Kernel::Vector_3                                  Vector;
typedef CGAL::Surface_mesh<Kernel::Point_3>               Surface_mesh;
typedef CGAL::Aff_transformation_3<Kernel>                Aff_transformation_3;
#pragma GCC diagnostic pop

#include "G4VSurfaceMesh.hh"

class G4SurfaceMeshCGAL : public G4VSurfaceMesh {
public:
    G4SurfaceMeshCGAL();
    G4SurfaceMeshCGAL(G4SurfaceMeshCGAL &smIn);
    G4SurfaceMeshCGAL(G4SurfaceMeshCGAL *smIn);
    ~G4SurfaceMeshCGAL();
    void fill(G4Polyhedron *polyIn);
    G4SurfaceMeshCGAL* Subtraction(G4SurfaceMeshCGAL *surfaceMesh);
    G4SurfaceMeshCGAL* Union(G4SurfaceMeshCGAL *surfaceMesh);
    G4SurfaceMeshCGAL* Intersection(G4SurfaceMeshCGAL *surfaceMesh);
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

