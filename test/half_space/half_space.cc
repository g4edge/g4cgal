#include "G4HalfSpaceSolid.hh"
#include "G4HalfSpaceZone.hh"
#include "G4HalfSpacePlane.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"

#include "G4BooleanProcessorCGAL.hh"
#include "G4BooleanSolid.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4RunManagerFactory.hh"
#include "G4TransportationManager.hh"
#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "FTFP_BERT.hh"
#include "CGALActionInitialization.hh"
#include "CGALDetectorConstruction.hh"
#include "CGALPrimaryGeneratorAction.hh"

#include <vector>

int main(int argc, char** argv)
{
    auto* runManager = G4RunManagerFactory::CreateRunManager();

    runManager->SetUserInitialization(new CGALDetectorConstruction());
    runManager->SetUserInitialization(new FTFP_BERT);
    runManager->SetUserInitialization(new CGALActionInitialization());

    runManager->Initialize();

    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    runManager->BeamOn(0);

    if (argc == 4)  // batch mode
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[3];
        UImanager->ApplyCommand(command + fileName);
    }
    else  // interactive mode
    {
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    return 0;
}
