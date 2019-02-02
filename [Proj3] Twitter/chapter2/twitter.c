#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"proj3.h"

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

int twit(char** list, int listsize, char* name, int key)
{
    struct UserPointer *user;
    struct FollowerPointer *user_wer;
    char *token;
    int i;

    if (key == 1) {
        for (i = 0; i < listsize; i++) {
            if (search(list[i]) == NULL) {
                printf("ERROR: call the twit function with the name that is not in the user list\n");
                return -1;
            }
        }
        for (user = init; user != NULL; user = user->nextuser) {
            user->retwit = 0;
            user->list = 0;
        }
        user = search(name);
        user->retwit = 1;
        user->list = 1;
    }
    user = search(name);
    for (user_wer = user->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
        if (user_wer->user->retwit == 0) {
            printf("%s ", user_wer->user->name);
            user_wer->user->retwit = 1;
        }
        for (i = 0; i < listsize; i++) {
            if (!strcmp(list[i], user_wer->user->name) && user_wer->user->list == 0) {
                user_wer->user->list = 1;
                twit(list, listsize, list[i], 0);
            }
        }
    }
    return 0;
}

int hop(char* username1, char* username2)
{
    struct UserPointer *user, **queue;
    struct FollowerPointer *user_wer;
    int i, count, user_count, name_count;
    int rear = 0, front = 0;

    user_count = 0;
    count = 0;
    name_count = 0;
    for (user = init; user != NULL; user = user->nextuser) {
        user_count++;
        for (user_wer = user->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
            count++;
        }
        if (!strcmp(username1, user->name) || !strcmp(username2, user->name)) name_count++;
        user->visit = 0;
        user->hophop = 0;
    }
    if (name_count != 2) {
        printf("ERROR: call the hop function with the name that is not in the user list\n");
        return -1;
    }
    queue = (struct UserPointer **)calloc(count+1, sizeof(struct UserPointer *));
    user = search(username1);
    queue[rear++] = user;
    user->visit = 1;
    user->hophop = 1;
    while (user->hophop < user_count - 1) {
        user = queue[front++];
        for (user_wer = user->nextfollower; user_wer != NULL; user_wer = user_wer->nextfollower) {
            if (!strcmp(user_wer->user->name, username2)) {
                free(queue);
                return user->hophop;
            }
            if (user_wer->user->visit == 0)
            {
                user_wer->user->visit = 1;
                queue[rear++] = user_wer->user;
                user_wer->user->hophop = user->hophop + 1;
            }
        }
    }
    free(queue);
    printf("ERROR: call the hop function with the name that is not in the follower list, hop is infinity\n");
    return -1;
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
    struct UserPointer *user;
    int res, i, arrsize, count;
    char str[128], *token, *followername, *followeename, *temp, **arr;
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
        else if (!strcmp(token, "twit")) {
            count = 0;
            for (user = init; user != NULL; user = user->nextuser) {
                count++;
            }
            arr = (char **)calloc(count, sizeof(char *));
            token = strtok(NULL, " ");
            arr[0] = strdup(token);
            temp = strtok(NULL, " ");
            if (strcmp(strtok(NULL, " "), "retwit")) {
                printf("ERROR: There is wrong command after twit in input file, there is no 'retwit'\n");
                fclose(fp);
                free(arr[0]);
                free(arr);
                Free();
                return ;
            }
            for (i = 1; i < count; i++) {
                token = strtok(NULL, " ");
                if (token[strlen(token)-2] == 13) {
                    token[strlen(token)-2] = '\0';
                    arr[i] = strdup(token);
                    break;
                }
                arr[i] = strdup(token);
            }
            arrsize = i+1;
            res = twit(arr, arrsize, arr[0], 1);
            count = 0;
            for (i = 1; i < arrsize; i++) {
                user = search(arr[i]);
                if (user->list == 1) count++;
            }
            if (count == 0) printf("ERROR: Everyone in retwit list don't follow writer\n");
            else if (res != -1) printf("see the twit %s\n", temp);
            for (i = 0; i < arrsize; i++) {
                free(arr[i]);
            }
            free(arr);
        }
        else if (!strcmp(token, "hop")) {
            followeename = strtok(NULL, " ");
            followername = strtok(NULL, CR);
            res = hop(followeename, followername);
            if (res != -1) printf("hop = %d\n", res);
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
