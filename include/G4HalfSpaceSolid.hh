#include "G4VSolid.hh"
#include "G4HalfSpaceZone.hh"

class G4HalfSpaceSolid : public G4VSolid {
public:
    G4HalfSpaceSolid();
    G4HalfSpaceSolid(const char *);
    G4HalfSpaceSolid(const G4String&);

    ~G4HalfSpaceSolid();

    void addZone(G4HalfSpaceZone *zone);
    void removeZone(G4HalfSpaceZone *zone);

    G4int numberOfZones();

    std::vector<G4HalfSpaceSolid*> connectedSolids();

    virtual G4bool CalculateExtent(const EAxis pAxis,
                                   const G4VoxelLimits& pVoxelLimit,
                                   const G4AffineTransform& pTransform,
                                   G4double& pMin, G4double& pMax) const;
    virtual EInside Inside(const G4ThreeVector& p) const;
    virtual G4ThreeVector SurfaceNormal(const G4ThreeVector& p) const;
    virtual G4double DistanceToIn(const G4ThreeVector& p,
                                  const G4ThreeVector& v) const;
    virtual G4double DistanceToIn(const G4ThreeVector& p) const;
    virtual G4double DistanceToOut(const G4ThreeVector& p,
                                   const G4ThreeVector& v,
                                   const G4bool calcNorm=false,
                                   G4bool* validNorm = nullptr,
                                   G4ThreeVector* n = nullptr) const;
    virtual G4double DistanceToOut(const G4ThreeVector& p) const;
    virtual G4GeometryType  GetEntityType() const;
    virtual std::ostream& StreamInfo(std::ostream& os) const;
    virtual void DescribeYourselfTo (G4VGraphicsScene& scene) const;

protected:
    std::vector<G4HalfSpaceZone*> _zones;

};