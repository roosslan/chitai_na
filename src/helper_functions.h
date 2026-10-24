#include "stdafx.h"
#pragma once

/* Запись строки в лог: LOG_SAVE << "текст" << значение; */
#define LOG_SAVE r_logger::log_line()	\
	<< "<" << r_logger::path_to_filename(__FILE__) << ":" << __LINE__ << "> " << __FUNCSIG__ << " | "

/* Директория, в которой лежит chitai_na.exe (без завершающего '\') */
CString get_exe_dir();

namespace r_logger
{
	void init_logging();
	std::string path_to_filename(std::string path);
	void write_line(const std::string& message);

	/* Накапливает сообщение и записывает его в лог одной строкой при разрушении */
	class log_line
	{
	public:
		log_line() = default;
		log_line(const log_line&) = delete;
		log_line& operator=(const log_line&) = delete;
		~log_line() { write_line(stream_.str()); }

		template <typename T>
		log_line& operator<<(const T& value) {
			stream_ << value;
			return *this;
		}

	private:
		std::ostringstream stream_;
	};
};
