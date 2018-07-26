<p align="left">
  <img src="share/MuonPortalPic.png" alt="MuonPortal project logo"/>
</p>

# MuonPortalGUI
MuonPortal GUI for tomography data imaging processing

## **About**  
MuonPortalGUI is a C++ software tool to perform muon tomography imaging computation. It was developed within the MuonPortal project and it is distributed for research use only under the GNU General Public License v3.0. 

## **Credit**
If you use MuonPortalGUI for your research, please acknowledge it in your papers by citing the following paper:

* `S. Riggi et al., "Muon tomography imaging algorithms for nuclear threat detection inside large volume containers with the Muon Portal detector", Nucl. Instr. and Meth. A 728 (2013) 59`

or consider including me (`S. Riggi, INAF - Osservatorio Astrofisico di Catania, Via S. Sofia 78, I-95123, Catania, Italy`)
as a co-author on your publications.

## **Status**
This software is currently not actively maintained. Building with recent versions of GEANT4/ROOT/VTK/Qt may require fixing the Makefile. The software was used with:
* OS: Ubuntu 14.04/16.04, Centos 6
* QT 4 (not tested with QT 5)
* VTK 5 (not tested with VTK 6) 
* ROOT 5, 6
* GEANT 4.9.X

## **Installation**  

### **Prerequisites**
Install the system libraries (example for Ubuntu):
* libphonon-dev libphonon4 phonon-backend-gstreamer
* libafterimage-dev

Install the project mandatory dependencies:  
* ROOT [https://root.cern.ch/]
* GEANT4 [https://geant4.web.cern.ch/] 
* VTK library [https://www.vtk.org/], built with 
* QT [https://www.qt.io], enable QVTKWidget build
* QJson [https://github.com/flavio/qjson.git]    
* MPI library: MPICH [https://www.mpich.org/] or OpenMPI [https://www.open-mpi.org/]

Make sure you have set the following environment variables to the external library installation dirs 
* ROOTSYS, set to ROOT installation prefix
* G4INSTALL, set to GEANT4 installation prefix
* G4LIB, set to GEANT4 installed library path
* MPI_DIR, set to MPI installation prefix
* BOOST_ROOT
* LOG4CXX_ROOT
* JSONCPP_ROOT

Add also the following paths to the PKG_CONFIG_PATH environment var: 
* $LOG4CXX_ROOT/lib/pkgconfig  
* $JSONCPP_ROOT/lib/pkgconfig

CAESAR depends also on the wcstools and linterp libraries which are already provided in the external/ directory. 
**NB: The provided wcslib was slightly modified with respect to the original release to avoid naming conflicts with the R package due to some #define macros used in WCS.**

cmake should find all needed include dirs and libraries used to build the project.

### **Build and install**
To build and install the project:

* Clone this repository into your local $SOURCE_DIR  
  ```git clone https://github.com/simoneriggi/ska-dsh_lmc.git $SOURCE_DIR```
* Create the build and install directories: $BUILD_DIR, $INSTALL_DIR  
* In the build directory:  
  ```cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DENABLE_TEST=ON -DBUILD_WITH_OPENMP=ON -DENABLE_MPI=ON -DBUILD_APPS=ON $SOURCE_DIR```   
  ```make```  
  ```make install```  
