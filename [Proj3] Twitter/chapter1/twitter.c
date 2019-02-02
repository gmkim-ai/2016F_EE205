#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "twitter.h"

/*functions required. you have to implement in them*/

/*when a new user is generated, call this function*/
struct UserPointer* create_user()
{
    struct UserPointer *temp;
    temp = (struct UserPointer *)calloc(1, sizeof(struct UserPointer));
    if (temp == NULL) printf("ERROR: There is no memory for allocate\n");

    return temp;
}

/*when a new follower is generated, call this function*/
struct FollowerPointer* create_follower()
{
    struct FollowerPointer *temp;
    temp = (struct FollowerPointer *)calloc(1, sizeof(struct FollowerPointer));
    if (temp == NULL) printf("ERROR: There is no memory for allocate\n");

    return temp;
}

/*when a new followee is generated, call this function*/
struct FolloweePointer* create_followee()
{
    struct FolloweePointer *temp;
    temp = (struct FolloweePointer *)calloc(1, sizeof(struct FolloweePointer));
    if (temp == NULL) printf("ERROR: There is no memory for allocate\n");

    return temp;
}

/*This function finds out the user with specific name. Use this function whenever you want */
struct UserPointer* search(char* name)
{
    struct UserPointer *p;
    for (p = init; p != NULL; p = p->nextuser) {
        if (!strcmp(name, p->name)) return p;
    }

    return NULL;
}

/*This function add new user. link the new user with existing users using nextuser pointer*/
void add(char* name)
{
    struct UserPointer *p, *temp;
    if (init != NULL) {
        for (p = init; p != NULL; p = p->nextuser) {
            if (!strcmp(name, p->name)) {
                printf("ERROR: add the existing user\n");
                return ;
            }
        }
    }
    if (init == NULL) {
        temp = create_user();
        init = temp;
        temp->name = strdup(name);
        if (temp->name == NULL) printf("ERROR: There is no memory for allocate\n");
    }
    else {
        for (p = init; p != NULL; p = p->nextuser) {
            if (p->nextuser == NULL) break;
        }
        temp = create_user();
        p->nextuser = temp;
        temp->name = strdup(name);
        if (temp->name == NULL) printf("ERROR: There is no memory for allocate\n");
    }
}

/*This function delete existing user.
if the deleting user is between existing two nodes, link the two nodes.
Also delete all followers and followees pointer with the user name */
void delete(char* name)
{
    struct UserPointer *user, *userprev;
    struct FollowerPointer *user_wer, *user_werprev;
    struct FolloweePointer *user_wee, *user_weeprev;
    if (init == NULL) {
        printf("ERROR: delete a user that is not in the user list\n");
        return ;
    }
    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(name, user->name)) {
            user = init;
            break;
        }
    }
    if (user == NULL) {
        printf("ERROR: delete a user that is not in the user list\n");
        return ;
    }
    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(name, user->name)) {
            user_werprev = user->nextfollower;
            user_weeprev = user->nextfollowee;
            if (user->nextfollower != NULL) {
                for (user_wer = user->nextfollower->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
                    free(user_werprev);
                    user_werprev = user_wer;
                }
                free(user_werprev);
            }
            if (user->nextfollowee != NULL) {
                for (user_wee = user->nextfollowee->nextfollowee; user_wee != NULL; user_wee = user_wee->nextfollowee) {
                    free(user_weeprev);
                    user_weeprev = user_wee;
                }
                free(user_weeprev);
            }
        }
        else {
            for (user_wer = user->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
                if (!strcmp(name, user_wer->user->name)) {
                    if (user_wer == user->nextfollower) {
                        user->nextfollower = user_wer->nextfollower;
                        free(user_wer);
                    }
                    else {
                        user_werprev->nextfollower = user_wer->nextfollower;
                        free(user_wer);
                    }
                    break;
                }
                user_werprev = user_wer;
            }
            for (user_wee = user->nextfollowee; user_wee != NULL; user_wee = user_wee->nextfollowee) {
                if (!strcmp(name, user_wee->user->name)) {
                    if (user_wee == user->nextfollowee) {
                        user->nextfollowee = user_wee->nextfollowee;
                        free(user_wee);
                    }
                    else {
                        user_weeprev->nextfollowee = user_wee->nextfollowee;
                        free(user_wee);
                    }
                    break;
                }
                user_weeprev = user_wee;
            }
        }
    }

    userprev = init;
    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(name, user->name)) {
            if (user == init) init = user->nextuser;
            else userprev->nextuser = user->nextuser;
            free(user->name);
            free(user);
        }
        userprev = user;
    }
}

/*This function add new follower of a user.
link the new follower with existing followers using nextfollower pointer
link the new followee with existing followees using nextfollowee pointer
*/
void follow(char* followeename, char* followername)
{
    struct UserPointer *user;
    struct FollowerPointer *user_wer, *temp_wer;
    struct FolloweePointer *user_wee, *temp_wee;
    int count = 0;
    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(followeename, user->name) || !strcmp(followername, user->name)) count++;
    }
    if (!strcmp(followeename, followername)) {
        printf("ERROR: follower and followee are same user\n");
        return ;
    }
    if (count != 2) {
        printf("ERROR: add a follower or followee whose name is not in the user list\n");
        return ;
    }
    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(followeename, user->name)) {
            for (user_wer = user->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
                if (!strcmp(user_wer->user->name, followername)) return ;
            }
        }
    }
    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(followeename, user->name)) {
            if (user->nextfollower == NULL) {
                temp_wer = create_follower();
                temp_wer->user = search(followername);
                user->nextfollower = temp_wer;
            }
            else {
                for (user_wer = user->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
                    if (user_wer->nextfollower == NULL) break;
                }
                temp_wer = create_follower();
                temp_wer->user = search(followername);
                user_wer->nextfollower = temp_wer;
            }
        }
        else if (!strcmp(followername, user->name)) {
            if (user->nextfollowee == NULL) {
                temp_wee = create_followee();
                temp_wee->user = search(followeename);
                user->nextfollowee = temp_wee;
            }
            else {
                for (user_wee = user->nextfollowee; user_wee != NULL; user_wee = user_wee->nextfollowee) {
                    if (user_wee->nextfollowee == NULL) break;
                }
                temp_wee = create_followee();
                temp_wee->user = search(followeename);
                user_wee->nextfollowee = temp_wee;
            }
        }
    }
}

/*This function delete existing follower.
if the deleting follower is between existing two nodes, link the two nodes*/
void unfollow(char* followeename, char* followername)
{
    struct UserPointer *user;
    struct FollowerPointer *user_wer, *user_werprev;
    struct FolloweePointer *user_wee, *user_weeprev;
    int count = 0;
    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(followeename, user->name) || !strcmp(followername, user->name)) count++;
    }
    if (!strcmp(followeename, followername)) {
        printf("ERROR: follower and followee are same user\n");
        return ;
    }
    if (count != 2) {
        printf("ERROR: use a follower or followee whose name is not in the user list\n");
        return ;
    }
    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(followeename, user->name)) {
            for (user_wer = user->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
                if (!strcmp(user_wer->user->name, followername)) count++;
            }
        }
    }
    if (count != 3) {
        printf("ERROR: delete a follower that is not in the follower list\n");
        return ;
    }
    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(followeename, user->name)) {
            for (user_wer = user->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
                if (!strcmp(followername, user_wer->user->name)) {
                    if (user_wer == user->nextfollower) {
                        user->nextfollower = user_wer->nextfollower;
                        free(user_wer);
                    }
                    else {
                        user_werprev->nextfollower = user_wer->nextfollower;
                        free(user_wer);
                    }
                    break;
                }
                user_werprev = user_wer;
            }
        }
        else if (!strcmp(followername, user->name)) {
            for (user_wee = user->nextfollowee; user_wee != NULL; user_wee = user_wee->nextfollowee) {
                if (!strcmp(followeename, user_wee->user->name)) {
                    if (user_wee == user->nextfollowee) {
                        user->nextfollowee = user_wee->nextfollowee;
                        free(user_wee);
                    }
                    else {
                        user_weeprev->nextfollowee = user_wee->nextfollowee;
                        free(user_wee);
                    }
                    break;
                }
                user_weeprev = user_wee;
            }
        }
    }
}

void find(char *temp, char *token)
{
    struct UserPointer *user;
    struct FollowerPointer *user_wer;
    struct FolloweePointer *user_wee;

    for (user = init; user != NULL; user = user->nextuser) {
        if (!strcmp(token, user->name)) {
            if (!strcmp(temp, "follower")) {
                for (user_wer = user->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
                    printf("%s ", user_wer->user->name);
                }
            }
            else {
                for (user_wee = user->nextfollowee; user_wee != NULL; user_wee = user_wee->nextfollowee) {
                    printf("%s ", user_wee->user->name);
                }
            }
            break;
        }
    }
    printf("\n\n");
}

void Free()
{
    struct UserPointer *user, *userprev;
    struct FollowerPointer *user_wer, *user_werprev;
    struct FolloweePointer *user_wee, *user_weeprev;
    for (user = init; user != NULL; user = user->nextuser) {
        user_werprev = user->nextfollower;
        user_weeprev = user->nextfollowee;
        if (user->nextfollower != NULL) {
            for (user_wer = user->nextfollower->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
                free(user_werprev);
                user_werprev = user_wer;
            }
            free(user_werprev);
        }
        if (user->nextfollowee != NULL) {
            for (user_wee = user->nextfollowee->nextfollowee; user_wee != NULL; user_wee = user_wee->nextfollowee) {
                free(user_weeprev);
                user_weeprev = user_wee;
            }
            free(user_weeprev);
        }
    }
    userprev = init;
    if (init != NULL) {
        for (user = init->nextuser; user != NULL; user = user->nextuser) {
            free(userprev->name);
            free(userprev);
            userprev = user;
        }
        free(userprev);
    }
}

void main(int argc, char **argv){
    FILE *fp;
    char str[128], *token, *followername, *followeename, *temp;
    init = NULL;
    char CR[2] = {13, '\0'};

    fp = fopen(argv[1], "r");

    while(fgets(str, 128, fp) != NULL) {
        token = strtok(str, " ");

        if (!strcmp(token, "add")) {
            token = strtok(NULL, CR);
            add(token);
        }
        else if (!strcmp(token, "del")) {
            token = strtok(NULL, CR);
            delete(token);
        }
        else if (!strcmp(token, "follow")) {
            followeename = strtok(NULL, " ");
            followername = strtok(NULL, CR);
            follow(followeename, followername);
        }
        else if (!strcmp(token, "unfollow")) {
            followeename = strtok(NULL, " ");
            followername = strtok(NULL, CR);
            unfollow(followeename, followername);
        }
        else if (!strcmp(token, "find")) {
            temp = strtok(NULL, " ");
            if (strcmp(temp, "follower") && strcmp(temp, "followee")) {
                printf("ERROR: There is wrong command after find in input file\n");
                fclose(fp);
                Free();
                return ;
            }
            token = strtok(NULL, CR);
            find(temp, token);
        }
        else if (token[0] == 13);
        else {
            printf("ERROR: There is wrong command in input file\n");
            fclose(fp);
            Free();
            return ;
        }
    }
    fclose(fp);
    Free();
}
