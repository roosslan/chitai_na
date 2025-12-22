
#include "stdafx.h"
#include "helper_functions.h"

void r_logger::init_logging()
{
	boost::log::add_common_attributes();

	const auto console_sink = boost::log::add_console_log(std::clog);
	console_sink->set_formatter(&LogFormatter);
	logging::core::get()->add_sink(console_sink);

	CString curr_dir;
	GetCurrentDirectory(MAX_PATH, curr_dir.GetBufferSetLength(MAX_PATH));
	curr_dir.ReleaseBuffer();
	const CString current_dir = curr_dir + L"\\mp4";

	const auto fs_sink = boost::log::add_file_log(
		boost::log::keywords::file_name = current_dir + "\\chitai_na_ % d. % m. % Y - % H_ % M_ % S.log",
		keywords::format = "%TimeStamp% % Message % ",
		boost::log::keywords::rotation_size = 10 * 1024 * 1024,
		boost::log::keywords::min_free_space = 30 * 1024 * 1024,
		boost::log::keywords::open_mode = std::ios_base::app);

	fs_sink->locked_backend()->auto_flush(true);

	LOG_SAVE << "chitai_na's r_logger init " << std::endl;
}

std::string r_logger::path_to_filename(std::string path) {
	return path.substr(path.find_last_of("/\\") + 1);
}

void r_logger::LogFormatter(logging::record_view const& rec, logging::formatting_ostream& strm)
{
	strm << rec[expr::smessage];
}