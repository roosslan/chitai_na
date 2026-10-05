#include "stdafx.h"
#include "helper_functions.h"

CString get_exe_dir()
{
	CString exe_path;
	const DWORD len = GetModuleFileName(nullptr, exe_path.GetBufferSetLength(MAX_PATH), MAX_PATH);
	exe_path.ReleaseBuffer(len);

	const int pos = exe_path.ReverseFind(L'\\');
	return pos >= 0 ? exe_path.Left(pos) : exe_path;
}

void r_logger::init_logging()
{
	boost::log::add_common_attributes();

	/* Логи пишутся в <директория exe>\logs, а не в mp4\ текущей директории */
	const CString log_dir = get_exe_dir() + L"\\logs";
	CreateDirectory(log_dir, nullptr);

	const std::wstring file_pattern = std::wstring(log_dir.GetString()) + L"\\chitai_na_%Y-%m-%d_%H-%M-%S.log";

	const auto fs_sink = boost::log::add_file_log(
		boost::log::keywords::file_name = file_pattern,
		boost::log::keywords::format = "[%TimeStamp%] %Message%",
		boost::log::keywords::rotation_size = 10 * 1024 * 1024,
		boost::log::keywords::open_mode = std::ios_base::app);

	fs_sink->locked_backend()->auto_flush(true);

	LOG_SAVE << "chitai_na's r_logger init";
}

std::string r_logger::path_to_filename(std::string path) {
	return path.substr(path.find_last_of("/\\") + 1);
}
