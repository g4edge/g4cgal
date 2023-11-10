#pragma once

#include "G4ThreeVector.hh"
#include "G4VHalfSpace.hh"

class G4HalfSpaceZone;

class G4HalfSpaceAARBox : public G4VHalfSpace {
public:
    G4HalfSpaceAARBox();
    G4HalfSpaceAARBox(G4double xmin, G4double xmax, G4double ymin, G4double ymax, G4double zmin, G4double zmax);
    ~G4HalfSpaceAARBox();

    virtual G4double Sdf(const G4ThreeVector&p) const override;
    std::vector<G4ThreeVector> Intersection(const G4ThreeVector& p, const G4ThreeVector& v) const override;

    virtual G4SurfaceMeshCGAL* GetSurfaceMesh() const override;


protected:
    G4double _xmin = -1.0;
    G4double _xmax =  1.0;
    G4double _ymin = -1.0;
    G4double _ymax =  1.0;
    G4double _zmin = -1.0;
    G4double _zmax =  1.0;

    G4HalfSpaceZone* _hsZone;
};