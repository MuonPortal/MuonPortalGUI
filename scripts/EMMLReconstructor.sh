
#!/bin/bash

if [ "$1" = "" ]; then
  echo "ERROR: Missing INPUTFILE_ARG --> Please specify it as argument!"
  exit 1
else
  INPUTFILE_ARG=$1
fi

if [ "$2" = "" ]; then
  echo "ERROR: Missing OUTPUTFILE_ARG --> Please specify it as argument!"
  exit 1
else
  OUTPUTFILE_ARG=$2
fi

if [ "$3" = "" ]; then
  echo "ERROR: Missing CONFIGFILE_ARG --> Please specify it as argument!"
  exit 1
else
  CONFIGFILE_ARG=$3
fi

if [ "$4" = "" ]; then
  echo "ERROR: Missing LOGFILE_ARG --> Please specify it as argument!"
  exit 1
else
  LOGFILE_ARG=$4
fi

echo "INPUTFILE_ARG: $INPUTFILE_ARG"
echo "OUTPUTFILE_ARG: $OUTPUTFILE_ARG"
echo "CONFIGFILE_ARG: $CONFIGFILE_ARG"
echo "LOGFILE_ARG: $LOGFILE_ARG"

CMD="$MUPORTAL_GUI_ROOT/EMMLTomographyImaging --in=$INPUTFILE_ARG --out=$OUTPUTFILE_ARG --cfg=$CONFIGFILE_ARG >& $LOGFILE_ARG &"
##CMD="ls -la"
echo "CMD: $CMD"

eval $CMD

echo "END"
