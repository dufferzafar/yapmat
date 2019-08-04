// For fork etc function
#include <cstddef>
#include <unistd.h>

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// POSIX file control
#include <fcntl.h>

#include <bits/stdc++.h>
#include <experimental/filesystem>
#include <memory>

using namespace std;

// NOTE: What if this doesn't exist on skae machines?
namespace fs = std::experimental::filesystem;

/*
Launches a process and ensures that it is always running.

Usage: wrapper <name> <executable> [<args>...]
*/

// TODO: Create a Process Class / Struct?

string YDIR = "/tmp/yapmat/";
string PNAME, PDIR;

static void daemonize()
{
    // ======================================================================= //
    //          https://github.com/pasce/daemon-skeleton-linux-c
    // ======================================================================= //

    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred - why could this happen? */
    if (pid < 0)
    {
        exit(1);
    }

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(0);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(1);

    /* Catch, ignore and handle signals */
    /* TODO: Implement a working signal handler */
    // signal(SIGCHLD, SIG_IGN);
    // signal(SIGHUP, SIG_IGN);
    // signal(SIGALRM, SIG_IGN);

    // Block all signals?
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    /* Fork off for the second time so there we have no tty */
    // For why: https://stackoverflow.com/questions/881388/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(1);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(0);

    /* Set new file permissions on all files created */
    // umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    // chdir("/");

    /* Close all open file descriptors */
    // close(0); close(1); close(2);
    // for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    // {
    //     close(x);
    // }

    {
    }
}

static void setup(int argc, char* argv[])
{
    // Create main yapmat directory
    if (!fs::is_directory(YDIR) || !fs::exists(YDIR))
    {
        fs::create_directory(YDIR);
    }

    // Get the process to launch from argv
    PNAME = argv[1];
    PDIR = YDIR + "/" + PNAME;

    // TODO: Move the directory creation logic to yapmat?
    // Create a directory for this process under $DIR
    if (!fs::is_directory(PDIR) || !fs::exists(PDIR))
    {
        fs::create_directory(PDIR);
    }
    // else
    // {
    //     cerr << "A process with same name is already being managed.";
    //     exit(1);
    // }

    // Write the entire command string to $PDIR/cmd
    ofstream cmd(PDIR + "/" + "cmd", ofstream::out);
    for (int i = 2; i < argc; i++)
    {
        cmd << argv[i] << " ";
    }
}

static void launch_process(int argc, char* argv[])
{
    pid_t pid = fork();

    if (pid < 0)
        exit(1);

    // Child
    if (pid == 0)
    {
        // Open files in $DIR/$pname/ to write to
        string stdout_path = PDIR + "/" + "stdout";
        int stdout = open(stdout_path.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);

        string stderr_path = PDIR + "/" + "stderr";
        int stderr = open(stderr_path.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);

        // Redirect output of child process to files
        dup2(stdout, 1);
        dup2(stderr, 2);

        // These FDs are no longer needed as the dup'ed handles are sufficient
        close(stdout);
        close(stderr);

        // Only use argv[2:]
        char *args[argc-1];
        for (int i = 2; i < argc; i++)
            args[i-2] = const_cast<char *>(argv[i]);
        args[argc-2] = nullptr;

        if (execvp(args[0], args) == -1) {
            exit(255);
        }
    }

    // Parent
    else
    {
        int status;

        // wait for the child to stop
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            int code = WEXITSTATUS(status);

            if (code == 255)
            {
                cerr << "Failed to execute process" << endl;
                exit(1);
            }
            else
            {
                // Perhaps log some info into a file?
                // cout << "Child stopped: " << pid << " code: " << code << endl;

                // Launch process again
                launch_process(argc, argv);
            }
        }

        else if (WIFSIGNALED(status))
        {
            int sig = WTERMSIG(status);
            cerr << "Failed because of signal: " << sig << endl;
        }

        else
        {
            // cerr << "Dunno what happened";
            exit(1);
        }
    }
}

int main(int argc, char* argv[])
{

    // Argument sanitation
    if (argc < 3) {
        cerr << "Usage: wrapper <name> <executable> [<args>...]";
        exit(1);
    }

    setup(argc, argv);

    // ======================================================================= //
    // Daemonize myself so that I detach from the current terminal
    // ======================================================================= //

    daemonize();

    // ======================================================================= //
    // Now that we have a daemon, fork() once again and execvp the process
    // ======================================================================= //

    launch_process(argc, argv);

    return 0;
}
