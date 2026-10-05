#include "stdafx.h"
#pragma once

#define LOG_SAVE BOOST_LOG_SEV(boost::log::trivial::logger::get(), boost::log::trivial::severity_level::trace)	\
	<< "<" << r_logger::path_to_filename(__FILE__) << ":" << __LINE__ << "> " BOOST_CURRENT_FUNCTION << " | " 	\
	<< boost::log::add_value("Line", __LINE__)

/* Директория, в которой лежит chitai_na.exe (без завершающего '\') */
CString get_exe_dir();

namespace r_logger
{
	void init_logging();
	std::string path_to_filename(std::string path);
};
