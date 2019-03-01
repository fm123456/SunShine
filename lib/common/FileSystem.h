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
	/* �ж��ļ������ļ����Ƿ����
	*/
	static bool Exist(const std::string& filepath);

	static bool CreateDirectory(const std::string& dir_path);

	/* ɾ���ļ��������Ŀ¼�Ļ�����ɾ������Ŀ¼
	*/
	static bool RemoveFile(const std::string& filepath);

	static std::string GetCurrentWorkDirectory();

	static void ChangeCurrentWorkDirectory(const std::string& work_dir);

	static bool IsFile(const std::string& filepath);

	static bool IsDirectory(const std::string dirpath);
};

#endif
