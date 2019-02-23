#ifndef PARSE_CONFIG_H_
#define PARSE_CONFIG_H_

#include <fstream>
#include <string>
#include <map>

#include "common/StringUtil.h"
#include "common/Log.h"

class ParseConfig 
{
public:
	ParseConfig(){}
	~ParseConfig(){};

	bool Init(const std::string& filepath)
	{
		std::string file_data;
		if (!ReadFile(filepath, file_data))
			return false;
		std::vector<std::string> content;
		StringUtil::Split(file_data, "\r\n", content);
		LOG_INFO("File Data %s", file_data.c_str());
		for (auto it : content)
		{
			if (it.empty())
			{
				LOG_INFO("line content is empty");
				continue;
			}
			LOG_INFO("line content:%s",it.c_str());
			std::vector<std::string> key_val;
			std::string pure_string = StringUtil::Replace(it, " ", "");
			StringUtil::Split(it, "=", key_val);
			if (key_val.size() < 2)
				continue;
			LOG_INFO("key:%s  val:%s", key_val[0].c_str(), key_val[1].c_str());
			m_config_map[key_val[0]] = key_val[1];
		}

		return true;
	}

	std::string GetValue(const std::string& key)
	{
		auto it = m_config_map.find(key);
		return it==m_config_map.end()?"":it->second;
	}
private:
	bool ReadFile(const std::string& file_path, std::string& file_data)
	{
		FILE* fp = fopen(file_path.c_str(), "rb");
		if (fp == NULL)
		{
			return false;
		}
		fseek(fp, 0, SEEK_END);
		int32_t length = ftell(fp);
		rewind(fp);

		file_data.resize(length);
		fread(&file_data[0], 1, length, fp);
		fclose(fp);
		return true;
	}

private:
	std::map<std::string,std::string> m_config_map;
};

#endif