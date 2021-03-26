
#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

GtkListStore * list_rooms;
GtkListStore * list_users;
GtkListStore * list_messages;

GtkWidget * tree;
GtkTreeSelection * selection;

char * username = (char *)malloc(20 * sizeof(char));
char * password = (char *)malloc(20 * sizeof(char));
char * room = (char *)malloc(20 * sizeof(char));
char * message = (char *)malloc(300 * sizeof(char));
char * host = "localhost";
char * sport = (char *)malloc(20 * sizeof(char));
int port = 2040;
int roomCount = 1;

#define MAX_MESSAGES 100
#define MAX_MESSAGE_LEN 300
#define MAX_RESPONSE (20 * 1024)

int lastMessage = 0;
 
int open_client_socket(char * host, int port) {
        // Initialize socket address structure
        struct  sockaddr_in socketAddress;
 
        // Clear sockaddr structure
        memset((char *)&socketAddress,0,sizeof(socketAddress));
 
        // Set family to Internet 
        socketAddress.sin_family = AF_INET;
 
        // Set port
        socketAddress.sin_port = htons((u_short)port);
 
        // Get host table entry for this host
        struct  hostent  *ptrh = gethostbyname(host);
        if ( ptrh == NULL ) {
                perror("gethostbyname");
                exit(1);
        }
 
        // Copy the host ip address to socket address structure
        memcpy(&socketAddress.sin_addr, ptrh->h_addr, ptrh->h_length);
 
        // Get TCP transport protocol entry
        struct  protoent *ptrp = getprotobyname("tcp");
        if ( ptrp == NULL ) {
                perror("getprotobyname");
                exit(1);
        }
 
        // Create a tcp socket
        int sock = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
        if (sock < 0) {
                perror("socket");
                exit(1);
        }
 
        // Connect the socket to the specified server
        if (connect(sock, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) < 0) {
                 perror("connect");
                 exit(1);
        }
 
        return sock;
}
int sendCommand(char * host, int port, char * command, char * user, char * password, char * args, char * response) {
        int sock = open_client_socket( host, port);
 
        // Send command
        write(sock, command, strlen(command));
        write(sock, " ", 1);
        write(sock, user, strlen(user));
        write(sock, " ", 1);
        write(sock, password, strlen(password));
        write(sock, " ", 1);
        write(sock, args, strlen(args));
        write(sock, "\r\n",2);
 
        // Keep reading until connection is closed or MAX_REPONSE
        int n = 0;
        int len = 0;
        while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
                len += n;
        }
 
        //printf("response:%s\n", response);

        close(sock);
}
 
void printUsage(){
        printf("Usage: talk-client host port user password\n");
        exit(1);
}
 
static void update_list_users(){
        GtkTreeIter iterators;
        char * temp = (char *)malloc(20 * sizeof(char));
        gchar * msg;
        char response[MAX_RESPONSE];
        sendCommand(host, port, "GET-USERS-IN-ROOM", username, password, room, response);
        temp = strdup(response);
        temp = strtok(temp, "\r\n");
        msg = g_strdup_printf("%s", temp);
        gtk_list_store_append(GTK_LIST_STORE(list_users), &iterators);
        gtk_list_store_set(GTK_LIST_STORE(list_users), &iterators, 0, msg, -1);
        while((temp = strtok(NULL, "\r\n")) != NULL){
                msg = g_strdup_printf("%s", temp);
                gtk_list_store_append(GTK_LIST_STORE(list_users), &iterators);
                gtk_list_store_set(GTK_LIST_STORE(list_users), &iterators, 0, msg, -1);
        }
}

void add_user() {
        // Try first to add user in case it does not exist.
        char response[ MAX_RESPONSE ];
        sendCommand(host, port, "ADD-USER", username, password, "", response);

        if (!strcmp(response,"OK\r\n")) {
                printf("User %s added\n", username);
        }
}

void send_message(char * msg) {
        char response[MAX_RESPONSE];
        sendCommand(host, port, "SEND-MESSAGE", username, password, msg, response);
 
        if (!strcmp(response, "OK\r\n")) {
                printf("Message %s sent\n", msg);
        }
}

/*void update_list_rooms() {
    GtkTreeIter iter;
    int i;
 
    // Add some messages to the window 
    for (i = 0; i < 10; i++) {
        gchar *msg = g_strdup_printf ("Room %d", i);
        gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
        gtk_list_store_set (GTK_LIST_STORE (list_rooms),
                            &iter,
                            0, msg,
                            -1);
        g_free (msg);
    }
}*/
static void update_list_rooms(){
        GtkTreeIter iterators;
        char * temp = (char *)malloc(20 * sizeof(char));
        gchar * msg;
        char response[MAX_RESPONSE];
        sendCommand(host, port, "LIST-ROOMS", username, password, room, response);
        temp = strdup(response);
        temp = strtok(temp, "\r\n");
        msg = g_strdup_printf("%s",  temp);
        gtk_list_store_append(GTK_LIST_STORE(list_rooms), &iterators);
        gtk_list_store_set(GTK_LIST_STORE(list_rooms), &iterators, 0, msg, -1);
	while((temp = strtok(NULL, "\r\n")) != NULL){
                msg = g_strdup_printf("%s", temp);
                gtk_list_store_append(GTK_LIST_STORE(list_rooms), &iterators);
                gtk_list_store_set(GTK_LIST_STORE(list_rooms), &iterators, 0, msg, -1);
	}
}

  
void join_room() {
        char response[MAX_RESPONSE];
        sendCommand(host, port, "ENTER-ROOM", username, password, room, response);
 
        if (!strcmp(response, "OK\r\n")) {
                gtk_list_store_clear(GTK_LIST_STORE(list_rooms));
		strcat(message, " has joined room");
		send_message(message);
		gtk_list_store_clear(GTK_LIST_STORE(list_rooms));
		update_list_rooms();
		printf("User %s added\n", username);
        }
}

void leave_room(char * temp) {
        char response[MAX_RESPONSE];
        sendCommand(host, port, "LEAVE-ROOM", username, password, temp, response);
 
        if (!strcmp(response, "OK\r\n")) {
                printf("User %s removed\n", username);
        }
}

void create_room() {
        char response[MAX_RESPONSE];
        sendCommand(host, port, "CREATE-ROOM", username, password, room, response);
 
        if (!strcmp(response, "OK\r\n")) {
                printf("Room %s added\n", room);
        }
}
static void update_list_messages(){
	GtkTreeIter iterators;
	char * temp = (char *)malloc(300 * sizeof(char));
	gchar * msg;
	char response[MAX_RESPONSE];
	sendCommand(host, port, "GET-MESSAGES", username, password, room, response);
	temp = strdup(response);
	temp = strtok(temp, "\r\n");
	msg = g_strdup_printf("%s", temp);
	gtk_list_store_append(GTK_LIST_STORE(list_messages), &iterators);
	gtk_list_store_set(GTK_LIST_STORE(list_messages), &iterators, 0, msg, -1);
	while((temp = strtok(NULL, "\r\n")) != NULL){
		msg = g_strdup_printf("%s", temp);
	        gtk_list_store_append(GTK_LIST_STORE(list_messages), &iterators);
		gtk_list_store_set(GTK_LIST_STORE(list_messages), &iterators, 0, msg, -1);
	}
}
void get_messages() {
        gtk_list_store_clear(GTK_LIST_STORE(list_messages));
	update_list_messages();
}
 
void print_users_in_room() {
        char response[MAX_RESPONSE];
        sendCommand(host, port, "GET-USERS-IN-ROOM", username, password, "", response);
 
        if (!strcmp(response, "OK\r\n")) {
                printf("User %s added\n", username);
        }
}
 
void print_users() {
        char response[MAX_RESPONSE];
        sendCommand(host, port, "GET-ALL-USERS", username, password, "", response);
 
        if (!strcmp(response, "OK\r\n")) {
                printf("User %s added\n", username);
        }
}

void printPrompt() {
        printf("talk> ");
        fflush(stdout);
}
 
void printHelp() {
        printf("Commands:\n");
        printf(" -who   - Gets users in room\n");
        printf(" -users - Prints all registered users\n");
        printf(" -help  - Prints this help\n");
        printf(" -quit  - Leaves the room\n");
        printf("Anything that does not start with \"-\" will be a message to the chat room\n")    ;
}
 
void * getMessagesThread(void * arg) {
        // This code will be executed simultaneously with main()
        // Get messages to get last message number. Discard the initial Messages
 
        while (1) {
                // Get messages after last message number received.

                // Print messages

                // Sleep for ten seconds
                usleep(2*1000*1000);
        }
}
 
void startGetMessageThread(){
        pthread_create(NULL, NULL, getMessagesThread, NULL);
}
/* Create the list of "messages" */
static GtkWidget *create_list( const char * titleColumn, GtkListStore *model )
{
    GtkWidget *scrolled_window;
    GtkWidget *tree_view;
    //GtkListStore *model;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;

    int i;
   
    /* Create a new scrolled window, with scrollbars only if needed */
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				    GTK_POLICY_AUTOMATIC, 
				    GTK_POLICY_AUTOMATIC);
   
    //model = gtk_list_store_new (1, G_TYPE_STRING);
    tree_view = gtk_tree_view_new ();
    gtk_container_add (GTK_CONTAINER (scrolled_window), tree_view);
    gtk_tree_view_set_model (GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (model));
    gtk_widget_show (tree_view);
   
    cell = gtk_cell_renderer_text_new ();

    column = gtk_tree_view_column_new_with_attributes (titleColumn,
                                                       cell,
                                                       "text", 0,
                                                       NULL);
  
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view),
	  		         GTK_TREE_VIEW_COLUMN (column));

    return scrolled_window;
}
   
/* Add some text to our text widget - this is a callback that is invoked
when our window is realized. We could also force our window to be
realized with gtk_widget_realize, but it would have to be part of
a hierarchy first */

static void insert_text( GtkTextBuffer *buffer, const char * initialText )
{
   GtkTextIter iter;
 
   gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
   gtk_text_buffer_insert (buffer, &iter, initialText,-1);
}
   
/* Create a scrolled text area that displays a "message" */
static GtkWidget *create_text( const char * initialText )
{
   GtkWidget *scrolled_window;
   GtkWidget *view;
   GtkTextBuffer *buffer;

   view = gtk_text_view_new ();
   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
		   	           GTK_POLICY_AUTOMATIC,
				   GTK_POLICY_AUTOMATIC);

   gtk_container_add (GTK_CONTAINER (scrolled_window), view);
   insert_text (buffer, initialText);

   gtk_widget_show_all (scrolled_window);

   return scrolled_window;
}

static void enter_username(GtkWidget *entry){
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    username = strdup(text);
    printf("Username Entry: %s\n", text);
}

static void enter_password(GtkWidget *entry){
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    password = strdup(text);
    add_user();
    printf("Password Entry: %s\n", text);
}

static void enter_room(GtkWidget *entry){
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    room = strdup(text);
    create_room();
    printf("Room Entry: %s\n", text);
}

static void enter_message(GtkWidget *entry){
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    message = strdup(text);
    send_message(message);
    printf("Message Entry: %s\n", text);
}

char * previous;
static void change(GtkWidget * widget, GtkWidget * widget2){
    GtkTreeIter iterator;
    GtkTreeModel * model;
    char * value;
    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iterator)){
    	gtk_tree_model_get(model, &iterator, 0, &value, -1);
	room = strdup(value);
	if(previous == NULL){
		previous = strdup(value);
		join_room();
	}else{
		leave_room(previous);
		join_room();
		gtk_list_store_clear(GTK_LIST_STORE(list_users));
		update_list_users();
	}
	previous = strdup(value);
   }
}

static gboolean time_handler(GtkWidget * widger){
    gtk_list_store_clear(GTK_LIST_STORE(list_rooms));
    update_list_rooms();
    gtk_list_store_clear(GTK_LIST_STORE(list_users));
    update_list_users();
    get_messages();
    return TRUE;
}

int main( int   argc,
          char *argv[] )
{
    GtkWidget *window;
    GtkWidget *list;
    GtkWidget *messages;
    GtkWidget *myMessage;
 
    gtk_init (&argc, &argv);
   
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Paned Windows");
    g_signal_connect (window, "destroy",
	              G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_widget_set_size_request (GTK_WIDGET (window), 1000, 1000);

    // Create a table to place the widgets. Use a 7x4 Grid (7 rows x 4 columns)
    GtkWidget *table = gtk_table_new (20, 6, TRUE);
    gtk_container_add (GTK_CONTAINER (window), table);
    gtk_table_set_row_spacings(GTK_TABLE (table), 6);
    gtk_table_set_col_spacings(GTK_TABLE (table), 6);
    gtk_widget_show (table);

    // Add list of rooms. Use columns 0 to 4 (exclusive) and rows 0 to 4 (exclusive)
    list_rooms = gtk_list_store_new (1, G_TYPE_STRING);
    list = create_list ("Rooms", list_rooms);
    gtk_table_attach_defaults (GTK_TABLE (table), list, 2, 4, 0, 9);
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
    g_signal_connect(tree, "row-activated", G_CALLBACK(change), selection);
    gtk_widget_show (list);

    // Add list of users
    list_users = gtk_list_store_new (1, G_TYPE_STRING);
    list = create_list ("Users", list_users);
    gtk_table_attach_defaults (GTK_TABLE (table), list, 4, 6, 0, 9);
    gtk_widget_show (list);

    // Add messages text
    list_messages = gtk_list_store_new (1, G_TYPE_STRING);
    list = create_list ("Messages", list_messages);
    gtk_table_attach_defaults (GTK_TABLE (table), list, 0, 6, 10, 16);
    gtk_widget_show (list);


    // Add send button. Use columns 0 to 1 (exclusive) and rows 4 to 7 (exclusive)
    GtkWidget *send_button = gtk_button_new_with_label ("Send");
    gtk_table_attach_defaults(GTK_TABLE (table), send_button, 2, 4, 18, 20); 
    gtk_widget_show (send_button);

    // Add addUser button
    GtkWidget *add_user_button = gtk_button_new_with_label ("Create Account");
    gtk_table_attach_defaults(GTK_TABLE (table), add_user_button, 0, 2, 3, 4);
    gtk_widget_show (add_user_button);

    // Add createroom button
    GtkWidget *create_room_button = gtk_button_new_with_label ("Create Room");
    gtk_table_attach_defaults(GTK_TABLE (table), create_room_button, 0, 2, 8, 9);
    gtk_widget_show (create_room_button);

    // Add login button
    GtkWidget *login_button = gtk_button_new_with_label ("Login");
    gtk_table_attach_defaults(GTK_TABLE (table), login_button, 0, 2, 4, 5);
    gtk_widget_show (login_button);

    //Add username entry field
    GtkWidget *user_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY (user_entry), 50);
    g_signal_connect_swapped(add_user_button, "clicked", G_CALLBACK(enter_username), user_entry);
    g_signal_connect_swapped(login_button, "clicked", G_CALLBACK(enter_username), user_entry);
    gtk_table_attach_defaults(GTK_TABLE(table), user_entry, 0, 2, 1, 2);
    gtk_widget_set_size_request(user_entry, 80, 30);
    gtk_widget_show(user_entry);

    //Add password entry field
    GtkWidget *pass_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY (pass_entry), 50);
    g_signal_connect_swapped(add_user_button, "clicked", G_CALLBACK(enter_password), pass_entry);
    g_signal_connect_swapped(login_button, "clicked", G_CALLBACK(enter_password), pass_entry);
    gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE);
    gtk_table_attach_defaults(GTK_TABLE(table), pass_entry, 0, 2, 2, 3);
    gtk_widget_set_size_request(pass_entry, 80, 30);
    gtk_widget_show(pass_entry);

    //Add room entry field
    GtkWidget *room_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY (room_entry), 50);
    g_signal_connect_swapped(create_room_button, "clicked", G_CALLBACK(enter_room), room_entry);
    gtk_table_attach_defaults(GTK_TABLE(table), room_entry, 0, 2, 7, 8);
    gtk_widget_set_size_request(pass_entry, 80, 30);
    gtk_widget_show(room_entry);

    //Add message entry field
    GtkWidget *message_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY (room_entry), 50);
    g_signal_connect_swapped(send_button, "clicked", G_CALLBACK(enter_message), message_entry);
    gtk_table_attach_defaults(GTK_TABLE(table), message_entry, 0, 6, 16, 18);
    gtk_widget_set_size_request(message_entry, 80, 50);
    gtk_widget_show(message_entry);

    gtk_widget_show (table);
    gtk_widget_show (window);

    g_timeout_add(10000, (GSourceFunc) time_handler, (gpointer) window);
    time_handler(window);

    gtk_main ();

    return 0;
}

