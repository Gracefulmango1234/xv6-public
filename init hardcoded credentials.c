// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


#define MAX_ATTEMPTS 3
#ifndef USERNAME
#define USERNAME "defaultuser"
#endif

#ifndef PASSWORD
#define PASSWORD "defaultpass"
#endif

void login() {  //int
    char uname[20], pass[20];
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS) {
        printf(1, "Enter Username: ");
        gets(uname, sizeof(uname));
        uname[strlen(uname) - 1] = 0;  // Remove newline character

        if (strcmp(uname, USERNAME) != 0) {
            printf(1, "Invalid Username. Try again.\n");
            attempts++;
            continue;
        }

        printf(1, "Enter Password: ");
        gets(pass, sizeof(pass));
        pass[strlen(pass) - 1] = 0;  // Remove newline character

        if (strcmp(pass, PASSWORD) == 0) {
            printf(1, "Login successful\n");
            return ; //return 0;
        } else {
            printf(1, "Incorrect Password. Try again.\n");
            attempts++;
        }
    }

    printf(1, "Too many failed attempts. Access denied.\n");
    // return 1;
    exit();
}

char *argv[] = { "sh", 0 };

// void print_credentials() {
//   printf(1, "USERNAME: %s\n", USERNAME);
//   printf(1, "PASSWORD: %s\n", PASSWORD);
// }

int
main(int argc, char* argv[]) //
{
  // login(); // Call the login function before starting the shell
  // print_credentials();
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){
    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){ //ask for login only when entering the child process of init i.e. opening shell in this case
        login(); // Call the login function before starting the shell
        exec("sh", argv);
        printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
}