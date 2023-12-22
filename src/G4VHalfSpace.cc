#include "G4VHalfSpace.hh"
#include <unsupported/Eigen/Polynomials>

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
        return;
    }
    else if(des > 0) {
        nSoln = 2;
        x1 = (-b - sqrt(des))/(2*a);
        x2 = (-b + sqrt(des))/(2*a);
        return;
    }
    else if(des == 0) {
        nSoln = 1;
        x1 = -b/(2*a);
        return;
    }
}

void G4VHalfSpace::CubicSolve(G4double a, G4double b, G4double c, G4double d, G4int &nSoln, G4double &x1, G4double &x2, G4double &x3) {
}

void G4VHalfSpace::QuinticSolve(G4double a, G4double b, G4double c, G4double d, G4double e, G4int &nSoln, G4double &x1, G4double &x2, G4double &x3, G4double &x4){
    Eigen::Matrix<G4double, 5, 1> coeff(a, b, c, d, e);
    Eigen::PolynomialSolver<G4double, Eigen::Dynamic> solver;
    solver.compute(coeff);
    const Eigen::PolynomialSolver<double, Eigen::Dynamic>::RootsType &r = solver.roots();

    for(int i =0;i<r.rows();++i)
    {
        std::cout << i << " " << r[i] << std::endl;
    }
}

void G4VHalfSpace::QuadricSolve(G4double a, G4double b, G4double c, G4double e, G4double f, G4double g, G4int &nSoln, G4double &x1, G4double &x2) {
}


std::vector<G4double> G4VHalfSpace::PolynomialSolve(std::vector<G4double> &params){

    std::vector<G4double> roots;
    Eigen::Matrix<G4double, 5, 1> coeff;

    for(auto i =0;i<params.size();++i) {
        coeff[i] = params[i];
    }

    Eigen::PolynomialSolver<G4double, Eigen::Dynamic> solver;
    solver.compute(coeff);
    const Eigen::PolynomialSolver<double, Eigen::Dynamic>::RootsType &r = solver.roots();

    for(int i =0;i<r.rows();++i)
    {
        if(r[i].imag() == 0) {
            roots.push_back(r[i].real());
        }
    }
    return roots;
}