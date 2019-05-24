#include "exerciser.h"

void exercise(connection *C) {
    // no problem with testing
    std::cout << "Info: query1" << std::endl;
    query1(C,
           1,35,40,
           0,0,0,
           0,0,0,
           0,0,0,
           0,0,0,
           0,0,0);
    std::cout << "Info: query2" << std::endl;
    query2(C, "Orange");
    std::cout << "Info: query3" << std::endl;
    query3(C, "Duke");
    std::cout << "Info: query4" << std::endl;
    query4(C, "NC", "LightBlue");
    std::cout << "Info: query5" << std::endl;
    query5(C, 11);
}
