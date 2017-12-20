#include "pipe_networking.h"
#include <signal.h>
#include <ctype.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("wkp");
    exit(0);
  }
}

int main() {
  signal(SIGINT,sighandler);


  while(1){
    int from_client = server_setup();
    int f = fork();
    if (!f){
      subserver(from_client);
    }
    else{
      remove("wkp");
    }
  }
}

void subserver(int from_client) { 
  int to_client = server_connect(from_client);
  char buf[BUFFER_SIZE];
  while(read(from_client,buf,BUFFER_SIZE)){
    printf("editing %s\n", buf);
    process(buf);
    write(to_client,buf,BUFFER_SIZE);
  }  					    
}

void process(char * s) {
  int i;
  for(i=0; i<strlen(s); i += 2){
    s[i] = toupper(s[i]);
    i++;
  }
}
