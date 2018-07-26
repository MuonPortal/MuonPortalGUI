

#include <FOFThreadObj.h>
#include <ConfigParser.h>

#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

using namespace std;


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
  {(char*)0, (int)0, (int*)0, (int)0}
};


void usage(){

	cout<<endl;
	cout<<"*** PROGRAM USAGE ***"<<endl;
	cout<<"FOFTomographyImaging --input=[input file] --jobdir=[job dir name] --output=[output file] --guihost=[gui hostname] --guiport=[gui port number] --guikey=[gui key id] --scanId=[scan id] --containerId=[container id] --timeStamp=[time stamp] --config=[config filename]"<<endl;
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
	

	if(argc<9){
		cerr<<"WARNING: Missing arguments, see program usage..."<<endl;
		usage();
		return 1;
	}


	int c = 0;
  int option_index = 0;
			
	
	while((c = getopt_long(argc, argv, "Ui:o:H:P:K:s:c:t:C:",options_tab, &option_index)) != -1) {
    
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
    	default:
      	usage();	
				exit(0);
    }
	}//close while
  cout << endl;


	cout<<endl;
	cout<<"*** FOFTomographyImaging ARGS ***"<<endl;
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
	cout<<"***********************************"<<endl;
	cout<<endl;

	//## Parse config file
	cout<<"FOFTomographyImaging(): INFO: Parsing config info..."<<endl;
	ConfigParser* parser= new ConfigParser(fConfigFileName); 
	parser->ReadConfig();

	//## EMML task	
	cout<<"FOFTomographyImaging(): INFO: Start FOF task..."<<endl;
	
	FOFThreadObj* FOF= new FOFThreadObj;
	FOF->SetInputFileName(fInputFileName);	
	FOF->SetOutputFileName(fOutputFileName);
	FOF->SetDestinationPath(fJobDirName);
	FOF->SetGuiHostName(fGuiHostName);
	FOF->SetGuiPort(fGuiPortNo);
	FOF->SetGuiKey(fGuiKey);
	FOF->SetScanId(fScanId);
	FOF->SetContainerId(fContainerId);
	FOF->SetTimeStamp(fTimeStamp);	
	FOF->process();
	

	return 0;

}//close main




