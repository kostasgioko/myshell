# myshell
A custom shell for the university course 'Operating Systems' project.

# Unix Shell
This program provides a command line user interface for Unix-like operating systems. The shell can work in interactive mode, where the user inputs commands in real time, or in batch mode where the shell executes a series of commands defined in a batchfile.

# Installation & Execution
OS X & Linux:

1) Clone the repository.
2) Open a terminal inside the directory.
3) Type "make" to create the executable.
4) Type "bin/myshell" for interactive mode and "bin/myshell batchfile" for batch mode.
5) Type "make purge" if you want to delete the executable.

# Usage example
1) Interactive Mode:

Some example commands to show how the shell works.

giokotos_8791>pwd\
/home/kostas/Desktop\
giokotos_8791>ls -l\
total 44\
-rwxr-xr-x 1 kostas kostas 13168 Ιαν  13 13:16 assignment\
-rw-rw-r-- 1 kostas kostas  7440 Ιαν  13 13:16 assignment.c\
-rw-r--r-- 1 kostas kostas    82 Ιαν  10 12:19 batchfile\
-rw-r--r-- 1 kostas kostas    20 Ιαν  10 12:03 file\
-rw-r--r-- 1 kostas kostas    33 Ιαν  10 12:05 file2\
-rw-r--r-- 1 kostas kostas   286 Ιαν  10 13:01 Makefile\
-rw-r--r-- 1 kostas kostas  2385 Ιαν  13 13:16 README\
giokotos_8791>cat file ; grep hello file2\
apple\
banana\
orange\
hello there\
hello back\
giokotos_8791>cat > file3 && cat file3\
this is a nice day\
this is a nice day\
giokotos_8791>quit\

2) Batch Mode:

Consider a batchfile containing the following commands:

pwd
ps
ls -la
quit

The shell will have the following output:

/home/kostas/Desktop\
  PID TTY          TIME CMD\
 2631 pts/0    00:00:00 bash\
 2760 pts/0    00:00:00 assignment\
 2762 pts/0    00:00:00 ps\
total 56\
drwxr-xr-x  2 kostas kostas  4096 Ιαν  13 13:22 .\
drwxr-xr-x 18 kostas kostas  4096 Ιαν  13 12:52 ..\
-rwxr-xr-x  1 kostas kostas 13168 Ιαν  13 13:16 assignment\
-rw-rw-r--  1 kostas kostas  7440 Ιαν  13 13:16 assignment.c\
-rw-r--r--  1 kostas kostas   101 Ιαν  13 13:22 batchfile\
-rw-r--r--  1 kostas kostas    20 Ιαν  10 12:03 file\
-rw-r--r--  1 kostas kostas    38 Ιαν  13 13:20 file2\
-rw-r--r--  1 kostas kostas    19 Ιαν  13 13:21 file3\
-rw-r--r--  1 kostas kostas   286 Ιαν  10 13:01 Makefile\
-rw-r--r--  1 kostas kostas  2385 Ιαν  13 13:16 README\
