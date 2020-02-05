#include <fstream>
#include <iostream>
#include <filesystem>
#include <string_view>

#include <nlohmann/json.hpp>

// TODO: C++20 modules require to be built separately from the main(.dll, .lib or .exe) file. Find a way to handle that.
//  --> add support for passing module as command line argument.

// TODO: build this tool by using bootstrapping.
// TODO: show configurations defined in current build.json file.
// TODO: support custom compilers by adding the "compiler" field.
// TODO: add flag that specifies if the code should be built as an executable, static library or shared library.

// TODO: generate a *.vcxproj file based on build.json file.
// TODO:(v) add prebuild and postbuild command.
// TODO(v): add support for /D and /U (define/undef).
// TODO(v): handle cases when a "property" isn't specified, as it won't allow the program to run.

class App
{
public:
    // args, includes, cpp, exename, objdir, libpath, libs, linkargs
    void Run(std::string_view config);
    void Quit();

private:
    nlohmann::json data;
    std::string cmd{"cl "};
};

void App::Run(std::string_view config)
{
    auto path{std::filesystem::current_path()};
    path /= "build.json";
    std::ifstream file{path};
    file >> data;
    file.close(); // maybe later

    if (data[config.data()].empty())
    {
        std::cout << "Config doesn't exist in the current build.json file!" << std::endl;
        exit(-1);
    }
    auto cfg{data[config.data()]};

    if (!cfg["before"].empty())
        for (auto it{cfg["before"].begin()}; it != cfg["before"].end(); ++it)
            system(it->get<std::string>().c_str());

    if (!cfg["args"].empty())
        for (auto it{cfg["args"].begin()}; it != cfg["args"].end(); ++it)
        {
            cmd += it->get<std::string>();
            cmd += " ";
        }

    if (!cfg["include"].empty())
        for (auto it{cfg["include"].begin()}; it != cfg["include"].end(); ++it)
        {
            cmd += "/I";
            cmd += it->get<std::string>();
            cmd += " ";
        }

    if (!cfg["cpp"].empty())
        for (auto it{cfg["cpp"].begin()}; it != cfg["cpp"].end(); ++it)
        {
            cmd += it->get<std::string>();
            cmd += " ";
        }
    else
    {
        std::cout << "Error: Please add the \"cpp\" field in the build.json file > Configuration \"" << config << "\".!";
        exit(-1);
    }

    if (!cfg["define"].empty())
        for (auto it{cfg["define"].begin()}; it != cfg["define"].end(); ++it)
        {
            cmd += "/D ";
            cmd += it->get<std::string>();
            cmd += " ";
        }

    if (!cfg["undef"].empty())
        for (auto it{cfg["undef"].begin()}; it != cfg["undef"].end(); ++it)
        {
            cmd += "/U ";
            cmd += it->get<std::string>();
            cmd += " ";
        }

    if (!cfg["exe"].empty())
    {
        cmd += "/Fe";
        cmd += cfg["exe"].get<std::string>();
        cmd += " ";
    }

    if (!cfg["obj"].empty())
    {
        cmd += "/Fo";
        cmd += cfg["obj"].get<std::string>();
        cmd += " ";
    }

    if (!cfg["link"].empty())
    {
        cmd += "/link ";

        if (!cfg["link"]["libpath"].empty())
        {
            cmd += "/LIBPATH:";
            cmd += cfg["link"]["libpath"].get<std::string>();
            cmd += " ";
        }

        if (!cfg["link"]["libs"].empty())
            for (auto it{cfg["link"]["libs"].begin()}; it != cfg["link"]["libs"].end(); ++it)
            {
                cmd += it->get<std::string>();
                cmd += " ";
            }

        if (!cfg["link"]["linkargs"].empty())
            for (auto it{cfg["link"]["linkargs"].begin()}; it != cfg["link"]["linkargs"].end(); ++it)
            {
                cmd += it->get<std::string>();
                cmd += " ";
            }
    }
    std::cout << cmd << std::endl;
    system(cmd.c_str());

    if (!cfg["after"].empty())
        for (auto it{cfg["after"].begin()}; it != cfg["after"].end(); ++it)
            system(it->get<std::string>().c_str());
}

void App::Quit()
{
    data.clear();
    cmd.clear();
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Incorrect number of arguments passed!";
        return -1;
    }

    App builder{};
    auto arg{std::string(argv[1])};
    builder.Run(arg);
    builder.Quit();
    return 0;
}