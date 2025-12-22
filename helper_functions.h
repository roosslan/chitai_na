#include "stdafx.h"
#pragma once

#define LOG_SAVE BOOST_LOG_SEV(boost::log::trivial::logger::get(), boost::log::trivial::severity_level::trace)	\
	<< "<" << r_logger::path_to_filename(__FILE__) << ":" << __LINE__ << "> " BOOST_CURRENT_FUNCTION << " | " 	\
	<< boost::log::add_value("Line", __LINE__)

namespace r_logger
{
	void init_logging();
	void LogFormatter(logging::record_view const& rec, logging::formatting_ostream& strm);
	std::string path_to_filename(std::string path);
};
