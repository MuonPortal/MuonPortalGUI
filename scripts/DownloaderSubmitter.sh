#!/bin/bash



echo "INFO: NARGS= $#"

if [ "$#" -lt 12 ]; then
	echo "ERROR: Invalid number of arguments...see script usage!"
	echo ""
	echo "*** USAGE ***	"
  echo "$0 --input=[INPUTFILE] --output=[OUTPUTFILE] --host=[HOSTNAME] --user=[USERNAME] --port=[PORT] --guihost=[GUI_HOSTNAME] --guiport=[GUI_PORT] --guikey=[GUI_KEY] --nretry=[NRETRY] --scanId=[SCAN_ID] --containerId=[CONTAINER_ID] --timeStamp=[TIME_STAMP] [--local] [--useQueueSystem]"
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
USE_QUEUE_SYSTEM=0


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
		--useQueueSystem)
			USE_QUEUE_SYSTEM=1	
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
echo "USE_QUEUE_SYSTEM= $USE_QUEUE_SYSTEM"
echo "*****************************"
echo ""


HOST_CONNECT_CMD="ssh $MUPORTAL_SEI_USER@$MUPORTAL_SEI_HOST"


###################################################
###    JOB PREPARATION: MAKE OUTPUT DIR       #####
###################################################

FILENAME=$(basename "$INPUTFILE")
FILENAME_EXT="${FILENAME##*.}"
FILENAME="${FILENAME%.*}"
echo "INFO: Base input filename: $FILENAME ..."


OUTPUT_DATA_DIR='data-'"$FILENAME"'_SCAN'"$SCAN_ID"'_ID'"$CONTAINER_ID"'_TIME'"$TIME_STAMP"
JOB_DATA_DIR="$MUPORTAL_GUI_DATA/$OUTPUT_DATA_DIR"


echo "INFO: Discover job directory ..."

JOB_DATA_DIR_CMD='echo $MUPORTAL_GUI_DATA'
JOB_DATA_DIR_REMOTE_CMD="$HOST_CONNECT_CMD '$JOB_DATA_DIR_CMD'" 

##echo "INFO: JOB_DATA_DIR_CMD: $JOB_DATA_DIR_CMD"
##echo "INFO: JOB_DATA_DIR_REMOTE_CMD: $JOB_DATA_DIR_REMOTE_CMD"

JOB_DATA_REMOTE_DIR=""
if [ "$IS_LOCAL" = "1" ]; then
	JOB_DATA_REMOTE_DIR=$JOB_DATA_DIR
else
	JOB_DATA_REMOTE_DIR=`eval $JOB_DATA_DIR_REMOTE_CMD`"/$OUTPUT_DATA_DIR"
fi


echo "INFO: Job output directory: $JOB_DATA_REMOTE_DIR ..."

#if [ "$IS_LOCAL" = "1" ]; then
#	MKDIR_CMD=""
#	MKDIR_REMOTE_CMD="mkdir $JOB_DATA_DIR"
#else
#	MKDIR_CMD='mkdir $MUPORTAL_GUI_DATA'"/$OUTPUT_DATA_DIR"
#	MKDIR_REMOTE_CMD="$HOST_CONNECT_CMD '$MKDIR_CMD'"
#fi

#echo "MKDIR_CMD: $MKDIR_CMD"
#echo "MKDIR_REMOTE_CMD: $MKDIR_REMOTE_CMD"

#echo "--> Making job output dir ..."
#eval "$MKDIR_REMOTE_CMD"

#####################################








###################################################
###    JOB PREPARATION: DEFINE COMMANDS       #####
###################################################

MAKER_CMD=""
MAKER_REMOTE_CMD=""
MAKER_CMD_STATUS=0

if [ "$IS_LOCAL" = "1" ]; then

	MAKER_CMD="$MUPORTAL_GUI_SCRIPT/DownloaderMaker.sh --input=$INPUTFILE --output=$OUTPUTFILE --host=$HOSTNAME --user=$USERNAME --port=$PORT --guihost=$GUI_HOSTNAME --guiport=$GUI_PORT --guikey=$GUI_KEY --nretry=$NRETRY --scanId=$SCAN_ID --containerId=$CONTAINER_ID --timeStamp=$TIME_STAMP --local"
	###MAKER_REMOTE_CMD="$MAKER_CMD >& downloadMaker.log < /dev/null & echo "'$?' 
	MAKER_REMOTE_CMD="$MAKER_CMD" 
else

	MAKER_CMD='$MUPORTAL_GUI_SCRIPT/DownloaderMaker.sh '"--input=$INPUTFILE --output=$OUTPUTFILE --host=$HOSTNAME --user=$USERNAME --port=$PORT --guihost=$GUI_HOSTNAME --guiport=$GUI_PORT --guikey=$GUI_KEY --nretry=$NRETRY --scanId=$SCAN_ID --containerId=$CONTAINER_ID --timeStamp=$TIME_STAMP"
	###MAKER_REMOTE_CMD="$HOST_CONNECT_CMD '$MAKER_CMD >& downloadMaker.log < /dev/null & echo "'$?'"'" 
	MAKER_REMOTE_CMD="$HOST_CONNECT_CMD '$MAKER_CMD'" 
fi

##MAKER_CMD_STATUS=`$MAKER_REMOTE_CMD &> /dev/null; echo $?`
#MAKER_CMD_STATUS=$(eval $MAKER_REMOTE_CMD)
eval $MAKER_REMOTE_CMD
MAKER_CMD_STATUS=`echo $?`

echo "INFO: MAKER_CMD: $MAKER_CMD"
echo "INFO: MAKER_REMOTE_CMD: $MAKER_REMOTE_CMD"
echo "INFO: MAKER_CMD_STATUS: $MAKER_CMD_STATUS"


if [ "$MAKER_CMD_STATUS" != "0" ]; then
	echo "ERROR: DownloadMaker script failed on remote host (exit_code=$MAKER_CMD_STATUS)..."
	exit $MAKER_CMD_STATUS
fi

####################################################




###################################################
###            JOB RUN                        #####
###################################################


RUN_CMD="$JOB_DATA_REMOTE_DIR/DownloadRun.sh"
RUN_REMOTE_CMD=""
RUN_CMD_STATUS=0
RUN_CMD_PID=-1

if [ "$IS_LOCAL" = "1" ]; then

	RUN_REMOTE_CMD="$RUN_CMD >& download.log < /dev/null & echo "'$!' 

else

	if [ "$USE_QUEUE_SYSTEM" = "1" ]; then
	
		RUN_REMOTE_CMD="$HOST_CONNECT_CMD 'qsub -q normal $RUN_CMD'" 

	else
		
		RUN_REMOTE_CMD="$HOST_CONNECT_CMD 'nohup $RUN_CMD >& $JOB_DATA_REMOTE_DIR/download.log < /dev/null & echo "'$!'"'" 

	fi

fi



echo "INFO: RUN_CMD: $RUN_CMD"
echo "INFO: RUN_REMOTE_CMD: $RUN_REMOTE_CMD"

echo "INFO: Executing the remote process..."
RUN_CMD_PID=$(eval $RUN_REMOTE_CMD)
RUN_CMD_STATUS=`echo $?`


echo "INFO: RUN_CMD_STATUS: $RUN_CMD_STATUS"
echo "INFO: RUN_CMD_PID: $RUN_CMD_PID"


if [ "$RUN_CMD_STATUS" != "0" ]; then
	echo "ERROR: Download run failed on remote host (exit_code=$RUN_CMD_STATUS)..."
	exit $RUN_CMD_STATUS
fi


echo $RUN_CMD_PID >> download-pid.txt

echo "END DOWNLOAD SUBMISSION"




