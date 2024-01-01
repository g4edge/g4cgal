#pragma once

#include "G4VSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Polyline.hh"
#include "G4Text.hh"
#include "G4Circle.hh"
#include "G4Square.hh"
#include "G4Polymarker.hh"
#include "G4Polyhedron.hh"

class G4VSurfaceMesh;
class G4VPrimitiveLocator;
class G4TessellatedSolid;

class G4SurfaceMeshSolid : public G4VSolid {
public:
  G4SurfaceMeshSolid();
  G4SurfaceMeshSolid(G4String name,
                     G4VSurfaceMesh *mesh,
                     G4VPrimitiveLocator *pl = nullptr);
  virtual ~G4SurfaceMeshSolid();

  /* G4VSolid virtual methods */
  virtual G4bool CalculateExtent(const EAxis pAxis,
                                 const G4VoxelLimits& pVoxelLimit,
                                 const G4AffineTransform& pTransform,
                                 G4double& pMin, G4double& pMax) const override;
  virtual EInside Inside(const G4ThreeVector& p) const  override;
  virtual G4ThreeVector SurfaceNormal(const G4ThreeVector& p) const  override;
  virtual G4double DistanceToIn(const G4ThreeVector& p,
                                const G4ThreeVector& v) const override;
  virtual G4double DistanceToIn(const G4ThreeVector& p) const override;
  virtual G4double DistanceToOut(const G4ThreeVector& p,
                                 const G4ThreeVector& v,
                                 const G4bool calcNorm=false,
                                 G4bool* validNorm = nullptr,
                                 G4ThreeVector* n = nullptr) const  override;
  virtual G4double DistanceToOut(const G4ThreeVector& p) const  override;
  virtual G4GeometryType  GetEntityType() const  override;
  virtual std::ostream& StreamInfo(std::ostream& os) const  override;
  virtual void DescribeYourselfTo (G4VGraphicsScene& scene) const override;

  virtual G4TessellatedSolid* GetTessellatedSolid() const;

  virtual void Test(G4ThreeVector testPoint,
                    G4ThreeVector testDir = G4ThreeVector());
  virtual void TestRandomInside();


private:
  G4VSurfaceMesh *_mesh;
  G4VPrimitiveLocator *_pl;

  std::map<G4VisAttributes*,
           std::variant<G4Polyline*,
           G4Text*,
           G4Circle*,
           G4Square*,
           G4Polymarker*,
           G4Polyhedron*>> _debugPolyhedra;
};