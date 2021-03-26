const char* usage =
"                                                               \n"
"usage: myhttpd [-f|-t|-p] [<port>]                             \n"
"                                                               \n"
"   -f: Create a new process for each request                   \n"
"   -t: Create a new thread for each request                    \n"
"   -p: Pool of threads                                         \n"
"                                                               \n"
"   1024 < [<port>] < 65536                                     \n"
"                                                               \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

int QueueLength = 5;
int cMode = 0; //0 = none, 1 = -f, 2 = -t, 3 = -p
int secretKey = 99;

pthread_mutex_t mt;
pthread_mutexattr_t mattr;

void processRequest(int socket);
void processThreadRequest(int socket);
void poolSlave(int socket);

extern "C" void killZombie(int sig);

int main(int argc, char** argv){
  // Add your HTTP implementation here
  
  int port;
  
  if(argc == 1){
    // Set to default port
    port = 42069;
  }

  if(argc == 3){
    // Get the command mode
    if(argv[1][0] == '-'){
      if(argv[1][1] == 'f'){
	cMode = 1;
      }else if(argv[1][1] == 't'){
	cMode = 2;
      }else if(argv[1][1] == 'p'){
	cMode = 3;
      }else{
	fprintf(stderr, "%s", usage);
	exit(-1);
      }
    }
    if(atoi(argv[2]) == 0){
      fprintf(stderr, "%s", usage);
      exit(-1);
    }else{
      port = atoi(argv[2]);
    }
  }else if(argc == 2){
    cMode = 0;
    // Get the port
    port = atoi(argv[1]);
  }else{
    fprintf(stderr, "%s", usage);
    exit(-1);
  }

  // Check if port is within values
  if(port < 1024 || port > 65536){
    fprintf(stderr, "%s", usage);
    exit(-1);
  }

  // Check for zombie process
  struct sigaction sigA;
  sigA.sa_handler = killZombie;
  sigemptyset(&sigA.sa_mask);
  sigA.sa_flags = SA_RESTART;

  int sAerr = sigaction(SIGCHLD, &sigA, NULL);
  if(sAerr){
    perror("sigaction");
    exit(-1);
  }

  // Set the IP address and port for this server
  struct sockaddr_in serverIPAddress; 
  memset(&serverIPAddress, 0, sizeof(serverIPAddress));
  serverIPAddress.sin_family = AF_INET;
  serverIPAddress.sin_addr.s_addr = INADDR_ANY;
  serverIPAddress.sin_port = htons((u_short) port);

  // Allocate a socket
  int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
  if(masterSocket < 0){
    perror("socket");
    exit(-1);
  }

  // Set socket options to reuse port. Otherwise we will
  // have to wait about 2 minutes before reusing the sae port number
  int optval = 1; 
  int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &optval, sizeof(int));

  // Bind the socket to the IP address and port
  int error = bind(masterSocket, (struct sockaddr *)&serverIPAddress, sizeof(serverIPAddress));
  if(error){
    perror("bind");
    exit(-1);
  }

  // Put socket in listening mode and set the 
  // size of the queue of unprocessed connections
  error = listen(masterSocket, QueueLength);
  if(error){
    perror("listen");
    exit(-1);
  }

  if(cMode == 3){
    pthread_mutexattr_init(&mattr);
    pthread_mutex_init(&mt, &mattr);
    pthread_t tid[5];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    for(int i = 0; i < 5; i++){
      pthread_create(&tid[i], &attr, (void * (*)(void *))poolSlave, (void *)masterSocket);
    }
    pthread_join(tid[0], NULL);
  }else{
    while(1){
      // Accept incoming connections
      struct sockaddr_in clientIPAddress;
      int alen = sizeof(clientIPAddress);
      int slaveSocket = accept(masterSocket, (struct sockaddr *)&clientIPAddress, (socklen_t*)&alen);

      if(slaveSocket < 0){
        perror("accept");
        exit(-1);
      }

      if(cMode == 1){
        pid_t slave = fork();
        if(!slave){
          // Process request
	  processRequest(slaveSocket);

	  // Close socket
	  close(slaveSocket);

	  exit(1);
        }
      }else if(cMode == 2){
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
        pthread_create(&tid, &attr, (void * (*)(void *))processThreadRequest, (void *)slaveSocket);
      }else{
        // Process request
        processRequest(slaveSocket);

        // Close socket
        close(slaveSocket);
      }
    }
  }
}

void processRequest(int socket){
  // Buffer to store name from client
  const int size = 1024;
  char currString[size + 1];
  int length = 0;
  int n;

  // Read characters
  unsigned char newChar;
  unsigned char oldChar = 0;

  // Flag for GET request
  int GET = 0;

  // Document path request
  char path[size + 1] = {0};

  while(n = read(socket, &newChar, sizeof(newChar))){
    if(newChar == ' '){
      if(!GET){
	GET = 1;
      }else{
	currString[length] = '\0';
	strcpy(path, currString);
      }
    }else if(newChar == '\n' && oldChar == '\r'){
      break;
    }else{
      oldChar = newChar;
      if(GET){
	currString[length] = newChar;
	length++;
      }
    }
  }

  // Check for end of request form
  while((n = read(socket ,&newChar, sizeof(newChar))) > 0){
    if(newChar == '\r'){
      n = read(socket, &newChar, sizeof(newChar));
      if(newChar == '\n'){
        n = read(socket, &newChar, sizeof(newChar));
	if(newChar == '\r'){
	  n = read(socket, &newChar, sizeof(newChar));	
	  if(newChar == '\n'){
	    break;
	  }	
	}
      }
    }	
  }

  // Check for secretKey
  if(!strncmp(path, "/99", strlen("/99"))){
    char temp[size + 1] = {0};
    strncpy(temp, path + 3, sizeof(path) - 3);
    temp[sizeof(path) - 3] = '\0';
    strcpy(path, temp);
  }

  // Map document path to server path
  char directory[size + 1] = {0};
  getcwd(directory, sizeof(directory));
  int cgi = 0;

  if(path[0] == '/' && path[1] != '\0'){
    int i = 0;
    char * start = (char *)calloc(1024, sizeof(char));
    while(path[i + 1] != '\0'){
      if(path[i + 1] == '/'){
	break;
      }
      if(path[i + 1] == '?'){
	break;
      }
      start[i] = path[i + 1];
      i++;
    }
    if(!strncmp(start, "/cgi-bin", strlen("/cgi-bin"))){
      cgi = 1;
      strcat(directory, "/http-root-dir/");
      strcat(directory, path);
    }else if(!strncmp(start, "/htdocs", strlen("/htdocs"))){
      strcat(directory, "/http-root-dir/");
      strcat(directory, path);
    }else if(!strncmp(start, "/icons", strlen("/icons"))){
      strcat(directory, "/http-root-dir/");
      strcat(directory, path);
    }
  }else{
    // No file selected
    // Give a default file
    if(!strcmp(path, "/")){
      strcpy(path, "/index.html");
    }  
    strcat(directory, "/http-root-dir/htdocs");
    strcat(directory, path);
  }

  // CGI-BIN stuff
  setenv("REQUEST_METHOD", "GET", 1);
  char * envVar = strstr(directory, "?");

  if(envVar != NULL){
    envVar++;
    char * temp;
    temp = strdup(envVar);
    envVar = temp;
    directory[strlen(directory) - (strlen(envVar) + 1)] = '\0';
    setenv("QUERY_STRING", strdup(envVar), 1);
    setenv("REQUEST_METHOD", "GET", 1);
    
    if(strstr(envVar, "=")){
      envVar[strlen(envVar)] = '&';
      envVar[strlen(envVar) + 1] = '\0';
      char * vv;
      vv = strtok(envVar, "&");
      while(vv != NULL){
	char * val = vv;
	int i = 0;
	while(*val != '='){
	  val++;
	  i++;
	}
	char * tmp;
	tmp = strdup(val + 1);
	vv[i] = '\0';
	setenv(vv, tmp, 1);
	vv = strtok(NULL, "&");
      }
    }
  }else{
    //setenv("QUERY_STRING", NULL, 1);
  }


  if(strstr(path, "..") != 0){
    char resolved[size + 1] = {0};
    char *res = realpath(path, resolved);
    if(res){
      if(strlen(resolved) >= strlen(directory) + 21){
	strcpy(directory, resolved);
      }
    }
  }

  // Determine content type
  char type[size + 1] = {0};
  
  if(strstr(path, ".html") != 0){
    strcpy(type, "text/html");
  }else if(strstr(path, ".jpg") != 0){
    strcpy(type, "image/jpg");
  }else if(strstr(path, ".gif") != 0){
    strcpy(type, "image/gif");
  }else{
    strcpy(type, "text/plain");
  }

  // Open the requested document
  FILE * document;
  if(strstr(type, "image/") != 0){
    document = fopen(directory, "rb");
  }else{
    document = fopen(directory, "r");
  }
  
  if(document > 0){
    write(socket, "HTTP/1.1 200 Document follows\r\n", 31);
    write(socket, "Server: CS-252-lab5\r\n", 21);

    if(cgi){
      fclose(document);
      FILE * fp = popen(directory, "r");
      long count = 0;
      char c;
      while(count = read(fileno(fp), &c, sizeof(c))){
	if(write(socket, &c, sizeof(c)) != count){
	  perror("write");
	}
      }
      fclose(fp);
    }else{
      write(socket, "Content-Type: ", 14);
      write(socket, type, strlen(type));
      write(socket, "\r\n\r\n", 4);

      long count = 0;
      char c;
      while(count = read(fileno(document), &c, sizeof(c))){
        if(write(socket, &c, sizeof(c)) != count){
	  perror("write");
        } 
      }
      fclose(document);
    }
  }else{
    write(socket, "HTTP/1.1 404 File Not Found\r\n", 29);
    write(socket, "Server: CS-252-lab5\r\n", 21);
    write(socket, "Content-Type: ", 14);
    write(socket, type, strlen(type));
    write(socket, "\r\n\r\n", 4);
    write(socket, "File not found", 14);
  }
}

void processThreadRequest(int socket){
  processRequest(socket);
  close(socket);
}

extern "C" void killZombie(int sig){
  int pid = wait3(0, 0, NULL);
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

void poolSlave(int socket){
  while(1){
    pthread_mutex_lock(&mt);
    
    // Get incoming connections
    struct sockaddr_in clientIPAddress;
    int alen = sizeof(clientIPAddress);
    int slaveSocket = accept(socket, (struct sockaddr *)&clientIPAddress, (socklen_t*)&alen);

    pthread_mutex_unlock(&mt);

    if(slaveSocket < 0){
      perror("accept");
      exit(-1);
    }

    // Process request
    processRequest(slaveSocket);

    // Close socket
    close(slaveSocket);
  }
}
