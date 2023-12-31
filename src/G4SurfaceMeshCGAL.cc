#include "G4SurfaceMeshCGAL.hh"

#include "G4Polyhedron.hh"
#include "G4TessellatedSolid.hh"
#include "G4TriangularFacet.hh"
#include "G4PolyhedronArbitrary.hh"

#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/transform.h>
#include <CGAL/Polygon_mesh_processing/repair.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_mesh_to_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>

#include <CGAL/boost/graph/helpers.h>

#include <vector>

// #define G4CGAL_DEBUG

G4SurfaceMeshCGAL::G4SurfaceMeshCGAL() : G4VSurfaceMesh() {
  sm = Surface_mesh_3();
}

G4SurfaceMeshCGAL::G4SurfaceMeshCGAL(G4SurfaceMeshCGAL& smIn) : G4VSurfaceMesh()
{
  sm = Surface_mesh_3(smIn.sm);
}

G4SurfaceMeshCGAL::G4SurfaceMeshCGAL(G4SurfaceMeshCGAL* smIn) : G4VSurfaceMesh()
{
  sm = Surface_mesh_3(smIn->sm);
}

G4SurfaceMeshCGAL::G4SurfaceMeshCGAL(Surface_mesh_3* smIn) {
  sm = Surface_mesh_3(*smIn);
}

G4SurfaceMeshCGAL::G4SurfaceMeshCGAL(Nef_polyhedron_3_ECER* nefIn) {
  Polyhedron_3_ECER phECER;
  Surface_mesh_3_ECER smECER;
  nefIn->convert_to_polyhedron(phECER);
  CGAL::copy_face_graph(phECER, smECER);
  Fill(smECER);
}

G4SurfaceMeshCGAL::G4SurfaceMeshCGAL(Nef_polyhedron_3_ECER& nefIn) {
  Polyhedron_3_ECER phECER;
  nefIn.convert_to_polyhedron(phECER);
  Surface_mesh_3_ECER smECER;
  CGAL::copy_face_graph(phECER, smECER);
  Fill(smECER);
}

G4SurfaceMeshCGAL::~G4SurfaceMeshCGAL() {}

void G4SurfaceMeshCGAL::Fill(G4Polyhedron* polyIn)
{
  G4VSurfaceMesh::Fill(polyIn);
  CGAL::Polygon_mesh_processing::triangulate_faces(sm);
}

void G4SurfaceMeshCGAL::Fill(Polyhedron_3_ECER& phIn) {
  Surface_mesh_3_ECER smECER;
  CGAL::copy_face_graph(phIn, smECER);
  Fill(smECER);
}

void G4SurfaceMeshCGAL::Fill(Surface_mesh_3_ECER& smIn)
{
  Point_3_ECER p;
  for (Surface_mesh_3_ECER::Vertex_index vd : smIn.vertices()) {
    p = smIn.point(vd);
    AddVertex(CGAL::to_double(p.x()),
              CGAL::to_double(p.y()),
              CGAL::to_double(p.z()));
  }

  int iCount = 0;
  for (Surface_mesh_3_ECER::Face_index fd : smIn.faces()) {
    std::vector<unsigned int> cell;

    for (Surface_mesh_3_ECER::Halfedge_index hd :
            CGAL::halfedges_around_face(smIn.halfedge(fd), smIn)) {
      cell.push_back((unsigned int)smIn.source(hd));
    }

    if (cell.size() == 3) {
      AddFace(Surface_mesh_3_ECER::Vertex_index((size_t)cell[0]),
              Surface_mesh_3_ECER::Vertex_index((size_t)cell[1]),
              Surface_mesh_3_ECER::Vertex_index((size_t)cell[2]));
    } else if (cell.size() == 4) {
      AddFace(Surface_mesh_3_ECER::Vertex_index((size_t)cell[0]),
              Surface_mesh_3_ECER::Vertex_index((size_t)cell[1]),
              Surface_mesh_3_ECER::Vertex_index((size_t)cell[2]),
              Surface_mesh_3_ECER::Vertex_index((size_t)cell[3]));
    }

    ++iCount;
  }
}

G4TessellatedSolid* G4SurfaceMeshCGAL::GetG4TessellatedSolid() {
  G4TessellatedSolid *ts = new G4TessellatedSolid();

  std::vector<G4ThreeVector> tvVertices;

  for (auto iV=0; iV<this->NumberOfVertices(); iV++) {
    auto v = this->GetVertex(iV);
    tvVertices.push_back(G4ThreeVector(v[0],v[1],v[2]));
  }

  for (auto iF=0; iF<this->NumberOfFaces(); iF++) {
    auto face = GetFace(iF);

    G4VFacet *facet = new G4TriangularFacet(tvVertices[face[0]],
                                            tvVertices[face[1]],
                                            tvVertices[face[2]],
                                            G4FacetVertexType::ABSOLUTE);
    ts->AddFacet(facet);
  }

  return ts;

}

Surface_mesh_3 G4SurfaceMeshCGAL::GetCGALSurface_mesh() {
  return sm;
}

Nef_polyhedron_3_ECER G4SurfaceMeshCGAL::GetCGALNef_polyhedron_3_ECER() {
  Nef_polyhedron_3_ECER nef;
  return nef;
}

Polyhedron_3_ECER G4SurfaceMeshCGAL::GetCGALPolyhedron_3_ECER() {
  Polyhedron_3_ECER poly;
  Build_mesh<Polyhedron_3_ECER::HalfedgeDS> mesh(&sm);
  poly.delegate(mesh);
  return poly;
}

G4SurfaceMeshCGAL* G4SurfaceMeshCGAL::Subtraction(G4SurfaceMeshCGAL* s1, G4bool &valid)
{
  Surface_mesh_3 s2 = Surface_mesh_3();
  valid = CGAL::Polygon_mesh_processing::corefine_and_compute_difference(sm, s1->sm, s2);

#ifdef G4CGAL_DEBUG
  G4cout << "G4SurfaceMeshCGAL::Subtraction> valid " << valid << G4endl;
#endif

  G4SurfaceMeshCGAL* res = new G4SurfaceMeshCGAL();
  res->sm = s2;

  return res;
}

G4SurfaceMeshCGAL* G4SurfaceMeshCGAL::Union(G4SurfaceMeshCGAL* s1, G4bool &valid)
{
  Surface_mesh_3 s2 = Surface_mesh_3();
  valid = CGAL::Polygon_mesh_processing::corefine_and_compute_union(sm, s1->sm, s2);

#ifdef G4CGAL_DEBUG
  G4cout << "G4SurfaceMeshCGAL::Union> valid " << valid << G4endl;
#endif

  G4SurfaceMeshCGAL* res = new G4SurfaceMeshCGAL();
  res->sm = s2;

  return res;
}

G4SurfaceMeshCGAL* G4SurfaceMeshCGAL::Intersection(G4SurfaceMeshCGAL* s1, G4bool &valid)
{
  Surface_mesh_3 s2 = Surface_mesh_3();
  valid = CGAL::Polygon_mesh_processing::corefine_and_compute_intersection(sm, s1->sm, s2);

#ifdef G4CGAL_DEBUG
  G4cout << "G4SurfaceMeshCGAL::Intersection> valid " << valid << G4endl;
#endif

  G4SurfaceMeshCGAL* res = new G4SurfaceMeshCGAL();
  res->sm = s2;

  return res;
}

void G4SurfaceMeshCGAL::Translate(G4double dx, G4double dy, G4double dz){
  auto tr3 = Vector_3(dx,dy,dz);
  auto at3 = Aff_transformation_3(CGAL::TRANSLATION, tr3);
  CGAL::Polygon_mesh_processing::transform(at3,sm);
}

void G4SurfaceMeshCGAL::Translate(const G4ThreeVector &t) {
  Translate(t.x(), t.y(), t.z());
}

void G4SurfaceMeshCGAL::Rotate(const G4ThreeVector &a, G4double angle) {
  G4double rot[3][3];
  auto normAxis = a / a.mag();

  auto cosAngle = cos(-angle);
  auto sinAngle = sin(-angle);
  auto verSin = 1 - cosAngle;

  auto x = normAxis.x();
  auto y = normAxis.y();
  auto z = normAxis.z();

  rot[0][0] = (verSin * x * x) + cosAngle;
  rot[0][1] = (verSin * x * y) - (z * sinAngle);
  rot[0][2] = (verSin * x * z) + (y * sinAngle);

  rot[1][0] = (verSin * y * x) + (z * sinAngle);
  rot[1][1] = (verSin * y * y) + cosAngle;
  rot[1][2] = (verSin * y * z) - (x * sinAngle);

  rot[2][0] = (verSin * z * x) - (y * sinAngle);
  rot[2][1] = (verSin * z * y) + (x * sinAngle);
  rot[2][2] = (verSin * z * z) + cosAngle;

  auto rotn = Aff_transformation_3(rot[0][0],rot[0][1],rot[0][2],
                                   rot[1][0],rot[1][1],rot[1][2],
                                   rot[2][0],rot[2][1],rot[2][2],1);
  CGAL::Polygon_mesh_processing::transform(rotn,sm);
}


G4int G4SurfaceMeshCGAL::AddVertex(double x, double y, double z)
{
  Point_3 p(x, y, z);
  return sm.add_vertex(p);
}

G4int G4SurfaceMeshCGAL::AddFace(int i1, int i2, int i3)
{
  return sm.add_face(Surface_mesh_3::Vertex_index(i1),
                     Surface_mesh_3::Vertex_index(i2),
                     Surface_mesh_3::Vertex_index(i3));

}

G4int G4SurfaceMeshCGAL::AddFace(int i1, int i2, int i3, int i4)
{
  return sm.add_face(Surface_mesh_3::Vertex_index(i1),
                     Surface_mesh_3::Vertex_index(i2),
                     Surface_mesh_3::Vertex_index(i3),
                     Surface_mesh_3::Vertex_index(i4));
}

std::vector<G4double> G4SurfaceMeshCGAL::GetVertex(G4int iVertex)
{
  std::vector<G4double> v = std::vector<G4double>();
  Surface_mesh_3::Vertex_index vi = Surface_mesh_3::Vertex_index(iVertex);
  Surface_mesh_3::Point p = sm.point(vi);
  v.push_back(CGAL::to_double(p.x()));
  v.push_back(CGAL::to_double(p.y()));
  v.push_back(CGAL::to_double(p.z()));
  return v;
}

std::vector<G4int> G4SurfaceMeshCGAL::GetFace(G4int iFace)
{
  std::vector<G4int> f = std::vector<G4int>();

  Surface_mesh_3::Face_index fd = Surface_mesh_3::Face_index(iFace);

  for (Surface_mesh_3::Halfedge_index hd : CGAL::halfedges_around_face(sm.halfedge(fd), sm)) {
    f.push_back((int)sm.source(hd));
  }
  return f;
}

int G4SurfaceMeshCGAL::NumberOfVertices()
{
  return sm.number_of_vertices();
}

int G4SurfaceMeshCGAL::NumberOfFaces()
{
  return sm.number_of_faces();
}

int G4SurfaceMeshCGAL::NumberOfBorderEdges() {
  int nBorderEdge = 0;
  for(Surface_mesh_3::halfedge_index hi : sm.halfedges()) {
    if(sm.is_border(hi)) {
      nBorderEdge++;
    }
  }
  G4cout << "G4SurfaceMeshCGAL::NumberOfBorderEdges> " << nBorderEdge << G4endl;
  return nBorderEdge;
}

int G4SurfaceMeshCGAL::NumberOfNonManifoldVertices() {
  // Count non manifold vertices
  int counter = 0;
  for(vertex_descriptor v : vertices(sm))
  {
    if(CGAL::Polygon_mesh_processing::is_non_manifold_vertex(v, sm))
    {
      std::cout << "vertex " << v << " is non-manifold" << std::endl;
      ++counter;
    }
  }
  return counter;
}

int G4SurfaceMeshCGAL::IsValid()
{
  return sm.is_valid(true);
}

int G4SurfaceMeshCGAL::IsTriangular()
{
  return CGAL::is_triangle_mesh(sm);
}

int G4SurfaceMeshCGAL::IsOutwardOriented()
{
  return CGAL::Polygon_mesh_processing::is_outward_oriented(sm);
}

int G4SurfaceMeshCGAL::IsClosed()
{
  return CGAL::is_closed(sm);
}

int G4SurfaceMeshCGAL::IsValidHalfEdgeGraph()
{
  return CGAL::is_valid_halfedge_graph(sm);
}

int G4SurfaceMeshCGAL::BoundAVolume()
{
  return CGAL::Polygon_mesh_processing::does_bound_a_volume(sm);
}

int G4SurfaceMeshCGAL::DoesSelfIntersect() {
  return CGAL::Polygon_mesh_processing::does_self_intersect(sm);
}

double G4SurfaceMeshCGAL::Area()
{
  return CGAL::to_double(CGAL::Polygon_mesh_processing::area(sm));
}

double G4SurfaceMeshCGAL::Volume()
{
  return CGAL::to_double(CGAL::Polygon_mesh_processing::volume(sm));
}

void G4SurfaceMeshCGAL::StitchBorders() {
  CGAL::Polygon_mesh_processing::stitch_borders(sm);
}

std::size_t G4SurfaceMeshCGAL::RemoveIsolatedVertices() {
  auto nIsolated = CGAL::Polygon_mesh_processing::remove_isolated_vertices(sm);
  G4cout << "G4SurfaceMeshCGAL::RemoveIsolatedVertices> " << nIsolated << G4endl;
  return nIsolated;
}


void G4SurfaceMeshCGAL::ReverseFaceOrientations() {
  CGAL::Polygon_mesh_processing::reverse_face_orientations(sm);
}

void G4SurfaceMeshCGAL::IsotropicRemesh(G4int nIter, G4double targetEdgeLength) {
  CGAL::Polygon_mesh_processing::isotropic_remeshing(faces(sm),
                                                     targetEdgeLength,
                                                     sm,CGAL::parameters::number_of_iterations(nIter).protect_constraints(true));
}

void G4SurfaceMeshCGAL::RemoveDuplicates() {
  std::vector<Point_3> points;
  std::vector<Polygon> polygons;
  CGAL::Polygon_mesh_processing::polygon_mesh_to_polygon_soup(sm,points,polygons);

  //remove duplicated points and polygons
  //CGAL::Polygon_mesh_processing::merge_duplicate_points_in_polygon_soup(points,polygons);
  //CGAL::Polygon_mesh_processing::merge_duplicate_polygons_in_polygon_soup(points,polygons);
  G4cout << points.size() << G4endl;
  CGAL::Polygon_mesh_processing::repair_polygon_soup(points,polygons,
                                                     CGAL::parameters::erase_all_duplicates(true).require_same_orientation(true));

  G4cout << points.size() << G4endl;


  CGAL::Polygon_mesh_processing::orient_polygon_soup(points,polygons);
  CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points,polygons,sm);
}

void G4SurfaceMeshCGAL::RemoveSelfIntersections() {
  // CGAL::Polygon_mesh_processing::remove_self_intersections(sm, CGAL::parameters::preserve_genus(false));
}

std::size_t G4SurfaceMeshCGAL::RemoveConnectedComponentsOfNegligibleSize() {
  auto ncompt = CGAL::Polygon_mesh_processing::remove_connected_components_of_negligible_size(sm);
#ifdef G4CGAL_DEBUG
    G4cout << "G4SurfaceMeshCGAL::RemoveConnectedComponentsOfNegligibleSize> Components " << ncompt << G4endl;
#endif
  return ncompt;
}

std::size_t G4SurfaceMeshCGAL::KeepLargestConnectedComponents(int iKeep)
{
  return CGAL::Polygon_mesh_processing::keep_largest_connected_components(sm, iKeep);
}

std::vector<G4SurfaceMeshCGAL*> G4SurfaceMeshCGAL::DecomposeConnected()
{
  typedef Surface_mesh_3 Mesh;
  typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;
  typedef Mesh::Property_map<face_descriptor, std::size_t> F_sizet_map;
  face_descriptor fd = *faces(sm).first;
  F_sizet_map fccmap = sm.add_property_map<face_descriptor, std::size_t>("f:CC").first;

  auto sm1 = Surface_mesh_3(sm);
  int ncompt = CGAL::Polygon_mesh_processing::connected_components(sm1, fccmap);
#ifdef G4CGAL_DEBUG
  G4cout << "G4SurfaceMeshCGAL::DecomposeConnected> Components " << ncompt << G4endl;
#endif
  return std::vector<G4SurfaceMeshCGAL*>();
}

void G4SurfaceMeshCGAL::WriteMesh(std::string fn)
{
  std::ofstream ofstr(fn);
  ofstr << sm;
}

void G4SurfaceMeshCGAL::DebugOutputHeader() {
  G4cout << "nv nf v h t o c b m s a v" << G4endl;
}
void G4SurfaceMeshCGAL::DebugOutput() {
  G4cout << NumberOfVertices() << " " << NumberOfFaces() << " " << IsValid() << " " << IsValidHalfEdgeGraph() << " " << IsTriangular() << " " << IsOutwardOriented() << " " << IsClosed() << " " << BoundAVolume() << " " << NumberOfNonManifoldVertices() << " " << DoesSelfIntersect() << " " << Area() << " " << Volume() << G4endl;
}