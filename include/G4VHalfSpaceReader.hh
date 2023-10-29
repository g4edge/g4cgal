#pragma once

#include <map>

#include "G4string.hh"

#include "G4HalfSpaceSolid.hh"

class G4VHalfSpaceReader {
public:
    G4VHalfSpaceReader();
    ~G4VHalfSpaceReader();
    virtual void Read() = 0;

protected:
    std::map<G4String, G4HalfSpaceSolid*> _solids;

};