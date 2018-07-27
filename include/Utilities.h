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
* @file Utilities.h
* @class Utilities
* @brief Utility functions
*
* Useful functions to be used in the simulation, or to handle output files
* @author S. Riggi
* @date 23/08/2010
*/


#ifndef Utilities_h
#define Utilities_h 1

#include <AnalysisConsts.h>
#include <Logger.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <time.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TMatrixD.h>
#include <TF1.h>
#include <TGraph.h>
#include <TVector3.h>

#include <QFileInfo>
#include <QString>
#include <QUuid>


#include <stdio.h>
#include <unistd.h>
#include <string.h> //for strncpy 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <pwd.h>

using namespace std;

namespace MuonPortalNS {

class Utilities{

  public:
		
		/** 
		\brief Class constructor: initialize structures.
 		*/
    Utilities();
		/**
		* \brief Class destructor: free allocated memory
		*/
   ~Utilities();

		
	public:

		
		/**
		* \brief Order vectors and get ordering index
		*/
		template<class T> struct index_cmp{

  		index_cmp(const T arr) : arr(arr) {}
  		bool operator()(const size_t a, const size_t b) const
 			{
    		return arr[a] < arr[b];
  		}
  		const T arr;
		};

		/**
		* \brief Reorder vector
		*/
		template< class T >
			static void reorder(std::vector<T> & unordered,std::vector<size_t> const & index_map,std::vector<T> & ordered){
  			// copy for the reorder according to index_map, because unsorted may also be
  			// sorted
  			std::vector<T> copy = unordered;
  			ordered.resize(index_map.size());
  			for(int i = 0; i<index_map.size();i++)
					ordered[i] = copy[index_map[i]];
			}

		/**
		* \brief Sort vector
		*/
		template <class T>
			static void sort(std::vector<T> & unsorted,std::vector<T> & sorted,std::vector<size_t> & index_map){
  			// Original unsorted index map
  			index_map.resize(unsorted.size());
 				for(size_t i=0;i<unsorted.size();i++)
					index_map[i] = i;
  
  			// Sort the index map, using unsorted for comparison
  			std::sort(index_map.begin(),index_map.end(),index_cmp<std::vector<T>& >(unsorted));
  			sorted.resize(unsorted.size());
  			reorder(unsorted,index_map,sorted);
			}

		/**
		* \brief Compute median
		*/
		template < typename T >
			static T GetMedian( std::vector<T>& vec ){
    		//size_t n = vec.size() / 2;
    		//nth_element(vec.begin(), vec.begin()+n, vec.end());
    		//return vec[n];

				double median;		
  			size_t n = vec.size();

  			std::sort(vec.begin(), vec.end());

  			if(n%2==0) median = (vec[n/2-1] + vec[n/2])/2;	
				else median = vec[n/2];

  			return median;
			}

		/**
		* \brief Compute matrix trace
		*/
		static double Trace(TMatrixD T){			
			double trace= 0;
			for(int i=0;i<T.GetNrows();i++){
				trace+= T(i,i);		
			}
  		return trace;
		}

		/**
		* \brief Compute timestamp difference
		*/
		static timespec TimeDiff(timespec start, timespec end)
		{
			timespec temp;
			if ((end.tv_nsec-start.tv_nsec)<0) {
				temp.tv_sec = end.tv_sec-start.tv_sec-1;
				temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
			} 
			else {
				temp.tv_sec = end.tv_sec-start.tv_sec;
				temp.tv_nsec = end.tv_nsec-start.tv_nsec;
			}
			return temp;
		}

		/**
		* \brief Compute timestamp sum
		*/
		static timespec TimeSum (timespec time1, timespec time2)
		{

			timespec  result ;
			result.tv_sec = time1.tv_sec + time2.tv_sec ;
    	result.tv_nsec = time1.tv_nsec + time2.tv_nsec ;
    	if (result.tv_nsec >= 1000000000L) {		/* Carry? */
      	result.tv_sec++ ;  result.tv_nsec = result.tv_nsec - 1000000000L ;
    	}

    	return (result) ;
		}

		/**
		* \brief Compute Convert time to seconds
		*/
		static double TimeToSec (timespec time)
		{
    	return ((double)time.tv_sec + (time.tv_nsec/1.e+09)) ;
		}

		/**
		* \brief Compute Convert time to nanoseconds
		*/
		static double TimeToNSec (timespec time)
		{
    	return (time.tv_sec*1.e+09 + (double)time.tv_nsec) ;
		}

		/**
		* \brief Execute system command
		*/
		static std::string ExecSystemCommand(const char* cmd);

		/**
		* \brief Count number of lines in ascii file
		*/
		static int GetNLinesInAsciiFile(std::ifstream in){

			//check
			if(!in.good()) {
    		ERROR_LOG("Cannot open file stream ...exit!");
				exit(1);
  		}
		
			//rewind file
			in.seekg(ios::beg); 

      int n = 0;
      std::string s;
      while(getline(in,s)) {
      	n++;
      }

			//in.close();

			return n;
		}//close function

	/**
	* \brief Find pattern in string and replace
	*/
	static void StringFindAndReplace(std::string& str, const std::string& oldstr, const std::string& newstr)
	{
  	size_t pos = 0;
		while((pos = str.find(oldstr, pos)) != std::string::npos){
    	str.replace(pos, oldstr.length(), newstr);
     	pos += newstr.length();
  	}	
	}//close function

	/**
	* \brief Convert number to string
	*/
	template < typename T >
		static std::string ConvertToString(T value)
		{
			ostringstream stream;
			stream << value;
			return stream.str();
		}//close function

	/**
	* \brief Retrieve current hostname
	*/
	static std::string GetHostName(){

		char hostname[1024];
		hostname[1023] = '\0';
		gethostname(hostname, 1023);

		return std::string(hostname);

	}//close function

	/**
	* \brief Retrieve current ipaddress
	*/
	static std::string GetIpAddress(){
		
			std::string ip_address;
			int fd;
 			struct ifreq ifr;

 			fd = socket(AF_INET, SOCK_DGRAM, 0);

 			// I want to get an IPv4 IP address 
 			ifr.ifr_addr.sa_family = AF_INET;
			
 			
			//## Find which interface is present and assign address
			ip_address= "";
			std::string p3p1_output= ExecSystemCommand("ifconfig -s | grep p3p1");
			std::string eth0_output= ExecSystemCommand("ifconfig -s | grep eth0");
			std::string eth1_output= ExecSystemCommand("ifconfig -s | grep eth1");
			std::string em1_output= ExecSystemCommand("ifconfig -s | grep em1");
			std::string wlan0_output= ExecSystemCommand("ifconfig -s | grep wlan0");
			
			if(p3p1_output!=""){
				strncpy(ifr.ifr_name, "p3p1", IFNAMSIZ-1);
				ioctl(fd, SIOCGIFADDR, &ifr);
				ip_address= std::string( inet_ntoa( ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr) );			
			}
			if(eth0_output!=""){
				strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
				ioctl(fd, SIOCGIFADDR, &ifr);
				ip_address= std::string( inet_ntoa( ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr) );		
			}
			if(eth1_output!=""){
				strncpy(ifr.ifr_name, "eth1", IFNAMSIZ-1);
				ioctl(fd, SIOCGIFADDR, &ifr);
				ip_address= std::string( inet_ntoa( ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr) );
				
			}
			if(em1_output!=""){
				strncpy(ifr.ifr_name, "em1", IFNAMSIZ-1);
				ioctl(fd, SIOCGIFADDR, &ifr);
				ip_address= std::string( inet_ntoa( ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr) );
				
			}
			if(wlan0_output!=""){
				strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
				ioctl(fd, SIOCGIFADDR, &ifr);
				ip_address= std::string( inet_ntoa( ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr) );
			}

			if(ip_address=="0.0.0.0" || ip_address==""){
				ERROR_LOG("Cannot determine ip address of current host...exit!");
				exit(1);	
			}
	
 			close(fd);

 			DEBUG_LOG("ip_address: "<<ip_address);

 			return ip_address;

	}//close function
	
	/**
	* \brief Retrieve current username
	*/
	static std::string GetUserName()
	{
		//Get user passwd struct
		std::string username= "";
		struct passwd* info= getpwuid(getuid());		
		if(info && info->pw_name) username= std::string(info->pw_name);
		endpwent();
	
		//Old method
		//username= std::string(getlogin());

		return username;

	}//close function

	/**
	* \brief Retrieve filename base without path
	*/
	static std::string GetBaseFileNameNoPath(std::string filename){
		
		QFileInfo fileInfo(filename.c_str());
		std::string fileBaseName= (fileInfo.baseName()).toStdString();
		
		return fileBaseName;

	}//close function

	/**
	* \brief Retrieve current base
	*/
	static std::string GetBaseFileName(std::string filename){
		
		QFileInfo fileInfo(filename.c_str());
		std::string fileBaseName= (fileInfo.completeBaseName()).toStdString();
		std::string fileExtension= (fileInfo.completeSuffix()).toStdString();
		std::string newfilename= fileBaseName + std::string(".") + fileExtension;
		
		return newfilename;

	}//close function

	/**
	* \brief Retrieve filename base without extension
	*/
	static std::string GetBaseFileNameNoExt(std::string filename){
		
		QFileInfo fileInfo(filename.c_str());
		std::string fileBaseName= (fileInfo.completeBaseName()).toStdString();
		
		return fileBaseName;

	}//close function

	/**
	* \brief Get current timestamp
	*/
	static long int GetTimeStamp(){
		
		time_t timev;
		long int t_sec= time(&timev);
	
		return t_sec;

	}//close function

	/**
	* \brief Get UUID
	*/
	static std::string GetUUID(){
		
		QUuid uuid= QUuid::createUuid();
		std::string key= (uuid.toString()).toStdString();
		StringFindAndReplace(key,std::string("{"),std::string(""));
		StringFindAndReplace(key,std::string("}"),std::string(""));
		return key;	

	}//close function

	
	private:
	
		
};//close class Utilities

}//close namespace 

#endif 

