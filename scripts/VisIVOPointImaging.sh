#!/bin/bash

if [ "$2" = "" ]; then
    echo "Usage: $0 FILENAME OutputDataDir"
    echo "  1) FILENAME: Name of input file with volume info"
    echo "  2) OutputDataDir "
    exit 0
fi


if [ "$1" = "" ]; then
  echo "ERROR: Missing FILENAME --> Please specify it as argument!"
  exit 0
else
  FILENAME=$1
fi


if [ "$2" = "" ]; then
  echo "ERROR: Missing OUTPUTDATADIR --> Please specify it as argument!"
  exit 0
else
  OUTPUTDATADIR=$2
fi

##OUTFILENAME=$OUTPUTDATADIR/VisIVOOut_$FILENAME
##IMGFILENAME_VREND=$OUTPUTDATADIR/VisIVOVolRend_$FILENAME

OUTFILENAME=VisIVOPointOut_$FILENAME
IMGFILENAME=VisIVOPoint_$FILENAME

echo "*** INFO ***"
echo "--> OutputDataDir= $OUTPUTDATADIR"
echo "--> OUTFILENAME= $OUTFILENAME"
echo "--> IMGFILENAME= $IMGFILENAME"


### Importing volume data in VisIVO in ascii format 
VisIVOImporter --fformat ascii --out $OUTFILENAME $FILENAME

### Making volume rendering
#VisIVOViewer --volume --backcolor black --vrendering --vrenderingfield S --showbox --showaxes --showlut --cycle scripts/xyrotation --color --colortable scripts/paletteVisIVO.dat --out $OUTPUTDATADIR/$IMGFILENAME $OUTFILENAME


#VisIVOViewer --x X --y Y --z Z --showbox --showaxes --showlut --cycle scripts/xyrotation --color --colorscalar W --colortable scripts/palettePointVisIVO.dat --out $OUTPUTDATADIR/$IMGFILENAME $OUTFILENAME

#VisIVOViewer --x X --y Y --z Z --showbox --showaxes --showlut --cycle scripts/xyrotation --color --colorscalar W --colortable scripts/palettePointVisIVO_mod2.dat --out $OUTPUTDATADIR/$IMGFILENAME $OUTFILENAME

VisIVOViewer --x X --y Y --z Z --showbox --showaxes --showlut --cycle scripts/xyrotation --color --colorscalar W --colortable scripts/palettePointVisIVO_mod.dat --out $OUTPUTDATADIR/$IMGFILENAME $OUTFILENAME

#VisIVOViewer --x X --y Y --z Z --showbox --showaxes --showlut --cycle scripts/xyrotation --color --colorscalar W --colortable physics_contour --out $OUTPUTDATADIR/$IMGFILENAME $OUTFILENAME


