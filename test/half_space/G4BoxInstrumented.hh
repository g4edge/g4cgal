#pragma one

#include "G4GeomTypes.hh"

#include "G4CSGSolid.hh"
#include "G4Polyhedron.hh"

class G4BoxInstrumented : public G4CSGSolid
{
  public:

    G4BoxInstrumented(const G4String& pName, G4double pX, G4double pY, G4double pZ);
      // Construct a box with name, and half lengths pX,pY,pZ

    ~G4BoxInstrumented() override;

    void ComputeDimensions(G4VPVParameterisation* p,
                           const G4int n,
                           const G4VPhysicalVolume* pRep) override;

    void BoundingLimits(G4ThreeVector& pMin, G4ThreeVector& pMax) const override;

    G4bool CalculateExtent(const EAxis pAxis,
                           const G4VoxelLimits& pVoxelLimit,
                           const G4AffineTransform& pTransform,
                                 G4double& pMin, G4double& pMax) const override;

  // Accessors and modifiers

    inline G4double GetXHalfLength() const;
    inline G4double GetYHalfLength() const;
    inline G4double GetZHalfLength() const;

    void SetXHalfLength(G4double dx) ;
    void SetYHalfLength(G4double dy) ;
    void SetZHalfLength(G4double dz) ;

  // Methods for solid

    inline G4double GetCubicVolume() override;
    inline G4double GetSurfaceArea() override;

    EInside Inside(const G4ThreeVector& p) const override;
    G4ThreeVector SurfaceNormal( const G4ThreeVector& p) const override;
    G4double DistanceToIn(const G4ThreeVector& p,
                          const G4ThreeVector& v) const override;
    G4double DistanceToIn(const G4ThreeVector& p) const override;
    G4double DistanceToOut(const G4ThreeVector& p, const G4ThreeVector& v,
                           const G4bool calcNorm = false,
                                 G4bool* validNorm = nullptr,
                                 G4ThreeVector* n = nullptr) const override;
    G4double DistanceToOut(const G4ThreeVector& p) const override;

    G4GeometryType GetEntityType() const override;
    G4ThreeVector GetPointOnSurface() const override;

    G4VSolid* Clone() const override;

    std::ostream& StreamInfo(std::ostream& os) const override;

  // Utilities for visualization

    void          DescribeYourselfTo (G4VGraphicsScene& scene) const override;
    G4VisExtent   GetExtent          () const override;
    G4Polyhedron* CreatePolyhedron   () const override;

    G4BoxInstrumented(__void__&);
      // Fake default constructor for usage restricted to direct object
      // persistency for clients requiring preallocation of memory for
      // persistifiable objects.

    G4BoxInstrumented(const G4BoxInstrumented& rhs);
    G4BoxInstrumented& operator=(const G4BoxInstrumented& rhs);
      // Copy constructor and assignment operator.

  private:

    G4ThreeVector ApproxSurfaceNormal( const G4ThreeVector& p) const;
      // Algorithm for SurfaceNormal() following the original
      // specification for points not on the surface

  private:

    G4double fDx = 0.0, fDy = 0.0, fDz = 0.0;
    G4double delta;  // Cached half Cartesian tolerance
};

#include "G4BoxInstrumented.icc"
