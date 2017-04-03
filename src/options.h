#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
struct Options{
    unsigned int students;
    std::string event;
};

Options get_options(int argc, char** argv);

#endif
