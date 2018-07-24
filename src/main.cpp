/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "movebackandforthtest.h"
#include "helloshapes.h"

#include <iostream>

auto PrintUsageAndExit() -> int {
  std::cout << "Specify a test number to run:\n\
        1: MoveBackAndForth and delete\n\
        2: Render a triangle\n\
        3: Render a square"
            << std::endl;
  return 0;
}

auto main(int argc, char *argv[]) -> int {
  std::cout << "Running Grok3d tests from directory " << std::endl;

  if (argc < 2) {
    return PrintUsageAndExit();
  }

  switch (*argv[1]) {
    case '1':
      CreationMovementDeleteTest();
      break;
    case '2':
      if (argc < 4) {
        std::cout << "Triangle test requires a vertex and frag shader passed as arguments 2 and 3" << std::endl;
        return -1;
      } else {
        HelloTriangleTest(argv);
      }
      break;
    case '3':
      if (argc < 4) {
        std::cout << "Square test requires a vertex and frag shader passed as arguments 2 and 3" << std::endl;
        return -1;
      } else {
        HelloSquareTest(argv);
      }
      break;
    default:
      std::cout << "no such test: " << argv[1] << std::endl;
      break;
  }

  return 0;
}
