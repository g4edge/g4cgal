#include "G4SolidTrackingTimer.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"

#include <stdarg.h>
#include <chrono>

G4SolidTrackingTimer::G4SolidTrackingTimer(G4VSolid *solidToTest,
                                           G4double xMin, G4double xMax,
                                           G4double yMin, G4double yMax,
                                           G4double zMin, G4double zMax) : _solidToTest(solidToTest) {
  gen = std::mt19937(rd());
  SetPositionRange(xMin, xMax,
                   yMin, yMax,
                   zMin, zMax);
}

G4SolidTrackingTimer::~G4SolidTrackingTimer() {}

void G4SolidTrackingTimer::SetPositionRange(G4double xMin, G4double xMax,
                                            G4double yMin, G4double yMax,
                                            G4double zMin, G4double zMax) {
  _xMin = xMin;
  _xMax = xMax;
  _yMin = yMin;
  _yMax = yMax;
  _zMin = zMin;
  _zMax = zMax;

  xDis = std::uniform_real_distribution<>(xMin, xMax);
  yDis = std::uniform_real_distribution<>(yMin, yMax);
  zDis = std::uniform_real_distribution<>(zMin, zMax);
  xpDis = std::uniform_real_distribution<>(-1, 1);
  ypDis = std::uniform_real_distribution<>(-1, 1);
  zpDis = std::uniform_real_distribution<>(-1, 1);
}

void G4SolidTrackingTimer::Run(G4int nTrials) {
  _inside = Inside(nTrials);
  _distanceToIn = DistanceToIn(nTrials);
  _distanceToInDir = DistanceToInDir(nTrials);
  _distanceToOut = DistanceToOut(nTrials);
  _distanceToOutDir = DistanceToOutDir(nTrials);
}

G4double G4SolidTrackingTimer::Inside(G4int nTrials) {
  auto begin = std::chrono::high_resolution_clock::now();

  auto pos = G4ThreeVector(xDis(gen), yDis(gen), zDis(gen));

  for(G4int iTrial=0; iTrial < nTrials; ++iTrial) {
    _solidToTest->Inside(pos);
  }

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/nTrials;
}

G4double G4SolidTrackingTimer::DistanceToIn(G4int nTrials) {
  auto begin = std::chrono::high_resolution_clock::now();

  auto pos = G4ThreeVector(xDis(gen), yDis(gen), zDis(gen));

  for(G4int iTrial=0; iTrial < nTrials; ++iTrial) {
    _solidToTest->DistanceToIn(pos);
  }

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/nTrials;
}

G4double G4SolidTrackingTimer::DistanceToInDir(G4int nTrials) {
  auto begin = std::chrono::high_resolution_clock::now();

  auto pos = G4ThreeVector(xDis(gen), yDis(gen), zDis(gen));
  auto dir = G4ThreeVector(xpDis(gen), ypDis(gen), zpDis(gen));
  dir = dir/dir.mag();
  for(G4int iTrial=0; iTrial < nTrials; ++iTrial) {
    _solidToTest->DistanceToIn(pos,dir);
  }

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/nTrials;
}

G4double G4SolidTrackingTimer::DistanceToOut(G4int nTrials) {
  auto begin = std::chrono::high_resolution_clock::now();

  auto pos = G4ThreeVector(xDis(gen), yDis(gen), zDis(gen));

  for(G4int iTrial=0; iTrial < nTrials; ++iTrial) {
    _solidToTest->DistanceToOut(pos);
  }

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/nTrials;
}

G4double G4SolidTrackingTimer::DistanceToOutDir(G4int nTrials) {
  auto begin = std::chrono::high_resolution_clock::now();

  auto pos = G4ThreeVector(xDis(gen), yDis(gen), zDis(gen));
  auto dir = G4ThreeVector(xpDis(gen), ypDis(gen), zpDis(gen));
  dir = dir/dir.mag();
  for(G4int iTrial=0; iTrial < nTrials; ++iTrial) {
    _solidToTest->DistanceToOut(pos,dir);
  }

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/nTrials;
}

void G4SolidTrackingTimer::Print(G4int iCount,...) {
  va_list args;
  va_start(args, iCount);

  G4cout << "inside (ns/call) " << _inside << G4endl;
  G4cout << "distToIn         " << _distanceToIn << G4endl;
  G4cout << "distToInDir      " << _distanceToInDir << G4endl;
  G4cout << "distToOut        " << _distanceToOut << G4endl;
  G4cout << "distToOutDir     " << _distanceToOutDir << G4endl;
}