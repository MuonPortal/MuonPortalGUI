
#include <POCAImageViewer.h>
#include <EMLLImageViewer.h>
#include <ACFImageViewer.h>
#include <ClusteringImageViewer.h>

#include <TApplication.h>

#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

using namespace std;
using namespace MuonPortalNS;

void viewPOCA(int argc, char *argv[],std::string filename);
void viewFOF(int argc, char *argv[],std::string filename);
void viewLL(int argc, char *argv[],std::string filename);
void viewACF(int argc, char *argv[],std::string filename);

void usage();

static const struct option options_tab[] = {
  /* name, has_arg, &flag, val */
  { "help", no_argument, 0, 'h' },
	{ "poca", required_argument, 0, 'p' },
	{ "likelihood", required_argument, 0, 'l' },
	{ "acf", required_argument, 0, 'a' },
	{ "fof", required_argument, 0, 'f' },
  {(char*)0, (int)0, (int*)0, (int)0}
};


int main(int argc, char *argv[]){

	
	int c = 0;
  int option_index = 0;
	std::string inputFileName= "";

	while((c = getopt_long(argc, argv, "hplafi:",options_tab, &option_index)) != -1) {
    
    switch (c) {
			case 'h':
      	usage();	
				exit(0);
    	case 'p':	
				inputFileName= string(optarg);	
      	viewPOCA(argc, argv,inputFileName);	
				break;
			case 'l':	
				inputFileName= string(optarg);	
      	viewLL(argc, argv,inputFileName);	
				break;
			case 'a':	
				inputFileName= string(optarg);	
      	viewACF(argc, argv,inputFileName);	
				break;
			case 'f':	
				inputFileName= string(optarg);	
      	viewFOF(argc, argv,inputFileName);	
				break;
    	default:
				cout<<"default"<<endl;
      	usage();	
				exit(0);
    }
	}//close while
  cout << endl;


	
	//theApp.Run();

	return 0;

}//close main



void viewPOCA(int argc, char *argv[],std::string filename){
	
	TApplication theApp("App",&argc,argv);
	new POCAImageViewer(gClient->GetRoot(),1000,700,filename);	
	theApp.Run();

}//close viewPOCA()



void viewLL(int argc, char *argv[],std::string filename){

	TApplication theApp("App",&argc,argv);
	new EMLLImageViewer(gClient->GetRoot(),1000,700,filename);	
	theApp.Run();
	
}//close viewLL()

void viewACF(int argc, char *argv[],std::string filename){

	TApplication theApp("App",&argc,argv);
	new ACFImageViewer(gClient->GetRoot(),1000,500,filename);	
	theApp.Run();
	
}//close viewACF()

void viewFOF(int argc, char *argv[],std::string filename){

	TApplication theApp("App",&argc,argv);
	new ClusteringImageViewer(gClient->GetRoot(),1000,500,filename);	
	theApp.Run();
	
}//close viewFOF()


void usage(){

	cout<<endl;
	cout<<"*** PROGRAM USAGE ***"<<endl;
	cout<<"Viewer --[option] [input file]"<<endl;
	cout<<"  [option]"<<endl;
	cout<<"      poca: poca viewer "<<endl;
	cout<<""<<endl;
	
}//close usage()

