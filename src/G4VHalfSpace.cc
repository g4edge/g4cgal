#include "G4VHalfSpace.hh"


G4VHalfSpace::G4VHalfSpace() {}

G4VHalfSpace::~G4VHalfSpace() {}

EInside G4VHalfSpace::Inside(const G4ThreeVector &p) const {
    auto sdf = Sdf(p);
    if (sdf > 1e-9) {
        return EInside::kOutside;
    }
    else if(sdf < -1e-9) {
        return EInside::kInside;
    }
    else {
        return EInside::kSurface;
    }
}

G4ThreeVector G4VHalfSpace::Normal(const G4ThreeVector& p, G4double d) const {
    G4ThreeVector n = G4ThreeVector(Sdf(p+G4ThreeVector(d,0,0))-Sdf(p-G4ThreeVector(d,0,0)),
                                    Sdf(p+G4ThreeVector(0,d,0))-Sdf(p-G4ThreeVector(0,d,0)),
                                    Sdf(p+G4ThreeVector(0,0,d))-Sdf(p-G4ThreeVector(0,0,d)));
    n = n/n.mag();

    return n;
}

void G4VHalfSpace::QuadraticSolve(G4double a, G4double b, G4double c,
                                  G4int &nSoln, G4double &x1, G4double &x2)  {
    nSoln = 0;
    x1 = 0;
    x2 = 0;

    auto des = b*b - 4*a*c;
    if(des < 0 ) {
        G4cout << "G4VHalfSpace::QuadraticSolve> 0, " << x1 << " " << x2 << G4endl;
        return;
    }
    else if(des > 0) {
        nSoln = 2;
        x1 = (-b - sqrt(des))/(2*a);
        x2 = (-b + sqrt(des))/(2*a);
        G4cout << "G4VHalfSpace::QuadraticSolve> 2, " << x1 << " " << x2 << G4endl;
        return;
    }
    else if(des == 0) {
        nSoln = 1;
        x1 = -b/(2*a);
        G4cout << "G4VHalfSpace::QuadraticSolve> 1, " << x1 << G4endl;
        return;
    }
}
