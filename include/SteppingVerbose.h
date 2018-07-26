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
* @file SteppingVerbose.h
* @class SteppingVerbose
* @brief Handle the verbosity of the stepping action
* @author Dr. Simone Riggi
* @date 05/04/2010
*/

class SteppingVerbose;

#ifndef SteppingVerbose_h
#define SteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

namespace MuonPortalNS {

class SteppingVerbose : public G4SteppingVerbose
{
 public:   
	/** 
	\brief Class constructor
 	*/
   SteppingVerbose();
	/**
	* \brief Class destructor
	*/
  ~SteppingVerbose();
	/**
	* \brief Set which information has to be printed for the current step
	*/
   void StepInfo();
	/**
	* \brief Set which information has to be printed for the track in the current step
	*/
   void TrackingStarted();

};

}//close namespace

#endif
