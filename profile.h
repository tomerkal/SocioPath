#ifndef PROFILE_H_INCLUDED
#define PROFILE_H_INCLUDED

        /** Main functions **/

void profile_menu(struct profile* profile);
void print_profile(struct profile *user_profile);
void update_status_menu(struct profile *user_profile);
    int long_status_menu(struct profile *user_profile, char *status);
void check_status(struct profile *user_profile, char *friend_username);
void check_requests(struct profile *user_profile);
    int approve_request(struct profile *user_profile, char *request_username, int flag);
    int refuse(struct profile *user_profile, char *request_username);
void print_friends(struct profile *user);
void search_network(char *query);
void send_request(struct profile *user, char *request_name);
void unfriend(struct profile *user, char *name);
void print_network(struct profile *user);
    int is_inNetwork(struct profile **network, int size, struct profile *user);


        /** General functions **/

void add_request(struct profile *user, struct profile *to_add);
void remove_friend_request(struct profile *user, int index);
void add_friend(struct profile *user, struct profile *to_add);
void remove_friend(struct profile *user, char *name);
struct profile* create_profile(char *name);
struct profile* find_profile(char *name);
void free_profiles();
void read_friends_line(FILE *fp, struct profile *user);
void read_requests_line(FILE *fp, struct profile *user);
int is_inList(struct profile **list, int size, char *name);
void make_space(struct profile *user, int space_index);

#endif // PROFILE_H_INCLUDED
