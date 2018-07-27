#!/bin/bash

if [ "$2" = "" ]; then
    echo "Usage: $0 FILENAME Nx Ny Nz OutputDataDir"
    echo "  1) FILENAME: Name of input file with volume info"
    echo "  2) Nx, Ny, Nz: Number of voxel in X, Y and Z dimensions"
		echo "  3) OutputDataDir "
    exit 0
fi


if [ "$1" = "" ]; then
  echo "ERROR: Missing FILENAME --> Please specify it as argument!"
  exit 0
else
  FILENAME=$1
fi


if [ "$2" = "" ]; then
  echo "ERROR: Missing Nx --> Please specify it as argument!"
  exit 0
else
  Nx=$2
fi

if [ "$3" = "" ]; then
  echo "ERROR: Missing Ny --> Please specify it as argument!"
  exit 0
else
  Ny=$3
fi

if [ "$4" = "" ]; then
  echo "ERROR: Missing Nz --> Please specify it as argument!"
  exit 0
else
  Nz=$4
fi

if [ "$5" = "" ]; then
  echo "ERROR: Missing OUTPUTDATADIR --> Please specify it as argument!"
  exit 0
else
  OUTPUTDATADIR=$5
fi

##OUTFILENAME=$OUTPUTDATADIR/VisIVOOut_$FILENAME
##IMGFILENAME_VREND=$OUTPUTDATADIR/VisIVOVolRend_$FILENAME

OUTFILENAME=VisIVOOut_$FILENAME
IMGFILENAME_VREND=VisIVOVolRend_$FILENAME

echo "*** INFO ***"
echo "--> (Nx,Ny,Nz)=($Nx,$Ny,$Nz)"
echo "--> OutputDataDir= $OUTPUTDATADIR"

echo "--> OUTFILENAME= $OUTFILENAME"
echo "--> IMGFILENAME_VREND= $IMGFILENAME_VREND"


### Importing volume data in VisIVO in ascii format 
VisIVOImporter --fformat ascii --volume --compx $Nx --compy $Ny --compz $Nz --out $OUTFILENAME $FILENAME

### Making volume rendering
#Xvfb :1 -screen 0 1024x768x8 &
#export DISPLAY=":1"

VisIVOViewer --volume --backcolor black --vrendering --vrenderingfield S --showbox --showaxes --showlut --cycle scripts/xyrotation --color --out $OUTPUTDATADIR/$IMGFILENAME_VREND $OUTFILENAME



