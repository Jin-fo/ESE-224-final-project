#include "_header.cpp"

/******************************************************************************
 * Team Class Definition
 * Represents a surgical team with members and surgery history
 ******************************************************************************/
class Team {
    private:
    
    struct Surgery {
        string type;
        string start_date;
        string end_date;
        int difficulty;
        int minutes;
        Surgery(string type, string start_date, string end_date, int difficulty, int minutes) {
            this->type = type;
            this->start_date = start_date;
            this->end_date = end_date;
            this->difficulty = difficulty;
            this->minutes = minutes;
        }
    };

    string name;
    vector<string> member;
    vector<Surgery> surgery;

    //priority attribute
    float efficiency;
    int total_points;
    float total_hours;
    
    // Analysis data structures
    struct SurgeryTypeData {
        string type;
        int frequency;
        float points;
        float duration;
        
        SurgeryTypeData(string t) {
            type = t;
            frequency = 0;
            points = 0;
            duration = 0;
        }
    };
    
    struct DifficultyData {
        int difficulty;
        float time;
        int count;
        
        DifficultyData(int diff) {
            difficulty = diff;
            time = 0;
            count = 0;
        }
    };

    public:
    /******************************************************************************
     * Team Constructors and Basic Operations
     ******************************************************************************/
    Team(string name, vector<string> member) {
        this->name = name;
        this->member = member;
        total_points = 0;
        total_hours = 0;
    }
    void add_member(string member) {
        this->member.push_back(member);
    }
    string get_name() const {
        return name;
    }
    void add_surgery(string type, string start_date, string end_date, int difficulty, int minutes) {
        surgery.push_back(Surgery(type, start_date, end_date, difficulty, minutes));
        average_points();
    }
    string operator*() const {
        string result = "";
        result += "\nTeam Members:\n";
        for(int i = 0; i < int(member.size()); i++) {
            result += "- " + member.at(i) + "\n";
        }
        result += "\nSurgeries:\n";
        result += "Surgery Type, Start Date, End Date, Difficulty, Minutes\n";
        for(int i = 0; i < int(surgery.size()); i++) {
            result += surgery.at(i).type + ", " + surgery.at(i).start_date + ", " + surgery.at(i).end_date + ", " + to_string(surgery.at(i).difficulty) + ", " + to_string(surgery.at(i).minutes) + "\n";
        }
        return result;
    }

    /******************************************************************************
     * Team Performance and Analysis Functions
     ******************************************************************************/
    void average_points() {
        total_points = 0;
        total_hours = 0;
        for (int i = 0; i < int(surgery.size()); i++) {
            total_points += surgery.at(i).difficulty;
            total_hours += surgery.at(i).minutes;
        }
        total_hours = total_hours/60;
        if (total_hours > 0) {
            efficiency = total_points/total_hours;
        } else {
            efficiency = 0; // Prevent division by zero
        }
    }
    
    // Returns points per hour efficiency
    float get_efficiency() const {
        return efficiency;
    }
    
    // Returns total surgeries count
    int get_surgery_count() const {
        return surgery.size();
    }
    
    // Returns vector of surgery type data
    vector<SurgeryTypeData> analyze_surgery_types() const {
        vector<SurgeryTypeData> result;
        
        for (size_t i = 0; i < surgery.size(); i++) {
            string type = surgery[i].type;
            int difficulty = surgery[i].difficulty;
            float minutes = surgery[i].minutes;
            
            // Find existing type or add new
            bool found = false;
            for (size_t j = 0; j < result.size(); j++) {
                if (result[j].type == type) {
                    result[j].frequency++;
                    result[j].points += difficulty;
                    result[j].duration += minutes;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                SurgeryTypeData data(type);
                data.frequency = 1;
                data.points = difficulty;
                data.duration = minutes;
                result.push_back(data);
            }
        }
        
        return result;
    }
    
    // Returns vector of difficulty analysis data
    vector<DifficultyData> analyze_difficulty_levels() const {
        vector<DifficultyData> result;
        
        for (size_t i = 0; i < surgery.size(); i++) {
            int difficulty = surgery[i].difficulty;
            float minutes = surgery[i].minutes;
            
            // Find existing difficulty or add new
            bool found = false;
            for (size_t j = 0; j < result.size(); j++) {
                if (result[j].difficulty == difficulty) {
                    result[j].time += minutes;
                    result[j].count++;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                DifficultyData data(difficulty);
                data.time = minutes;
                data.count = 1;
                result.push_back(data);
            }
        }
        
        // Calculate average time for each difficulty
        for (size_t i = 0; i < result.size(); i++) {
            if (result[i].count > 0) {
                result[i].time /= result[i].count;
            }
        }
        
        return result;
    }
    
    // Calculate predicted energy need based on efficiency
    float calculate_energy_need() const {
        if (surgery.size() == 0) {
            return 5.0f; // Base value for teams with no history
        }
        
        // More efficient teams should get more energy
        return max(5.0f, efficiency * 2);
    }
    
    friend class Hospital;
};

/******************************************************************************
 * Hospital Class Definition
 * Manages teams of surgeons and surgery scheduling
 ******************************************************************************/
class Hospital {
    private:
    friend class Company;
    friend class Team;
    int CAP;

    string substation;
    string name;

    string folder_name;
    string files_name;
    vector<Team> team;

    //fsm
    enum State {START, ASSIGN, REPORT, END};
    State state;
    int step = 0;;
    bool end;

    // Custom structures for FSM data tracking
    struct TeamEnergyData {
        string team_name;
        float points_per_hour;
        float allocated_energy;
        
        TeamEnergyData(string name, float pph) {
            team_name = name;
            points_per_hour = pph;
            allocated_energy = 0;
        }
    };
    
    // Helper function to sort teams by efficiency
    void sort_teams_by_efficiency(vector<TeamEnergyData>& data) {
        for (size_t i = 0; i < data.size(); i++) {
            for (size_t j = i + 1; j < data.size(); j++) {
                if (data[j].points_per_hour > data[i].points_per_hour) {
                    TeamEnergyData temp = data[i];
                    data[i] = data[j];
                    data[j] = temp;
                }
            }
        }
    }
    
    // Helper function to sort surgery types by frequency
    void sort_surgery_types_by_frequency(vector<Team::SurgeryTypeData>& data) {
        for (size_t i = 0; i < data.size(); i++) {
            for (size_t j = i + 1; j < data.size(); j++) {
                if (data[j].frequency > data[i].frequency) {
                    Team::SurgeryTypeData temp = data[i];
                    data[i] = data[j];
                    data[j] = temp;
                }
            }
        }
    }

/******************************************************************************
* Hospital Power Allocation Implementation
* Optimizes resource distribution during outages
* This algorithm distributes limited power capacity to surgical teams during outages
* based on their efficiency (points per hour). Teams with higher efficiency receive
* proportionally more power to maximize overall patient outcomes:
* 1. For each team, calculates proportion of total efficiency
* 2. If no efficiency data exists, distributes power equally
* 3. Allocates power as: team_allocation = CAP * (team_efficiency/total_efficiency)
* 4. Tracks and returns total allocation for verification

******************************************************************************/
    float allocate_power_by_efficiency(vector<TeamEnergyData>& team_data, float total_points_per_hour) {
        float total_allocated = 0;
        
        for (size_t i = 0; i < team_data.size(); i++) {
            // Calculate proportion based on team efficiency
            float proportion = (total_points_per_hour > 0) ? 
                            (team_data.at(i).points_per_hour / total_points_per_hour) : 
                            (1.0f / team_data.size());
            
            // Allocate power based on proportion
            float allocation = CAP * proportion;
            team_data.at(i).allocated_energy = allocation;
            total_allocated += allocation;
            
            // Calculate expected outcome based on allocation
            float expected_points = team_data.at(i).points_per_hour * allocation;
            
            // Output allocation results
            cout << "Team " << team_data.at(i).team_name 
                << " (Efficiency: " << team_data.at(i).points_per_hour << ")"
                << " allocated " << allocation << " units"
                << " (" << (proportion * 100) << "% of CAP)"
                << " - Expected points: " << expected_points << endl;
        }
        
        return total_allocated;
    }
    
    int min_value(int a, int b) {
        return (a < b) ? a : b;
    }

    public:
    /******************************************************************************
     * Hospital Constructors and Basic Configuration
     ******************************************************************************/
    Hospital(string name = "NA", int CAP = 0) {
        this->name = name;
        this->CAP = CAP;
        
        this->substation = "NA";
        this->folder_name = "NA";
        this->files_name = "NA";
        this->state = START;
    }
    void set_name(string name) {
        this->name = name;
    }
    void set_file_path(string folder_name) {
        this->folder_name = folder_name;
        // Check if folder exists and print debug info
        this->files_name = folder_name + substation + "_HospitalDatabase_V1.1.csv";
    }
    void set_substation(string substation) {
        this->substation = substation;
        set_file_path(this->folder_name);
    }
    void set_CAP(int CAP) {
        this->CAP = CAP;
    }
    int get_CAP() const {
        return CAP;
    }
    string get_name() const {
        return name;
    }
    Team* search_team(string team_name) {
        for (int i = 0; i < int(team.size()); i++) {
            if (team.at(i).get_name() == team_name) {
                return &team.at(i);
            }
        }
        return nullptr;
    }
    void clear_team() {
        team.clear();
    }

    /******************************************************************************
     * File Operations for Database Management
     ******************************************************************************/
    void file_team() {
        // Try to open the file if it exists
        ifstream file(files_name);
        if (file.is_open()) {
            cout << "Path: " << files_name << endl;
            
            string line;
            getline(file, line); // Skip header line
            
            while (getline(file, line)) {
                stringstream ss(line);
                string start_date, end_date, sub_name, hospital_name, team_name, surgery_type;
                string minutes_str, difficulty_str;
                
                getline(ss, start_date, ',');
                getline(ss, end_date, ',');
                getline(ss, sub_name, ',');
                getline(ss, hospital_name, ',');
                getline(ss, team_name, ',');
                getline(ss, surgery_type, ',');
                getline(ss, minutes_str, ',');
                getline(ss, difficulty_str, ',');
                
                int surgery_time = stoi(minutes_str);
                int difficulty = stoi(difficulty_str);

                Team* team_ptr = search_team(team_name);
                if (!team_ptr) {
                    add_team(team_name);
                    team_ptr = search_team(team_name);
                }
                team_ptr->add_surgery(surgery_type, start_date, end_date, difficulty, surgery_time);
            }
            file.close();
        } else {
            cout << "Error: Failed to open file: " << files_name << endl;
        }
    }
    
    /******************************************************************************
     * Team and Surgery Management
     ******************************************************************************/
    void add_complete_surgery(string team_name, string surgery_type, string start_date, string end_date, int difficulty, int time) {
        Date start_date_obj(start_date);
        Date end_date_obj(end_date);
        Date diff = end_date_obj - start_date_obj;
        if (start_date_obj > end_date_obj) {
            cout << "Start date is greater than end date." << endl;
            return;
        }
        if (diff.to_minutes() < time) {
            cout << "Surgery time is greater than the time between start and end date." << endl;
            return;
        }
        for (int i = 0; i < int(team.size()); i++) {
            if (team.at(i).get_name() == team_name) {
                team.at(i).add_surgery(surgery_type, start_date, end_date, difficulty, time);
                return;
            }
        }
        cout << "Team '" << team_name << "' not found in " << name << " hospital" << endl;

    }

    void add_team(string name) {
        for (int i = 0; i < int(team.size()); i++) {
            if (team.at(i).get_name() == name) {
                return;
            }
        }
        vector<string> empty_members;
        team.push_back(Team(name, empty_members));
    }
    void add_member(string team_name, string member) {
        for (int i = 0; i < int(team.size()); i++) {
            if (team.at(i).get_name() == team_name) {
                // Check if member already exists
                for (const string& existing_member : team.at(i).member) {
                    if (existing_member == member) {
                        cout << "Surgeon '" << member << "' is already in team '" << team_name << "'." << endl;
                        return;
                    }
                }
                team.at(i).add_member(member);
                cout << "Surgeon '" << member << "' has been added to team '" << team_name << "'." << endl;
                return;
            }
        }
        cout << "Team '" << team_name << "' not found in " << name << " hospital." << endl;
    }
    void remove_team(string team_name) {
        for (int i = 0; i < int(team.size()); i++) {
            if (team.at(i).get_name() == team_name) {
                cout << "Team '" << team_name << "' has been removed from " << name << " hospital." << endl;
                cout << "\n" << *team.at(i);
                team.erase(team.begin() + i);
                return;
            }
        }
        cout << "Team '" << team_name << "' not found in " << name << " hospital." << endl;
    }
    void remove_member(string team_name, string member) {
        for (int i = 0; i < int(team.size()); i++) {
            if (team.at(i).get_name() == team_name) {
                // Find and remove the member
                for (int j = 0; j < int(team.at(i).member.size()); j++) {
                    if (team.at(i).member.at(j) == member) {
                        team.at(i).member.erase(team.at(i).member.begin() + j);
                        cout << "Surgeon '" << member << "' has been removed from team '" << team_name << "'." << endl;
                        return;
                    }
                }
                cout << "Surgeon '" << member << "' not found in team '" << team_name << "'." << endl;
                return;
            }
        }
        cout << "Team '" << team_name << "' not found in " << name << " hospital." << endl;
    }
    void add_surgery() {
        return;
    }

// For example, the average time 
// predicts the amount of time needed to complete a surgery of certain difficulty, but other 
// estimation procedures can be used too. (2) Use the information about the difficulty of previous 
// surgeries to estimate what are the more likely surgeries that might be needed while the power at 
// the hospital is down. (3) Use the predictions in steps (1) and (2) to predict the amount of energy 
// required by each team of surgeons at each of the five hospitals. (4) Distribute the available 
// capacity CAP of the hospital [CAP is due to the backup generators], so that a hospital's teams of 
// surgeons can achieve the most outcome for the available capacity CAP. Explain in your report why 
// your algorithmic strategy is a good solution.    

    /******************************************************************************
     * Finite State Machine (FSM) for Hospital Operation Simulation
     ******************************************************************************/
    void set_flag(bool flag) {
        end = flag;
    }
    void FSM() {
        switch(state) {
            case START: {
                if (end == false) {
                    step++;
                }
                cout << "---------------------------------\n"
                     << "   Hospital: " << name << "\n"
                     << "   STATE: START, step: " << step << "\n" << endl;
                cout << "\n [ANALYZING SURGERIES]\n" << endl;
                
                if (team.empty()) {
                    display_teams();
                    cout << "No teams found in hospital." << endl;
                    state = END;
                } else {
                    end = false;
                    state = ASSIGN;
                }
                break;
            }
            case ASSIGN: {
                step++;
                cout << "---------------------------------\n"
                     << "   Hospital: " << name << "\n"
                     << "   STATE: ASSIGN, step: " << step << "\n"
                     << "\n[ANALYZING HISTORICAL DATA]" << endl;
                
                for (size_t i = 0; i < team.size(); i++) {
                    vector<Team::DifficultyData> team_difficulty_data = team.at(i).analyze_difficulty_levels();
                    
                    cout << "Team " << team.at(i).get_name() << " difficulty analysis:" << endl;
                    for (size_t j = 0; j < team_difficulty_data.size(); j++) {
                        cout << "  Difficulty " << team_difficulty_data.at(j).difficulty 
                             << ": Avg time = " << team_difficulty_data.at(j).time << " minutes" << endl;
                    }
                }
                
                state = REPORT;
                break;
            }
            case REPORT: {
                step++;
                cout << "---------------------------------\n"
                     << "   Hospital: " << name << "\n"
                     << "   STATE: REPORT, step: " << step << "\n" << endl;
                
                // Collect surgery type data across all teams
                vector<Team::SurgeryTypeData> hospital_surgery_types;
                
                for (size_t i = 0; i < team.size(); i++) {
                    vector<Team::SurgeryTypeData> team_surgery_data = team.at(i).analyze_surgery_types();
                    
                    // Merge team data into hospital data
                    for (size_t j = 0; j < team_surgery_data.size(); j++) {
                        string type = team_surgery_data[j].type;
                        bool found = false;
                        
                        // Check if we already have this type
                        for (size_t k = 0; k < hospital_surgery_types.size(); k++) {
                            if (hospital_surgery_types.at(k).type == type) {
                                hospital_surgery_types.at(k).frequency += team_surgery_data.at(j).frequency;
                                hospital_surgery_types.at(k).points += team_surgery_data.at(j).points;
                                hospital_surgery_types.at(k).duration += team_surgery_data.at(j).duration;
                                found = true;
                                break;
                            }
                        }
                        
                        if (!found) {
                            hospital_surgery_types.push_back(team_surgery_data[j]);
                        }
                    }
                }
                
                // Sort and display top surgery types
                sort_surgery_types_by_frequency(hospital_surgery_types);
                
                cout << "\n[MOST COMMON SURGERY TYPES]\n";
                int max_display = min_value(5, static_cast<int>(hospital_surgery_types.size()));
                for (int i = 0; i < max_display; i++) {
                    float avg_points = 0;
                    if (hospital_surgery_types.at(i).frequency > 0) {
                        avg_points = hospital_surgery_types.at(i).points / hospital_surgery_types.at(i).frequency;
                    }
                    
                    cout << (i+1) << ". " << hospital_surgery_types.at(i).type 
                         << " (Frequency: " << hospital_surgery_types.at(i).frequency
                         << ", Avg Points: " << avg_points << ")" << endl;
                }
                
                state = END;
                end = true;
                break;
            }
            case END: {
                if (end == false) {
                    state = START;
                }
                int end_step = step;
                cout << "---------------------------------\n"
                        << "   Hospital: " << name << "\n"
                        << "   STATE: END, step: " << end_step << "\n" << endl;
                
                // Collect team efficiency data
                vector<TeamEnergyData> team_data;
                float total_points_per_hour = 0;
                
                for (size_t i = 0; i < team.size(); i++) {
                    string team_name = team.at(i).get_name();
                    float points_per_hour = team.at(i).get_efficiency();
                    
                    TeamEnergyData ted(team_name, points_per_hour);
                    team_data.push_back(ted);
                    total_points_per_hour += points_per_hour;
                }
                
                // Sort teams by efficiency
                sort_teams_by_efficiency(team_data);
                
                // Distribute CAP based on team efficiency
                cout << "\n[POWER ALLOCATION BASED ON EFFICIENCY]\n";
                cout << "Available capacity (CAP): " << CAP << endl;
                
                allocate_power_by_efficiency(team_data, total_points_per_hour);
                float total_allocated = 0;
                for (size_t i = 0; i < team_data.size(); i++) {
                    total_allocated += team_data.at(i).allocated_energy;
                }

                cout << "\nTotal allocation: " << total_allocated << "/" << CAP << endl;
                cout << "This allocation maximizes patient outcomes based on team efficiency." << endl;
                
                break;
            }
        }
    }
    
    /******************************************************************************
     * Display and Reporting Functions
     ******************************************************************************/
    void display_team(const Team* team) const {
        if (team == nullptr) {
            cout << "Error: Invalid team pointer" << endl;
            return;
        }
        try {
            cout << "\n==== Team Information ====\n"
                 << "---------------------------------\n"
                 << "Team Name: " << team->get_name() << "\n"
                 << **team << "\n"
                 << "Average Points per Hour: " << team->efficiency << "\n"
                 << "---------------------------------\n" << endl;
        } catch (const exception& e) {
            cout << "Error displaying team information: " << e.what() << endl;
        }
    }
    void display_team(string team_name) {
        Team* team = search_team(team_name);
        if (team == nullptr) {
            cout << "Team '" << team_name << "' not found." << endl;
        }
        else {
            display_team(team);
        }
    }
    void display_teams() {
        cout << "===== " << name << " Hospital Teams and Surgeries =====" << endl;
        
        if (int(team.size()) == 0) {
            cout << "No teams found in this hospital." << endl;
            return;
        }
        
        int total_surgeries = 0;
        
        for(int i = 0; i < int(team.size()); i++) {
            cout << "\n" << substation << " - " << name << ", TEAM: " << team.at(i).get_name() << endl;
            cout << "----------------------------------------" << endl;
            cout << *team.at(i);
            cout << "----------------------------------------" << endl;
            total_surgeries++;
        }
    }
    void display_surgery(string start_date_str, string end_date_str) const {
        try {
            Date start_date(start_date_str);
            Date end_date(end_date_str);
            
            if (start_date > end_date) {
                cout << "Error: Start date is after end date" << endl;
                return;
            }

            cout << "\n==== SURGERIES IN " << name << " HOSPITAL ====\n"
                 << "----------------------------------------\n"
                 << "Date Range: " << start_date_str << " to " << end_date_str << "\n"
                 << "----------------------------------------\n";

            bool found_any = false;
            for (int i = 0; i < int(team.size()); i++) {
                bool team_header_printed = false;
                
                // Check each surgery for this team
                for (int j = 0; j < int(team.at(i).surgery.size()); j++) {
                    const Team::Surgery& s = team.at(i).surgery.at(j);
                    Date surgery_start(s.start_date);
                    Date surgery_end(s.end_date);
                    
                    // Check if this surgery's start date is within the range
                    if (!(surgery_start < start_date) && !(surgery_start > end_date)) {
                        // Print team header if not already printed
                        if (!team_header_printed) {
                            cout << "\nTeam: " << team.at(i).get_name() << "\n";
                            cout << "Surgery Type, Start Date, End Date, Difficulty, Minutes\n";
                            team_header_printed = true;
                        }
                        
                        // Print just this surgery
                        cout << s.type << ", " << s.start_date << ", " << s.end_date 
                             << ", " << s.difficulty << ", " << s.minutes << "\n";
                        found_any = true;
                    }
                }
                
                if (team_header_printed) {
                    cout << "----------------------------------------\n";
                }
            }

            if (!found_any) {
                cout << "No surgeries found in the specified date range." << endl;
            }
        }
        catch (const exception& e) {
            cout << "\nError: " << e.what() << "\n";
        }
    }
};
