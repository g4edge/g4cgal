#pragma once

#include "G4TwoVector.hh"
#include "G4ThreeVector.hh"

template <class T> class vector2;
template <class T> class vector3;
template <class T> class vector4;

template <class T>
class vector2{
  vector2() : x(0), y(0) {}
  vector2(T xIn, T yIn)
      : x(xIn), y(yIn) {}
  vector2(vector3<T> v)
      : x(v.x), y(v.y) {}
  double mag()
  {return  sqrt(x*x + y*y);}
  vector2<T> abs()
  {return  vector2<T>(fabs(x),fabs(y));}
  double dot(vector2<T> rhs)
  {return  x*rhs.x + y*rhs.y;};
  friend vector2<T> operator+(vector2<T> lhs,
                              vector2<T> rhs)
  {
    return vector2<T>(lhs.x+rhs.x,
                      lhs.y+rhs.y);
  };
  friend vector2<T> operator-(vector2<T> lhs,
                              vector2<T> rhs)
  {
    return vector3<T>(lhs.x-rhs.x,
                      lhs.y-rhs.y,
                      lhs.z-rhs.z);
  };
  friend vector2<T> operator*(vector2<T> lhs, T rhs)
  {
    return vector2<T>(rhs*lhs.x,
                      rhs*lhs.y);
  }
  friend vector2<T> operator*(T lhs, vector2<T> rhs)
  {
    return vector2<T>(lhs*rhs.x,
                      lhs*rhs.y);
  }
  friend vector2<T> operator/(vector2<T> lhs, T rhs)
  {
    return vector2<T>(lhs.x/rhs,
                      lhs.y/rhs);};
  friend std::ostream& operator<<(std::ostream &ostr, vector2<T> rhs) {
    ostr << rhs.x << " " << rhs.y;
    return ostr;
  }
  vector2<T> norm() {
    auto m = mag();
    return vector2<T>(x/m,
                      y/m);
  }
  T x;
  T y;
};

template <class T>
class vector3 {
public:
  vector3() : x(0), y(0), z(0) {}
  vector3(T xIn, T yIn, T zIn)
      : x(xIn), y(yIn), z(zIn) {}
  vector3(vector2<T> v, T zIn)
      : x(v.x), y(y.z), z(zIn) {}
  vector3(G4ThreeVector v) {
    x = v.x();
    y = v.y();
    z = v.z();
  }
  G4ThreeVector threeVector() {
    return G4ThreeVector(x,y,z);
  }
  double mag()
  {return  sqrt(x*x + y*y + z*z);};
  vector3<T> abs()
  {return  vector3<T>(fabs(x),fabs(y),fabs(z));};
  double dot(vector3<T> rhs)
  {return  x*rhs.x + y*rhs.y + z*rhs.z;};
  vector3<T> cross(vector3<T> rhs)
  {
    return vector3<T>( y*rhs.z - z*rhs.y,
                       z*rhs.x - x*rhs.z,
                       x*rhs.y - y*rhs.x);
  };
  vector3<T>& operator=(vector3<T> other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }
  friend vector3<T> operator+(vector3<T> lhs,
                              vector3<T> rhs)
  {
    return vector3<T>(lhs.x+rhs.x,
                      lhs.y+rhs.y,
                      lhs.z+rhs.z);
  };
  friend vector3<T> operator-(vector3<T> lhs,
                              vector3<T> rhs)
  {
    return vector3<T>(lhs.x-rhs.x,
                      lhs.y-rhs.y,
                      lhs.z-rhs.z);
  };
  friend vector3<T> operator*(vector3<T> lhs, T rhs)
  {
    return vector3<T>(rhs*lhs.x,
                      rhs*lhs.y,
                      rhs*lhs.z);
  }
  friend vector3<T> operator*(T lhs, vector3<T> rhs)
  {
    return vector3<T>(lhs*rhs.x,
                      lhs*rhs.y,
                      lhs*rhs.z);
  }
  friend vector3<T> operator/(vector3<T> lhs, T rhs)
  {
    return vector3<T>(lhs.x/rhs,
                      lhs.y/rhs,
                      lhs.z/rhs);};
  friend std::ostream& operator<<(std::ostream &ostr, vector3<T> rhs) {
    ostr << rhs.x << " " << rhs.y << " " << rhs.z;
    return ostr;
  }
  vector3<T> norm() {
    auto m = mag();
    return vector3<T>(x/m,
                      y/m,
                      z/m);
  }
  T x;
  T y;
  T z;
};

template <class T>
class vector4 {
public:
  vector4() : x(0), y(0), z(0), w(0) {}
  vector4(T xIn, T yIn, T zIn, T wIn)
      : x(xIn), y(yIn), z(zIn), w(wIn) {}
  double mag()
  {return  sqrt(x*x + y*y + z*z + w*w);}
  vector4<T> abs()
  {return vector4<T>(fabs(x),fabs(y),fabs(z), fabs(w));};
  friend std::ostream& operator<<(std::ostream &ostr, vector4<T> rhs) {
    ostr << rhs.x << " " << rhs.y << " " << rhs.z << " " << rhs.w;
    return ostr;
  }
  T x;
  T y;
  T z;
  T w;
};

template <class T>
class plane3 {
public:
  plane3(vector3<T> pIn, vector3<T> nIn)
      : p(pIn), n(nIn) {}
  plane3(vector3<T> p1, vector3<T> p2, vector3<T> p3)
  {
    n = (p2-p1)/(p2-p1).norm().cross((p3-p1)/(p3-p1).norm());
    p = p1;
  }
  T closest(vector3<T> point)
  {return point - (point - p).dot(n);}
  vector3<T> p;
  vector3<T> n;

};

template <class T>
class triangle3 {
public:
  triangle3(vector3<T> p1In, vector3<T> p2In, vector3<T> p3In)
      : p1(p1In), p2(p2In), p3(p3In) {}
  plane3<T> plane()
  { return plane3(p1,p2,p3);}
  T closest(vector3<T> point)
  {return 0;}
  vector3<T> p1;
  vector3<T> p2;
  vector3<T> p3;
};

template <class T>
class line3 {
public:
  line3(vector3<T> p1In, vector3<T> p2In)
      : p1(p1In), p2(p2In) {}
  T closest(vector3<T> point)
  {return 0;}
  vector3<T> p1;
  vector3<T> p2;
};

template <class T>
class ray3 {
public:
  ray3(vector3<T> pIn, vector3<T> dIn)
      : p(pIn), d(dIn) {}
  vector3<T> p;
  vector3<T> d;
};

template<typename T> T clamp(T value, T minValue, T maxValue) {
  if(value<minValue) {
    return minValue;
  }
  else if(value>maxValue) {
    return maxValue;
  }
  else {
    return value;
  }
};
template<typename T> T cross(T v1, T v2) {return v1.cross(v2);};
template<typename T> T dot(vector2<T> v1, vector2<T> v2) {return v1.dot(v2);};
template<typename T> T dot(vector3<T> v1, vector3<T> v2) {return v1.dot(v2);};
template<typename T> T dot(vector4<T> v1, vector4<T> v2) {return v1.dot(v2);};
template<typename T> T dot2(vector2<T> v1) {return v1.dot(v1);};
template<typename T> T dot2(vector3<T> v1) {return v1.dot(v1);};
template<typename T> T length(T v) {return fabs(v);};
template<typename T> T length(vector2<T> v) {return v.mag();};
template<typename T> T length(vector3<T> v) {return v.mag();};
template<typename T> T length(vector4<T> v) {return v.mag();};
template<typename T> T min(T v1, T v2) {return v1 < v2 ? v1 : v2;};
template<typename T> T min(T v1, T v2, T v3) {return min<T>(min<T>(v1,v2),v3);};
template<typename T> vector2<T> min(vector2<T> v1, vector2<T> v2) {return vector2<T>(v1.x < v2.x ? v1.x : v2.x,
                                                                            v1.y < v2.y ? v1.y : v2.y);};
template<typename T> vector3<T> min(vector3<T> v1, vector3<T> v2) {return vector3<T>(v1.x < v2.x ? v1.x : v2.x,
                                                                            v1.y < v2.y ? v1.y : v2.y,
                                                                            v1.z < v2.z ? v1.z : v2.z);};
template<typename T> vector2<T> min(vector2<T> v1, T v2) {return vector2<T>(v1.x < v2 ? v1.x : v2,
                                                                   v1.y < v2 ? v1.y : v2);};
template<typename T> vector3<T> min(vector3<T> v1, T v2) {return vector3<T>(v1.x < v2 ? v1.x : v2,
                                                                   v1.y < v2 ? v1.y : v2,
                                                                   v1.z < v2 ? v1.z : v2);};
template<typename T> vector2<T> max(vector2<T> v1, vector2<T> v2) {return vector2<T>(v1.x > v2.x ? v1.x : v2.x,
                                                                            v1.y > v2.y ? v1.y : v2.y);};
template<typename T> vector3<T> max(vector3<T> v1, vector3<T> v2) {return vector3<T>(v1.x > v2.x ? v1.x : v2.x,
                                                                            v1.y > v2.y ? v1.y : v2.y,
                                                                            v1.z > v2.z ? v1.z : v2.z);};
template<typename T> vector2<T> max(vector2<T> v1, T v2) {return vector2<T>(v1.x > v2 ? v1.x : v2,
                                                                            v1.y > v2 ? v1.y : v2);};
template<typename T> vector3<T> max(vector3<T> v1, T v2) {return vector3<T>(v1.x > v2 ? v1.x : v2,
                                                                            v1.y > v2 ? v1.y : v2,
                                                                            v1.z > v2 ? v1.z : v2);};
template<typename T> T max(vector2<T> v1, T v2) {return vector2<T>(v1.x > v2 ? v1.x : v2,
                                                                   v1.y > v2 ? v1.y : v2);};
template<typename T> T max(vector3<T> v1, T v2) {return vector3<T>(v1.x > v2 ? v1.x : v2,
                                                                   v1.y > v2 ? v1.y : v2,
                                                                   v1.z > v2 ? v1.z : v2);};
template<typename T> T ndot(vector2<T> v1, vector2<T> v2) {return v1.x*v2.x - v1.y*v2.y;};
template <typename T> int sign(T val) {return (T(0) < val) - (val < T(0));};

G4TwoVector clamp(G4TwoVector &v, G4TwoVector &min, G4TwoVector &max) {
  return G4TwoVector(v.x() < min.x() ? min.x() : v.x() > max.x() ? max.x() : v.x(),
                     v.y() < min.y() ? min.y() : v.y() > max.y() ? max.y() : v.y());
}
G4ThreeVector clamp(G4ThreeVector &v, G4ThreeVector &min, G4ThreeVector &max) {
  return G4ThreeVector(v.x() < min.x() ? min.x() : v.x() > max.x() ? max.x() : v.x(),
                       v.y() < min.y() ? min.y() : v.y() > max.y() ? max.y() : v.y(),
                       v.z() < min.z() ? min.z() : v.z() > max.z() ? max.z() : v.z());
}
G4ThreeVector cross(G4ThreeVector &v1, G4ThreeVector &v2) {return v1.cross(v2);};
G4double dot(G4ThreeVector &v1, G4ThreeVector &v2) {return v1.dot(v2);};
G4double dot2(G4ThreeVector &v) {return v.dot(v);};
G4double dot(G4TwoVector &v1, G4TwoVector &v2) {return v1.dot(v2);};
G4double dot2(G4TwoVector &v) {return v.dot(v);};
G4double length(G4ThreeVector v) {return v.mag();};
G4double length(G4TwoVector v) {return v.mag();};
G4ThreeVector min(G4ThreeVector &v1, G4ThreeVector &v2) {return G4ThreeVector(v1.x() < v2.x() ? v1.x() : v2.x(),
                                                                              v1.y() < v2.y() ? v1.y() : v2.y(),
                                                                              v1.z() < v2.z() ? v1.z() : v2.z());};
G4ThreeVector min(G4ThreeVector &v1, G4double v2) {return G4ThreeVector(v1.x() < v2 ? v1.x() : v2,
                                                                        v1.y() < v2 ? v1.y() : v2,
                                                                        v1.z() < v2 ? v1.z() : v2);};
G4TwoVector min(G4TwoVector &v1, G4TwoVector &v2) {return G4TwoVector(v1.x() < v2.x() ? v1.x() : v2.x(),
                                                                      v1.y() < v2.y() ? v1.y() : v2.y());};
G4TwoVector min(G4TwoVector &v1, G4double v2) {return G4TwoVector(v1.x() < v2 ? v1.x() : v2,
                                                                  v1.y() < v2 ? v1.y() : v2);};
G4ThreeVector max(G4ThreeVector &v1, G4ThreeVector &v2) {return G4ThreeVector(v1.x() > v2.x() ? v1.x() : v2.x(),
                                                                              v1.y() > v2.y() ? v1.y() : v2.y(),
                                                                              v1.z() > v2.z() ? v1.z() : v2.z());};
G4ThreeVector max(G4ThreeVector &v1, G4double v2) {return G4ThreeVector(v1.x() > v2 ? v1.x() : v2,
                                                                        v1.y() > v2 ? v1.y() : v2,
                                                                        v1.z() > v2 ? v1.z() : v2);};
G4TwoVector max(G4TwoVector &v1, G4TwoVector &v2) {return G4TwoVector(v1.x() > v2.x() ? v1.x() : v2.x(),
                                                                      v1.y() > v2.y() ? v1.y() : v2.y());};
G4TwoVector max(G4TwoVector &v1, G4double v2) {return G4TwoVector(v1.x() > v2 ? v1.x() : v2,
                                                                  v1.y() > v2 ? v1.y() : v2);};
G4TwoVector ndot(G4TwoVector &v1, G4TwoVector &v2) {
  return G4TwoVector(v1.x()*v2.x() - v2.y()*v2.y());
};
G4TwoVector sign(G4TwoVector &v) {
  return G4TwoVector( (0 < v.x())- (v.x() < 0),
                      (0 < v.y())- (v.y() < 0));
};
G4ThreeVector sign(G4ThreeVector &v) {
  return G4ThreeVector( (0 < v.x())- (v.x() < 0),
                        (0 < v.y())- (v.y() < 0),
                      (0 < v.z())- (v.z() < 0));
}