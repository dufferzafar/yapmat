#include <cstddef>
#include <unistd.h>

#include <bits/stdc++.h>
#include <experimental/filesystem>

// NOTE: What if this doesn't exist on skae machines?
namespace fs = std::experimental::filesystem;

using namespace std;

string YDIR = "/tmp/yapmat";

static void yapmat_manage(string name, char* argv[]) {

    cerr << "yapmat_manage: Not implemented yet";

    if (name == "") {
        // name = get_random_name();
    }

    // Create directory $YDIR/$name

    // fork() & exec() the wrapper command
    // passing all the right parameters to it

}

static void yapmat_list() 
{

    cout << "Name" << "\t : \t" << "Command Line" << endl
         << "-------------------------------" << endl;

    for (const auto &entry : fs::directory_iterator(YDIR)) 
    {
        string path = entry.path();
        string id = path.substr(path.find_last_of("/") + 1);

        ifstream cmdline_file(path + "/" + "cmdline");
        string cmdline; getline(cmdline_file, cmdline);

        cout << id << "\t : \t" << cmdline << endl;
    }

}

static void yapmat_logs(string id) {

    if (id == "all") 
    {
        for (const auto &entry : fs::directory_iterator(YDIR))
        {
            string path = entry.path();
            string id = path.substr(path.find_last_of("/") + 1);


            cout << endl 
                 << "--------------------------------------------------"
                 << endl
                 << "Logs of Process: '" << id << "'" 
                 << endl
                 << "--------------------------------------------------"
                 << endl << endl;

            ifstream stdout_file(path + "/" + "stdout");
            string line;

            while(getline(stdout_file, line)) 
                cout << line << endl;

        }
    } 
    else {

        cout << endl 
             << "--------------------------------------------------"
             << endl
             << "Logs of Process: '" << id << "'" 
             << endl
             << "--------------------------------------------------"
             << endl << endl;

        ifstream stdout_file(YDIR + "/" + id + "/" + "stdout");
        string line;

        while (getline(stdout_file, line))
            cout << line << endl;
    }

}

static void yapmat_status(string id) 
{
    cout << "Name" << "\t : \t" << "Status"   << endl
         << "-------------------------------" << endl;

    if (id == "all") 
    {

        for (const auto &entry : fs::directory_iterator(YDIR))
        {
            string path = entry.path();
            string id = path.substr(path.find_last_of("/") + 1);

            ifstream status_file(path + "/" + "status");
            string status; status_file >> status;

            cout << id << "\t : \t" << status << endl;
        }
    
    } 
    else 
    {
        ifstream status_file(YDIR + "/" + id + "/" + "status");
        string status; status_file >> status;

        cout << id << "\t : \t" << status << endl;
    }
}

static void yapmat_start(string id) {

    cerr << "yapmat_start: Not implemented yet";

    // How does manage differ from start?

    // echo "running" > $YDIR/$id/status

}


static void yapmat_stop(string id, bool stop_exit = true) 
{
    string pdir = YDIR + "/" + id;

    if (!fs::is_directory(pdir)) {
        cerr << "Process '" << id << "' is not being managed by yapmat." << endl;
        exit(1);
    }

    // Read the status of the process and do nothing if it is already stopped
    ifstream status_file_in(pdir + "/" + "status");
    string status; status_file_in >> status;

    if (status == "stopped") 
    {
        cerr << "Process '" << id << "' is already stopped." << endl;

        if (stop_exit)
            exit(1);
        else
            return;
    }

    ifstream wpidfile(pdir + "/" + "wrapper.pid");
    int wpid; wpidfile >> wpid;

    cout << "Sending SIGKILL to wrapper process '"
         << id << "' with pid " << wpid << endl;

    // Send SIGKILL to wrapper process
    if (kill(wpid, SIGKILL))
        cerr << "Failed to stop process." << endl;
    else
        cout << "Process stopped successfully." << endl;

    ofstream status_file_out(pdir + "/" + "status", ofstream::out);
    status_file_out << "stopped";

}

static void yapmat_restart(string id) {

    cerr << "yapmat_restart: Not implemented yet";

    yapmat_stop(id);

    // Ensure that process isn't running anymore?

    yapmat_start(id);

}

static void yapmat_unmanage(string id) 
{
    // Stop the running process
    yapmat_stop(id, false);

    // Delete process' directory
    string pdir = YDIR + "/" + id;
    cout << "Removing directory: " << pdir;

    fs::remove_all(pdir);
}

static void yapmat_kill(string id, int signum) {

    cerr << "yapmat_kill: Not implemented yet";

    // Create a signal handler in the wrapper program
    // that forwards all signals received to the child program

    // Read process id from $YDIR/$id/wpid

    // Send signal signum to

}

static void print_usage(){
    cerr << "Usage: yapmat <command> [<args>]" << endl << endl
         << "       yapmat manage [-n name] executable" << endl
         << "       yapmat list" << endl
         << "       yapmat logs [<id>|all]" << endl
         << "       yapmat status [<id>|all]" << endl
         << "       yapmat start|stop|restart <id> " << endl
         << "       yapmat unmanage <id>" << endl;

    exit(1);
}

int main(int argc, char* argv[])
{

    if (argc < 2) print_usage();

    string command = argv[1];
    if (command == "manage") {

        if (argc < 3) print_usage();

        // This assumes that -n would always be at position 2
        string name = (string (argv[2]) == "-n") ? argv[3] : "";

        // Perhaps send something else instead of the entire argv?
        yapmat_manage(name, argv);

    } else if (command == "list") {

        yapmat_list();

    } else if (command == "logs") {

        string id = (argc > 2) ? argv[2] : "all";

        yapmat_logs(id);

    } else if (command == "status") {

        string id = (argc > 2) ? argv[2] : "all";
        yapmat_status(id);

    } else if (command == "start") {

        if (argc < 3) print_usage();

        yapmat_start(argv[2]);

    } else if (command == "stop") {

        if (argc < 3) print_usage();

        yapmat_stop(argv[2]);

    } else if (command == "restart") {

        if (argc < 3) print_usage();

        yapmat_restart(argv[2]);

    } else if (command == "unmanage") {

        if (argc < 3) print_usage();

        yapmat_unmanage(argv[2]);

    } else if (command == "kill") {

        if (argc < 4) print_usage();

        // if argv[2] is a number then

        yapmat_kill(argv[3], stoi(argv[2]));

    } else {
        cout << "Unknown command: " << command << endl << endl;

        print_usage();
    }

}
