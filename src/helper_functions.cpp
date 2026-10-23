#include "stdafx.h"
#include "helper_functions.h"

CString get_exe_dir() {
	CString exe_path;
	const DWORD len = GetModuleFileName(nullptr, exe_path.GetBufferSetLength(MAX_PATH), MAX_PATH);
	exe_path.ReleaseBuffer(len);

	const int pos = exe_path.ReverseFind(L'\\');
	return pos >= 0 ? exe_path.Left(pos) : exe_path;
}

namespace
{
	std::mutex log_mutex;
	std::ofstream log_file;
}

void r_logger::init_logging() {
	/* Логи пишутся в <директория exe>\logs, а не в mp4\ текущей директории */
	const CString log_dir = get_exe_dir() + L"\\logs";
	CreateDirectory(log_dir, nullptr);

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString file_name;
	file_name.Format(L"%s\\chitai_na_%04u-%02u-%02u_%02u-%02u-%02u.log", log_dir.GetString(),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	{
		std::lock_guard lock(log_mutex);
		log_file.open(file_name.GetString(), std::ios_base::app);
	}

	LOG_SAVE << "chitai_na's r_logger init";
}

void r_logger::write_line(const std::string& message) {
	SYSTEMTIME st;
	GetLocalTime(&st);

	std::lock_guard lock(log_mutex);
	if (!log_file.is_open())
		return;

	log_file << std::format("[{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}] ",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds)
		<< message << std::endl;
}

std::string r_logger::path_to_filename(std::string path) {
	return path.substr(path.find_last_of("/\\") + 1);
}
