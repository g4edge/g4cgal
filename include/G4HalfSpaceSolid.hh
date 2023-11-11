#pragma once

#include "G4VSolid.hh"
#include "G4HalfSpaceZone.hh"

class G4HalfSpaceSolid : public G4VSolid, G4VHalfSpace {
public:
    G4HalfSpaceSolid();
    G4HalfSpaceSolid(const char *);
    G4HalfSpaceSolid(const G4String&);

    ~G4HalfSpaceSolid();

    G4double Sdf(const G4ThreeVector &) const override;
    virtual std::vector<G4ThreeVector> Intersection(const G4ThreeVector& p, const G4ThreeVector &v) const override;
    virtual void Translate(const G4ThreeVector& t) override;
    virtual void Rotate(const G4RotationMatrix& r) override;
    virtual void Transform(const G4AffineTransform& a) override;
    virtual G4SurfaceMeshCGAL* GetSurfaceMesh() const override;

    void addZone(G4HalfSpaceZone *zone);
    void removeZone(G4HalfSpaceZone *zone);
    G4int numberOfZones();

    std::vector<G4HalfSpaceSolid*> connectedSolids();

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

protected:
    std::vector<G4HalfSpaceZone*> _zones;

};