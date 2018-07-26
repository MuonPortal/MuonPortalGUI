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
* @file Logger.h
* @class Logger
* @brief Logger
*
* Logger class for managing logging msg
* @author S. Riggi
* @date 20/01/2015
*/

#ifndef Logger_h
#define Logger_h 1


//== LOG4CXX HEADERS ==
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/rollingfileappender.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/net/syslogappender.h>
#include <log4cxx/helpers/exception.h>

//Boost
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include <map>
#include <string>

using namespace std;


namespace MuonPortalNS {


class Logger  {

	public:
		Logger(std::string level="OFF",std::string tag="logger")
			: m_level(level), m_tag(tag)		
		{

		};
		virtual ~Logger(){};
	
	public:
		virtual int Init() = 0;

		virtual std::string GetHost() const {
			char hostname[HOST_NAME_MAX];
			gethostname(hostname, HOST_NAME_MAX);
			std::string syslog_host(hostname); 
			return syslog_host;
		}

		
		virtual void Log(const std::string sLevel, const std::string msg, const std::string msg_prefix=""){
			log4cxx::LevelPtr logLevel= log4cxx::Level::toLevel(sLevel,log4cxx::Level::getOff());
			if(logger && logger->isEnabledFor(logLevel) ){
				log4cxx::helpers::MessageBuffer oss_; \
      	logger->forcedLog(logLevel, oss_.str(oss_ << msg_prefix << msg), log4cxx::spi::LocationInfo(m_tag.c_str(),__LOG4CXX_FUNC__,__LINE__) );
			}
		}	
			
		virtual void SetLogLevel(const std::string sLevel){
			log4cxx::LevelPtr thisLogLevel= logger->getLevel();//retrieve the current log level
			if(!thisLogLevel) thisLogLevel= log4cxx::Level::getOff();
			logger->setLevel(log4cxx::Level::toLevel(sLevel,thisLogLevel));// In case of wrong string the Log level is set to the old or to OFF
		}//close SetLogLevel()

	protected:
		std::string m_level;
		std::string m_tag;		
		log4cxx::LoggerPtr logger;
		log4cxx::LayoutPtr layout;
		log4cxx::AppenderPtr appender;		
		
};//close Logger class



class ConsoleLogger : public Logger {

	public:
		ConsoleLogger(const std::string level,const std::string tag,const std::string target)
			: Logger(level,tag), m_target(target)
		{
			
		};

		virtual ~ConsoleLogger(){};

	private:
		
	public:
		virtual int Init(){
			//Create logger
			logger= log4cxx::LoggerPtr(log4cxx::Logger::getLogger(m_tag));
			if(!logger) return -1;
			
			//Define log layout
			//layout= log4cxx::LayoutPtr( new log4cxx::PatternLayout("%d %-5p [%c] [%l] %m%n") );
			layout= log4cxx::LayoutPtr( new log4cxx::PatternLayout("%d %-5p [%c] %m%n") );
			if(!layout) return -1;

			//Create and add appender
			appender= log4cxx::AppenderPtr( new log4cxx::ConsoleAppender(layout,m_target) );	
			appender->setOption("TARGET",m_target);
			logger->addAppender(appender);
	
			//Set logging level (OFF is set if a wrong name is given!)
			log4cxx::LevelPtr level= log4cxx::Level::toLevel(m_level,log4cxx::Level::getInfo());
			logger->setLevel(level);  

			return 0;
		}//close Init()

	
	protected:
		std::string m_target;
		
};//close ConsoleLogger class



class LoggerManager {
	
	public:
		static LoggerManager& Instance() {
    	// Since it's a static variable, if the class has already been created,
      // It won't be created again.
      // And it is thread-safe in C++11.
      static LoggerManager myInstance;
 
      // Return a reference to our instance.
      return myInstance;
    }
 
    // delete copy and move constructors and assign operators
    LoggerManager(LoggerManager const&) = delete;             // Copy construct
    LoggerManager(LoggerManager&&) = delete;                  // Move construct
    LoggerManager& operator=(LoggerManager const&) = delete;  // Copy assign
    LoggerManager& operator=(LoggerManager &&) = delete;      // Move assign

	public:
		
		static int CreateConsoleLogger(const std::string level,const std::string tag="logger",const std::string target="System.out") {
				
			//Skip if already created
			if(m_logger) {	
				return -1;
			}

			//Create syslogger
			m_target= 3;//eFile;
			m_logger= new ConsoleLogger(level,tag,target);
			m_logger->Init();
			
			return 0;
		}//close CreateConsoleLogger()

		Logger* GetLogger(){return m_logger;}
		int GetLoggerTarget(){return m_target;}

		

	protected:
		LoggerManager(){
			
		};
		virtual ~LoggerManager(){
			/*
			if(m_logger){
				delete m_logger;
				m_logger= 0;
			}	
			*/	
		};

	private:
		static int m_target;
		static Logger* m_logger;

	
};//close LoggerManager()



class ScopedLogger {
	public:
		//--> Constructor
		ScopedLogger(std::string level,std::string prefix="")
  		: m_level(level), m_msgprefix(prefix)
		{}

			
		//--> Destructor
  	~ScopedLogger(){ 
			//logging command
			Logger* logger= LoggerManager::Instance().GetLogger();
			if(logger) logger->Log(m_level, m_sstream.str(), m_msgprefix);			

		}//close destructor

	public:
		//Get stream
  	std::stringstream& stream(){ 	
			return m_sstream; 
		}	
	
	private:
		std::stringstream m_sstream;
  	std::string m_level;
		std::string m_msgprefix;
		

};//close ScopedLogger




inline std::string getClassName(std::string fullFuncName,std::string funcName){

	//Init pattern to be searched
	std::string result= "";
	//std::string pattern("::(.*)::");//FIX ME!!!
	std::string pattern("([-A-Za-z0-9_]+)::");
	pattern+= funcName;

	//Create regex
	boost::regex expression;
	try {
  	expression = pattern;
  }
  catch (boost::regex_error& e) {
  	return result;
  }

	//Find match
	boost::smatch matches;
	if (boost::regex_search(fullFuncName, matches, expression) && matches.size()>1) {
		result= std::string(matches[1].first, matches[1].second);
  }//close if
	
	return result;

}//close function

inline std::string getClassNamePrefix(std::string fullFuncName,std::string funcName){
	std::string className= getClassName(fullFuncName,funcName);
	std::string sprefix= "::";
	if(className=="") return className;
	return className+sprefix;
}

#define __CLASS__ getClassName(__PRETTY_FUNCTION__,__FUNCTION__)
#define __CLASS_PREFIX__ getClassNamePrefix(__PRETTY_FUNCTION__,__FUNCTION__)

//== LOG MACROS ===
#define LOG_PREFIX \
	__CLASS_PREFIX__ + __FUNCTION__ + std::string("() - ")

#define LOG(Level, What) \
	ScopedLogger(Level,LOG_PREFIX).stream() << What

#define INFO_LOG(What) LOG("INFO",What)
#define WARN_LOG(What) LOG("WARN",What)
#define DEBUG_LOG(What) LOG("DEBUG",What)
#define ERROR_LOG(What) LOG("ERROR",What)
#define FATAL_LOG(What) LOG("FATAL",What)


}//close namespace 

#endif

