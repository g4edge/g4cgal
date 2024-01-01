#pragma once

#include "G4Types.hh"
#include <random>

class G4VSolid;

class G4SolidTrackingTimer {
public:
  G4SolidTrackingTimer(G4VSolid *,
                       G4double xMin, G4double xMax,
                       G4double yMin, G4double yMax,
                       G4double zMin, G4double zMax);
  ~G4SolidTrackingTimer();

  void SetPositionRange(G4double xMin, G4double xMax,
                        G4double yMin, G4double yMax,
                        G4double zMin, G4double zMax);

  void Run(G4int nTrials);
  G4double Inside(G4int nTrials);
  G4double DistanceToIn(G4int nTrials);
  G4double DistanceToInDir(G4int nTrials);
  G4double DistanceToOut(G4int nTrials);
  G4double DistanceToOutDir(G4int nTrials);

  void Print(G4int, ...);

private:
  G4VSolid *_solidToTest;

  G4double _xMin = -50;
  G4double _xMax =  50;
  G4double _yMin = -50;
  G4double _yMax =  50;
  G4double _zMin = -50;
  G4double _zMax =  50;

  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<> xDis, yDis, zDis;
  std::uniform_real_distribution<> xpDis, ypDis, zpDis;

  G4double _inside;
  G4double _distanceToIn;
  G4double _distanceToInDir;
  G4double _distanceToOut;
  G4double _distanceToOutDir;
};