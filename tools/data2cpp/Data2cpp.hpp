#pragma once

#include <fstream>
#include <string_view>
#include <yaml-cpp/yaml.h>

class data2cpp
{
public:
    void open(std::string_view yamlFile);
    void write();
    void close();

private:
    void writeHComment(std::string_view text);
    void writeHIncludes();
    void writeBGuard();
    void writeEGuard();

    void writeCComment(std::string_view text);
    void writeCIncludes();

    std::string_view name;
    std::string base, h, c;
    YAML::Node data;
    std::ofstream hpp, cpp;
};