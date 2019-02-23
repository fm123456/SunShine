#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include <filesystem>
#include <string>

class Path
{
public:
	Path(const std::string& filepath);
private:
	std::string m_filepath;
};

class FileSystem
{
public:
	/* 判断文件或者文件夹是否存在
	*/
	static bool Exist(const std::string& filepath);

	static bool CreateDirectory(const std::string& dir_path);

	/* 删除文件，如果是目录的话的则删除整个目录
	*/
	static bool RemoveFile(const std::string& filepath);

	static std::string GetCurrentWorkDirectory();

	static void ChangeCurrentWorkDirectory(const std::string& work_dir);

	static bool IsFile(const std::string& filepath);

	static bool IsDirectory(const std::string dirpath);
};

#endif
