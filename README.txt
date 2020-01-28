# --------------------- #
# COP4610      Project1 #

# Paul Santora          #
# Jared Usher           #
# Hengwei Xing          #


# - FILE LISTING - #
 - main.c
 - utility.h
 - utility.c
 - makefile
 - README.txt

Tar File Name
 project1_Santora_Usher_Xing.tar


# - MAKEFILE DESCRIPTION - #

 makefile creates main.o object 
 file and shell executable

To Compile Project

   make
   
To remove *.o and shell targets

   make clean


# - DIVISION OF LABOR - #

Hengwei Xing
 - Part 3: Prompt
 - Part 4: Shortcut Resolution
 - Part 5: $PATH Resolution

Jared Usher
 - Part 6: Execution
 - Part 7: I/O Redirection

Paul Santora
 - Part 8: Pipes
 - Part 10: Built-ins
   - exit
   - cd 
   - echo

 /*
 - Part 9: Background Processing
 - Part 10:
   - Jobs
 */
 

# - CODE INFORMATION - #

 The Max column-width for code
 writen by us is 99.
 
 The Functions AddToken and AddNull
 which were provided for us have a 
 column width of 107.

# - KNOWN BUGS - #
 - cd . 
   if first command entered
   in the shell, directory changes
   to NULL, then fixes itself after 
   next command is entered
 - cd /
   can not go to root directory
 - External Commands
   cannot determine if command
   is external command or not
 
 
 
 
