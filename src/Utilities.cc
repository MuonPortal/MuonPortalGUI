// ******************************************************************************
// * License and Disclaimer                                                     *
// *                                                                            *
// * Copyright 2018 Simone Riggi																			          *
// *																																	          *
// * This file is part of MuonPortalGUI																          *
// * MuonPortalGUI is free software: you can redistribute it and/or modify it   *
// * under the terms of the GNU General Public License as published by          *
// * the Free Software Foundation, either * version 3 of the License,           *
// * or (at your option) any later version.                                     *
// * MuonPortalGUI is distributed in the hope that it will be useful, but 			*
// * WITHOUT ANY WARRANTY; without even the implied warranty of                 * 
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
// * See the GNU General Public License for more details. You should            * 
// * have received a copy of the GNU General Public License along with          * 
// * MuonPortalGUI. If not, see http://www.gnu.org/licenses/.                   *
// ******************************************************************************
/**
* @file Utilities.cc
* @class Utilities
* @brief Utility functions
*
* Useful functions to be used in the simulation, or to handle output files
* @author S. Riggi
* @date 23/08/2010
*/

#include <Utilities.h>
#include <AnalysisConsts.h>


#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TF1.h>
#include <TH2.h>
#include <TLegend.h>
#include <TCut.h>
#include <TEventList.h>
#include <TMath.h>
#include <TPad.h>
#include <TVirtualPad.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TVector3.h>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <math.h>

using namespace std;

namespace MuonPortalNS {

Utilities::Utilities(){

}

Utilities::~Utilities(){

}

std::string Utilities::ExecSystemCommand(const char* cmd) 
{	
	FILE* pipe = popen(cmd, "r");
  if (!pipe) {	
		cerr<<"ERROR: Cannot get exec input"<<endl;
		exit(1);
	}

  char buffer[128];
  std::string parsedCmdOutput = "";
  while(!feof(pipe)) {
 		if(fgets(buffer, 128, pipe) != NULL)
    	parsedCmdOutput += buffer;
  }
  pclose(pipe);

	return parsedCmdOutput;

}//close Utilities::ExecSystemCommand

}//close namespace

