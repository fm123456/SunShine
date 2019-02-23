#include "FileSystem.h"


bool FileSystem::Exist(const std::string& filepath)
{
	return std::filesystem::exists(filepath);
}

bool FileSystem::CreateDirectory(const std::string& dir_path)
{
	return std::filesystem::create_directory(dir_path);
}

bool FileSystem::RemoveFile(const std::string& filepath)
{
	if (IsDirectory(filepath))
		return std::filesystem::remove_all(filepath);
	return std::filesystem::remove(filepath);
}

bool FileSystem::IsFile(const std::string& filepath)
{
	return std::filesystem::is_regular_file(filepath);
}

bool FileSystem::IsDirectory(const std::string dirpath)
{
	return std::filesystem::is_directory(dirpath);
}

std::string FileSystem::GetCurrentWorkDirectory()
{
	return std::filesystem::current_path().string();
}

void FileSystem::ChangeCurrentWorkDirectory(const std::string& work_dir)
{
	std::filesystem::current_path(work_dir);
}