<p align="left">
  <img src="share/CaesarLogo.png" alt="Caesar source finder logo"/>
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
This software is currently not actively maintained. Building with recent versions of GEANT4/ROOT/VTK/Qt may require fixing the Makefile.

## **Installation**  

### **Prerequisites**
Install the project mandatory dependencies:  
* ROOT [https://root.cern.ch/]
* GEANT4 [https://geant4.web.cern.ch/] 
* cfitsio [https://heasarc.gsfc.nasa.gov/fitsio/fitsio.html], to be built with multithread support (e.g. give --enable-reentrant option in configure)
* protobuf [https://github.com/google/protobuf]
* jsoncpp [https://github.com/open-source-parsers/jsoncpp]
* python (>=2.7) [https://www.python.org/], install also these additional modules: pyfits, astropy
* cmake (>=2.8) [https://cmake.org]  
  
Optional dependencies are:
* MPICH [https://www.mpich.org/], needed when the build option ENABLE_MPI=ON        
* OpenMP [http://www.openmp.org/], needed when the build option BUILD_WITH_OPENMP=ON    
* GoogleTest [https://github.com/google/googletest], needed for unit testing when the build option ENABLE_TEST=ON   
* Doxygen [www.doxygen.org/], needed to generate the API documentation   

Make sure you have set the following environment variables to the external library installation dirs 
* ROOTSYS
* OPENCV_DIR
* R_DIR
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
