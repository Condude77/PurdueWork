
const char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                          \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
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
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "IRCServer.h"
#include "HashTableVoid.h"

using namespace std;

int QueueLength = 5;
vector<string> usernameVector;
vector<string> passwordVector;
vector<string> roomVector;

fstream passwordFile;

HashTableVoid h; //stores username and passwords that are in each room
HashTableVoid h2; //stores messages and username for a room

/*int userTotal = 0;

struct UserList{
	char * username;
	char * password;
};

typedef UserList UserList;
UserList * UserArray;
*/

//test

int
IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);
  
	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1; 
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
			     (char *) &optval, sizeof( int ) );
	
	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			  (struct sockaddr *)&serverIPAddress,
			  sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}
	
	// Put socket in listening mode and set the 
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

void
IRCServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);

	initialize();
	
	while ( 1 ) {
		
		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
					  (struct sockaddr *)&clientIPAddress,
					  (socklen_t*)&alen);
		
		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}
		
		// Process request.
		processRequest( slaveSocket );		
	}
}

int
main( int argc, char ** argv )
{
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}
	
	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;

	// It will never return
	ircServer.runServer(port);
	
}

//
// Commands:
//   Commands are started y the client.
//
//   Request: ADD-USER <USER> <PASSWD>\r\n
//   Answer: OK\r\n or DENIED\r\n
//
//   REQUEST: GET-ALL-USERS <USER> <PASSWD>\r\n
//   Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//
//   REQUEST: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LIST-ROOMS <USER> <PASSWD>\r\n
//   Answer: room1\r\n
//           room2\r\n
//           ...
//           \r\n
//
//   Request: ENTER-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LEAVE-ROOM <USER> <PASSWD>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
//   Answer: OK\n or DENIED\n
//
//   Request: GET-MESSAGES <USER> <PASSWD> <LAST-MESSAGE-NUM> <ROOM>\r\n
//   Answer: MSGNUM1 USER1 MESSAGE1\r\n
//           MSGNUM2 USER2 MESSAGE2\r\n
//           MSGNUM3 USER2 MESSAGE2\r\n
//           ...\r\n
//           \r\n
//
//    REQUEST: GET-USERS-IN-ROOM <USER> <PASSWD> <ROOM>\r\n
//    Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//

void
IRCServer::processRequest( int fd )
{
	// Buffer used to store the comand received from the client
	const int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;
	
	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;
	
	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&
		read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}
		
		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}
	
	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;
	
	/*std::string commandArray[4];
	int c = 0;
	std::stringstream ssin(commandLine);
	while(ssin.good() && c < 4){
		ssin >> commandArray[c];
		++c;
	}

	const char * command = commandArray[0].c_str();
	const char * user = commandArray[1].c_str();
	const char * password = commandArray[2].c_str();
	const char * args = commandArray[3].c_str();
	*/

	printf("RECEIVED: %s\n", commandLine);

	/*printf("The commandLine has the following format:\n");
	printf("COMMAND <user> <password> <arguments>. See below.\n");
	printf("You need to separate the commandLine into those components\n");
	printf("For now, command, user, and password are hardwired.\n");

	const char * command = "ADD-USER";
	const char * user = "peter";
	const char * password = "spider";
	const char * args = "";
	*/
	
	char command1[1025];
	char user1[1025];
	char password1[1025];
	char args1[1025];
	memset(command1, 0, 1025);
	memset(user1, 0, 1025);
	memset(password1, 0, 1025);
	memset(args1, 0, 1025);

	sscanf(commandLine, "%s %s %s %[^\n]", command1, user1, password1, args1);
	const char * command = command1;
	const char * user = user1;
	const char * password = password1;
	const char * args = args1;

	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if(!strcmp(command, "CREATE-ROOM")){
		createRoom(fd, user, password, args);
	}
	else if(!strcmp(command, "LIST-ROOMS")){
		listRooms(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else {
		const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	// Send OK answer
	//const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);	
}

void
IRCServer::initialize()
{
	passwordFile.open("password.txt"); 	//file is organized user\npassword\n\n
	string word;
	while(getline(passwordFile, word)){
		usernameVector.push_back(word);
		getline(passwordFile, word);
		passwordVector.push_back(word);
	}
	passwordFile.close();
	
	/*userTotal = 0;
	userArray = (UserList *)malloc(1000 * sizeof(UserList));
	FILE * file = fopen("password.txt", "a+");
	*/

	// Open password file
	
	// Initialize users in room

	// Initalize message list

}

bool
IRCServer::checkPassword(int fd, const char * user, const char * password) {
	// Here check the password
	for(int i = 0; i < passwordVector.size(); i++){
		if((passwordVector[i] == password) && usernameVector[i] == user){
			return true;
		}
	}
	return false;
}

void
IRCServer::addUser(int fd, const char * user, const char * password, const char * args)
{
	// Here add a new user. For now always return OK.
	
	for(int i = 0; i < usernameVector.size(); i++){
		if(usernameVector[i] == user){			//Check if user already exists
			const char * msg = "DENIED\r\n";
			write(fd, msg, strlen(msg));
			return;
		}
	}

	usernameVector.push_back(user);
	passwordVector.push_back(password);
	passwordFile.open("password.txt", fstream::in | fstream::out | fstream::app);
	passwordFile << user << "\n" << password << "\n\n";
	passwordFile.close();
	const char * msg = "OK\r\n";
	write(fd, msg, strlen(msg));

	/*FILE * file = fopen("password.txt", "a+");
	char * username = strdup(user);
	char * pass = strdup(password);
	for(int i = 0; i < userTotal; i++){
		if(strcmp(UserArray[i].username, username) == 0){
			const char * msg = "DENIED\r\n";
			write(fd, msg, strlen(msg));
			return;
		}
	}
	UserArray[userTotal].username = username;
	UserArray[userTotal].password = pass;
	fprintf(file, "%s %s\n", username, pass);
	userTotal++;
	fclose(file);
	const char * msg =  "OK\r\n";
	write(fd, msg, strlen(msg));
	return;
	*/
}

void
IRCServer::createRoom(int fd, const char * user, const char * password, const char * args)
{
	if(!checkPassword(fd, user, password)){
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}
	for(int i = 0; i < roomVector.size(); i++){
		if(roomVector[i] == args){			//checks if room already exists
			const char * msg = "DENIED\r\n";
			write(fd, msg, strlen(msg));
			return;
		}
	}
	roomVector.push_back(args);
	const char * msg = "OK\r\n";
	write(fd, msg, strlen(msg));
	return;
}

void
IRCServer::listRooms(int fd, const char * user, const char * password, const char * args)
{
	if(!checkPassword(fd, user, password)){
                const char * msg = "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
		return;
        }
	vector<string> temp(roomVector);
	sort(temp.begin(), temp.end());
	for(int i = 0; i < roomVector.size(); i++){
		const char * msg = temp[i].c_str();
		write(fd, msg, strlen(msg));
		const char * msg2 = "\r\n";
		write(fd, msg2, strlen(msg2));
	}
}

void
IRCServer::enterRoom(int fd, const char * user, const char * password, const char * args)
{
	if(!checkPassword(fd, user, password)){
                const char * msg = "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	int check = 0;
	int roomNumber = 0;
	for(int i = 0; i < roomVector.size(); i++){
		if(roomVector[i] == args){
			check = 1;			//checks if the room exists
			break;
		}
		roomNumber++;
	}
	if(check){	//room exists
		h.insertUser(user, (void *)password, roomNumber);
		const char * msg = "OK\r\n";
		write(fd, msg, strlen(msg));
	}else{		//room does not exist
		const char * msg = "ERROR (No room)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void
IRCServer::leaveRoom(int fd, const char * user, const char * password, const char * args)
{
	if(!checkPassword(fd, user, password)){
                const char * msg = "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	int roomNumber = 0;
	for(int i = 0; i < roomVector.size(); i++){
                if(roomVector[i] == args){
                        break;
                }
                roomNumber++;
        }
	HashTableVoidIterator it(&h);
	if(it.userInRoom(user, roomNumber)){
		h.removeUser(user, roomNumber);
		const char * msg = "OK\r\n";
		write(fd, msg, strlen(msg));
	}else{
		const char * msg = "ERROR (No user in room)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void
IRCServer::sendMessage(int fd, const char * user, const char * password, const char * args)
{
	if(!checkPassword(fd, user, password)){
                const char * msg = "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	char room[100];
	char message[1025];
	sscanf(args, "%s %[^\n]", room, message);
	int roomNumber = 0;
        for(int i = 0; i < roomVector.size(); i++){
                if(roomVector[i] == room){
                        break;
                }
                roomNumber++;
        }
	HashTableVoidIterator it(&h);
	if(it.userInRoom(user, roomNumber)){
		h2.insertMessage(message, (void *)user, roomNumber);
		const char * msg = "OK\r\n";
		write(fd, msg, strlen(msg));
	}else{
		const char * msg = "ERROR (user not in room)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void
IRCServer::getMessages(int fd, const char * user, const char * password, const char * args)
{
	if(!checkPassword(fd, user, password)){
                const char * msg = "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	int lastMessageSent;
	char room[100];
	sscanf(args, "%d %[^\n]", &lastMessageSent, room);
	int roomNumber = 0;
	lastMessageSent++;
	for(int i = 0; i < roomVector.size(); i++){
                if(roomVector[i] == room){
                        break;
                }
                roomNumber++;
        }
	HashTableVoidIterator it(&h);
	void * temp;
	if(it.userInRoom(user, roomNumber)){
		const char * msg;
		HashTableVoidIterator it2(&h2);
		it2.retrieveMessages(fd, msg, temp, roomNumber, lastMessageSent);
	}else{
		const char * msg = "ERROR (User not in room)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void
IRCServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args)
{
	if(!checkPassword(fd, user, password)){
                const char * msg = "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	int roomNumber = 0;
        for(int i = 0; i < roomVector.size(); i++){
                if(roomVector[i] == args){
                        break;
                }
                roomNumber++;
        }
	const char * msg;
	void * temp;
	HashTableVoidIterator it(&h);
	it.retrieveUser(fd, msg, temp, roomNumber);
}

void
IRCServer::getAllUsers(int fd, const char * user, const char * password,const  char * args)
{
	if(!checkPassword(fd, user, password)){
                const char * msg = "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	vector<string> temp(usernameVector);
	sort(temp.begin(), temp.end());
	for(int i = 0; i < temp.size(); i++){
		const char * msg = temp[i].c_str();
		write(fd, msg, strlen(msg));
		const char * msg2 = "\r\n";
		write(fd, msg2, strlen(msg2));
	}
	const char * msg = "\r\n";
	write(fd, msg, strlen(msg));
}
