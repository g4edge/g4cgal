#include "G4SurfaceMeshCGAL.hh"
#include "G4Polyhedron.hh"

#include <CGAL/Polygon_mesh_processing/corefinement.h>

G4SurfaceMeshCGAL::G4SurfaceMeshCGAL() : G4VSurfaceMesh() {}

G4SurfaceMeshCGAL::G4SurfaceMeshCGAL(G4SurfaceMeshCGAL &smIn) : G4VSurfaceMesh() {
    sm = Surface_mesh(smIn.sm);
}

G4SurfaceMeshCGAL::G4SurfaceMeshCGAL(G4SurfaceMeshCGAL *smIn) : G4VSurfaceMesh() {
    sm = Surface_mesh(smIn->sm);
}


G4SurfaceMeshCGAL::~G4SurfaceMeshCGAL()
{
}

void G4SurfaceMeshCGAL::fill(G4Polyhedron *polyIn) {
    G4VSurfaceMesh::fill(polyIn);
    CGAL::Polygon_mesh_processing::triangulate_faces(sm);
}

G4SurfaceMeshCGAL* G4SurfaceMeshCGAL::Subtraction(G4SurfaceMeshCGAL *s1) {
    Surface_mesh s2 = Surface_mesh();
    CGAL::Polygon_mesh_processing::corefine_and_compute_difference(sm,s1->sm,s2);

    G4SurfaceMeshCGAL* res = new G4SurfaceMeshCGAL();
    res->sm = s2;

    return res;
}

G4SurfaceMeshCGAL* G4SurfaceMeshCGAL::Union(G4SurfaceMeshCGAL *s1) {
    Surface_mesh s2 = Surface_mesh();
    CGAL::Polygon_mesh_processing::corefine_and_compute_union(sm,s1->sm,s2);

    G4SurfaceMeshCGAL* res = new G4SurfaceMeshCGAL();
    res->sm = s2;

    return res;
}
G4SurfaceMeshCGAL* G4SurfaceMeshCGAL::Intersection(G4SurfaceMeshCGAL *s1) {
    Surface_mesh s2 = Surface_mesh();
    CGAL::Polygon_mesh_processing::corefine_and_compute_intersection(sm,s1->sm,s2);

    G4SurfaceMeshCGAL* res = new G4SurfaceMeshCGAL();
    res->sm = s2;

    return res;
}

void G4SurfaceMeshCGAL::AddVertex(double x, double y, double z) {
    Point p(x, y, z);
    sm.add_vertex(p);
    return;
}

void G4SurfaceMeshCGAL::AddFace(int i1, int i2, int i3) {
    sm.add_face(Surface_mesh::Vertex_index(i1),
                Surface_mesh::Vertex_index(i2),
                Surface_mesh::Vertex_index(i3));
    return;
}

void G4SurfaceMeshCGAL::AddFace(int i1, int i2, int i3, int i4) {
    sm.add_face(Surface_mesh::Vertex_index(i1),
                Surface_mesh::Vertex_index(i2),
                Surface_mesh::Vertex_index(i3),Surface_mesh::Vertex_index(i4));
    return;
}

std::vector<G4double> G4SurfaceMeshCGAL::GetVertex(G4int iVertex) {
    std::vector<G4double> v = std::vector<G4double>();
    Surface_mesh::Vertex_index vi = Surface_mesh::Vertex_index(iVertex);
    Surface_mesh::Point p = sm.point(vi);
    v.push_back(CGAL::to_double(p.x()));
    v.push_back(CGAL::to_double(p.y()));
    v.push_back(CGAL::to_double(p.z()));
    return v;
}

std::vector<G4int> G4SurfaceMeshCGAL::GetFace(G4int iFace) {
    std::vector <G4int> f = std::vector<G4int>();

    Surface_mesh::Face_index fd = Surface_mesh::Face_index(iFace);

    for (Surface_mesh::Halfedge_index hd: CGAL::halfedges_around_face(sm.halfedge(fd), sm)) {
        f.push_back((int) sm.source(hd));
    }
    return f;
}

int G4SurfaceMeshCGAL::NumberOfVertices() {
    return sm.number_of_vertices();
}

int G4SurfaceMeshCGAL::NumberOfFaces() {
    return sm.number_of_faces();
}

int G4SurfaceMeshCGAL::IsValid() {
    return sm.is_valid(true);
}

int G4SurfaceMeshCGAL::IsTriangular() {
    return CGAL::is_triangle_mesh(sm);
}

int G4SurfaceMeshCGAL::IsOutwardOriented() {
    return CGAL::Polygon_mesh_processing::is_outward_oriented(sm);
}

int G4SurfaceMeshCGAL::IsClosed() {
    return CGAL::is_closed(sm);
}

int G4SurfaceMeshCGAL::IsValidHalfEdgeGraph() {
    return CGAL::is_valid_halfedge_graph(sm);
}


int G4SurfaceMeshCGAL::BoundAVolume() {
    return CGAL::Polygon_mesh_processing::does_bound_a_volume(sm);
}

double G4SurfaceMeshCGAL::Volume() {
    return CGAL::to_double(CGAL::Polygon_mesh_processing::volume(sm));
}

std::size_t G4SurfaceMeshCGAL::KeepLargestConnectedComponents(int iKeep) {
    return CGAL::Polygon_mesh_processing::keep_largest_connected_components(sm, iKeep);
}

void G4SurfaceMeshCGAL::WriteMesh(std::string fn) {
    std::ofstream ofstr(fn);
    ofstr << sm;
}

std::vector<G4SurfaceMeshCGAL*> G4SurfaceMeshCGAL::DecomposeConnected() {
    typedef Surface_mesh Mesh;
    typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;
    typedef Mesh::Property_map<face_descriptor, std::size_t> F_sizet_map;
    face_descriptor fd = *faces(sm).first;
    F_sizet_map fccmap = sm.add_property_map<face_descriptor, std::size_t>("f:CC").first;

    auto sm1 = Surface_mesh(sm);
    int ncompt = CGAL::Polygon_mesh_processing::connected_components(sm1,fccmap);
#ifdef G4CGAL_DEBUG
    G4cout << "Components " << ncompt << G4endl;
#endif
    return std::vector<G4SurfaceMeshCGAL*>();
}