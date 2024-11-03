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

//int main(int argc, char** argv) {
//    sgf::TryToLoadPak("assets/en2pak/en2pak.pkg");
//    auto file = sgf::TryToLoadFile("assets/en2pak/raxml/PeaShooter.reanim");
//
//    std::cout << file->ReadString(file->GetSize()) << std::endl;
//
//    return EXIT_SUCCESS;
//}
