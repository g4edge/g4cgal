#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(geant4cgal)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available Vis drivers by default
# You can set WITH_GEANT4_VIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_VIS "Build example with Geant4 Vis drivers" ON)
if(WITH_GEANT4_VIS)
  find_package(Geant4 REQUIRED gdml vis_all)
else()
  find_package(Geant4 REQUIRED gdml)
endif()

#----------------------------------------------------------------------------
# Add external 3rd party libraries
#

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
find_package(CGAL 5 REQUIRED)
find_package(Catch2 REQUIRED)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})


file(GLOB CGALLibSources ${PROJECT_SOURCE_DIR}/src/*cc)
file(GLOB CGALLibHeaders ${PROJECT_SOURCE_DIR}/include/*.hh)
add_library(g4cgal SHARED ${CGALLibSources} ${CGALLibHeaders})
target_compile_options(g4cgal PRIVATE -Werror )
target_link_libraries(g4cgal ${Geant4_LIBRARIES} CGAL::CGAL)

file(GLOB LoadGdmlSources ${PROJECT_SOURCE_DIR}/test/load_gdml/*cc)
file(GLOB LoadGdmlHeaders ${PROJECT_SOURCE_DIR}/test/load_gdml/*hh)
add_executable(load_gdml ${LoadGdmlSources} ${LoadGdmlHeaders})
target_compile_options(load_gdml PRIVATE -Werror)
target_link_libraries(load_gdml ${Geant4_LIBRARIES} CGAL::CGAL g4cgal)

file(GLOB HalfSpaceSources ${PROJECT_SOURCE_DIR}/test/half_space/*cc)
file(GLOB HalfSpaceHeaders ${PROJECT_SOURCE_DIR}/test/half_space/*hh)
add_executable(half_space ${HalfSpaceSources} ${HalfSpaceHeaders})
target_compile_options(half_space PRIVATE -Werror)
target_link_libraries(half_space ${Geant4_LIBRARIES} CGAL::CGAL g4cgal)
