#include "G4VHalfSpace.hh"

G4VHalfSpace::G4VHalfSpace() {}

G4VHalfSpace::~G4VHalfSpace() {}

void G4VHalfSpace::QuadraticSolve(G4double a, G4double b, G4double c,
                                  G4int &nSoln, G4double &x1, G4double &x2) {
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
