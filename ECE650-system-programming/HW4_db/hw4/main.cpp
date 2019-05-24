#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <pqxx/pqxx>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

/* just write helper functions here */
void drop_tables(connection *C) {
    std::string drop_tables_SQL = "DROP TABLE IF EXISTS PLAYER, TEAM, STATE, COLOR;";

    work W(*C);
    try {
        W.exec(drop_tables_SQL);
        W.commit();
    }
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in drop_tables func.\n";
        throw err_msg;
    }
    return;
}

void add_tables(connection *C) {
    // PLAYER
    std::string create_table_PLAYER_SQL =
        "CREATE TABLE PLAYER ("
        "PLAYER_ID    SERIAL            NOT NULL,"
        "TEAM_ID      SERIAL            NOT NULL,"
        "UNIFORM_NUM  INT               NOT NULL,"  // does it allow "00"?
        "FIRST_NAME   TEXT              NOT NULL,"
        "LAST_NAME    TEXT              NOT NULL,"
        "MPG          INT               NOT NULL,"
        "PPG          INT               NOT NULL,"
        "RPG          INT               NOT NULL,"
        "APG          INT               NOT NULL,"
        "SPG          NUMERIC(4,1)      NOT NULL,"
        "BPG          NUMERIC(4,1)      NOT NULL,"
        "PRIMARY KEY (PLAYER_ID),"
        "FOREIGN KEY (TEAM_ID) REFERENCES TEAM (TEAM_ID));";
    // TEAM
    std::string create_table_TEAM_SQL =
        "CREATE TABLE TEAM ("
        "TEAM_ID      SERIAL            NOT NULL,"
        "NAME         TEXT              NOT NULL    UNIQUE,"
        "STATE_ID     SERIAL            NOT NULL,"
        "COLOR_ID     SERIAL            NOT NULL,"
        "WINS         INT               NOT NULL,"
        "LOSSES       INT               NOT NULL,"
        "PRIMARY KEY (TEAM_ID),"
        "FOREIGN KEY (STATE_ID) REFERENCES STATE (STATE_ID),"
        "FOREIGN KEY (COLOR_ID) REFERENCES COLOR (COLOR_ID));";
    // STATE
    std::string create_table_STATE_SQL =
        "CREATE TABLE STATE ("
        "STATE_ID     SERIAL            NOT NULL,"
        "NAME         TEXT              NOT NULL    UNIQUE,"
        "PRIMARY KEY (STATE_ID));";
    // COLOR
    std::string create_table_COLOR_SQL =
        "CREATE TABLE COLOR ("
        "COLOR_ID     SERIAL            NOT NULL,"
        "NAME         TEXT              NOT NULL    UNIQUE,"
        "PRIMARY KEY (COLOR_ID));";

    work W(*C);
    try {
        W.exec(create_table_COLOR_SQL);
        W.exec(create_table_STATE_SQL);
        W.exec(create_table_TEAM_SQL);
        W.exec(create_table_PLAYER_SQL);
        W.commit();
    }
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in add_tables func.\n";
        throw err_msg;
    }
    return;
}

// getline borrowed from my previous 551 hw
void load_player(connection *C) {
    std::ifstream input_file;
    try {
        input_file.open("player.txt");
    }
    catch (const std::exception & e) {
        throw e;
        return;
    }
    int player_id, team_id, uniform_num, mpg, ppg, rpg, apg;
    std::string first_name, last_name;
    double spg, bpg;
    std::string input;
    while(!std::getline(input_file, input).eof()) {
        if(input.empty()) {
            continue;
        }
        std::stringstream line(input);
        line >> player_id >> team_id >> uniform_num >> first_name >> last_name >> mpg >> ppg >> rpg >> apg >> spg >> bpg;
        try {
            add_player(C, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
        }
        catch (const pqxx::pqxx_exception & e) {
            std::cerr << e.base().what() << std::endl;
            std::string err_msg = "Error: in load_player func.\n";
            throw err_msg;
        }
    }
    input_file.close();
}

void load_team(connection *C) {
    std::ifstream input_file;
    try {
        input_file.open("team.txt");
    }
    catch (const std::exception & e) {
        throw e;
        return;
    }
    int team_id, state_id, color_id, wins, losses;
    std::string name;
    std::string input;
    while(!std::getline(input_file, input).eof()) {
        if(input.empty()) {
            continue;
        }
        std::stringstream line(input);
        line >> team_id >> name >> state_id >> color_id >> wins >> losses;
        try {
            add_team(C, name, state_id, color_id, wins, losses);
        }
        catch (const pqxx::pqxx_exception & e) {
            std::cerr << e.base().what() << std::endl;
            std::string err_msg = "Error: in load_team func.\n";
            throw err_msg;
        }
    }
    input_file.close();
}

void load_state(connection *C) {
    std::ifstream input_file;
    try {
        input_file.open("state.txt");
    }
    catch (const std::exception & e) {
        throw e;
        return;
    }
    int state_id;
    string name;
    std::string input;
    while(!std::getline(input_file, input).eof()) {
        if(input.empty()) {
            continue;
        }
        std::stringstream line(input);
        line >> state_id >> name;
        try {
            add_state(C, name);
        }
        catch (const pqxx::pqxx_exception & e) {
            std::cerr << e.base().what() << std::endl;
            std::string err_msg = "Error: in load_state func.\n";
            throw err_msg;
        }

    }
    input_file.close();
}

void load_color(connection *C) {
    std::ifstream input_file;
    try {
        input_file.open("color.txt");
    }
    catch (const std::exception & e) {
        throw e;
        return;
    }
    int color_id;
    string name;
    std::string input;
    while(!std::getline(input_file, input).eof()) {
        if(input.empty()) {
            continue;
        }
        std::stringstream line(input);
        line >> color_id >> name;
        try {
            add_color(C, name);
        }
        catch (const pqxx::pqxx_exception & e) {
            std::cerr << e.base().what() << std::endl;
            std::string err_msg = "Error: in load_color func.\n";
            throw err_msg;
        }
    }
    input_file.close();
}

void load_entries(connection *C) {
    try {
        load_state(C);
        load_color(C);
        load_team(C);
        load_player(C);
    }
    catch (const std::exception & e1) {
        throw e1;
    }
    catch (const std::string & err_msg) {
        throw err_msg;
    }
    catch (...) {
        std::cerr << "Error: loading entry unknown failure." << std::endl;
    }
}

/* main function below */
int main(int argc, char * argv[]) {

    //Allocate & initialize a Postgres connection object
    connection * C;

    try {
        //Establish a connection to the database
        //Parameters: database name, user name, user password
        C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
        if (C -> is_open()) {
            std::cout << "Opened database successfully: " << C -> dbname() << std::endl;
        }
        else {
            std::cout << "Can't open database" << std::endl;
            return 1;
        }
    }
    catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
    //      load each table with rows from the provided source txt files
    try {
        // 1. drop existing tables
        drop_tables(C);
        // 2. create new tables
        add_tables(C);
        // 3. read from text files and add to table
        load_entries(C);
    }
    catch (const std::exception & e1) {
        std::cerr << e1.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const pqxx::pqxx_exception & e2) {
        std::cerr << e2.base().what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::string & err_msg) {
        std::cerr << err_msg << std::endl;
    }
    catch (...) {
        std::cerr << "Error! Unknown failure. Abort." << std::endl;
        return EXIT_FAILURE;
    }

    exercise(C);
    //Close database connection
    C -> disconnect();
    delete(C);  // dont foget to delete to ensure valgrind memleak free!
    // valgrind --leak-check=full --show-leak-kinds=all ./test
    return EXIT_SUCCESS;
}
