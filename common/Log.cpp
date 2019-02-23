#include <atomic>
#include <thread>
#include <stdint.h>
#include <stdio.h>
#include <mutex>
#include <string>
#include <list>
#include <stdarg.h>
#include <condition_variable>


#include "Singleton.h"
#include "Log.h"
#include "DateTime.h"
#include "Util.h"
#include "SpinLock.h"

#define  MAX_FILE_SIZE 8388608  //8*1024*1024 = 8M
#define  MAX_LOG_SIZE  10240     //10 * 1024 = 10K


const static char* LevelStr[] = { "[Fatal]", "[Error]", "[Warm]", "[Info]", "[Debug]" };
class LogFile
{
public:
	LogFile()
	{
		char tmp[256];
		int n = sprintf(tmp, "./log/log_%d_%lu", GetProcessId(), DateTime::NowMilliSecond());
		m_file_name = std::string(tmp, n);
		OpenNextFile();
	}
	~LogFile()
	{
		CloseFile();
	}

	void WriteLog(const std::string& content)
	{
		if (!m_fp || content.empty()) return;
		size_t write_size = fwrite(content.c_str(), 1, content.length(), m_fp);
		m_cur_file_size += write_size;

		if (m_cur_file_size > MAX_FILE_SIZE)
		{
			CloseFile();
			OpenNextFile();
		}
	}

	void Fflush()
	{
		if (m_fp)
		{
			::fflush(m_fp);
		}
	}
private:
	void CloseFile()
	{
		if (m_fp)
		{
			::fclose(m_fp);
			m_fp = NULL;
		}
	}

	void OpenNextFile()
	{
		m_cur_file_size = 0;
		char tmp[512] = { 0 };
		sprintf(tmp, "%s_%lu.txt", m_file_name.c_str(), m_file_index++);
		m_fp = fopen(tmp, "a");
	}

private:
	FILE* m_fp = NULL;
	size_t m_file_index = 0;
	std::string m_file_name;
	size_t m_cur_file_size = 0;
};


class LogImpl : public Singleton<LogImpl>, public Log
{
public:
	LogImpl()
	{
		m_level = LOGDebug;
		m_write_thread = std::thread(&LogImpl::WriteThread, this);
		m_exist.store(false, std::memory_order_relaxed);
	}

	virtual ~LogImpl()
	{
		m_exist.store(true, std::memory_order_relaxed);

		if (m_write_thread.joinable())
			m_write_thread.join();
	}

	void SetLevel(int32_t level)
	{
		m_level = level;
	}

	void LogFormat(LogLevel level, const char* fmt, ...) override
	{
		if (nullptr == fmt || level > m_level)
			return;

		char fmt_buff[MAX_LOG_SIZE + 2];
		int32_t date_len = DateTime::FormatMilliString(fmt_buff, MAX_LOG_SIZE);
		if (date_len <= 0)
			return;

		int head_size = sprintf(fmt_buff + date_len, "-%s", LevelStr[level]);
		if (head_size < 0)
			return;
		head_size += date_len;
		va_list ap;
		va_start(ap, fmt);
		int size = vsnprintf(fmt_buff + head_size, MAX_LOG_SIZE - head_size, fmt, ap);
		if (size < 0)
			return;
		fmt_buff[head_size + size] = '\r';
		fmt_buff[head_size + size + 1] = '\n';

		PushData(std::string(fmt_buff, head_size + size + 2));
		va_end(ap);
	}


private:
	void WriteThread()
	{
		LogFile log_file;

		while (1)
		{
			while (!CheckWriteNotEmptyAndSwap())
			{
				if (m_exist.load(std::memory_order_relaxed))
					return;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			for (auto it : m_datas[m_read_index])
			{
				log_file.WriteLog(it);
			}
			log_file.Fflush();
			m_datas[m_read_index].clear();
		}
	}

	void PushData(const std::string& data)
	{
		SpinLock spin_lock(m_spin_mutex);
		m_datas[m_write_index].push_back(data);
	}

	bool CheckWriteNotEmptyAndSwap()
	{
		SpinLock spin_lock(m_spin_mutex);
		if (m_datas[m_write_index].size() == 0)
			return false;
		std::swap(m_write_index, m_read_index);
		return true;
	}

private:
	std::thread m_write_thread;
	std::list<std::string> m_datas[2];
	int32_t m_write_index = 0;
	int32_t m_read_index = 1;

	std::atomic<bool> m_exist;
	SpinMutex m_spin_mutex;

	int32_t m_level;

};

Log& Log::GetLogInstance()
{
	return LogImpl::instance();
}
