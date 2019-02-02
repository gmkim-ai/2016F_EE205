/*basic structure. you can add variables if you want*/

typedef int bool;
struct UserPointer{
  char* name; //name of user
  struct FollowerPointer *nextfollower; // a user and their followers are linked by this pointer
  struct FolloweePointer *nextfollowee; // a user and their followees are linked by this pointer
  struct UserPointer *nextuser; // users are linked by this pointer
  bool visit;
  bool retwit;
  bool list;
  int hophop;
};

struct FollowerPointer{
  struct UserPointer *user; //every follower pointer indicates the user pointer whose name is the follower name
  struct FollowerPointer *nextfollower; //followers of a user are linked by this pointer
};

struct FolloweePointer{
  struct UserPointer *user; //every followee pointer indicates the user pointer whose name is the followee name
  struct FolloweePointer *nextfollowee; //followees of a user are linked by this pointer
};

/*structure pointer. you can declare other necessary pointers and use them*/
struct UserPointer *init, *current;

struct UserPointer* create_user();
struct FollowerPointer* create_follower();
struct FolloweePointer* create_followee();
struct UserPointer* search(char* name);
void add(char* name);
void delete(char* name);
void follow(char* followeename, char* followername);
void unfollow(char* followeename, char* followername);
void find(char *temp, char *token);
void Free();
int twit(char** list, int listsize, char* name, int key);
int hop(char* username1, char* username2);
