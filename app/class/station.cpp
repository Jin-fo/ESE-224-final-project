#include "_header.cpp"
using namespace std;

// Forward declarations
class Document;
class Ticket;
class Header;
class Status;
class Comment;
class Station;
class Company;

/******************************************************************************
 * Document Class Hierarchy
 * Base class for document records and specialized document types
 ******************************************************************************/
class Document {

    protected:
    friend class Date;
    string index; //line number
    Date date; //time
    string id; // i.e EB########
    string remark;
    
    public:
    /******************************************************************************
     * Constructors and Basic Operations
     ******************************************************************************/
    Document(string remark = "") {
        index = "";
        id = "EB########";
        remark = "";
    }
    
    /******************************************************************************
     * Getters and Setters
     ******************************************************************************/
    string get_id() const {
        return id;
    }
    Date get_date() {
        return this->date;
    }
    void set_line(string index) {
        this->index = index;
    }
    void set_id(string id) {
        this->id = id;
    }
    void set_date(string timestamp) {
        // Remove the newline character if it exists
        if (!timestamp.empty() && timestamp[timestamp.length() - 1] == '\n') {
            timestamp.erase(timestamp.length() - 1);
        }
        this->date = Date(timestamp);
    }
    
    /******************************************************************************
     * Display Functions
     ******************************************************************************/
    string display() {
        string line = index + ", " + *date + ", " + id + ":" + remark;
        return line;
    }
    string get_remark() {
        return remark;
    }
};

/******************************************************************************
 * Header Class
 * Document subclass for ticket headers
 ******************************************************************************/
class Header : public Document {
    private:
    string address;
    string ENE;

    public:
    /******************************************************************************
     * Constructors and Basic Operations
     ******************************************************************************/
    Header(string address = "") : Document() {
        this->address = address;
        ENE = "ENE";
        set_line("1");
        //set_current_time();
    }
    
    /******************************************************************************
     * Getters and Setters
     ******************************************************************************/
    string get_address() {
        return address;
    }
    void set_ENE(string new_ENE) {
        ENE = new_ENE;
    }
    void set_address(string new_address) {
        address = new_address;
    }
    void set_remark() {
        remark = ENE + ", " + address + "\n";
    }
    
    friend class Ticket;
};

/******************************************************************************
 * Status Class
 * Document subclass for status information
 ******************************************************************************/
class Status : public Document {
    private:
    string structure;
    string voltage;
    string ground;
    string msp;
    string harmonic;
    string v_non_shunt;
    string status;

    public:
    /******************************************************************************
     * Constructors and Basic Operations
     ******************************************************************************/
    Status(string structure = "na", string voltage = "na", string ground = "na", string msp = "na", string harmonic = "na", string v_non_shunt = "na", string status = "na") : Document() {
        this->structure = structure;
        this->voltage = voltage;
        this->ground = ground;
        this->msp = msp;
        this->harmonic = harmonic;
        this->v_non_shunt = v_non_shunt;
        this->status = status;
        remark = "STRUCTURE=" + structure + 
                ", VOLTAGE=" + voltage + 
                ", GROUND=" + ground + 
                ", MSPLATE=" + msp + 
                ", HARMONIC=" + harmonic + 
                ", V NON-SHUNT=" + v_non_shunt + 
                ", STATUS=" + status;
        set_line(""); 
    }
    
    /******************************************************************************
     * Getters and Setters
     ******************************************************************************/
    void set_structure(string new_structure) {
        structure = new_structure;
    }
    void set_voltage(string new_voltage) {
        voltage = new_voltage;
    }
    void set_ground(string new_ground) {
        ground = new_ground;
    }
    void set_msp(string new_msp) {
        msp = new_msp;
    }
    void set_harmonic(string new_harmonic) {
        harmonic = new_harmonic;
    }
    void set_v_non_shunt(string new_v_non_shunt) {
        v_non_shunt = new_v_non_shunt;
    }
    void set_status(string new_status) {
        status = new_status;
    }
    string get_structure() {
        return structure;
    }
    string get_voltage() {
        return voltage;
    }
    string get_ground() {
        return ground;
    }
    string get_msp() {
        return msp;
    }
    string get_harmonic() {
        return harmonic;
    }
    string get_v_non_shunt() {
        return v_non_shunt;
    }
    string get_status() {
        return status;
    }
    void set_remark() {
        remark = "STRUCTURE=" + structure + 
                ", VOLTAGE=" + voltage + 
                ", GROUND=" + ground + 
                ", MSPLATE=" + msp + 
                ", HARMONIC=" + harmonic + 
                ", V NON-SHUNT=" + v_non_shunt + 
                ", STATUS=" + status;
    }
    
    /******************************************************************************
     * Display Functions
     ******************************************************************************/
    virtual string display() {
        string line = ", , " + get_remark() + '\n';
        return line;
    }
    friend class Ticket;
};

/******************************************************************************
 * Comment Class
 * Document subclass for comment information
 ******************************************************************************/
class Comment : public Document {
    private:
    string comment;

    public:
    /******************************************************************************
     * Constructors and Basic Operations
     ******************************************************************************/
    Comment(string comment_text = "") : Document() {
        this->comment = comment_text;
        set_line("");
    }
    
    /******************************************************************************
     * Getters and Setters
     ******************************************************************************/
    void set_comment(string new_comment) {
        comment = new_comment;
    }
    void set_remark() {
        remark = comment;
    }
    void to_word_string() {

    }
    
    friend class Ticket;
};

//------------------end of document heiearchy--------------------------------
/////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * Ticket Class Definition
 * Represents a power outage ticket with header, status, and comments
 ******************************************************************************/
class Ticket {
    private:
        string col_names;
        Header header;
        Status status; // nature of damage line
        vector<Comment> comment;

        //difficuty attributes
        bool hard;
        int time_score;
        int sim_value;

        //repair attributes
        bool surveyed;
        bool repaired;

        //testing attributes
        int comment_line;
        string file_path;

    public:
        /******************************************************************************
         * Constructors and Basic Operations
         ******************************************************************************/
        Ticket(Header header = Header(), Status status = Status(), vector<Comment> comment = vector<Comment>(), bool surveyed = false, bool repaired = false) {
            this->header = header;
            this->status = status;
            this->comment = comment;
            this->col_names = "";
            this->hard = false;
            this->time_score = 0;
            this->sim_value = 0;
            this->surveyed = surveyed;
            this->repaired = repaired;
        }
        Ticket(const Ticket& ticket) {
            this->header = ticket.header;
            this->status = ticket.status;
            this->comment = ticket.comment;
            this->col_names = ticket.col_names;
            this->hard = ticket.hard;
            this->time_score = ticket.time_score;
            this->sim_value = ticket.sim_value;
            this->surveyed = ticket.surveyed;
            this->repaired = ticket.repaired;
            this->file_path = ticket.file_path;
            this->comment_line = ticket.comment_line;
        }
        Ticket& operator=(const Ticket& ticket) {
            if (this != &ticket) {
                this->header = ticket.header;
                this->status = ticket.status;
                this->comment = ticket.comment;
                this->col_names = ticket.col_names;
                this->hard = ticket.hard;
                this->time_score = ticket.time_score;
                this->sim_value = ticket.sim_value;
                this->surveyed = ticket.surveyed;
                this->repaired = ticket.repaired;
                this->file_path = ticket.file_path;
                this->comment_line = ticket.comment_line;
                this->hard = ticket.hard;
            }
            return *this;
        }  
        
        /******************************************************************************
         * Getters and Setters
         ******************************************************************************/
        string get_sim_value() {
            return to_string(sim_value);
        }
        string get_id() {
            return header.get_id();
        }
        string operator*() {
            string field;

            field += header.display();
            field += status.display();
            for (int i = 0; i < int(comment.size()); i++) {
                field += comment.at(i).display() + '\n';
            }
            return field;
        }
        
        /******************************************************************************
         * Ticket Processing Functions
         ******************************************************************************/
        bool is_surveyed() {
            //slect randomly to be true or false
            this->surveyed = rand() % 2; //0 or 1
            return surveyed;
        }
        bool is_repaired() {            
            // Try to add next comment - if none left, repair is complete
            if (!write_comment()) {
                this->repaired = true;
                return true;
            }
            return this->repaired;
        }
        //*********************************************************************** */
        // station functionality, B: Updates the ticket for an outage by adding a new line as shown in lines 2 and below in 
        // Figure 2. Each line represents a repairing step. 
        //*********************************************************************** */
        bool write_comment() {
            // Try to open file and handle basic errors
            ifstream infile(file_path);
            if (!infile.is_open()) {
                return false;
            }
            
            try {
                string line;
                
                // Skip header and previously read comments
                if (!getline(infile, line)) return false;
                
                for (int i = 0; i < comment_line + 3; i++)
                    if (!getline(infile, line)) return false;
                
                // Parse and add the next comment
                Comment newComment;
                istringstream iss(line);
                string lineNum, timestamp, id, comment_text;
                
                getline(iss, lineNum, ',');
                getline(iss, timestamp, ',');
                getline(iss, id, ':');
                getline(iss, comment_text);
                
                newComment.set_line(lineNum);
                newComment.set_date(timestamp);
                newComment.set_id(id);
                newComment.set_comment(comment_text);
                newComment.set_remark();
                
                // Add to comments and update position
                comment.push_back(newComment);
                comment_line++;
                return true;
            }
            catch (const exception& e) {
                cout << "Error in write_comment: " << e.what() << endl;
                return false;
            }
        }
        
        /******************************************************************************
         * Analysis and Calculations
         ******************************************************************************/
        void calculate_difficulty(int time_threshold) {
            if (time_score > time_threshold) {
                this->hard = true;
            }
            else {
                this->hard = false;
            }
        }
        Date get_date() {
            return Date(header.get_date());
        }
        void calculate_time_score() {
            Date date0 = comment.at(0).get_date();
            Date date1 = comment.at(comment.size()-1).get_date();
            Date delta = date1 - date0;
            this->time_score = delta.to_minutes();
            //cout << "time_score: " << time_score << endl;
        }
        void set_status(Status status) {
            this->status = status;
        }
        friend class Station;
};
//------------------end of ticket class------------------------------------ 
////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * Station Class Definition
 * Manages power station operations, crews, and ticket processing
 ******************************************************************************/
class Station {
    private:
        friend class Ticket;
        friend class Company;
        string name;
        float priority;
        int allot_TOT;
        int hard_threshold;
        int Y;
        int K;
        
        /******************************************************************************
         * Station Sub-Components
         ******************************************************************************/
        struct Crew {
            string id;
            bool on_hard;
            bool assigned;
            string job_type;    
            Ticket ticket;
            Crew(bool on_hard = false, string job_type = "idle") {
                this->id = generate_id();
                this->on_hard = on_hard;
                this->assigned = false;
                this->job_type = job_type;
            }
            string generate_id() {
                return "na" + to_string(rand() % 9999999);
            }
            void set_on_hard(bool on_hard) {
                this->on_hard = on_hard;
            }
            void assign_ticket(Ticket ticket) {
                this->ticket = ticket;
                this->assigned = true;
                if (ticket.surveyed == false && ticket.repaired == false) {
                    this->job_type = "survey";
                }
                else if (ticket.surveyed == true && ticket.repaired == false) {
                    this->job_type = "repair";
                }
                else if (ticket.surveyed == true && ticket.repaired == true) {
                    this->job_type = "na";
                }
            }
            Ticket get_ticket() {
                return ticket;
            }
        };
        
        struct Word {
            string word;
            int count;
            Word(string word = "", int count = 0) {
                this->word = word;
                this->count = count;
            }
        };
        
        /******************************************************************************
         * Database Structure
         ******************************************************************************/
        struct Database {
            string folder_name;
            string file_path;
            vector<string> file_name;
            vector<Ticket> archive;
            vector<Word> word_dict;
        }; Database db;

        /******************************************************************************
         * Station Core Data Members
         ******************************************************************************/
        vector<Crew> crew;
        vector<Ticket> passive;
        vector<Ticket> active;

        //Y search temp
        vector<Ticket> sim_rank;
        //FSM state
        enum State {START, ASSIGN, REPORT, END};
        State state = START;
        int step = 0;
        bool end;
    public:
        friend class Ticket;
        
        /******************************************************************************
         * Constructors and Basic Configuration
         ******************************************************************************/
        Station(string name = "NA", float priority = 0) {
            allot_TOT = 0;
            this->priority = priority;
            this->name = name;
            state = START;
        }
        void set_name(string name) {
            this->name = name;
        }
        void set_file_path(string folder_name) {
            db.folder_name = folder_name;
            db.file_path = db.folder_name + name + "_tickets";
        }
        void set_TOT(int TOT) {
            crew.clear();
            this->allot_TOT = TOT;
            for (int i = 0; i < int(TOT); i++) {
                if (i < int(TOT/2)) {
                    crew.push_back(Crew(true));
                }
                else {
                    crew.push_back(Crew(false));
                }
            }
        }
        void set_Y(int Y) {
            this->Y = Y;
        }
        void set_K(int K) {
            this->K = K;
        }
        string get_name() {
            return name;
        }
        int get_TOT() {
            return allot_TOT;   
        }
        void add_TOT(int amount) {
            allot_TOT += amount;
            for (int i = 0; i < amount; i++) {
                crew.push_back(Crew(false));
            }
            for (int i = 0; i < int(crew.size()); i++) {
                if (i < int(allot_TOT/2)) {
                    crew.at(i).set_on_hard(true);
                }
                else {
                    crew.at(i).set_on_hard(false);
                }
            }
        }
        float get_priority() {
            return priority;
        }
        void clear_ticket() {
            db.archive.clear();
            db.word_dict.clear();
            active.clear();
        }
        void calbrate_hard_threshold() {
            int sum = 0;
            for (int i = 0; i < int(db.archive.size()); i++) {
                sum += db.archive.at(i).time_score;
            }
            hard_threshold = sum / int(db.archive.size());
            cout << "Hard threshold: " << hard_threshold << endl;
        }
        /******************************************************************************
         * File Operations
         ******************************************************************************/
        bool file_exist(string path) {
            try {
                if (!filesystem::exists(path)) {
                    return false;
                }
                return true;
            }
            catch (const exception& e) {
                cout << e.what() << endl;
                return false;
            }
        }
        void file_ticket() {
            // Check if directory exists before attempting to iterate
            if (!file_exist(db.file_path)) {
                return;
            }
            for (const auto& entry : filesystem::directory_iterator(db.file_path)) {
                db.file_name.push_back(entry.path().string());
            }     
            for (int i = 0; i < int(db.file_name.size()); i++) {
                ifstream infile;
                string file = db.file_name.at(i); 
                infile.open(file);

                Ticket ticket;
                ticket.surveyed = true;
                ticket.repaired = true;
                ticket = file_to_ticket(ticket, file);
                
                ticket.calculate_time_score();
                archive_ticket(ticket);
            }
            calbrate_hard_threshold();
            for (int i = 0; i < int(db.archive.size()); i++) {
                db.archive.at(i).calculate_difficulty(hard_threshold);
            }
        }
        void file_test(string file) {
            if (!file_exist(file)) {
                return;
            }
            Ticket ticket;
            ticket.surveyed = false;
            ticket.repaired = false;
            ticket = file_to_ticket(ticket, file);
            cout << "\nCall-In Ticket:" << endl;
            display_ticket(ticket);
            passive.push_back(ticket);
        }
        
        /******************************************************************************
         * Ticket Processing
         ******************************************************************************/
        Ticket file_to_ticket(Ticket ticket, string file) {
            ticket.file_path = file;
            ifstream infile(file);
            if (infile.is_open()) {
                cout << "Path: " << file << endl;
            }
            else {
                cout << "Error opening file: " << file << endl;
            }
            
            // Define state for parsing
            enum ParseState {HEADER, STATUS, COMMENTS, DONE};
            ParseState parseState = HEADER;
            string line;
            
            getline(infile, ticket.col_names); //set col_names
            
            while (getline(infile, line) && parseState != DONE) { // Read the actual data
                
                if (line.empty()) break; // Skip empty lines
                switch(parseState) {
                    case HEADER:
                        {
                            istringstream iss(line);
                            string lineNum, timestamp, id, ene, address;
                            
                            getline(iss, lineNum, ',');
                            getline(iss, timestamp, ',');
                            getline(iss, id, ',');
                            getline(iss, ene, ',');
                            getline(iss, address);
                            
                            // Skip empty lines or header
                            if (lineNum.empty() && timestamp.empty() && id.empty()) {
                                continue;
                            }
                            
                            ticket.header.set_line(lineNum);
                            ticket.header.set_date(timestamp);
                            ticket.header.set_id(id);
                            ticket.header.set_ENE(ene);
                            ticket.header.set_address(address);
                            ticket.header.set_remark();
                            if (ticket.surveyed == false) {
                                parseState = DONE;
                                continue;
                            }
                            else {
                                parseState = STATUS;
                            }
                        }
                        break;
                        
                    case STATUS:
                        {
                            istringstream iss(line);
                            string ignore;
                            
                            // Skip the first two fields (line number and timestamp)
                            getline(iss, ignore, ',');
                            getline(iss, ignore, ',');
                            
                            // Check if the status part is wrapped in quotes
                            if (line.find("\"") != string::npos) {
                                // Extract the quoted part
                                size_t start = line.find("\"");
                                size_t end = line.rfind("\"");
                                
                                if (start != string::npos && end != string::npos && end > start) {
                                    string quoted_part = line.substr(start + 1, end - start - 1);
                                    
                                    // Now parse the status fields from the quoted part
                                    istringstream status_ss(quoted_part);
                                    string status_field, value;
                                    
                                    // Parse structure
                                    if (getline(status_ss, status_field, '=') && getline(status_ss, value, ',')) {
                                        ticket.status.set_structure(value);
                                    }
                                    
                                    // Parse voltage
                                    if (getline(status_ss, status_field, '=') && getline(status_ss, value, ',')) {
                                        ticket.status.set_voltage(value);
                                    }
                                    
                                    // Parse ground
                                    if (getline(status_ss, status_field, '=') && getline(status_ss, value, ',')) {
                                        ticket.status.set_ground(value);
                                    }
                                    
                                    // Parse msplate
                                    if (getline(status_ss, status_field, '=') && getline(status_ss, value, ',')) {
                                        ticket.status.set_msp(value);
                                    }
                                    
                                    // Parse harmonic
                                    if (getline(status_ss, status_field, '=') && getline(status_ss, value, ',')) {
                                        ticket.status.set_harmonic(value);
                                    }
                                    
                                    // Parse v_non_shunt
                                    if (getline(status_ss, status_field, '=') && getline(status_ss, value, ',')) {
                                        ticket.status.set_v_non_shunt(value);
                                    }
                                    
                                    // Parse status (last field might not have a comma)
                                    if (getline(status_ss, status_field, '=') && getline(status_ss, value)) {
                                        ticket.status.set_status(value);
                                    }
                                    
                                    ticket.status.set_remark();
                                    
                                    if (ticket.repaired == false) {
                                        parseState = DONE;
                                        continue;
                                    }
                                    else {
                                        parseState = COMMENTS;
                                    }
                                    break;
                                }
                            }
                            else {
                                // Original parsing method
                                string structure, voltage, ground, msp, harmonic, v_non_shunt, status;
                                
                                getline(iss, structure, ',');
                                getline(iss, voltage, ',');
                                getline(iss, ground, ',');
                                getline(iss, msp, ',');
                                getline(iss, harmonic, ',');
                                getline(iss, v_non_shunt, ',');
                                getline(iss, status);
                                
                                // Skip empty lines
                                ticket.status.set_structure(structure); 
                                ticket.status.set_voltage(voltage);
                                ticket.status.set_ground(ground);
                                ticket.status.set_msp(msp);
                                ticket.status.set_harmonic(harmonic);
                                ticket.status.set_v_non_shunt(v_non_shunt);
                                ticket.status.set_status(status);
                                ticket.status.set_remark();
                                
                                if (ticket.repaired == false) {
                                    parseState = DONE;
                                    continue;
                                }
                                else {
                                    parseState = COMMENTS;
                                }
                            }
                        }
                        break;
                        
                    case COMMENTS:
                        {
                            if (ticket.repaired == false) {
                                for (int i = 0; i < int(ticket.comment_line); i++) {
                                    getline(infile, line); //skip the line to prevent duplicate comments
                                }
                            }
                            ticket.comment_line++;
                            Comment newComment;
                            istringstream iss(line);
                            string lineNum, timestamp, id, comment_text, word;
                            
                            getline(iss, lineNum, ',');
                            getline(iss, timestamp, ',');
                            getline(iss, id, ':');
                            while(getline(iss, word, ' ')) {
                                comment_text += word + " ";
                                add_word(word);
                            }
                             
                            newComment.set_line(lineNum);
                            newComment.set_date(timestamp);
                            newComment.set_id(id);
                            newComment.set_comment(comment_text);
                            newComment.set_remark();
                            
                            ticket.comment.push_back(newComment);
                        }
                        break;
                        
                    case DONE:
                        // Explicitly handle DONE case to avoid warning
                        break;
                }
            }
            infile.close();
            return ticket;
        }
        void archive_ticket(Ticket ticket) {
            db.archive.push_back(ticket);
        }
        void activate_ticket(Ticket ticket) {
            active.push_back(ticket);
        }
        /******************************************************************************
         * Finite State Machine (FSM) Implementation
         ******************************************************************************/
        void set_state(string s) {
            if (s == "START") state = START;
            else if (s == "ASSIGN") state = ASSIGN;
            else if (s == "REPORT") state = REPORT;
            else if (s == "END") state = END;
        }

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
                         << "   Station: " << name << "\n"
                         << "   STATE: START, step: " << step << "\n" << endl;
                    cout << "\n [READY TO ASSIGN]\n" << endl;
                    if ((passive.empty() && active.empty()) || crew.empty()) {
                        state = END;
                    }
                    else {
                        end = false;
                        state = ASSIGN;
                    }
                    break;
                }
                    
                case ASSIGN: {
                    step++;
                    cout << "---------------------------------\n"
                         << "   Station: " << name << "\n"
                         << "   STATE: ASSIGN, step: " << step << "\n" << endl;
                    if (!passive.empty()) {
                        begin_survey();
                        if (passive.empty()) begin_repair();
                        state = REPORT;
                    }
                    else if (!active.empty()) {
                        begin_repair();
                        state = REPORT;
                    }
                    else state = END;
                    break;
                }
                    
                case REPORT: {
                    step++;
                    cout << "---------------------------------\n"
                         << "   Station: " << name << "\n"
                         << "   STATE: REPORT, step: " << step << "\n" << endl;
                    
                    running_survey();
                    running_repair();
                    
                    // Count working crews
                    bool has_tickets = !passive.empty() || !active.empty();
                    int working_crews = 0;
                    for (auto& c : crew) if (c.assigned) working_crews++;
                    
                    // Determine next state
                    if (!has_tickets && working_crews == 0) {
                        end = true;
                        state = END;
                    }
                    else if (has_tickets && working_crews < static_cast<int>(crew.size())) {
                        state = ASSIGN;
                    }
                    break;
                }
                    
                case END: {
                    int end_step = 0;
                    if (end == false) {
                        state = START;
                    }
                    end_step = step;
                        cout << "---------------------------------\n"
                         << "   Station: " << name << "\n"
                         << "   STATE: END, step: " << end_step << "\n" << endl;
                    break;
                }
            }
        }
        
        /******************************************************************************
         * Crew and Ticket Assignment Management
         ******************************************************************************/
        //--------------------------------------------------------------------
        // After a power outage occurs, a team assesses the damage and creates the ticket with the first 
        // row indicating the nature of the damage, as shown in Figure 2. 
        //--------------------------------------------------------------------
        void begin_survey() {
            for (int i = 0; i < int(crew.size()) && !passive.empty(); i++) {
                if (crew.at(i).assigned == false) {
                    Ticket temp = passive.at(0);
                    crew.at(i).assign_ticket(temp);
                    crew.at(i).assigned = true;
                    cout << "[Survey]: " << crew.at(i).id 
                         << "      \t- assigned to ticket: " 
                         << temp.header.get_id() << endl;
                    passive.erase(passive.begin());
                }
            }
        }
        void running_survey() {
            Ticket temp;
            string file;
            
            for (int i = 0; i < int(crew.size()); i++) {
                temp = crew.at(i).get_ticket();
                if (crew.at(i).assigned == true && temp.surveyed == false) {
                    if (temp.is_surveyed() == false) {
                        cout << "[Survey]: " << crew.at(i).id 
                             << "      \t- is still surveying" << endl;
                    }
                    else {
                        //--------------------------------------------------------------------
                        // step 2: Then, the nature of the damage 
                        // is used to assess the expected amount of repair time based on the most similar previous damages 
                        // from the station as well as the most similar damages from other stations. 
                        //--------------------------------------------------------------------
                        file = temp.file_path;
                        cout << "[Survey]: " << crew.at(i).id 
                             << "      \t- surveyed ticket: " 
                             << temp.header.get_id() << ", ";
                        temp = file_to_ticket(temp, file);
                        activate_ticket(temp);
                        crew.at(i).assigned = false;
                        crew.at(i).job_type = "idle";
                    }
                }
            }
        }
        
        /******************************************************************************
         * Repair Crew Management and Optimization
         ******************************************************************************/
        //--------------------------------------------------------------------
        // step 3: Next, it uses the 
        // number of serviced customers in Table I to estimate the total number of customer hour 
        // interruptions for all the five substations, assuming that there are enough crews to repair all 
        // substations at the same time.
        //--------------------------------------------------------------------
        //survey also define difficulty based on status
        //estimate which station has the most difficult ticket, assign more crews to that station
        //reallocate tot. ~ultize the reserve pool at first allocation.
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        // Step 5: Extend the solution for Step 4 so that the total time hospitals 
        // are without energy is minimized too. 
        //--------------------------------------------------------------------
        // Step 6: Extend the solution for Step 4 so that hospitals with more 
        // effective surgery teams are the least amount of time without power
        //--------------------------------------------------------------------

        //--------------------------------------------------------------------
        // step 4: However, the number of existing crews (TOT) is less than the 
        // required number. Find the dispatching scheme of the TOT crews, so that the total number of 
        // customer hours is minimized. 
        //--------------------------------------------------------------------
        void begin_repair() {
            cout << "\n==== BEGIN REPAIR FUNCTION ====\n";
            cout << "Active tickets: " << active.size() << ", Available crew: " << crew.size() << endl;
            
            // First pass: try to match crew with correct difficulty
            cout << "\n-- First pass: matching by difficulty --\n";
            for (int i = 0; i < int(crew.size()) && !active.empty(); i++) {
                if (crew.at(i).assigned == false) {
                    Ticket temp = active.at(0);
                    Y_search(temp);
                    temp.time_score = Y_time_score_avg();
                    temp.calculate_difficulty(hard_threshold);
                    
                    cout << "Ticket difficulty: " << (temp.hard ? "Hard" : "Easy") << endl;
                    cout << "Crew #" << i << " skill level: " << (crew.at(i).on_hard ? "Hard" : "Easy") << endl;
                    
                    // Check for difficulty match
                    if (crew.at(i).on_hard == temp.hard) {
                        crew.at(i).assign_ticket(temp);
                        crew.at(i).assigned = true;
                        cout << "[MATCH] Crew #" << i << " (" << crew.at(i).id 
                             << ") assigned to ticket: " << temp.header.get_id() << endl;
                        active.erase(active.begin());
                    }
                }
            }
            
            // Second pass: assign remaining tickets regardless of difficulty
            if (!active.empty()) {
                cout << "\n-- Second pass: assigning remaining tickets --\n";
                cout << "Remaining tickets: " << active.size() << endl;
                
                for (int i = 0; i < int(crew.size()) && !active.empty(); i++) {
                    if (crew.at(i).assigned == false) {
                        Ticket temp = active.at(0);
                        
                        cout << "Ticket difficulty: " << (temp.hard ? "Hard" : "Easy") << endl;
                        cout << "Crew #" << i << " skill level: " << (crew.at(i).on_hard ? "Hard" : "Easy") << endl;
                        
                        crew.at(i).assign_ticket(temp);
                        crew.at(i).assigned = true;
                        cout << "[MISMATCH] Crew #" << i << " (" << crew.at(i).id 
                             << ") assigned to ticket despite difficulty mismatch: " 
                             << temp.header.get_id() << endl;
                        active.erase(active.begin());
                    }
                }
            }
            
            // Summary
            cout << "\n==== ASSIGNMENT SUMMARY ====\n";
            cout << "Remaining tickets: " << active.size() << endl;
            int unassigned_crew = 0;
            for (int i = 0; i < int(crew.size()); i++) {
                if (!crew.at(i).assigned) {
                    unassigned_crew++;
                }
            }
            cout << "Unassigned crew members: " << unassigned_crew << endl;
            cout << "============================\n";
        }
        void running_repair() {
            // Keep track of how many crews are still repairing
            int still_repairing = 0;
            
            for (int i = 0; i < int(crew.size()); i++) {
                if (crew.at(i).assigned && crew.at(i).ticket.surveyed && !crew.at(i).ticket.repaired) {
                    if (crew.at(i).ticket.is_repaired()) {
                        cout << "[Repair]: " << crew.at(i).id 
                             << "      \t- repaired ticket: " << crew.at(i).ticket.header.get_id() << endl;
                        
                        crew.at(i).assigned = false;
                        crew.at(i).job_type = "idle";
                        archive_ticket(crew.at(i).ticket);
                    } else {
                        cout << "[Repair]: " << crew.at(i).id 
                             << "      \t- is still repairing" << endl;
                        display_ticket(crew.at(i).ticket);
                        still_repairing++;
                    }
                }
            }
            
            if (still_repairing > 0) {
                cout << still_repairing << " crews are still repairing tickets. Repair continues until completion." << endl;
            }
        }

        /******************************************************************************
         * Search and Analysis Functions
         ******************************************************************************/
        void Y_search(Ticket ticket) { 
            int highest_sim = 0;
            sim_rank.clear();
            
            // First pass: determine the highest similarity score
            for (Ticket& t : db.archive) {
                t.sim_value = 0;
                
                if (t.status.get_structure() == ticket.status.get_structure() && !ticket.status.get_structure().empty()) {
                    t.sim_value += 1;
                }
                if (t.status.get_voltage() == ticket.status.get_voltage() && !ticket.status.get_voltage().empty()) {
                    t.sim_value += 1;
                }
                if (t.status.get_ground() == ticket.status.get_ground() && !ticket.status.get_ground().empty()) {
                    t.sim_value += 1;
                }
                if (t.status.get_msp() == ticket.status.get_msp() && !ticket.status.get_msp().empty()) {
                    t.sim_value += 1;
                }
                if (t.status.get_harmonic() == ticket.status.get_harmonic() && !ticket.status.get_harmonic().empty()) {
                    t.sim_value += 1;
                }
                if (t.status.get_v_non_shunt() == ticket.status.get_v_non_shunt() && !ticket.status.get_v_non_shunt().empty()) {
                    t.sim_value += 1;
                }
                if (t.status.get_status() == ticket.status.get_status() && !ticket.status.get_status().empty()) {
                    t.sim_value += 1;
                }
                
                if (t.sim_value > highest_sim) {
                    highest_sim = t.sim_value;
                }
            }
            
            // Only add tickets with non-zero similarity
            if (highest_sim > 0) {
                // Second pass: add all tickets with the highest similarity score
                for (Ticket& t : db.archive) {
                    if (t.sim_value == highest_sim) {
                        sim_rank.push_back(t);
                    }
                }
                
                // If we don't have enough, add tickets with lower similarity scores
                if (sim_rank.size() < static_cast<size_t>(Y)) {
                    for (int sim = highest_sim - 1; sim > 0 && sim_rank.size() < static_cast<size_t>(Y); sim--) {
                        for (Ticket& t : db.archive) {
                            if (t.sim_value == sim) {
                                bool already_added = false;
                                for (const Ticket& st : sim_rank) {
                                    if (st.header.get_id() == t.header.get_id()) {
                                        already_added = true;
                                        break;
                                    }
                                }
                                if (!already_added && sim_rank.size() < static_cast<size_t>(Y)) {
                                    sim_rank.push_back(t);
                                }
                            }
                        }
                    }
                }
                
                // Limit to top Y
                if (sim_rank.size() > static_cast<size_t>(Y)) {
                    sim_rank.resize(static_cast<size_t>(Y));
                }
            }
        }

        int Y_time_score_avg() { //via Y_search
            int sum = 0;
            size_t count = min(static_cast<size_t>(Y), sim_rank.size());
            
            if (count == 0) return 0;
            
            for (size_t i = 0; i < count; i++) {
                sum += sim_rank.at(i).time_score;
            }
            return sum / static_cast<int>(count);
        }
        
        /******************************************************************************
         * Word Frequency and Analysis
         ******************************************************************************/
        void K_words(int n) { 
            if (db.word_dict.empty()) {
                cout << "No words found." << endl;
                return;
            }
            
            int display_count = min(n, int(db.word_dict.size()));
            for (int i = 0; i < display_count; i++) {
                cout << (i+1) << ". " << db.word_dict.at(i).word 
                     << " (" << db.word_dict.at(i).count << ")" << endl;
            }
            return;
        }
        void add_word(string word) {
            if (word == "" || word == "Comment:") {
                return;
            }
            for (int i = 0; i < int(db.word_dict.size()); i++) { 
                if (db.word_dict.at(i).word == word) {
                    db.word_dict.at(i).count++;
                    int j = i;
                    while ((j > 0) && (db.word_dict.at(j).count > db.word_dict.at(j-1).count)) {
                        Word temp = db.word_dict.at(j);
                        db.word_dict.at(j) = db.word_dict.at(j-1);
                        db.word_dict.at(j-1) = temp;
                        j--;
                    }
                    return;
                }
            }
            Word new_word(word, 1);
            db.word_dict.push_back(new_word);
        }
        /******************************************************************************
         * Display and Reporting Functions
         ******************************************************************************/
        void display_Y_search() {
            if (sim_rank.empty()) {
                cout << "No matching tickets found." << endl;
                return;
            }
            
            cout << "\n==== TOP " << min(static_cast<size_t>(Y), sim_rank.size()) << " SIMILAR TICKETS ====\n";
            
            for (size_t i = 0; i < min(static_cast<size_t>(Y), sim_rank.size()); i++) {
                cout << "\n--- Rank #" << (i+1) << " (Similarity Score: " << sim_rank.at(i).sim_value << ") ---\n";
                display_ticket(sim_rank.at(i));
            }
            
            // If there were tickets found but they had zero similarity
            if (sim_rank.size() > 0 && sim_rank.at(0).sim_value == 0) {
                cout << "\nNote: No tickets exactly match your criteria. Showing the most recent tickets instead.\n";
            }
        }
        void display_ticket(Ticket ticket) {
            cout << "--------------------------------" << endl;
            cout << ticket.col_names << endl;
            cout << *ticket << endl;
            cout << "Time score: " << ticket.time_score << endl;
            cout << "Difficulty: " << (ticket.hard ? "Hard" : "Easy") << endl;
            cout << "--------------------------------" << endl;
        }
        void display_crew() {
            for (int i = 0; i < int(crew.size()); i++) {
                cout << "--------------------------------" << endl;
                cout << "Crew: " << crew.at(i).id << "\n"
                     << "\tassigned: " << (crew.at(i).assigned) << " - " << crew.at(i).job_type << "\n"
                     << "\ton hard: " << (crew.at(i).on_hard) << endl;
                cout << "--------------------------------" << endl;
            }
        }
        void display_passive() {
            if (passive.empty()) {
                cout << "No passive tickets" << endl;
                return;
            }
            for(int i = 0; i < int(passive.size()); i++) {
                cout << "\nTicket:" << i+1 << endl;
                cout << "--------------------------------" << endl;
                cout << passive.at(i).col_names << endl;
                cout << *passive.at(i) << endl;
                cout << "--------------------------------" << endl;
            }
        }
        void display_active() { //display all active tickets of the station
            if (active.empty()) {
                cout << "No active tickets" << endl;
                return;
            }
            for(int i = 0; i < int(active.size()); i++) {
                cout << "\nTicket:" << i+1 << endl;
                cout << "--------------------------------" << endl;
                cout << active.at(i).col_names << endl;
                cout << *active.at(i) << endl;
                cout << "Difficulty: " << (active.at(i).hard ? "Hard" : "Easy") << endl;
                cout << "--------------------------------" << endl;
            }
            return;
        }
        void display_archive(Date start_date_obj, Date end_date_obj) {
            if (db.archive.empty()) {
                cout << "No archive tickets" << endl;
                return;
            }
            
            cout << "\n==== ARCHIVE TICKETS BETWEEN " << *start_date_obj << " AND " << *end_date_obj << " ====\n";
            
            int count = 0;
            for (int i = 0; i < int(db.archive.size()); i++) {
                Date ticket_date = db.archive.at(i).header.get_date();
                
                // Check if ticket is before start date
                if (ticket_date.get_year() < start_date_obj.get_year() ||
                    (ticket_date.get_year() == start_date_obj.get_year() && ticket_date.get_month() < start_date_obj.get_month()) ||
                    (ticket_date.get_year() == start_date_obj.get_year() && ticket_date.get_month() == start_date_obj.get_month() && 
                     ticket_date.get_day() < start_date_obj.get_day()) ||
                    (ticket_date.get_year() == start_date_obj.get_year() && ticket_date.get_month() == start_date_obj.get_month() && 
                     ticket_date.get_day() == start_date_obj.get_day() && ticket_date.get_hour() < start_date_obj.get_hour()) ||
                    (ticket_date.get_year() == start_date_obj.get_year() && ticket_date.get_month() == start_date_obj.get_month() && 
                     ticket_date.get_day() == start_date_obj.get_day() && ticket_date.get_hour() == start_date_obj.get_hour() && 
                     ticket_date.get_minute() < start_date_obj.get_minute()) ||
                    (ticket_date.get_year() == start_date_obj.get_year() && ticket_date.get_month() == start_date_obj.get_month() && 
                     ticket_date.get_day() == start_date_obj.get_day() && ticket_date.get_hour() == start_date_obj.get_hour() && 
                     ticket_date.get_minute() == start_date_obj.get_minute() && ticket_date.get_second() < start_date_obj.get_second())) {
                    continue;
                }
                
                // Check if ticket is after end date
                if (ticket_date.get_year() > end_date_obj.get_year() ||
                    (ticket_date.get_year() == end_date_obj.get_year() && ticket_date.get_month() > end_date_obj.get_month()) ||
                    (ticket_date.get_year() == end_date_obj.get_year() && ticket_date.get_month() == end_date_obj.get_month() && 
                     ticket_date.get_day() > end_date_obj.get_day()) ||
                    (ticket_date.get_year() == end_date_obj.get_year() && ticket_date.get_month() == end_date_obj.get_month() && 
                     ticket_date.get_day() == end_date_obj.get_day() && ticket_date.get_hour() > end_date_obj.get_hour()) ||
                    (ticket_date.get_year() == end_date_obj.get_year() && ticket_date.get_month() == end_date_obj.get_month() && 
                     ticket_date.get_day() == end_date_obj.get_day() && ticket_date.get_hour() == end_date_obj.get_hour() && 
                     ticket_date.get_minute() > end_date_obj.get_minute()) ||
                    (ticket_date.get_year() == end_date_obj.get_year() && ticket_date.get_month() == end_date_obj.get_month() && 
                     ticket_date.get_day() == end_date_obj.get_day() && ticket_date.get_hour() == end_date_obj.get_hour() && 
                     ticket_date.get_minute() == end_date_obj.get_minute() && ticket_date.get_second() > end_date_obj.get_second())) {
                    continue;
                }
                
                // If we get here, the ticket is within the date range
                count++;
                cout << "\nTicket:" << count << endl;
                cout << "--------------------------------" << endl;
                cout << db.archive.at(i).col_names << endl;
                cout << *db.archive.at(i) << endl;
                cout << "Time score: " << db.archive.at(i).time_score << endl;
                cout << "Difficulty: " << (db.archive.at(i).hard ? "Hard" : "Easy") << endl;
                cout << "--------------------------------" << endl;
            }
            
            if (count == 0) {
                cout << "No tickets found within the specified date range." << endl;
            } else {
                cout << "\nTotal tickets found: " << count << endl;
            }
        }
        void display_archive() { //display all archive tickets of the station
            if (db.archive.empty()) {
                cout << "No archive tickets" << endl;
                return;
            }
            for (int i = 0; i < int(db.archive.size()); i++) {
                cout << "\nTicket:" << i+1 << endl;
                cout << "--------------------------------" << endl;
                cout << db.archive.at(i).col_names << endl;
                cout << *db.archive.at(i) << endl;
                cout << "Time score: " << db.archive.at(i).time_score << endl;
                cout << "Difficulty: " << (db.archive.at(i).hard ? "Hard" : "Easy") << endl;
                cout << "--------------------------------" << endl;
            }
        }
};