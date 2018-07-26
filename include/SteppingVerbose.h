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


#endif
