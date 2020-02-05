#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Please pass the name of the header file you wish to create the module from. ";
        return -1;
    }

    std::string ret{"cl /EHsc /W4 /std:c++latest /experimental:module /module:exportHeader"};
    ret += argv[1];
    ret += ".hpp";
    ret += " /Fo";
    ret += argv[1];
    ret += ".obj";

    system(ret.data());

    return 0;
}