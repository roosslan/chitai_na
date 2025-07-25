
#include "stdafx.h"
#include "helper_functions.h"

void rLogger::InitLogging()
{
	boost::log::add_common_attributes();

	auto consoleSink = boost::log::add_console_log(std::clog);
	consoleSink->set_formatter(&LogFormatter);
	logging::core::get()->add_sink(consoleSink);

	std::string strCurrentPath{ std::filesystem::current_path().u8string() };

	auto fsSink = boost::log::add_file_log(
		boost::log::keywords::file_name = strCurrentPath + "\\chitai_na_%d.%m.%Y-%H_%M_%S.log",
		keywords::format = "%TimeStamp% % Message % ",
		boost::log::keywords::rotation_size = 10 * 1024 * 1024,
		boost::log::keywords::min_free_space = 30 * 1024 * 1024,
		boost::log::keywords::open_mode = std::ios_base::app);

	fsSink->locked_backend()->auto_flush(true);

	LOG_SAVE << "chitai_na's rLogger init " << std::endl;
}

std::string rLogger::PathToFilename(std::string path) {
	return path.substr(path.find_last_of("/\\") + 1);
}

void rLogger::LogFormatter(logging::record_view const& rec, logging::formatting_ostream& strm)
{
	// strm << logging::extract< int >("Line", rec) << ":";
	strm << rec[expr::smessage];
}