#include "options.h"
#include <argp.h>
#include <sstream>
#include <iostream>

namespace{
    error_t parse_options(int key, char* arg, struct argp_state* state){
        Options& options = *reinterpret_cast<Options*>(state->input);
        std::stringstream stream;
        stream << arg;
        switch(key){
            case ARGP_KEY_END:
                if(state->arg_num < 2){
                    std::cout << "Too few arguments." << std::endl;
                    argp_usage(state);
                }
                break;
            case ARGP_KEY_ARG:
                switch(state->arg_num){
                    case 0:
                        stream >> options.students;
                        if(!(options.students > 0)){
                            std::cout << "Invalid amount of students." << std::endl;
                            argp_usage(state);
                        }
                        break;
                    case 1:
                        stream >> options.event;
                        break;
                    default:
                        std::cout << "Too many arguments." << std::endl;
                        argp_usage(state);
                }
                break;
            default:
                return ARGP_ERR_UNKNOWN;
        }
        return 0;
    }
}
const char* argp_program_version = "team-divide 0.0.1";
const char* argp_program_bug_address = "auxchar@gmail.com";
const std::string args_doc = "STUDENTS EVENT_KEY";
const std::string doc = "Divides teams from a regional up such that no student has two teams to watch in a match at the same time.";

Options get_options(int argc, char** argv){
    Options options;
    struct argp argp = {0, parse_options, args_doc.c_str(), doc.c_str(), 0, 0, 0};
    argp_parse(&argp, argc, argv, 0, 0, &options);
    return options;
}
