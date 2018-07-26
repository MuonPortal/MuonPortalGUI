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
* @file AnalysisConsts.h
* @class AnalysisConsts
* @brief List of constants
*
* @author S. Riggi
* @date 21/02/2012
*/

#ifndef __AnalysisConsts_H
#define __AnalysisConsts_H

#include "TMath.h"

namespace MuonPortalNS {

//###########################		
//### PHYSICAL CONSTANTS
//###########################
static double PlanckConstant= TMath::H();//in Joule
static double SpeedOfLight= TMath::C();//in m/s
static double Joule2eV= 6.24150647996E+18;

//###########################		
//### DETECTOR CONSTANTS
//###########################
//Realistic QE from Thorn EMI (now Phototube) model 9826B (QB)
static const int NUMENTRIES_QETable= 80;
static const double PhotonEnergy[NUMENTRIES_QETable]={1.92134,1.93604,1.95036,1.99556,2.04224,2.09965,2.14172,2.17631,2.20691,2.22035,2.23395,2.24243,2.27077,2.27954,2.29388,2.32267,2.34242,2.3625,2.38294,2.39907,2.40372,2.41967,2.44689,2.46391,2.48067,2.49767,2.50271,2.51438,2.53132,2.54902,2.56643,2.58516,2.61625,2.64133,2.66117,2.68771,2.71478,2.73575,2.75643,2.78553,2.83069,2.84563,2.88469,2.91659,2.95834,2.99189,3.01811,3.0538,3.0996,3.14681,3.20538,3.2774,3.34189,3.37555,3.40897,3.46809,3.56789,3.66059,3.70323,3.73334,3.7628,3.77885,3.79737,3.8314,3.84924,3.86725,3.88787,3.90624,3.92603,3.94603,3.96495,3.96876,3.9713,3.99048,4.00854,4.04912,4.07039,4.07708,4.08245,4.08514};
		
static const double QE[NUMENTRIES_QETable]={0.002064,0.002565,0.004543,0.009493,0.01592,0.02481,0.03419,0.04357,0.05343,0.05836,0.06231,0.06675,0.07464,0.07957,0.0845,0.09387,0.1003,0.1072,0.1121,0.1151,0.1185,0.1235,0.1294,0.1328,0.1368,0.1407,0.1447,0.1466,0.1521,0.157,0.1619,0.1654,0.1723,0.1772,0.1812,0.1851,0.1901,0.1925,0.196,0.1989,0.2029,0.2054,0.2078,0.2108,0.2133,0.2153,0.2167,0.2177,0.2202,0.2217,0.2227,0.2242,0.2242,0.2242,0.2237,0.2222,0.2193,0.2139,0.21,0.207,0.2036,0.2001,0.1937,0.1868,0.1824,0.177,0.1701,0.1652,0.1583,0.1533,0.1479,0.1435,0.1396,0.1361,0.1317,0.1208,0.1149,0.1066,0.1011,0.03662};

}//close namespace

#endif
