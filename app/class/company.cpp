#include "hospital.cpp"
#include "station.cpp"
#include <random>

using namespace std;

/******************************************************************************
 * Company Class
 * 
 * Purpose:
 *   Manages the power grid network consisting of power stations and hospitals.
 *   Handles resource allocation, event simulation, and system analysis.
 *
 * Private Members:
 *   - struct Net: Connects a station to a hospital with priority
 *   - TOT: Total resources to allocate to stations
 *   - CAP: Capacity parameter for hospitals
 *   - K, Y, Z: Parameters for search operations and resource allocation
 *   - test_folder_path: Path to test files
 *   - test_files_path: Vector of test file paths
 *   - temp_station: Temporary station for Z_search results
 *
 * Public Members:
 *   - struct TeamInfo: Stores team ranking information
 *   - ranked_teams: Vector of TeamInfo for performance ranking
 *   - nets: Vector of Net objects connecting stations and hospitals
 *
 * Main Functions:
 *   - Constructors and Basic Operations:
 *     > Company(): Default constructor
 *     > clear_nets(): Clears networks and resets system
 *     > connect(): Connects a station to a hospital
 *
 *   - Parameter and Resource Management:
 *     > set_parameter(): Sets system parameters (TOT, CAP, K, Y, Z)
 *     > get/set methods for parameters
 *
 *   - File Operations and Test Management:
 *     > set_test_folder_path(): Sets path for test files
 *     > file_exist(): Checks if a file exists
 *     > file_test_ticket(): Loads test files
 *     > select_random_file(): Selects a random test file
 *
 *   - Network Component Access:
 *     > select_station(): Finds a station by name
 *     > select_hospital(): Finds a hospital by name
 *
 *   - Resource Allocation:
 *     > allot_TOT(): Distributes TOT resources among stations
 *
 *   - Event Simulation:
 *     > update_event(): Updates the system state for all components
 *
 *   - Station Analysis:
 *     > Z_search(): Searches for similar tickets across all stations
 *     > display_Z_search(): Displays Z_search results
 *
 *   - Hospital Team Analysis:
 *     > team_search(): Ranks teams by performance
 *     > display_team(): Displays team rankings
 ******************************************************************************/
class Company {
    private:
        struct Net { //each station is connected to a hospital
            Station station;       
            Hospital hospital;
            int priority;
            Net(Station station, Hospital hospital) {
                priority = 0;
                this->station = station;
                this->hospital = hospital;
            }
        };

        int TOT;
        int CAP;
        int K;
        int Y;
        int Z;
        string test_folder_path;
        vector<string> test_files_path;
        Station temp_station; // Temporary dummy station to store Z_search results tickets
        
    public:
        struct TeamInfo {
            string hospital;
            string team;
            float points_per_hour;
        };
        
        vector<TeamInfo> ranked_teams;
        vector<Net> nets;
        
        /******************************************************************************
         * Constructors and Basic Operations
         ******************************************************************************/
        Company() {
            TOT = 0;
            CAP = 0;
            K = 0;
            Y = 0;
            Z = 0;
        }
        void clear_nets() {
            for (int i = 0; i < int(nets.size()); i++) {
                nets.at(i).station.clear_ticket();
                nets.at(i).hospital.clear_team();
            }
            nets.clear();
            test_files_path.clear();
        }
        void connect(Station station, Hospital hospital) {
            hospital.set_substation(station.get_name());
            Net net(station, hospital);
            nets.push_back(net);
        }
        
        /******************************************************************************
         * Parameter and Resource Management
         ******************************************************************************/
        void set_parameter(int TOT, int CAP, int K = 1, int Y = 1, int Z = 1) {
            this->TOT = TOT;
            this->CAP = CAP;
            this->K = K > 0 ? K : 1;
            this->Y = Y > 0 ? Y : 1;
            this->Z = Z > 0 ? Z : 1;
            set_Y(this->Y);
            set_K(this->K);
            for (int i = 0; i < int(nets.size()); i++) {
                nets.at(i).station.set_TOT(this->TOT);
                nets.at(i).hospital.set_CAP(this->CAP);
            }
        }
        int get_TOT() {
            return TOT;
        }
        int get_CAP() {
            return CAP;
        }
        int get_K() {
            return K;
        }
        void set_K(int k) {
            for (int i = 0; i < int(nets.size()); i++) {
                nets.at(i).station.set_K(k);
            }
        }
        int get_Y() {
            return Y;
        }
        void set_Y(int y) {
            for (int i = 0; i < int(nets.size()); i++) {
                nets.at(i).station.set_Y(y);
            }
        }
        int get_Z() {
            return Z;
        }
        
        /******************************************************************************
         * File Operations and Test Management
         ******************************************************************************/
        void set_test_folder_path(string path) {
            this->test_folder_path = path;
        }
        bool file_exist(string path) {
            if (!filesystem::exists(path)) {
                cout << "Warning: Path does not exist: " << path << endl;
                return false;
            }
            return true;
        }
        void file_test_ticket() {
            if (!file_exist(test_folder_path)) {
                return;
            }
            
            for (const auto& entry : filesystem::directory_iterator(test_folder_path)) {
                test_files_path.push_back(entry.path().string());
            }
            cout << "\nPath: " << test_folder_path << endl;
            return;
        }
        string select_random_file() {
            static bool seeded = false;
            if (!seeded) {
                // Seed with current time only once
                srand(time(0));
                seeded = true;
            }
            
            int random_index = rand() % test_files_path.size();
            cout << "random_index: " << random_index << endl;
            string selected_file = test_files_path.at(random_index);
            return selected_file;
        }

        /******************************************************************************
         * Network Component Access and Selection
         ******************************************************************************/
        int select_station(string station_name) {
            for (int i = 0; i < int(nets.size()); i++) {
                if (nets.at(i).station.get_name() == station_name) {
                    return i;
                }
            }
            return -1;
        }

        int select_hospital(string hospital_name) {
            for (int i = 0; i < int(nets.size()); i++) {
                if  (nets.at(i).hospital.get_name() == hospital_name) {
                    return i;
                }
            }
            return -1;
        }

        /******************************************************************************
         * Resource Allocation and Management
         ******************************************************************************/
        void allot_TOT() {
            int remaining = TOT;    
            // Initialize all stations to 0 TOT
            for (int i = 0; i < int(nets.size()); i++) {
                nets.at(i).station.set_TOT(0);
            }
            // Give 1 TOT to each station if possible
            if (remaining >= int(nets.size())) {
                for (int i = 0; i < int(nets.size()); i++) {
                    if (remaining > 0) {
                        nets.at(i).station.set_TOT(1);
                        remaining--;
                    } else {
                        nets.at(i).station.set_TOT(0);
                    }
                }
            
                // Distribute remaining TOT based on priority percentages
                // and track highest priority station
                int allocated = 0;
                int highest_idx = 0;
                float highest_priority = 0;
                
                for (int i = 0; i < int(nets.size()); i++) {
                    int add = nets.at(i).station.get_priority() * remaining;
                    nets.at(i).station.add_TOT(add);
                    allocated += add;
                    
                    // Track highest priority
                    if (nets.at(i).station.get_priority() > highest_priority) {
                        highest_priority = nets.at(i).station.get_priority();
                        highest_idx = i;
                    }
                }
                
                // Give any remaining to highest priority station
                if (remaining - allocated > 0) {
                    nets.at(highest_idx).station.add_TOT(remaining - allocated);
                }
            } else {
                cout << "TOT is less than the number of substations" << endl;
                return;
            }
        }
        
        /******************************************************************************
         * Event Simulation and System Update
         ******************************************************************************/
        void update_event() {
            for (int i = 0; i < int(nets.size()); i++) {
                cout << "//////////////////////////////////////////////////////////" << endl;
                cout << "///////////////////| S t a t i o n |//////////////////////" << endl;
                cout << "//////////////////////////////////////////////////////////" << endl;
                nets.at(i).station.FSM();
                cout << "//////////////////////////////////////////////////////////" << endl;
                cout << "//////////////////| H o s p i t a l |/////////////////////" << endl;
                cout << "//////////////////////////////////////////////////////////" << endl;
                nets.at(i).hospital.FSM();
                
            }
            cout << "\n//////////////////////////////////////////////////////////\n" << endl;
            cout << "[o]COMPLETED update_event()\n" << endl;
        }
        
        /******************************************************************************
         * Station Analysis and Ticket Search
         ******************************************************************************/
        void Z_search(Ticket ticket) {
            temp_station.sim_rank.clear();
            
            cout << "\nSearching for similar tickets across all substations...\n";
            
            // Search through all stations
            for (int i = 0; i < int(nets.size()); i++) {
                Station& station = nets.at(i).station;

                station.Y_search(ticket);

                for (size_t j = 0; j < station.sim_rank.size(); j++) {
                    temp_station.sim_rank.push_back(station.sim_rank.at(j));
                }
            }
            
            // Sort tickets by similarity score (highest first) using bubble sort
            // This approach works because Station can access Ticket's private members
            for (size_t i = 0; i < temp_station.sim_rank.size(); i++) {
                for (size_t j = 0; j < temp_station.sim_rank.size() - i - 1; j++) {
                    if (temp_station.sim_rank.at(j).get_sim_value() < temp_station.sim_rank.at(j+1).get_sim_value()) {
                        Ticket temp = temp_station.sim_rank.at(j);
                        temp_station.sim_rank.at(j) = temp_station.sim_rank.at(j+1);
                        temp_station.sim_rank.at(j+1) = temp;
                    }
                }
            }
        }
        
        void display_Z_search() {
            cout << "\n==== TOP " << min(static_cast<size_t>(Z), temp_station.sim_rank.size()) 
                 << " SIMILAR TICKETS ACROSS ALL STATIONS ====\n";
            
            for (size_t i = 0; i < min(static_cast<size_t>(Z), temp_station.sim_rank.size()); i++) {
                cout << "\n--- Rank #" << (i+1) << " ---\n";
                
                Station station_for_id;
                station_for_id.sim_rank.clear();
                station_for_id.sim_rank.push_back(temp_station.sim_rank.at(i));
                string ticket_id = station_for_id.sim_rank.at(0).get_id();
                
                station_for_id.display_ticket(temp_station.sim_rank.at(i));
            }
            
            if (temp_station.sim_rank.empty()) {
                cout << "No matching tickets found across any station." << endl;
            }
        }

        /******************************************************************************
         * Hospital Team Analysis and Ranking
         ******************************************************************************/
        void team_search() {
            ranked_teams.clear();
            
            // First pass: Collect all teams from hospitals
            for (int i = 0; i < int(nets.size()); i++) {
                Hospital& hospital = nets.at(i).hospital;
                string hospital_name = hospital.get_name();

                // Access teams directly as a friend class
                for (size_t j = 0; j < hospital.team.size(); j++) {
                    Team& team = hospital.team.at(j);
                    string team_name = team.get_name();
                    float points_per_hour = team.get_efficiency();
                    
                    // Create TeamInfo record
                    TeamInfo info;
                    info.hospital = hospital_name;
                    info.team = team_name;
                    info.points_per_hour = points_per_hour;
                    
                    ranked_teams.push_back(info);
                }
            }
                  
            for (size_t i = 0; i < ranked_teams.size(); i++) {
                for (size_t j = 0; j < ranked_teams.size() - i - 1; j++) {
                    if (ranked_teams.at(j).points_per_hour < ranked_teams.at(j+1).points_per_hour) {
                        TeamInfo temp = ranked_teams.at(j);
                        ranked_teams.at(j) = ranked_teams.at(j+1);
                        ranked_teams.at(j+1) = temp;
                    }
                }
            }
        }

        void display_team() {
            cout << "\n==== TOP " << ranked_teams.size() << " TEAMS RANKED BY POINTS PER HOUR ====\n";
            for (size_t i = 0; i < ranked_teams.size(); i++) {
                cout << "\n--- Rank #" << (i+1) << " (Points/Hour: " << ranked_teams.at(i).points_per_hour << ") ---\n";
                cout << "Hospital: " << ranked_teams.at(i).hospital << "\n";
                cout << "Team: " << ranked_teams.at(i).team << "\n";
            }
            
            if (ranked_teams.empty()) {
                cout << "No teams found across any hospital." << endl;
            }
        }
};