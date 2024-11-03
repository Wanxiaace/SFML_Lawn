#include <iostream>
#include "GameApp.h"
#include "GamePacker/GamePacker.h"

#undef main

const char* gListDir = "F:/VSProjects/SFML_Lawn/SFML_Lawn/Release/assets/en2pak/";
const char* gPathBase = "assets/en2pak/";


using namespace std;
using namespace sgf;

int main(int argc,char** argv)
{
#ifndef _DEBUG
    if (argc < 3)
    {
        std::cout << "Packer [ResourceList Dir] [BasePath]" << std::endl;
        return EXIT_FAILURE;
    }
    else {
        gListDir = argv[2];
        gPathBase = argv[3];
    }
#endif // !__DEBUG

    ResourceList list;
    GamePacker pak;
    list.Load((std::string(gListDir) + "ResourceList.xml").c_str());

    for (auto& x : list.mResouces)
    {
        std::cout << gPathBase << x.path << std::endl;
        std::cout << gListDir << x.path << std::endl;

        FileReadStream stream;
        stream.OpenFile(gListDir + x.path);
        int size = stream.GetSize();
        stream.Seek(FileReadStream::CURSOR_SET,0);
        char* buf = new char[size];
        stream.Read(buf, size);
        pak.AppendNewFile(gPathBase + x.path, buf, size);
        stream.Close();
    }
    pak.WriteToFile("out.pkg");

    return EXIT_SUCCESS;
}
