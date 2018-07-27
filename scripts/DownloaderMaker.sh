#!/bin/bash

## Arguments: Downloader job parameters (ACQ host, file to be downloaded, gui host, ...) 


## Action: Create on-fly a download script to be executed on remote host with parameters preset from the current script arguments. The download script will:
## 1) Create an output data dir on remote host
## 2) Run the download task


echo "INFO: NARGS= $#"

if [ "$#" -lt 12 ]; then
	echo "ERROR: Invalid number of arguments...see script usage!"
	echo ""
	echo "*** USAGE ***	"
  echo "$0 --input=[INPUTFILE] --output=[OUTPUTFILE] --host=[HOSTNAME] --user=[USERNAME] --port=[PORT] --guihost=[GUI_HOSTNAME] --guiport=[GUI_PORT] --guikey=[GUI_KEY] --nretry=[NRETRY] --scanId=[SCAN_ID] --containerId=[CONTAINER_ID] --timeStamp=[TIME_STAMP]"
	echo "**************"
  exit 1
fi



## Check if GUI exe environment variable exists and is properly set
if [ "$MUPORTAL_GUI_ROOT" = "" ]; then
  echo "ERROR: Missing environment variable MUPORTAL_GUI_ROOT --> Please set it to GUI executable path!"
  exit 1 
fi

if [ "$MUPORTAL_GUI_DATA" = "" ]; then
  echo "ERROR: Missing environment variable $MUPORTAL_GUI_DATA --> Please set it to GUI output data path!"
  exit 1 
fi

## Parse arguments
IS_LOCAL=0

for i in $*
do
	case $i in
  	--input=*)
			INPUTFILE=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
    --output=*)
			OUTPUTFILE=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
    --host=*)
			HOSTNAME=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--user=*)
			USERNAME=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--port=*)
			PORT=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--guihost=*)
			GUI_HOSTNAME=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--guiport=*)
			GUI_PORT=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--guikey=*)
			GUI_KEY=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--nretry=*)
			NRETRY=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--scanId=*)
			SCAN_ID=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--containerId=*)
			CONTAINER_ID=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--timeStamp=*)
			TIME_STAMP=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
		--local)
			IS_LOCAL=1
		;;
    *)
    # unknown option
		echo "ERROR: Unknown option...exit!"
		exit 1
		;;
  	esac
done

echo ""
echo "*****  PARSED ARGUMENTS ****"
echo "INPUTFILE= $INPUTFILE"
echo "OUTPUTFILE= $OUTPUTFILE"
echo "HOSTNAME= $HOSTNAME"
echo "USERNAME= $USERNAME"
echo "PORT= $PORT"
echo "GUI_HOSTNAME= $GUI_HOSTNAME"
echo "GUI_PORT= $GUI_PORT"
echo "GUI_KEY= $GUI_KEY"
echo "NRETRY= $NRETRY"
echo "SCAN_ID= $SCAN_ID"
echo "CONTAINER_ID= $CONTAINER_ID"
echo "TIME_STAMP= $TIME_STAMP"
echo "IS_LOCAL= $IS_LOCAL"
echo "*****************************"
echo ""

###################################################
###    JOB PREPARATION: MAKE OUTPUT DIR       #####
###################################################
FILENAME=$(basename "$INPUTFILE")
FILENAME_EXT="${FILENAME##*.}"
FILENAME="${FILENAME%.*}"
echo "INFO: Base input filename: $FILENAME ..."


OUTPUT_DATA_DIR='data-'"$FILENAME"'_SCAN'"$SCAN_ID"'_ID'"$CONTAINER_ID"'_TIME'"$TIME_STAMP"
JOB_DATA_DIR="$MUPORTAL_GUI_DATA/$OUTPUT_DATA_DIR"

echo "INFO: Making job output directory ($JOB_DATA_DIR) ..."
##mkdir $JOB_DATA_DIR
##MKDIR_CMD_STATUS=`mkdir $JOB_DATA_DIR &> /dev/null; echo $?`
mkdir $JOB_DATA_DIR
MKDIR_CMD_STATUS=`echo $?`
echo "INFO: MKDIR_CMD_STATUS: $MKDIR_CMD_STATUS ..."

if [ "$MKDIR_CMD_STATUS" != "0" ]; then
	echo "WARNING: Directory already exists ..."
	exit $MKDIR_CMD_STATUS
fi



echo "INFO: Entering job output directory ($JOB_DATA_DIR) ..."
cd $JOB_DATA_DIR
#####################################


###########################################################
###    JOB PREPARATION: CREATE COMMANDS & SCRIPT      #####
###########################################################
CMD="$MUPORTAL_GUI_ROOT/Downloader --input=$INPUTFILE --jobdir=$JOB_DATA_DIR --output=$OUTPUTFILE --host=$HOSTNAME --user=$USERNAME --port=$PORT --guihost=$GUI_HOSTNAME --guiport=$GUI_PORT --guikey=$GUI_KEY --maxretry=$NRETRY --scanId=$SCAN_ID --containerId=$CONTAINER_ID --timeStamp=$TIME_STAMP --local=$IS_LOCAL"


echo "INFO: Download command: $CMD ..."


shfile="DownloadRun.sh"
echo "INFO: Creating download sh script $shfile ..."
(
	echo '#!/bin/sh'
	echo ''
	echo "#PBS -o $JOB_DATA_DIR"
 	echo "#PBS -o $JOB_DATA_DIR"
  echo '#PBS -r n'          
  echo '#PBS -S /bin/sh'
  echo "#PBS -N downloadjob"
  echo '#PBS -M simone.riggi@gmail.com'
  echo '#PBS -m be'
  echo '#PBS -p 1'

	echo ''
	echo 'echo "--> Setting up the software environment ..."'
	echo "source $SOFTDIR/setvars.sh"

	echo ''
	echo 'echo "--> Run script ..."'
	echo "$CMD"

	echo ''
	echo 'echo "END RUN"'
	echo ''

) > $shfile

chmod +x $shfile




