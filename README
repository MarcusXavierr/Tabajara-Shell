# Tabajara Shell
This is just a simple shell created for educational purposes. The code is implemented in C and it's pretty straightforward.

## Compiling the shell
To compile the project, you just need a C compiler and the `make` utility.

First generate the build directory and the makefile:
```bash
mkdir build && cd build
cmake ..
```

Then compile the project:
```bash
make
```

The shell is a simple interactive shell. You can run it with the following command:
```bash
./tsh
```

## Commands
After runing your shell, you can run the following built-in commands:
```bash
quit
fg %job_id OR fg PID
bg %job_id OR bg PID
jobs
```
You can also run other executables specifying the full path to the executable (I'll add short path support later):

```bash
/bin/ls
/bin/cat /etc/passwd
/bin/echo "Hello World!"
./tsh
```

## Processes
Child processes are created with `fork()` and are represented in memory as a `job`.

Each job has a `pid` and a `state`.

Jobs states: FG (foreground), BG (background), ST (stopped).
Job state transitions and enabling actions:
 FG -> ST  : ctrl-z
 ST -> FG  : fg command
 ST -> BG  : bg command
 BG -> FG  : fg command

At most 1 job can be in the FG state.
