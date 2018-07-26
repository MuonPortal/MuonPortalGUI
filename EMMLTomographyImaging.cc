

#include <EMTomographyRecoThreadObj.h>
#include <EMTomographyRecoParallelThreadObj.h>
#include <ConfigParser.h>

//## MPI
#include <mpi.h>

#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

using namespace std;
using namespace MuonPortalNS;

static const struct option options_tab[] = {
  // name, has_arg, &flag, val
  { "help", no_argument, 0, 'U' },
  { "input", required_argument, 0, 'i' },
  { "output", required_argument, 0, 'o' },
	{ "jobdir", required_argument, 0, 'j' },
	{ "config", required_argument, 0, 'C' },
	{ "guihost", required_argument, 0, 'H' },
	{ "guiport", required_argument, 0, 'P' },
	{ "guikey", required_argument, 0, 'K' },
	{ "scanId", required_argument, 0, 's' },
	{ "containerId", required_argument, 0, 'c' },
	{ "timeStamp", required_argument, 0, 't' },
	{ "useMPIVersion", no_argument, 0, 'm' },
  {(char*)0, (int)0, (int*)0, (int)0}
};


void usage(){

	cout<<endl;
	cout<<"*** PROGRAM USAGE ***"<<endl;
	cout<<"EMMLTomographyImaging --input=[input file] --jobdir=[job dir name] --output=[output file] --guihost=[gui hostname] --guiport=[gui port number] --guikey=[gui key id] --scanId=[scan id] --containerId=[container id] --timeStamp=[time stamp] --config=[config filename] [--useMPIVersion]"<<endl;
	cout<<""<<endl;

}//close usage()




int main(int argc, char *argv[]){

	//## Default parameters
	std::string fInputFileName= "";
	std::string fOutputFileName= "";
	std::string fConfigFileName= "";
	std::string fJobDirName= "";
	std::string fGuiHostName= "localhost";
	int fGuiPortNo= 5901;
	std::string fGuiKey= "";
	int fScanId= 1;
	std::string fContainerId= "XXX";
	long int fTimeStamp= 0;
	bool fUseMPIVersion= false;
	

	if(argc<9){
		cerr<<"WARNING: Missing arguments, see program usage..."<<endl;
		usage();
		return 1;
	}


	int c = 0;
  int option_index = 0;
			
	
	while((c = getopt_long(argc, argv, "Ui:o:H:P:K:s:c:t:C:m",options_tab, &option_index)) != -1) {
    
    switch (c) {
			case 'U':
      	usage();	
				exit(0);
    	case 'i':
				fInputFileName= std::string(optarg);	
				break;	
			case 'j':
				fJobDirName= std::string(optarg);	
				break;
			case 'o':
				fOutputFileName= std::string(optarg);	
				break;
			case 'H':
      	fGuiHostName= std::string(optarg);	
				break;	
			case 'P':
      	fGuiPortNo= atoi(optarg);	
				break;
			case 'K':
      	fGuiKey= std::string(optarg);	
				break;	
			case 's':
      	fScanId= atoi(optarg);	
				break;
			case 'c':
      	fContainerId= std::string(optarg);	
				break;
			case 't':
      	fTimeStamp= atol(optarg);	
				break;
			case 'C':
      	fConfigFileName= std::string(optarg);	
				break;
			case 'm':
      	fUseMPIVersion= true;	
				break;
    	default:
      	usage();	
				exit(0);
    }
	}//close while
  cout << endl;


	cout<<endl;
	cout<<"*** EMMLTomographyImaging ARGS ***"<<endl;
	cout<<"NARGS: "<<argc<<endl;
	cout<<"fInputFileName: "<<fInputFileName<<endl;
	cout<<"fJobDirName: "<<fJobDirName<<endl;
	cout<<"fOutputFileName: "<<fOutputFileName<<endl;
	cout<<"fConfigFileName: "<<fConfigFileName<<endl;
	cout<<"fGuiHostName: "<<fGuiHostName<<endl;
	cout<<"fGuiPortNo: "<<fGuiPortNo<<endl;
	cout<<"fGuiKey: "<<fGuiKey<<endl;
	cout<<"fScanId: "<<fScanId<<endl;
	cout<<"fContainerId: "<<fContainerId<<endl;
	cout<<"fTimeStamp: "<<fTimeStamp<<endl;
	cout<<"fUseMPIVersion: "<<fUseMPIVersion<<endl;
	cout<<"***********************************"<<endl;
	cout<<endl;

	//## Parse config file
	cout<<"EMMLTomographyImaging(): INFO: Parsing config info..."<<endl;
	ConfigParser* parser= new ConfigParser(fConfigFileName); 
	parser->ReadConfig();

	//## EMML task	
	cout<<"EMMLTomographyImaging(): INFO: Start EMML task..."<<endl;
	
	if(fUseMPIVersion){
		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int nproc;
		int myid;

		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &nproc);
		MPI_Comm_rank(MPI_COMM_WORLD, &myid);
		int namelen;
		MPI_Get_processor_name(processor_name,&namelen);
		double startTime= MPI_Wtime();
		printf("\nProcess %d running on %s\n\n",myid,processor_name);


		EMTomographyRecoParallelThreadObj* EMML= new EMTomographyRecoParallelThreadObj;	
		EMML->SetInputFileName(fInputFileName);	
		EMML->SetOutputFileName(fOutputFileName);
		EMML->SetDestinationPath(fJobDirName);
		EMML->SetGuiHostName(fGuiHostName);
		EMML->SetGuiPort(fGuiPortNo);
		EMML->SetGuiKey(fGuiKey);
		EMML->SetScanId(fScanId);
		EMML->SetContainerId(fContainerId);
		EMML->SetTimeStamp(fTimeStamp);	
		EMML->process();

		MPI_Finalize();
	}
	else{
		EMTomographyRecoThreadObj* EMML= new EMTomographyRecoThreadObj;
		EMML->SetInputFileName(fInputFileName);	
		EMML->SetOutputFileName(fOutputFileName);
		EMML->SetDestinationPath(fJobDirName);
		EMML->SetGuiHostName(fGuiHostName);
		EMML->SetGuiPort(fGuiPortNo);
		EMML->SetGuiKey(fGuiKey);
		EMML->SetScanId(fScanId);
		EMML->SetContainerId(fContainerId);
		EMML->SetTimeStamp(fTimeStamp);	
		EMML->process();
	}
	


	return 0;

}//close main




