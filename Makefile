#
# General Makefile for the OfflineUser package
#
#

# Replace the wildcard expression with .cc file list if you do
# not want to compile all .cc files in this directory
#
TOPDIR= $(shell pwd)

BINDIR  = $(TOPDIR)/bin
LIBDIR  = $(TOPDIR)/lib
SRCTOPDIR = $(TOPDIR)/src
INCTOPDIR = $(TOPDIR)/include

SRCDIR = $(SRCTOPDIR)
SRCDIR_GUI = $(SRCTOPDIR)

INCDIR  = -I$(INCTOPDIR)
INCDIR_GUI= $(INCTOPDIR)
OBJDIR  = $(TOPDIR)/obj

USER_SRCS = $(wildcard $(SRCDIR)/*.cc)
OBJS = $(USER_SRCS:.cc=.o)

MAIN_GUI = MPToolkit.cc
MAIN_GUI_OBJ = $(MAIN_GUI:.cc=.o)
MAIN_VIEWER = Viewer.cc
MAIN_VIEWER_OBJ = $(MAIN_VIEWER:.cc=.o)
MAIN_ADVANCEDVIEWER = AdvancedViewer.cc
MAIN_ADVANCEDVIEWER_OBJ = $(MAIN_ADVANCEDVIEWER:.cc=.o)
MAIN_EMMLIMAGING = EMMLTomographyImaging.cc
MAIN_EMMLIMAGING_OBJ = $(MAIN_EMMLIMAGING:.cc=.o)
MAIN_POCAIMAGING = POCATomographyImaging.cc
MAIN_POCAIMAGING_OBJ = $(MAIN_POCAIMAGING:.cc=.o)
MAIN_FOFIMAGING = FOFTomographyImaging.cc
MAIN_FOFIMAGING_OBJ = $(MAIN_FOFIMAGING:.cc=.o)
MAIN_ACFIMAGING = ACFTomographyImaging.cc
MAIN_ACFIMAGING_OBJ = $(MAIN_ACFIMAGING:.cc=.o)
MAIN_DOWNLOADER = Downloader.cc
MAIN_DOWNLOADER_OBJ = $(MAIN_DOWNLOADER:.cc=.o)


MOC_HEAD = $(subst $(INCDIR_GUI)/,,$(shell grep -l Q_OBJECT $(INCDIR_GUI)/*.h))
MOC_SOURCES = $(patsubst %.h, $(SRCDIR_GUI)/moc_%.cxx, $(MOC_HEAD) )
MOC_SOURCES += $(SRCDIR_GUI)/moc_serializerrunnable.cxx
MOC_SOURCES += $(SRCDIR_GUI)/moc_parserrunnable.cxx

MOC_HEADERS = $(patsubst %, $(INCDIR_GUI)/%, $(MOC_HEAD) )
MOC_OBJ = $(patsubst %.cxx, %.o, $(MOC_SOURCES) )
MOC_OBJ += $(SRCDIR_GUI)/moc_parserrunnable.o $(SRCDIR_GUI)/moc_serializerrunnable.o


VIEWER_OBJS = $(OBJDIR)/POCAImageViewer.o $(OBJDIR)/EMLLImageViewer.o $(OBJDIR)/ACFImageViewer.o $(OBJDIR)/ClusteringImageViewer.o $(OBJDIR)/TASImageNew.o

VIEWER_HEADERS = $(INCTOPDIR)/POCAImageViewer.h $(INCTOPDIR)/EMLLImageViewer.h $(INCTOPDIR)/ACFImageViewer.h $(INCTOPDIR)/ClusteringImageViewer.h $(INCTOPDIR)/TASImageNew.h
ADVANCED_VIEWER_HEADERS = $(INCTOPDIR)/VolumeRenderingViewer.h
EMMLIMAGING_HEADERS = $(INCTOPDIR)/EMTomographyRecoThreadObj.h $(INCTOPDIR)/G4DetectorGeometry.h $(INCTOPDIR)/G4PhysicsList.h $(INCTOPDIR)/G4PrimaryGenerator.h $(INCTOPDIR)/SteppingVerbose.h

## Get platform 32/64 bit
LBITS   = $(shell getconf LONG_BIT)

## Get SVN revision number
SVN_REVISION_VERSION = -D'SVN_REVISION_VERSION="$(shell svnversion -n .)"'

# Set executable a name
EXE = MPToolkit
VIEWER_EXE = Viewer
ADVANCEDVIEWER_EXE = AdvancedViewer
EMMLIMAGING_EXE= EMMLTomographyImaging
POCAIMAGING_EXE= POCATomographyImaging
FOFIMAGING_EXE= FOFTomographyImaging
ACFIMAGING_EXE= ACFTomographyImaging
DOWNLOADER_EXE= Downloader

# Set shared library name
SHARED_LIB= libViewer.so
#############################################################

## You should not need to change anything below this line ###

.PHONY: all downloader depend clean


######################################
###  CPPFLAGS & CXXFLAGS  ############
######################################
# - Project include dir
CPPFLAGS = $(INCDIR)

# - System includes
ifeq ($(LBITS),64)
  # do 64 bit stuff here
	CPPFLAGS += -I/usr/include -pthread -m64
	CXXFLAGS = -std=c++11 -O2 -Wall -Wunused -Wuninitialized -fPIC -pthread -m64
	SYSLIBDIR = /usr/lib/x86_64-linux-gnu/
else
  # do 32 bit stuff here
	CPPFLAGS += -I/usr/include -pthread -m32
  CXXFLAGS = -std=c++11 -O2 -Wall -Wunused -Wuninitialized -fPIC -pthread -m32
	SYSLIBDIR = /usr/lib/x86_64-linux-gnu/
endif

SOFLAGS = -fPIC -ggdb3 -Wall -shared

# - ROOT headers
CPPFLAGS_ROOT = -I$(ROOTSYS)/include

# - GEANT4 headers
CPPFLAGS_GEANT4 = $(shell $(G4INSTALL)/bin/geant4-config --cflags)

# - MPI headers
CPPFLAGS_MPI= -I$(MPI_DIR)/include

# - QT headers
QTINCDIR=$(QTDIR)/include
CPPFLAGS_QT= -I$(QTINCDIR) -I$(QTINCDIR)/phonon -I$(QTINCDIR)/Qt3Support -I$(QTINCDIR)/QtDBus -I$(QTINCDIR)/QtDesigner -I$(QTINCDIR)/QtHelp -I$(QTINCDIR)/QtNetwork  -I$(QTINCDIR)/QtScript -I$(QTINCDIR)/QtSql -I$(QTINCDIR)/QtTest -I$(QTINCDIR)/QtXml -I$(QTINCDIR)/Qt -I$(QTINCDIR)/QtCore -I$(QTINCDIR)/QtDeclarative -I$(QTINCDIR)/QtGui -I$(QTINCDIR)/QtMultimedia -I$(QTINCDIR)/QtOpenGL -I$(QTINCDIR)/QtScriptTools -I$(QTINCDIR)/QtSvg -I$(QTINCDIR)/QtUiTools -I$(QTINCDIR)/QtXmlPatterns

# - QJSON headers
CPPFLAGS_QJSON= -I$(QJSON_DIR)/include

# - VTK headers
CPPFLAGS_VTK = -I$(VTK_INCDIR)

# - libAfterImage
CPPFLAGS_LIBAFTERIMAGE= -I$(LIBAFTERIMAGE_INCDIR)

# - BOOST
CPPFLAGS_BOOST= -I$(BOOST_ROOT)/include

# - LOG4CXX
CPPFLAGS_LOG4CXX= -I$(LOG4CXX_DIR)/include

## CPPFLAGS
CPPFLAGS += $(CPPFLAGS_ROOT) 
CPPFLAGS += $(CPPFLAGS_GEANT4) 
CPPFLAGS += $(CPPFLAGS_MPI) 
CPPFLAGS += $(CPPFLAGS_QT) 
CPPFLAGS += $(CPPFLAGS_VTK)
##CPPFLAGS += -I/usr/include/freetype2 -I/usr/include/libAfterImage/
CPPFLAGS += $(CPPFLAGS_LIBAFTERIMAGE)
CPPFLAGS += $(CPPFLAGS_QJSON)
CPPFLAGS += $(CPPFLAGS_BOOST)
CPPFLAGS += $(CPPFLAGS_LOG4CXX)

###########################
###  LDFLAGS   ############
###########################
# - ROOT
LDFLAGS_ROOT = -L$(ROOTSYS)/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lMathMore -lThread -lMinuit -lGeom -lGeomPainter -lASImage -lGui -pthread -lm -ldl -rdynamic 

# - GEANT4
LDFLAGS_GEANT4 = $(shell $(G4INSTALL)/bin/geant4-config --libs) -L$(G4LIB) -lG4zlib

# - CLHEP
##LDFLAGS_CLHEP = -L$(CLHEP_BASE_DIR)/lib -lCLHEP

# - MPI
LDFLAGS_MPI= -L$(MPI_DIR)/lib -lmpichcxx -lmpich -lmpl -lopa 

# - QT
LDFLAGS_QT= -L$(QTDIR)/lib -lphonon -lQt3Support -lQtCLucene -lQtCore -lQtDBus -lQtDeclarative -lQtDesignerComponents -lQtDesigner -lQtGui -lQtHelp -lQtMultimedia -lQtNetwork -lQtOpenGL -lQtScript -lQtScriptTools -lQtSql -lQtSvg -lQtTest -lQtXmlPatterns -lQtXml

# - QJSON
LDFLAGS_QJSON= -L$(QJSON_DIR)/lib -lqjson

# - VTK
LDFLAGS_VTK = -L$(VTK_LIBDIR) -lvtkNetCDF -lMapReduceMPI -lvtkFiltering -lvtkHybrid -lvtkverdict -lmpistubs -lQVTK -lvtkftgl -lvtkViews -lvtkalglib -lvtkImaging -lvtkproj4 -lvtkGenericFiltering -lvtkCharts -lvtkVolumeRendering -lvtkInfovis -lvtkGeovis -lvtkRendering  -lvtkCommon -lvtksqlite -lvtkWidgets -lvtkIO -lvtksys -lvtkDICOMParser -lvtkGraphics -lvtkexoIIc -lvtkmetaio -lvtkNetCDF_cxx -lLSDyna -lvtkexpat -lvtkfreetype -lvtkhdf5_hl -lvtkhdf5 -lvtkzlib -lvtklibxml2 -lvtkpng -lvtkjpeg -lvtktiff 

# - BOOST
LDFLAGS_BOOST= -L$(BOOST_ROOT)/lib -lboost_regex

# - LOG4CXX
LDFLAGS_LOG4CXX= -L$(LOG4CXX_DIR)/lib -llog4cxx

## LDFLAGS
LDFLAGS = -L$(SYSLIBDIR) -lrt -lAfterImage -lAfterBase -lfreetype -lX11
LDFLAGS+= $(LDFLAGS_ROOT) 
LDFLAGS+= $(LDFLAGS_GEANT4) 
LDFLAGS+= $(LDFLAGS_MPI) 
LDFLAGS+= $(LDFLAGS_QT) 
LDFLAGS+= $(LDFLAGS_QJSON) 
LDFLAGS+= $(LDFLAGS_VTK) 
LDFLAGS+= $(LDFLAGS_BOOST)
LDFLAGS+= $(LDFLAGS_LOG4CXX)
################################################################


##########################################
###  PROJECT BINARIES                  ###
##########################################
all: GETOBJS PRINTINFO $(EXE) $(SHARED_LIB) $(VIEWER_EXE) $(ADVANCEDVIEWER_EXE) $(EMMLIMAGING_EXE) $(DOWNLOADER_EXE) $(POCAIMAGING_EXE) $(FOFIMAGING_EXE) $(ACFIMAGING_EXE) PUTOBJS PUTBINARIES PUTLIBRARIES


##########################################
###  QT MOC FILE GENERATION            ###
##########################################
### Generate moc objects
$(SRCDIR_GUI)/moc_AboutTabMenu.o : $(SRCDIR_GUI)/moc_AboutTabMenu.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@ 

$(SRCDIR_GUI)/moc_AnalysisTabMenu.o : $(SRCDIR_GUI)/moc_AnalysisTabMenu.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@ 

$(SRCDIR_GUI)/moc_ContainerTab.o : $(SRCDIR_GUI)/moc_ContainerTab.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_POCATab.o : $(SRCDIR_GUI)/moc_POCATab.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_EMLLTab.o : $(SRCDIR_GUI)/moc_EMLLTab.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_ACFTab.o : $(SRCDIR_GUI)/moc_ACFTab.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_ClusteringTab.o : $(SRCDIR_GUI)/moc_ClusteringTab.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@


$(SRCDIR_GUI)/moc_DownloaderThreadObj.o : $(SRCDIR_GUI)/moc_DownloaderThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_EMAnalysisThreadObj.o : $(SRCDIR_GUI)/moc_EMAnalysisThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_ACFAnalysisThreadObj.o : $(SRCDIR_GUI)/moc_ACFAnalysisThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_ACFViewerThreadObj.o : $(SRCDIR_GUI)/moc_ACFViewerThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_EMTomographyRecoThreadObj.o : $(SRCDIR_GUI)/moc_EMTomographyRecoThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_EMTomographyRecoParallelThreadObj.o : $(SRCDIR_GUI)/moc_EMTomographyRecoParallelThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_EMLLViewerThreadObj.o : $(SRCDIR_GUI)/moc_EMLLViewerThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_POCAThreadObj.o : $(SRCDIR_GUI)/moc_POCAThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_POCAViewerThreadObj.o : $(SRCDIR_GUI)/moc_POCAViewerThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_FOFThreadObj.o : $(SRCDIR_GUI)/moc_FOFThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_ClusteringViewerThreadObj.o : $(SRCDIR_GUI)/moc_ClusteringViewerThreadObj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_Gui.o : $(SRCDIR_GUI)/moc_Gui.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_GuiServer.o : $(SRCDIR_GUI)/moc_GuiServer.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_JSONServer.o : $(SRCDIR_GUI)/moc_JSONServer.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_MainTabMenu.o : $(SRCDIR_GUI)/moc_MainTabMenu.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_QScienceSpinBox.o : $(SRCDIR_GUI)/moc_QScienceSpinBox.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_TabMenu.o : $(SRCDIR_GUI)/moc_TabMenu.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_TomographyRecTabMenu.o : $(SRCDIR_GUI)/moc_TomographyRecTabMenu.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(SRCDIR_GUI)/moc_MyThread.o : $(SRCDIR_GUI)/moc_MyThread.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_VolumeRenderingViewer.o : $(SRCDIR_GUI)/moc_VolumeRenderingViewer.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@


$(SRCDIR_GUI)/moc_serializerrunnable.o : $(SRCDIR_GUI)/moc_serializerrunnable.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(SRCDIR_GUI)/moc_parserrunnable.o : $(SRCDIR_GUI)/moc_parserrunnable.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@


### Generate moc sources
$(SRCDIR_GUI)/moc_AboutTabMenu.cxx: $(INCDIR_GUI)/AboutTabMenu.h
	moc $< -o $@ 
$(SRCDIR_GUI)/moc_AnalysisTabMenu.cxx: $(INCDIR_GUI)/AnalysisTabMenu.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_ContainerTab.cxx: $(INCDIR_GUI)/ContainerTab.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_POCATab.cxx: $(INCDIR_GUI)/POCATab.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_EMLLTab.cxx: $(INCDIR_GUI)/EMLLTab.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_ACFTab.cxx: $(INCDIR_GUI)/ACFTab.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_ClusteringTab.cxx: $(INCDIR_GUI)/ClusteringTab.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_DownloaderThreadObj.cxx: $(INCDIR_GUI)/DownloaderThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_POCAThreadObj.cxx: $(INCDIR_GUI)/POCAThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_POCAViewerThreadObj.cxx: $(INCDIR_GUI)/POCAViewerThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_EMAnalysisThreadObj.cxx: $(INCDIR_GUI)/EMAnalysisThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_ACFAnalysisThreadObj.cxx: $(INCDIR_GUI)/ACFAnalysisThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_ACFViewerThreadObj.cxx: $(INCDIR_GUI)/ACFViewerThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_EMTomographyRecoThreadObj.cxx: $(INCDIR_GUI)/EMTomographyRecoThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_EMTomographyRecoParallelThreadObj.cxx: $(INCDIR_GUI)/EMTomographyRecoParallelThreadObj.h
	moc $< -o $@

$(SRCDIR_GUI)/moc_EMLLViewerThreadObj.cxx: $(INCDIR_GUI)/EMLLViewerThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_FOFThreadObj.cxx: $(INCDIR_GUI)/FOFThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_ClusteringViewerThreadObj.cxx: $(INCDIR_GUI)/ClusteringViewerThreadObj.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_Gui.cxx: $(INCDIR_GUI)/Gui.h
	moc $< -o $@ 
$(SRCDIR_GUI)/moc_GuiServer.cxx: $(INCDIR_GUI)/GuiServer.h
	moc $< -o $@ 
$(SRCDIR_GUI)/moc_JSONServer.cxx: $(INCDIR_GUI)/JSONServer.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_MainTabMenu.cxx: $(INCDIR_GUI)/MainTabMenu.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_QScienceSpinBox.cxx: $(INCDIR_GUI)/QScienceSpinBox.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_TabMenu.cxx: $(INCDIR_GUI)/TabMenu.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_TomographyRecTabMenu.cxx: $(INCDIR_GUI)/TomographyRecTabMenu.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_WorldTab.cxx: $(INCDIR_GUI)/WorldTab.h
	moc $< -o $@
$(SRCDIR_GUI)/moc_MyThread.cxx: $(INCDIR_GUI)/MyThread.h
	moc $< -o $@

$(SRCDIR_GUI)/moc_VolumeRenderingViewer.cxx: $(INCDIR_GUI)/VolumeRenderingViewer.h
	moc $< -o $@

$(SRCDIR_GUI)/moc_serializerrunnable.cxx: $(QJSON_DIR)/include/qjson/serializerrunnable.h
	moc $< -o $@

$(SRCDIR_GUI)/moc_parserrunnable.cxx: $(QJSON_DIR)/include/qjson/parserrunnable.h
	moc $< -o $@


PRINTINFO: 
	@echo 'Compiling $(EXE) on a $(LBITS) bit machine' 
	@echo 'SRCDIR= $(SRCDIR)' 
	@echo 'USER_SRCS= $(USER_SRCS)'
	@echo 'OBJS= $(OBJS)' 
	@echo 'INCDIR_GUI= $(INCDIR_GUI)'
	@echo 'MOC_HEADERS= $(MOC_HEADERS)'
	@echo 'MOC_SOURCES= $(MOC_SOURCES)'
	@echo 'MOC_OBJ= $(MOC_OBJ)'


GETOBJS:
	@echo "Put object files again to $(SRCDIR) dir"
	- mv -f $(OBJDIR)/*.o $(SRCDIR) 2>/dev/null
	- mv -f $(SRCDIR)/MyDict.o $(TOPDIR) 2>/dev/null



PUTOBJS:
	@echo "Moving object files to $(OBJDIR) dir"
	- mv -f $(SRCDIR)/*.o $(OBJDIR) 2>/dev/null
	- mv -f *.o $(OBJDIR) 2>/dev/null

PUTBINARIES:
	@echo "Moving binary files to $(BINDIR) dir"
	- mv -f $(EXE) $(BINDIR) 2>/dev/null
	- mv -f $(VIEWER_EXE) $(BINDIR) 2>/dev/null
	- mv -f $(ADVANCEDVIEWER_EXE) $(BINDIR) 2>/dev/null
	- mv -f $(EMMLIMAGING_EXE) $(BINDIR) 2>/dev/null
	- mv -f $(POCAIMAGING_EXE) $(BINDIR) 2>/dev/null
	- mv -f $(FOFIMAGING_EXE) $(BINDIR) 2>/dev/null
	- mv -f $(ACFIMAGING_EXE) $(BINDIR) 2>/dev/null
	- mv -f $(DOWNLOADER_EXE) $(BINDIR) 2>/dev/null

PUTLIBRARIES:
	@echo "Moving library files to $(LIBDIR) dir"
	- mv -f $(SHARED_LIB) $(LIBDIR) 2>/dev/null

ViewerDict.cc: $(VIEWER_HEADERS) ViewerLinkDef.h
	rootcint -f $@ -c -p $(CPPFLAGS) $^

$(SHARED_LIB): $(OBJS) ViewerDict.o
	@echo Generating $@ shared library...
	@$(CXX) $(CXXFLAGS) $(SOFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)


$(EXE): $(MAIN_GUI_OBJ) $(OBJS) $(MOC_OBJ) ViewerDict.o 
	@echo "Compiler is $(CXX) or $(CC), options are $(CXXFLAGS)"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(MAIN) $(LDFLAGS)

$(VIEWER_EXE): $(MAIN_VIEWER_OBJ) $(OBJS) $(MOC_OBJ) ViewerDict.o 
	@echo "Building viewer ..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(MAIN) $(LDFLAGS)

$(ADVANCEDVIEWER_EXE): $(MAIN_ADVANCEDVIEWER_OBJ) $(OBJS) $(MOC_OBJ) ViewerDict.o 
	@echo "Building advanced viewer ..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(MAIN) $(LDFLAGS)

$(EMMLIMAGING_EXE): $(MAIN_EMMLIMAGING_OBJ) $(OBJS) $(MOC_OBJ) ViewerDict.o 
	@echo "Building EMML imaging ..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(MAIN) $(LDFLAGS)

$(POCAIMAGING_EXE): $(MAIN_POCAIMAGING_OBJ) $(OBJS) $(MOC_OBJ) ViewerDict.o 
	@echo "Building POCA imaging ..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(MAIN) $(LDFLAGS)

$(FOFIMAGING_EXE): $(MAIN_FOFIMAGING_OBJ) $(OBJS) $(MOC_OBJ) ViewerDict.o 
	@echo "Building FOF imaging ..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(MAIN) $(LDFLAGS)

$(ACFIMAGING_EXE): $(MAIN_ACFIMAGING_OBJ) $(OBJS) $(MOC_OBJ) ViewerDict.o 
	@echo "Building ACF imaging ..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(MAIN) $(LDFLAGS)

$(DOWNLOADER_EXE): $(MAIN_DOWNLOADER_OBJ) $(OBJS) $(MOC_OBJ) ViewerDict.o 
	@echo "Building Downloader ..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(MAIN) $(LDFLAGS)


#############################################################
# gcc can generate the dependency list

depend: Make-depend

Make-depend: $(USER_SRCS) $(MOC_SOURCES)
	@echo "Generate the dependency list..."
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $^ > $@


clean:
	@echo "Cleaning up the project..."
	- rm -f *.o $(OBJDIR)/*.o $(SRCDIR)/*.o $(BINDIR)/*.o
	- rm -f *.so $(LIBDIR)/*.so
	- rm -f $(BINDIR)/$(EXE) $(BINDIR)/$(VIEWER_EXE) $(BINDIR)/$(ADVANCEDVIEWER_EXE) $(BINDIR)/$(EMMLIMAGING_EXE) $(BINDIR)/$(POCAIMAGING_EXE) $(BINDIR)/$(FOFIMAGING_EXE) $(BINDIR)/$(ACFIMAGING_EXE) $(BINDIR)/$(DOWNLOADER_EXE)
	- rm -f ViewerDict.cc ViewerDict_rdict.pcm
	- rm -f $(MOC_SOURCES)
	- rm -f *.ps core Make-depend

 

#############################################################
# 'make run' will run the thing

run: $(EXE)
	./$(EXE) && touch $@


#############################################################
# the lines below are for running with debugger 'make run_gdb'

.INTERMEDIATE: gdb.cmdl

# batch mode gdb needs a file with commands
gdb.cmdl:
	echo "r -b bootstrap.xml" > $@

run_gdb: gdb.cmdl $(EXE)
	gdb -batch -x $< ./$(EXE) && touch $@

include Make-depend
