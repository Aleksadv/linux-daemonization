#include "Daemon.hpp"
#include <iostream>


int main(int argc, char const *argv[])
{
   
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " <config.file>\n";
        return 1;
    }

    return Daemon::getInstance().run(argv[1])
}
