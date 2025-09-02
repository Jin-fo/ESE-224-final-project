#include "class/company.cpp"
#include <limits>
 //path to the folder where all csv surgery, tickets, and test files are stored(make sure you only have csv files)
#define HOSPITAL_DIR "C:\\Users\\jinyc\\OneDrive\\Documents\\ESE224\\final-project-jin_project\\Build\\surgeon_db\\"
#define STATION_DIR "C:\\Users\\jinyc\\OneDrive\\Documents\\ESE224\\final-project-jin_project\\Build\\powerline_db\\"
#define TEST_DIR "C:\\Users\\jinyc\\OneDrive\\Documents\\ESE224\\final-project-jin_project\\Build\\test_tickets\\"
Company company;

/******************************************************************************
 * System Initialization Functions
 ******************************************************************************/
void initization() {
    try {
        Station EB = Station("EB", 0.124);
        Hospital H1 = Hospital("H1", 100);

        Station HD = Station("HD", 0.248);
        Hospital H2 = Hospital("H2", 100);

        Station OF = Station("OF", 0.099);
        Hospital H3 = Hospital("H3", 100);

        Station RL = Station("RL", 0.33);
        Hospital H4 = Hospital("H4", 100);

        Station TM = Station("TM", 0.199);
        Hospital H5 = Hospital("H5", 100);

        company.connect(EB, H1);
        company.connect(OF, H2);
        company.connect(RL, H3);
        company.connect(TM, H4);
        company.connect(HD, H5);
    }
    catch (const exception& e) {
        cout << "\nError: " << e.what() << "\n";
    }
}

/******************************************************************************
 * File and Data Loading Functions
 ******************************************************************************/
void upload_file() {
    try {
        for (int i = 0; i < int(company.nets.size()); i++) {
            company.nets.at(i).station.set_file_path(STATION_DIR);
            company.nets.at(i).hospital.set_file_path(HOSPITAL_DIR);
            
            cout << "\n==== " << company.nets.at(i).station.get_name() << " FILED TICKET ====\n" << endl;
            company.nets.at(i).station.file_ticket();
            cout << "\n==== " << company.nets.at(i).hospital.get_name() << " FILED TEAM ====\n" << endl;
            company.nets.at(i).hospital.file_team();   
        }
        cout << "\n==== TEST TICKET ====" << endl;
        company.set_test_folder_path(TEST_DIR);
        company.file_test_ticket();
    }
    catch (const exception& e) {
        cout << "\nError: " << e.what() << "\n";
    }
}

/******************************************************************************
 * Configuration and Parameter Setting Functions
 ******************************************************************************/
void set_parameter(string parameter) {
    string TOT_str, CAP_str, K_str, Y_str, Z_str;
    try {
        istringstream iss(parameter);
        getline(iss, TOT_str, ',');
        getline(iss, CAP_str, ',');
        getline(iss, K_str, ',');
        getline(iss, Y_str, ',');
        getline(iss, Z_str, ',');
        company.set_parameter(stoi(TOT_str), stoi(CAP_str), stoi(K_str), stoi(Y_str), stoi(Z_str)); 
    }
    catch (const exception& e) {
        cout << "\nError: " << e.what() << "\n";
    }
}

/******************************************************************************
 * Search and Analysis Functions
 ******************************************************************************/
void main_sub_search(int station_index, int num = 0) {
     string structure, voltage, ground, msp, harmonic, v_non_shunt, status;

    int y_temp = company.get_Y();
    if (num > 0) {
        company.set_Y(num);
    }

    cout << "\n==== Status Value Search ====\n"
         << "Enter status values for each field (leave blank to ignore that field).\n"
         << "The system will find the top " << company.get_Y() << " tickets with the most matching fields.\n\n";

    cout << "STRUCTURE: ";
    getline(cin, structure);
    
    cout << "VOLTAGE: ";
    getline(cin, voltage);
    
    cout << "GROUND: ";
    getline(cin, ground);
    
    cout << "MSPLATE: ";
    getline(cin, msp);
    
    cout << "HARMONIC: ";
    getline(cin, harmonic);
    
    cout << "V NON-SHUNT: ";
    getline(cin, v_non_shunt);
    
    cout << "STATUS: ";
    getline(cin, status);
    
    Ticket temp_ticket;
    Status temp_status(structure, voltage, ground, msp, harmonic, v_non_shunt, status);
    temp_ticket.set_status(temp_status);
    
    if (station_index != -1) {
        //*********************************************************************** */ 
        // Station functionality, E: Identifies the Y most similar tickets for previous power outages at the same substation. Y 
        // is a parameter of your program.  
        //*********************************************************************** */
        cout << "\nSearching for top " << company.get_Y() << " most similar tickets...\n";
        company.nets.at(station_index).station.Y_search(temp_ticket);
        company.nets.at(station_index).station.display_Y_search();
    }
    else {
        //*********************************************************************** */
        // Station functionality, F: Identifies the Z most similar tickets for power outages at different substations. Z is a 
        // parameter of your program.
        //*********************************************************************** */
        cout << "\nSearching across all stations for top " << company.get_Z() << " most similar tickets...\n";
        company.Z_search(temp_ticket);
        company.display_Z_search();
    }
    company.set_Y(y_temp);
}

/******************************************************************************
 * Main Program Function
 ******************************************************************************/                    
int main() {
    //states and flags
    enum {REST, INITIALIZATION, SIMULATION, EXECUTION, EXE_STATION, SEARCH_TICKET, EXE_HOSPITAL, UPDATE, DONE};
    int state = REST;
    bool firstRun = true;
    
    //option inputs
    char input;

    //station inputs
    string station_name;

    //hospital inputs
    string hospital_name;
    string team_name;
    string member;
    string surgery_type;
    string start_date;
    string end_date;
    int difficulty;
    int time;

    //temporary variables
    string file_path;
    string parameter;
    int index;


    
    company.clear_nets();

    while (input != 'X') {
    
        system("cls");
        
        try {
            switch (state) {
                /******************************************************************************
                 * REST State: Initial/Reset State
                 ******************************************************************************/
                case REST:
                    if (!firstRun) {
                        company.clear_nets();
                    }
                    cout << "\n==== POWER SYSTEM SIMULATION ====\n"
                         << "---------------------------------\n"
                         << "    S | Start\n"
                         << "    X | Exit\n"
                         << "---------------------------------\n"
                         << ">> ";
                    cin >> input;
                    if (input == 'S') {
                        state = INITIALIZATION;
                    }
                    break;
                
                /******************************************************************************
                 * INITIALIZATION State: Setup Network Structure
                 ******************************************************************************/
                case INITIALIZATION:
                    initization();
                    state = SIMULATION;
                    break;
                
                /******************************************************************************
                 * SIMULATION State: Configure and Begin Simulation
                 ******************************************************************************/
                case SIMULATION:
                    if (firstRun) {
                        upload_file();
                        cout << "\n[o]"; system("pause");
                        firstRun = false;
                    }

                    system("cls");
                    cout << "\n==== SIMULATION PARAMETERS ====\n"
                         << "---------------------------------\n"
                         << "   Enter: TOT, CAP, K, Y, Z (comma separated)\n"
                         << "   Example: 100, 100, 10, 10, 10\n"
                         << "---------------------------------\n"
                         << ">> ";
                    
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, parameter);
                    set_parameter(parameter);

                    system("cls");
                    cout << "\n==== SIMULATION PARAMETERS ====\n"
                         << "---------------------------------\n"
                         << "   TOT: " << company.get_TOT() << "\n"
                         << "   CAP: " << company.get_CAP() << "\n"
                         << "   K: " << company.get_K() << "\n"
                         << "   Y: " << company.get_Y() << "\n"
                         << "   Z: " << company.get_Z() << "\n"
                         << "---------------------------------\n";
                    
                    company.allot_TOT();
                    
                    cout << "\n[o]"; system("pause");
                    state = EXECUTION;
                    break;
                
                /******************************************************************************
                 * EXECUTION State: Main Execution Menu
                 ******************************************************************************/
                case EXECUTION:
                    for (int i = 0; i < int(company.nets.size()); i++) {
                        cout << company.nets.at(i).station.get_name() << ", N:" << company.nets.at(i).station.get_TOT() << " | "
                             << company.nets.at(i).hospital.get_name() << ", CAP:" << company.nets.at(i).hospital.get_CAP() << endl;
                    }
                    cout << "\n==== EXECUTION REQUEST ====\n"
                         << "---------------------------------\n"
                         << "   1 | Station\n"
                         << "   2 | Hospital\n"
                         << "   - | -----------------\n"
                         << "   A | Quick Start\n"
                         << "   F | Forward\n"
                         << "   X | Exit\n"
                         << "---------------------------------\n"
                         << ">> ";
                    cin >> input;
                    if (input == '1') {
                        state = EXE_STATION;
                    }
                    else if (input == '2') {
                        state = EXE_HOSPITAL;
                    }
                    else if (input == 'A') {
                        for (int i = 0; i < int(company.nets.size()); i++) {
                            //induce outage thats 2 times of each station's capacity in TOT.
                            for (int j = 0; j < int(company.nets.at(i).station.get_TOT())*2; j++) {
                                file_path = company.select_random_file();
                                company.nets.at(i).station.set_state("START");
                                //*********************************************************************** */
                                //Station functionality, A: Adds a new ticket for a new power outage including the information shown in Figure 2
                                //*********************************************************************** */
                                company.nets.at(i).station.file_test(file_path);
                                company.nets.at(i).station.set_flag(false);
                            }
                        }
                        state = UPDATE;
                    }
                    else if (input == 'F') {
                        state = UPDATE;
                    }
                    break;
                
                /******************************************************************************
                 * EXE_STATION State: Station Operations Menu
                 ******************************************************************************/
                case EXE_STATION:
                    cout << "\n==== STATION EXECUTIBLE ====\n"
                         << "---------------------------------\n"
                         << "   I | Induce Outage\n"
                         << "  -- | -------------\n"
                         << "   1 | Display Crew\n"
                         << "   2 | Display Passive\n"
                         << "   3 | Display Active\n"
                         << "   4 | Display Archive\n"
                         << "  -- | -------------\n"
                         << "   5 | Search a Ticket\n"
                         << "   6 | Search by Range of Date\n"
                         << "   7 | Identify Y/Z Most Similar\n"
                         << "   8 | Identify K Frequent Words\n"
                         << "  -- | -------------\n"
                         << "   B | Back\n"
                         << "---------------------------------\n"
                         << ">> ";
                    cin >> input;
                    if (input == 'I') {
                        //*********************************************************************** */
                        //Station functionality, A: Adds a new ticket for a new power outage including the information shown in Figure 2
                        //*********************************************************************** */
                        cout << "\n==== Select Station ====\n"
                             << "---------------------------------\n"
                             << "   {EB, HD, OF, RL, TM}\n"
                             << "---------------------------------\n"
                             << ">> "; cin >> station_name;
                        index = company.select_station(station_name);
                        if (index == -1) {
                            cout << "\nError: Station '" << station_name << "' not found\n";
                            system("pause");
                        }
                        else {
                            file_path = company.select_random_file();
                            company.nets.at(index).station.file_test(file_path);
                            company.nets.at(index).station.set_flag(false);
                        }
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == '1') {
                        cout << "\n==== Select Station ====\n"
                        << "---------------------------------\n"
                        << "   {EB, HD, OF, RL, TM}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> station_name;
                        index = company.select_station(station_name);
                        company.nets.at(index).station.display_crew();
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == '2') {
                        cout << "\n==== Select Station ====\n"
                        << "---------------------------------\n"
                        << "   {EB, HD, OF, RL, TM}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> station_name;
                        index = company.select_station(station_name);
                        company.nets.at(index).station.display_passive();
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == '3') {
                        cout << "\n==== Select Station ====\n"
                        << "---------------------------------\n"
                        << "   {EB, HD, OF, RL, TM}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> station_name;
                        index = company.select_station(station_name);
                        company.nets.at(index).station.display_active();
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == '4') {
                        cout << "\n==== Select Station ====\n"
                        << "---------------------------------\n"
                        << "   {EB, HD, OF, RL, TM}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> station_name;
                        index = company.select_station(station_name);
                        company.nets.at(index).station.display_archive();
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == '5') {
                        //*********************************************************************** */
                        // Station functionality, C: Displays the entire content of the ticket for a specific outage; set Y = 1  
                        //*********************************************************************** */
                        cout << "\n==== Select Station ====\n"
                        << "---------------------------------\n"
                        << "   {EB, HD, OF, RL, TM, ALL}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> station_name;
                        index = company.select_station(station_name);
                        if (index == -1) {
                            cout << "[o]Search across all stations" << endl;
                        }                       
                        else {
                            cout << "[o]Search within station: " << station_name << endl;
                        }
                        
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        main_sub_search(index, 1);
                        cout << "\n[o]"; system("pause");
                    }

                    else if (input == '6') {
                        //*********************************************************************** */
                        // Station functionality, D: Displays all power outages that occurred at a station occurred within a certain period of 
                        // time described by a starting date and an end date.
                        //*********************************************************************** */
                        cout << "\n==== Select Station ====\n"
                        << "---------------------------------\n"
                        << "   {EB, HD, OF, RL, TM}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> station_name;
                        index = company.select_station(station_name);
                        if (index == -1) {
                            cout << "\nError: Station '" << station_name << "' not found\n";
                            system("pause");
                        }
                        else {
                            string date, date2; 
                            bool flag = false;
                            Date start_date_obj;
                            Date end_date_obj;
                            
                            while (flag == false) {
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                system("cls");
                                cout << "\n==== DATE RANGE SEARCH ====\n"
                                    << "Station: " << station_name << "\n"
                                    << "Format: YYYY-MM-DD or YYYY_MM_DD HH:MM:SS\n"
                                    << "Date From: ";
                                getline(cin, date);
                                
                                try {
                                    start_date_obj = Date(date);
                                    flag = true;
                                } catch (const exception& e) {
                                    cout << "\nError: " << e.what() << "\n";
                                    cout << "Please try again with a valid date format.\n";
                                    system("pause");
                                }
                            }
                            flag = false;
                            while (flag == false) { 
                                cout << "Date To: ";
                                getline(cin, date2);
                                
                                try {
                                    end_date_obj = Date(date2);
                                    if (end_date_obj.get_year() < start_date_obj.get_year() || 
                                        (end_date_obj.get_year() == start_date_obj.get_year() && 
                                        end_date_obj.get_month() < start_date_obj.get_month()) || 
                                        (end_date_obj.get_year() == start_date_obj.get_year() && 
                                        end_date_obj.get_month() == start_date_obj.get_month() && 
                                        end_date_obj.get_day() < start_date_obj.get_day())) {
                                        throw runtime_error("End date must be after start date");
                                    }
                                    
                                    flag = true;
                                } catch (const exception& e) {
                                    cout << "\nError: " << e.what() << "\n";
                                    cout << "Please try again with a valid date format.\n";
                                    system("pause");
                                }
                            }
                            
                            system("cls");
                            company.nets.at(index).station.display_archive(start_date_obj, end_date_obj);
                            cout << "\n[o]"; system("pause");
                        }
                    }
                    else if (input == '7') {
                        cout << "\n==== Select Station ====\n"
                            << "---------------------------------\n"
                            << "   {EB, HD, OF, RL, TM, ALL}\n"
                            << "---------------------------------\n"
                            << ">> "; cin >> station_name;
                        index = company.select_station(station_name);
                        if (index == -1) {
                            cout << "[o]Search across all stations" << endl;
                        }                       
                        else {
                            cout << "[o]Search within station: " << station_name << endl;
                        }
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        //*********************************************************************** */ 
                        // Station functionality, E: Identifies the Y most similar tickets for previous power outages at the same substation. Y 
                        // is a parameter of your program.  
                        //*********************************************************************** */
                        //*********************************************************************** */
                        // Station functionality, F: Identifies the Z most similar tickets for power outages at different substations. Z is a 
                        // parameter of your program.
                        //*********************************************************************** */
                        main_sub_search(index, 0);
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == '8') {
                        //*********************************************************************** */
                        // Station functionality, G: Displays the K most frequent words that occurred in the comments column of the tickets 
                        // issued for a specific substation. K is a parameter of your program.
                        //*********************************************************************** */
                        cout << "\n==== Select Station ====\n"
                             << "---------------------------------\n"
                             << "   {EB, HD, OF, RL, TM}\n"
                             << "---------------------------------\n"
                             << ">> "; cin >> station_name;
                        index = company.select_station(station_name);
                        if (index == -1) {
                            cout << "\nError: Station '" << station_name << "' not found\n";
                            system("pause");
                        }
                        else {
                            system("cls");
                            cout << "\n==== TOP " << company.get_K() << " FREQUENT WORDS IN " << station_name << " ====\n";
                            company.nets.at(index).station.K_words(company.get_K());
                            cout << "\n[o]"; system("pause");
                        }
                    }
                    else if (input == 'B') {
                        state = EXECUTION;
                    }
                    break;
                
                /******************************************************************************
                 * EXE_HOSPITAL State: Hospital Operations Menu
                 ******************************************************************************/
                case EXE_HOSPITAL:
                    cout << "\n==== HOSPITAL EXECUTIBLE ====\n"
                         << "---------------------------------\n"
                         << "   C | Completed Surgery\n"
                         << "  -- | -------------\n"
                         << "   1 | Add to Team\n"
                         << "   2 | Remove from Team\n"
                         << "  -- | -------------\n"
                         << "   3 | Display All Teams\n"
                         << "   4 | Search a Team\n"
                         << "   5 | Search by Range of Date\n"
                         << "   6 | Rank All Teams\n"
                         << "  -- | -------------\n"
                         << "   B | Back\n"
                         << "---------------------------------\n"
                         << ">> ";
                    cin >> input;
                    if (input == 'C') {
                        //*********************************************************************** */
                        // Hospital functionality, E: Adds a completed surgery to a specific team. The information includes the surgery time 
                        // in minutes and the difficulty of the surgery (in points, 1 being the easiest and 10 the most 
                        // difficult surgery).  
                        //*********************************************************************** */
                        cout << "\n==== Select Hospital ====\n"
                        << "---------------------------------\n"
                        << "   {H1, H2, H3, H4, H5}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> hospital_name;
                        index = company.select_hospital(hospital_name);
                        cout << "\n==== Status Value Search ====\n"
                             << "Enter values for a complete surgery." << endl;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Team Name: ";
                        getline(cin, team_name);
                        
                        cout << "Surgery Type: ";
                        getline(cin, surgery_type);
                        
                        cout << "Start Date(YYYY-MM-DD): ";
                        getline(cin, start_date);
                        
                        cout << "End Date(YYYY-MM-DD): ";
                        getline(cin, end_date);
                        
                        cout << "Difficulty (1-10): ";
                        cin >> difficulty;
                        
                        cout << "Time(minutes): ";
                        cin >> time;
                        
                        company.nets.at(index).hospital.add_complete_surgery(team_name, surgery_type, start_date, end_date, difficulty, time);
                        company.nets.at(index).hospital.display_team(team_name);
                        company.nets.at(index).station.set_flag(false);
                        cout << "\n[o]"; system("pause");
                    }
                    if (input == '1') {
                        //*********************************************************************** */
                        //Hospital functionality, A: Adds a new team of surgeons to a hospital.  
                        //*********************************************************************** */
                        cout << "\n==== Select Hospital ====\n"
                        << "---------------------------------\n"
                        << "   {H1, H2, H3, H4, H5}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> hospital_name;
                        index = company.select_hospital(hospital_name);
                        if (index == -1) {
                            cout << "\nError: Hospital '" << hospital_name << "' not found\n";
                        }
                        else {
                            cout << "Enter team name: "; 
                            cin >> team_name;
                            Team* team = company.nets.at(index).hospital.search_team(team_name);
                            if (team == nullptr) {
                                company.nets.at(index).hospital.add_team(team_name);
                            } 
                            else {
                                cout << "\nAdding to existing team: '" << team_name << "'\n";
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            //*********************************************************************** */
                            //Hospital functionality, C: Adds a new surgeon to the team. 
                            //*********************************************************************** */
                            while (true) {
                                cout << "Member (or press Enter to finish): ";
                                getline(cin, member);
                                if (member.empty()) break;
                                company.nets.at(index).hospital.add_member(team_name, member);
                            }
                        company.nets.at(index).hospital.display_team(team_name);
                        company.nets.at(index).station.set_flag(false);
                        }
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == '2') {
                        //*********************************************************************** */
                        //Hospital functionality, B: Removes a team of surgeons from a hospital. 
                        //*********************************************************************** */
                        cout << "\n==== Select Hospital ====\n"
                        << "---------------------------------\n"
                        << "   {H1, H2, H3, H4, H5}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> hospital_name;
                        index = company.select_hospital(hospital_name);
                        if (index == -1) {
                            cout << "\nError: Hospital '" << hospital_name << "' not found\n";
                            system("pause");
                        }
                        else {
                            string team_name;
                            cout << "Enter team name: ";
                            cin >> team_name;
                            Team* team = company.nets.at(index).hospital.search_team(team_name);
                            if (team == nullptr) {
                                cout << "\nError: Team '" << team_name << "' not found\n";
                            }
                            else {
                                company.nets.at(index).hospital.display_team(team);
                                cout << "\n==== Remove Option ====\n"
                                     << "---------------------------------\n"
                                     << " 1 | Remove member from team \n"
                                     << " 2 | Remove whole team \n"
                                     << "---------------------------------\n"
                                     << ">> "; cin >> input;
                                if (input == '1') {
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                //*********************************************************************** */
                                //Hospital functionality, D: Removes a surgeon from a team. 
                                //*********************************************************************** */
                                    while (true) {
                                        cout << "Enter member name(press Enter to finish): ";
                                        
                                        getline(cin, member);
                                        if (member.empty()) break;
                                        company.nets.at(index).hospital.remove_member(team_name, member);
                                    }
                                }
                                else if (input == '2') {
                                    //*********************************************************************** */
                                    //Hospital functionality: Remove whole team
                                    //*********************************************************************** */
                                    company.nets.at(index).hospital.remove_team(team_name);
                                }
                            }
                            company.nets.at(index).station.set_flag(false);
                            cout << "\n[o]"; system("pause");
                        }
                    }
                    else if (input == '3') {
                        for (int i = 0; i < int(company.nets.size()); i++) {
                            company.nets.at(i).hospital.display_teams();
                        }
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == '4') {
                        //*********************************************************************** */
                        //hospital functionality, F: Displays all surgeries performed by a team as well as its average number of points per 
                        //hour earned by the team.
                        //*********************************************************************** */
                        cout << "\n==== Select Hospital ====\n"
                        << "---------------------------------\n"
                        << "   {H1, H2, H3, H4, H5}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> hospital_name;
                        index = company.select_hospital(hospital_name);
                        if (index == -1) {
                            cout << "\nError: Hospital '" << hospital_name << "' not found\n";
                            system("pause");
                        }
                        else {
                            cout << "Enter team name: ";
                            cin >> team_name;
                            company.nets.at(index).hospital.display_team(team_name);
                        }
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == '5') {
                        //*********************************************************************** */
                        //hospital functionality, G: Displays all surgeries performed at a hospital during a period of time identified by a start 
                        //date and an end date here   
                        //*********************************************************************** */
                        cout << "\n==== Select Hospital ====\n"
                        << "---------------------------------\n"
                        << "   {H1, H2, H3, H4, H5}\n"
                        << "---------------------------------\n"
                        << ">> "; cin >> hospital_name;
                        index = company.select_hospital(hospital_name);
                        if (index == -1) {
                            cout << "\nError: Hospital '" << hospital_name << "' not found\n";
                            system("pause");
                        }
                        else {
                            cout << "Enter start date(DD-MM-YYYY): ";
                            cin >> start_date;
                            cout << "Enter end date(DD-MM-YYYY): ";
                            cin >> end_date;
                            company.nets.at(index).hospital.display_surgery(start_date, end_date);
                        }
                        cout << "\n[o]"; system("pause");
                        
                    }
                    else if (input == '6') {
                        //*********************************************************************** */
                        // Hospital functionality H: Displays all teams at the five hospitals
                        // in the order of their average number of points per hour.
                        //*********************************************************************** */
                        cout << "\n==== RANKING TEAMS BY POINTS/HOUR ====\n"
                             << "This function displays all surgical teams across all hospitals\n"
                             << "ranked by their average number of points per hour.\n\n";
                        company.team_search();
                        company.display_team();
                        cout << "\n[o]"; system("pause");
                    }
                    else if (input == 'B') {
                        state = EXECUTION;
                    }
                    break;
                
                /******************************************************************************
                 * UPDATE State: Run Simulation Step
                 ******************************************************************************/
                case UPDATE:
                    {
                        cout << "\n==== UPDATE RESULT====\n"
                             << "---------------------------------\n"
                             << endl;
                        company.update_event();
                        cout << "\n[o]"; system("pause");
                        state = EXECUTION;
                        break;
                    }
                
                /******************************************************************************
                 * DONE State: Exit Program
                 ******************************************************************************/
                case DONE:
                    return 0;
            }
        }
        catch (const exception& e) {
            cout << "\nError: " << e.what() << "\n";
        }
    }
    return 0;
}    

