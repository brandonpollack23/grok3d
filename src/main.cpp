/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "../inc/movebackandforthtest.h"

#include <iostream>

auto PrintUsageAndExit() -> int
{
    std::cout << "Specify a test number to run:\n\
        1: MoveBackAndForth and delete" << std::endl;
    return 0;
}

auto main(int argc, char* argv[]) -> int
{
    std::cout << "Running Grok3d tests" << std::endl;

    if(argc < 2)
    {
        return PrintUsageAndExit();
    }
    
    switch(*argv[1])
    {
    case '1':
        CreationMovementDeleteTest();
        break;
    default:
        std::cout << "no such test: " << argv[1] << std::endl;
        break;
    }

    return 0;
}
