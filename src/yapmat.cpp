#include <cstddef>
#include <unistd.h>

#include <bits/stdc++.h>
#include <experimental/filesystem>

// NOTE: What if this doesn't exist on skae machines?
namespace fs = std::experimental::filesystem;

using namespace std;

string YDIR = "/tmp/yapmat/";

static void yapmat_manage(string name, char* argv[]) {

    cerr << "yapmat_manage: Not implemented yet";

    if (name == "") {
        // name = get_random_name();
    }

    // Create directory $YDIR/$name

    // fork() & exec() the wrapper command
    // passing all the right parameters to it

}

static void yapmat_list() {

    cerr << "yapmat_list: Not implemented yet";

    // List all directories under YDIR

    // Perhaps even print the contents of $YDIR/$proc/cmd

}

static void yapmat_logs(string id) {

    cerr << "yapmat_logs: Not implemented yet";

    if (id == "all") {

        // Iterate, read, and print $YDIR/$proc/stdout (err?)

    } else {

        // Read & print $YDIR/<id>/stdout (err?)

    }

}

static void yapmat_status(string id) {

    cerr << "yapmat_status: Not implemented yet";

    if (id == "all") {

        // Iterate over $YDIR/$proc
        // and print the status - running, stopped

    } else {

        // Just print status of $YDIR/<id>

    }

}

static void yapmat_start(string id) {

    cerr << "yapmat_start: Not implemented yet";

    // How does manage differ from start?

    // echo "running" > $YDIR/$id/status

}


static void yapmat_stop(string id) {

    // Do not delete the directory of the process
    string pdir = YDIR + "/" + id;

    if (!fs::is_directory(pdir))
    {
        cerr << "Process '" << id << "' is not being managed by yapmat." << endl;
        exit(1);
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

    ofstream status(pdir + "/" + "status", ofstream::out);
    status << "stopped";

}


static void yapmat_restart(string id) {

    cerr << "yapmat_restart: Not implemented yet";

    yapmat_stop(id);

    // Ensure that process isn't running anymore?

    yapmat_start(id);

}

static void yapmat_unmanage(string id) {

    cerr << "yapmat_unmanage: Not implemented yet";

    // Stop the running process
    yapmat_stop(id);

    // Delete the directory $YDIR/$id

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

        if (argc < 3) print_usage();

        yapmat_logs(argv[2]);

    } else if (command == "status") {

        if (argc < 3) print_usage();

        yapmat_status(argv[2]);

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
        cout << "default case" << endl;
        print_usage();
    }

}
