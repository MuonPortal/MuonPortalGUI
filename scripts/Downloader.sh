#!/bin/bash


echo "Downloader.sh: NARGS= $#"

if [ "$#" -lt 12 ]; then
	echo "ERROR: Invalid number of arguments...see script usage!"
	echo ""
	echo "*** USAGE ***	"
  echo "$0 --input=[INPUTFILE] --output=[OUTPUTFILE] --host=[HOSTNAME] --user=[USERNAME] --port=[PORT] --guihost=[GUI_HOSTNAME] --guiport=[GUI_PORT] --guikey=[GUI_KEY] --nretry=[NRETRY] --scanId=[SCAN_ID] --containerId=[CONTAINER_ID] --timeStamp=[TIME_STAMP] [--local]"
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

if [ "$MUPORTAL_GUI_SCRIPT" = "" ]; then
  echo "ERROR: Missing environment variable MUPORTAL_GUI_SCRIPT --> Please set it to GUI script executable path!"
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
    --default)
			DEFAULT=YES
		;;
    *)
    # unknown option
		echo "ERROR: Unknown option...exit!"
		exit 1
		;;
  	esac
done

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




## Start the downloader process
CMD='$MUPORTAL_GUI_ROOT/Downloader '"--input=$INPUTFILE "'--output=$MUPORTAL_GUI_DATA'"/$OUTPUTFILE --host=$HOSTNAME --user=$USERNAME --port=$PORT --guihost=$GUI_HOSTNAME --guiport=$GUI_PORT --guikey=$GUI_KEY --maxretry=$NRETRY --scanId=$SCAN_ID --containerId=$CONTAINER_ID --timeStamp=$TIME_STAMP --local=$IS_LOCAL"


echo "--> Executing the remote process..."
eval $REMOTE_CMD


echo "END DOWNLOAD TASK"


