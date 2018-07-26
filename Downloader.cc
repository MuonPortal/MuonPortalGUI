#include <DownloaderThreadObj.h>
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
	{ "host", required_argument, 0, 'h' },	
	{ "user", required_argument, 0, 'u' },	
	{ "port", required_argument, 0, 'p' },
	{ "guihost", required_argument, 0, 'H' },
	{ "guiport", required_argument, 0, 'P' },
	{ "guikey", required_argument, 0, 'K' },
	{ "maxretry", required_argument, 0, 'r' },
	{ "scanId", required_argument, 0, 's' },
	{ "containerId", required_argument, 0, 'c' },
	{ "timeStamp", required_argument, 0, 't' },
	{ "local", required_argument, 0, 'l' },
  {(char*)0, (int)0, (int*)0, (int)0}
};


void usage(){

	cout<<endl;
	cout<<"*** PROGRAM USAGE ***"<<endl;
	cout<<"Downloader --input=[input file] --jobdir=[job dir name] --output=[output file] --host=[hostname] --user=[username] --port=[port number] --guihost=[gui hostname] --guiport=[gui port number] --guikey=[gui key id] --maxretry=[max download retries] --scanId=[scan id] --containerId=[container id] --timeStamp=[time stamp] --local=[is local download]"<<endl;
	cout<<""<<endl;

}//close usage()




int main(int argc, char *argv[]){

	//## Default parameters
	std::string fInputFileName= "";
	std::string fOutputFileName= "";
	std::string fJobDirName= "";
	std::string fRemoteHostName= "localhost";
	std::string fRemoteUserName= "riggi";
	int fRemotePortNo= 873;
	int fMaxRetryAttempts= 3;
	std::string fGuiHostName= "localhost";
	int fGuiPortNo= 5901;
	std::string fGuiKey= "";
	int fScanId= 1;
	std::string fContainerId= "XXX";
	long int fTimeStamp= 0;
	int fIsLocal= 0;

	//if(argc<9){
	cout<<"Downloader::INFO: argc="<<argc<<endl;
	if(argc<12){
		cerr<<"WARNING: Missing arguments, see program usage..."<<endl;
		usage();
		return 1;
	}


	int c = 0;
  int option_index = 0;
			
	
	while((c = getopt_long(argc, argv, "Ui:j:o:h:u:p:H:P:K:r:s:c:t:l:",options_tab, &option_index)) != -1) {
    
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
			case 'h':
				fRemoteHostName= std::string(optarg);	
				break;
    	case 'u':
      	fRemoteUserName= std::string(optarg);	
				break;
			case 'p':
      	fRemotePortNo= atoi(optarg);	
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
			case 'r':
      	fMaxRetryAttempts= atoi(optarg);	
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
			case 'l':
      	fIsLocal= atoi(optarg);	
				break;
    	default:
      	usage();	
				exit(0);
    }
	}//close while
  cout << endl;


	cout<<"fInputFileName: "<<fInputFileName<<endl;
	cout<<"fJobDirName: "<<fJobDirName<<endl;
	cout<<"fOutputFileName: "<<fOutputFileName<<endl;
	cout<<"fRemoteHostName: "<<fRemoteHostName<<endl;
	cout<<"fRemoteUserName: "<<fRemoteUserName<<endl;
	cout<<"fRemotePortNo: "<<fRemotePortNo<<endl;
	cout<<"fGuiHostName: "<<fGuiHostName<<endl;
	cout<<"fGuiPortNo: "<<fGuiPortNo<<endl;
	cout<<"fGuiKey: "<<fGuiKey<<endl;
	cout<<"fMaxRetryAttempts: "<<fMaxRetryAttempts<<endl;
	cout<<"fScanId: "<<fScanId<<endl;
	cout<<"fContainerId: "<<fContainerId<<endl;
	cout<<"fTimeStamp: "<<fTimeStamp<<endl;
	cout<<"fIsLocal: "<<fIsLocal<<endl;
	
	//## Downloader task
	DownloaderThreadObj* downloader= new DownloaderThreadObj;
	downloader->SetInputFileName(fInputFileName);	
	downloader->SetDestinationPath(fJobDirName);
	downloader->SetOutputFileName(fOutputFileName);
	downloader->SetRemoteHostName(fRemoteHostName);
	downloader->SetRemoteUserName(fRemoteUserName);
	downloader->SetRemotePort(fRemotePortNo);
	downloader->SetGuiHostName(fGuiHostName);
	downloader->SetGuiPort(fGuiPortNo);
	downloader->SetGuiKey(fGuiKey);
	downloader->SetMaxNAttempts(fMaxRetryAttempts);
	downloader->SetScanId(fScanId);
	downloader->SetContainerId(fContainerId);
	downloader->SetTimeStamp(fTimeStamp);	
	downloader->SetLocalDownload(fIsLocal);	
	downloader->process();

	

	return 0;

}//close main




