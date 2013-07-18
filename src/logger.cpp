/*
 * Copyright (C) 2013 Evidence Srl - www.evidence.eu.com
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <new>
#include <cstdlib>

#include "logger.hpp"

namespace log {

// Definition (and initialization) of static attributes
Logger* Logger::m_ = 0;

#ifdef LOGGER_MULTITHREAD
	std::mutex Logger::lock_ ;

	inline void Logger::lock()
	{
		lock_.lock();
	}

	inline void Logger::unlock()
	{
	    lock_.unlock();
	}
#else
	inline void Logger::lock(){}
	inline void Logger::unlock(){}
#endif


/**
 * @brief Constructor.
 *
 * It is a private constructor, called only by getInstance() and only the
 * first time. It is called inside a lock, so lock inside this method
 * is not required.
 * It only initializes the initial time. All configuration is done inside the
 * configure() method.
 */
Logger::Logger():
		logFile_("")
{
	initialTime_ = std::chrono::system_clock::now();
}

/**
 * @brief Method to configure the logger. 
 *
 * This method is called by the LOG_FILE() macro.
 * @param outputFile Name of the file used for logging
 */
void Logger::setFile (const std::string& outputFile)
{
		Logger::lock();

		// Compute the whole file name:
		std::ostringstream oss;
		auto now = std::chrono::system_clock::now();
		std::time_t currTime = std::chrono::system_clock::to_time_t(now);
		struct tm *currTm = std::localtime(&currTime);
		oss << outputFile << "_" <<
				(1900 + currTm->tm_year) << "-" <<
				currTm->tm_mon << "-" <<
				currTm->tm_mday << "_" <<
				currTm->tm_hour << "-" <<
				currTm->tm_min << "-" <<
				currTm->tm_sec << ".log";
		logFile_ = oss.str().c_str();

		// Open a new stream:
		out_.open(logFile_.c_str(), std::ios::app);

		Logger::unlock();
}

/**
 * @brief Destructor.
 *
 * It only closes the file, if open, and cleans memory.
 */

Logger::~Logger()
{
	Logger::lock();
	if (logFile_ != "")
		out_.close();
	delete m_;
	Logger::unlock();

}

/**
 * @brief Method to get a reference to the object (i.e., Singleton)
 *
 * This is a static method.
 * @return Reference to the object.
 */
Logger& Logger::getInstance()
{
	if (m_ == 0){
	    Logger::lock();
	    if (m_ == 0)
		m_ = new Logger;
	    Logger::unlock();
	}
	return *m_;
}


/**
 * @brief Method used to print messages.
 *
 * This method is called by the DEBUG(), WARNING() and ERROR() macros.
 * @param severitylevel Severity of the debug message
 * @param file Source file where the method has been called (set equal to __FILE__
 * 	      by the DEBUG macro)
 * @param line Number of line in the source code where the method has been
 * called (automatically set equal to __LINE__ by the DEBUG macro)
 * @param message Message to be logged
 */
void Logger::print(const std::string& file,
			const int line,
			const std::string& message)
{
	std::chrono::time_point<std::chrono::system_clock> currentTime = 
		std::chrono::system_clock::now();

	int elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>
                             (currentTime - initialTime_).count();

	Logger::lock();
	
	if (logFile_ != "") {
		out_ << "[ " << elapsed_seconds << " ] " << message <<
		"\t[ " << file << ":" << line << "]" << std::endl;
	}

	std::cerr << "[ " << elapsed_seconds << " ] " << message <<
	"\t[ " << file << ":" << line << "]" << std::endl;

	Logger::unlock();
}

} // log
