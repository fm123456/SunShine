#include <iostream>
#include <string>

#include "json/json.h"


bool ReadJsonFromFile(Json::Value& js, const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "r");
    if (!fp)  return false;

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);

    char* databuff = new char[size];
    rewind(fp);
    fread(databuff, sizeof(char), size, fp);

    Json::Reader js_reader;
    if (!js_reader.parse(databuff, databuff + size, js))
        return false;

    delete[] databuff;
    return true;
}

int main()
{
    Json::Value js;

    std::string filename = "TestJson.json";
    if (!ReadJsonFromFile(js, filename))
        return -1;

    if (js["EntryTask"].isNull())
        return -1;

    Json::Value js1 = js["EntryTask"];

    std::string type = js1["Type"].asString();
    std::cout << "type: " << type.c_str() << std::endl;

    return 0;
}


