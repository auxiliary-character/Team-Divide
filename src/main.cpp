#include "jsonget.h"
#include "options.h"
#include "connection_matrix.h"
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

namespace{
    struct State{
        ConnectionMatrix& matrix;
        std::vector<unsigned int>& team_student_assignment;
        std::vector<unsigned int>& teams_per_student;
        Options& options;
        unsigned int teams;
    };

    struct StudentCompare{
        std::vector<unsigned int>& teams_per_student;
        bool operator()(int i, int j){
            return (teams_per_student[i] < teams_per_student[j]);
        }
    };

    bool fits(State& state, int unsigned team, unsigned int student){
        for(unsigned int i=0; i<state.teams; ++i){
            if((state.matrix.get(team,i)) && (state.team_student_assignment[i] == student && i!=team)){
                return false;
            }
        }
        return true;
    }

    bool assign(State& state, unsigned int team){
        std::vector<unsigned int> students(state.options.students);
        for(unsigned int i=0; i<state.options.students; ++i){
            students[i] = i;
        }
        StudentCompare comp = {state.teams_per_student};
        sort(students.begin(), students.end(), comp);

        for(auto&& student : students){
            if(fits(state, team, student + 1)){
                state.team_student_assignment[team] = student + 1;
                ++state.teams_per_student[student];
                return true;
            }
        }
        return false;
    }

    bool solve(State& state){
        for(unsigned int team=0; team < state.teams; ++team){
            if(!assign(state, team)){
                return false;
            }
        }
        return true;
    }
}

int main(int argc, char** argv){
    Options options = get_options(argc, argv);
    header_t headers;
    headers["x-tba-app-id"] = "auxchar:team-divide:0.0.1";
    Json::Value teams = json_get("https://www.thebluealliance.com/api/v2/event/" + options.event +"/teams", headers);

    std::map<std::string,unsigned int> team_index;
    ConnectionMatrix connection_matrix(teams.size());
    std::vector<unsigned int> team_student_assignment(teams.size());

    for(unsigned int i=0; i<teams.size(); ++i){
        auto team = teams[i];
        team_index[team["key"].asString()] = i;
    }

    Json::Value matches = json_get("https://www.thebluealliance.com/api/v2/event/" + options.event + "/matches", headers);
    std::vector<std::array<unsigned int, 6>> coincidental_teams;
    for(auto match : matches){
        if(match["comp_level"].asString() == "qm"){
            auto red = match["alliances"]["red"];
            auto blue = match["alliances"]["blue"];
            std::array<unsigned int, 6> row;
            for(unsigned int i=0; i<6; ++i){
                auto alliance = i<3 ? "red" : "blue";
                row[i] = team_index[match["alliances"][alliance]["teams"][i%3].asString()];
            }
            coincidental_teams.push_back(row);
        }
    }

    for(auto row : coincidental_teams){
        for(unsigned int i=0; i<6; ++i){
            for(unsigned int j=i+1; j<6; ++j){
                connection_matrix.set(row[i], row[j], true);
            }
        }
    }

    std::cout << "Connection Matrix:" << std::endl;
    for(unsigned int i=0; i<teams.size(); ++i){
        for(unsigned int j=0; j<teams.size(); ++j){
            std::cout << (connection_matrix.get(i,j) ? "1" : "0");
        }
        std::cout << std::endl;
    }

    std::vector<unsigned int> teams_per_student(options.students);
    State state = {connection_matrix, team_student_assignment, teams_per_student,  options, teams.size()};
    bool solved = solve(state);
    std::cout << (solved ? "Solved" : "Unsolved") << std::endl;
    for(unsigned int i=0; i<options.students+1; ++i){
        if(i>0){
            std::cout << "Student " << i << ":";
        }
        else{
            if(!solved){
                std::cout << "Unallocated:";
            }
        }
        for(unsigned int j=0; j<teams.size(); ++j){
            if(team_student_assignment[j] == i){
                std::cout << " " << teams[j]["team_number"].asInt();
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
