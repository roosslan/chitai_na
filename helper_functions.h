#pragma once

#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "stdafx.h"

#define LOG_SAVE BOOST_LOG_SEV(boost::log::trivial::logger::get(), boost::log::trivial::severity_level::trace)	\
	<< "<" << rLogger::PathToFilename(__FILE__) << ":" << __LINE__ << "> " BOOST_CURRENT_FUNCTION << " | " 	\
	<< boost::log::add_value("Line", __LINE__)

namespace rLogger
{
	void InitLogging();
	void LogFormatter(logging::record_view const& rec, logging::formatting_ostream& strm);
	std::string PathToFilename(std::string path);
};

#endif