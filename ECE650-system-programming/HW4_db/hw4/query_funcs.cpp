#include "query_funcs.h"

/* some references */
// to avoid SQL injection in a string, see https://libpqxx.readthedocs.io/en/latest/a01485.html

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg) {
    work W(*C);
    std::string add_player_SQL = 
    "INSERT INTO PLAYER (TEAM_ID,UNIFORM_NUM,FIRST_NAME,LAST_NAME,MPG,PPG,RPG,APG,SPG,BPG) "
    "VALUES (" + to_string(team_id) + ", " + to_string(jersey_num) + ", '" + W.esc(first_name) + "', '" + W.esc(last_name) + "', " + to_string(mpg) + ", " + to_string(ppg) + ", " + to_string(rpg) + ", " + to_string(apg) + ", " + to_string(spg) + ", " + to_string(bpg) +");";
    try {
        W.exec(add_player_SQL);
        W.commit();
    } 
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in add_player func.\n";
        throw err_msg;
    }
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses) {
	work W(*C);
	std::string add_team_SQL = 
	"INSERT INTO TEAM (NAME,STATE_ID,COLOR_ID,WINS,LOSSES) "
	"VALUES ('" + W.esc(name) + "', " + to_string(state_id) + ", " + to_string(color_id) + ", " + to_string(wins) + ", " + to_string(losses) + ");";
  	try {
        W.exec(add_team_SQL);
        W.commit();
    } 
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in add_team func.\n";
        throw err_msg;
    }
}


void add_state(connection *C, string name) {
	work W(*C);
	std::string add_state_SQL = 
	"INSERT INTO STATE (NAME) "
	"VALUES ('" + W.esc(name) + "');";
	try {
        W.exec(add_state_SQL);
        W.commit();
    } 
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in add_state func.\n";
        throw err_msg;
    }
}


void add_color(connection *C, string name) {
	work W(*C);
	std::string add_color_SQL = 
	"INSERT INTO COLOR (NAME) "
	"VALUES ('" + W.esc(name) + "');";
    try {
        W.exec(add_color_SQL);
        W.commit();
    } 
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in add_color func.\n";
        throw err_msg;
    }
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            ) {
    nontransaction N(*C);
    std::string query1_SQL = 
    "SELECT * "
    "FROM PLAYER "
    "WHERE";
    bool is_valid = false;
    if (use_mpg != 0) {
        query1_SQL += " MPG <= " + to_string(max_mpg) + " AND " + "MPG >= " + to_string(min_mpg)+" AND ";
        is_valid = true;
    }
    if (use_ppg != 0) {
        query1_SQL += " PPG <= " + to_string(max_ppg) + " AND " + "PPG >= " + to_string(min_ppg)+" AND ";
        is_valid = true;
    }
    if (use_rpg != 0) {
        query1_SQL += " RPG <= " + to_string(max_rpg) + " AND " + "RPG >= " + to_string(min_rpg)+" AND ";
        is_valid = true;
    }
    if (use_apg != 0) {
        query1_SQL += " APG <= " + to_string(max_apg) + " AND " + "APG >= " + to_string(min_apg)+" AND ";
        is_valid = true;
    }
    if (use_spg != 0) {
        query1_SQL += " SPG <= " + to_string(max_spg) + " AND " + "SPG >= " + to_string(min_spg)+" AND ";
        is_valid = true;
    }
    if (use_bpg != 0) {
        query1_SQL += " BPG <= " + to_string(max_bpg) + " AND " + "BPG >= " + to_string(min_bpg)+" AND ";
        is_valid = true;
    }
    query1_SQL += "TRUE;";  // serve as a placeholder for previous conditions  // ORDER BY MPG DESC, PLAYER_ID ASC
    if (!is_valid) {
        // invalid where clause, just return nothing
        return;
    }
    result res;
    try {
       res = N.exec(query1_SQL);
    } 
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in query1 func.\n";
        throw err_msg;
    }
    std::cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << std::endl;
    for (result::const_iterator row = res.begin(); row != res.end(); row++) {
        for (pqxx::tuple::const_iterator field = row.begin(); field != row.end(); field++) {
            std::cout << field.c_str() << " ";
        }
        std::cout << std::endl;
    }
}


void query2(connection *C, string team_color) {
    nontransaction N(*C);
    std::string query2_SQL = 
    "SELECT TEAM.NAME "
    "FROM TEAM, COLOR "
    "WHERE TEAM.COLOR_ID = COLOR.COLOR_ID AND COLOR.NAME = '" + N.esc(team_color) + "';";
    result res;
    try {
       res = N.exec(query2_SQL);
    } 
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in query2 func.\n";
        throw err_msg;
    }
    // https://libpqxx.readthedocs.io/en/latest/a01484.html
    // not sure if using NAME is ok here as the header
    std::cout << "NAME" << std::endl;
    for (result::const_iterator row = res.begin(); row != res.end(); row++) {
        std::cout << row[0].c_str() << std::endl;
    }
}


void query3(connection *C, string team_name) {
    nontransaction N(*C);
    std::string query3_SQL = 
    "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME "
    "FROM TEAM, PLAYER "
    "WHERE TEAM.TEAM_ID = PLAYER.TEAM_ID AND TEAM.NAME = '" + N.esc(team_name) + 
    "' ORDER BY PPG DESC;";
    result res;
    try {
       res = N.exec(query3_SQL);
    } 
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in query3 func.\n";
        throw err_msg;
    }
    std::cout << "FIRST_NAME LAST_NAME" << std::endl;
    for (result::const_iterator row = res.begin(); row != res.end(); row++) {
        std::cout << row[0].c_str() << " " << row[1].c_str() << std::endl;
    }
}


void query4(connection *C, string team_state, string team_color) {
    nontransaction N(*C);
    std::string query4_SQL = 
    "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, PLAYER.UNIFORM_NUM "
    "FROM TEAM, PLAYER, STATE, COLOR "
    "WHERE TEAM.TEAM_ID = PLAYER.TEAM_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID AND TEAM.STATE_ID = STATE.STATE_ID AND STATE.NAME = '" + N.esc(team_state) + "' AND COLOR.NAME = '" + N.esc(team_color) + "';";
    result res;
    try {
       res = N.exec(query4_SQL);
    } 
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in query4 func.\n";
        throw err_msg;
    }
    std::cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" << std::endl;
    for (result::const_iterator row = res.begin(); row != res.end(); row++) {
        std::cout << row[0].c_str() << " " << row[1].c_str() << " " << row[2].c_str() << std::endl;
    }
}


void query5(connection *C, int num_wins) {
    nontransaction N(*C);
    std::string query5_SQL = 
    "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS "
    "FROM TEAM, PLAYER "
    "WHERE TEAM.TEAM_ID = PLAYER.TEAM_ID AND TEAM.WINS > " + to_string(num_wins) + ";";
    result res;
    try {
       res = N.exec(query5_SQL);
    } 
    catch (const pqxx::pqxx_exception & e) {
        std::cerr << e.base().what() << std::endl;
        std::string err_msg = "Error: in query5 func.\n";
        throw err_msg;
    }
    std::cout << "FIRST_NAME LAST_NAME NAME WINS" << std::endl;
    for (result::const_iterator row = res.begin(); row != res.end(); row++) {
        std::cout << row[0].c_str() << " " << row[1].c_str() << " " << row[2].c_str() << " " << row[3].c_str() << std::endl;
    }
}
