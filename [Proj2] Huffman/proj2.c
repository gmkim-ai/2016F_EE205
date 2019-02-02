#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "proj2.h"

void letter(struct node *root, struct node *order_table)
{
    struct node *p;
    for (p = order_table; p != NULL; p = p->next_order) {
        printf("%d %d %s\n", p->letter, p->count, p->code);
    }
}

void tree(struct node *root)
{
    int left_c, right_c, letter;
    if (root->left != NULL) tree(root->left);
    if (root->left == NULL) left_c = 0;
    else left_c = root->left->count;
    if (root->right == NULL) right_c = 0;
    else right_c = root->right->count;
    if (root->letter == -1) printf("%d %d %d\n", root->count, left_c, right_c);
    else printf("%d %d %d %d\n", root->count, left_c, right_c, root->letter);
    if (root->right != NULL) tree(root->right);
}

void code(struct node *root, struct node *order_table, char *file)
{
    FILE *fp;
    char alpha;
    struct node *p;
    int bin = 0, dec = 0, i;
    fp = fopen(file, "r");
	if (fp == NULL) {
        printf("ERROR: There is no %s file\n", file);
	}

    for (p = order_table; p != NULL; p = p->next_order) {
        printf("%d %s\n", p->letter, p->code);
    }

	while (EOF != (alpha = fgetc(fp))) {
        for (p = order_table; p != NULL; p = p->next_order) {
            if (p->letter == alpha) {
                printf("%s", p->code);
                break;
            }
        }
	}
	printf("\n");
	for (p = order_table; p != NULL; p = p->next_order) {
        for (i = 0; i < 50; i++) {
            if (p->code[i] == '\0') break;
        }
        bin += i * p->count;
        dec += p->count;
    }
	printf("Ratio: %.3f\n", (float)bin / (float)(dec * 7));
	fclose(fp);
}

void decode(struct node *root, FILE *fp)
{
    char alpha;
    struct node *temp = root;

    while (EOF != (alpha = fgetc(fp))) {
        if (alpha == 10) break;
        if (alpha == '0') temp = temp->left;
        else if (alpha == '1') temp = temp->right;
        else printf("ERROR: There is a number not '0' or '1'\n");
        if (temp->letter != -1) {
            printf("%c", temp->letter);
            temp = root;
        }
	}
	printf("\n");
	fclose(fp);
}

struct node* init_table(const char *file)
{
	FILE *fp;
	char alpha;
	struct node *front, *p;

	fp = fopen(file, "r");
	if (fp == NULL) {
        printf("ERROR: There is no %s file\n", file);
        return NULL;
	}

	if (EOF != (alpha = fgetc(fp))) {
        front = (struct node *)calloc(1, sizeof(struct node));
        front->letter = alpha;
        front->count = 1;
	}
	else return NULL;
	while (EOF != (alpha = fgetc(fp))) {
        if (alpha == 10 && (alpha = fgetc(fp)) != EOF) {
            for (p = front; p != NULL; p=p->next) {
                if (p->letter == 10) {
                    p->count++;
                    break;
                }
                else if (p->next == NULL) {
                    p->next = (struct node *)calloc(1, sizeof(struct node));
                    p->next->letter = 10;
                    p->next->count = 1;
                    break;
                }
            }
        }
        else if (alpha == EOF) break;
        for (p = front; p != NULL; p=p->next) {
            if (p->letter == alpha) {
                p->count++;
                break;
            }
            else if (p->next == NULL) {
                p->next = (struct node *)calloc(1, sizeof(struct node));
                p->next->letter = alpha;
                p->next->count = 1;
                break;
            }
        }
	}
	fclose(fp);

	return front;
}

struct node* init_order_table(struct node *table)
{
    struct node *order_table, *p, *q, *temp, *pprev, *qprev;
    order_table = (struct node *)calloc(1, sizeof(struct node));
    order_table = table;
    for (p = order_table; p != NULL; p = p->next) {
        p->next_order = p->next;
    }
    pprev = order_table;
    qprev = order_table;

    for (p = order_table; p != NULL; p = p->next_order) {
        for (q = p->next_order; q != NULL; q = q->next_order) {
            if (p->letter > q->letter) {
                if (p == order_table) {
                    order_table = q;
                    qprev->next_order = p;
                    temp = p->next_order;
                    p->next_order = q->next_order;
                    q->next_order = temp;
                }
                else {
                    pprev->next_order = q;
                    qprev->next_order = p;
                    temp = p->next_order;
                    p->next_order = q->next_order;
                    q->next_order = temp;
                }
                temp = p;
                p = q;
                q = temp;
            }
            qprev = q;
        }
        qprev = p->next_order;
        pprev = p;
    }
    return order_table;
}

struct Queue* create_queue(struct node *table)
{
    int count = 0;
    struct node *p;
    struct Queue *queue = (struct Queue *)calloc(1, sizeof(struct Queue));
    for (p = table; p != NULL; p = p->next) {
        count++;
    }
    queue->front = queue->rear = -1;
    queue->size = count;
    queue->array = (struct node**)calloc(queue->size, sizeof(struct node *));
    return queue;
}

struct node* order_down(struct node *root, struct Queue *queue, struct node *table, struct node *temp)
{
    struct node *p, *var, *val1, *val2;
    val1 = (struct node *)calloc(1, sizeof(struct node));
    val2 = (struct node *)calloc(1, sizeof(struct node));
    int i, index_temp;
    if (temp->heap_left == NULL && temp->heap_right == NULL) {
        free(val1); free(val2);
        return root;
    }
    if ((temp->heap_right == NULL && temp->heap_left != NULL && temp->count > temp->heap_left->count) || (temp->heap_right != NULL && temp->heap_left != NULL && temp->count > temp->heap_left->count && temp->heap_left->count <= temp->heap_right->count)) {
        if (temp == root) {
            root = temp->heap_left;
            val1->heap_left = temp->heap_left->heap_left;
            val1->heap_right = temp->heap_left->heap_right;
            val2->heap_right = temp->heap_right;
            temp->heap_left->heap_right = val2->heap_right;
            temp->heap_left->heap_left = temp;
            temp->heap_left = val1->heap_left;
            temp->heap_right = val1->heap_right;
        }
        else {
            for (p = table; p != NULL; p = p->next) {
                if (p->heap_left == temp) {
                    val1->heap_left = temp->heap_left->heap_left;
                    val1->heap_right = temp->heap_left->heap_right;
                    val2->heap_right = temp->heap_right;
                    p->heap_left = temp->heap_left;
                    temp->heap_left->heap_right = val2->heap_right;
                    temp->heap_left->heap_left = temp;
                    temp->heap_left = val1->heap_left;
                    temp->heap_right = val1->heap_right;
                    break;
                }
                else if (p->heap_right == temp) {
                    val1->heap_left = temp->heap_left->heap_left;
                    val1->heap_right = temp->heap_left->heap_right;
                    val2->heap_right = temp->heap_right;
                    p->heap_right = temp->heap_left;
                    temp->heap_left->heap_right = val2->heap_right;
                    temp->heap_left->heap_left = temp;
                    temp->heap_left = val1->heap_left;
                    temp->heap_right = val1->heap_right;
                    break;
                }
            }
        }
        for (i=0; i<queue->rear+1; i++) {
            if (queue->array[i] == temp) {
                var = queue->array[i];
                index_temp = i;
            }
            else if (queue->array[i]->heap_left == temp || queue->array[i]->heap_right == temp) {
                queue->array[index_temp] = queue->array[i];
                queue->array[i] = var;
                break;
            }
        }
        root = order_down(root, queue, table, temp);
    }
    else if ((temp->heap_right != NULL && temp->heap_left == NULL && temp->count > temp->heap_right->count) || (temp->heap_right != NULL && temp->heap_left != NULL && temp->count > temp->heap_right->count && temp->heap_left->count > temp->heap_right->count)) {
        if (temp == root) {
            root = temp->heap_right;
            val1->heap_left = temp->heap_right->heap_left;
            val1->heap_right = temp->heap_right->heap_right;
            val2->heap_left = temp->heap_left;
            temp->heap_right->heap_left = val2->heap_left;
            temp->heap_right->heap_right = temp;
            temp->heap_left = val1->heap_left;
            temp->heap_right = val1->heap_right;
        }
        else {
            for (p = table; p != NULL; p = p->next) {
                if (p->heap_left == temp) {
                    val1->heap_left = temp->heap_right->heap_left;
                    val1->heap_right = temp->heap_right->heap_right;
                    val2->heap_left = temp->heap_left;
                    p->heap_left = temp->heap_right;
                    temp->heap_right->heap_left = val2->heap_left;
                    temp->heap_right->heap_right = temp;
                    temp->heap_left = val1->heap_left;
                    temp->heap_right = val1->heap_right;
                    break;
                }
                else if (p->heap_right == temp) {
                    val1->heap_left = temp->heap_right->heap_left;
                    val1->heap_right = temp->heap_right->heap_right;
                    val2->heap_left = temp->heap_left;
                    p->heap_right = temp->heap_right;
                    temp->heap_right->heap_left = val2->heap_left;
                    temp->heap_right->heap_right = temp;
                    temp->heap_left = val1->heap_left;
                    temp->heap_right = val1->heap_right;
                    break;
                }
            }
        }
        for (i=0; i<queue->rear+1; i++) {
            if (queue->array[i] == temp) {
                var = queue->array[i];
                index_temp = i;
            }
            else if (queue->array[i]->heap_left == temp || queue->array[i]->heap_right == temp) {
                queue->array[index_temp] = queue->array[i];
                queue->array[i] = var;
                break;
            }
        }
        root = order_down(root, queue, table, temp);
    }
    free(val1); free(val2);
    return root;
}

struct node* pop(struct node *heap_root, struct Queue *queue, struct node *table)
{
    struct node *p, *pprev = table;
    if (queue->rear == 0) {
        heap_root = NULL;
        queue->rear--;
        return heap_root;
    }
    for (p = table; p != NULL; p = p->next) {
        if (p == heap_root) {
            if (p == table) table = p->next;
            else pprev->next = p->next;
            break;
        }
        pprev = p;
    }
	queue->array[queue->rear]->heap_left = heap_root->heap_left;
	if (queue->array[queue->rear] == heap_root->heap_left) queue->array[queue->rear]->heap_left = NULL;
	queue->array[queue->rear]->heap_right = heap_root->heap_right;
	if (queue->array[queue->rear] == heap_root->heap_right) queue->array[queue->rear]->heap_right = NULL;
	heap_root = queue->array[queue->rear];
	if (queue->rear != 0 && queue->rear % 2 == 0) queue->array[(queue->rear-1)/2]->heap_right = NULL;
	else if (queue->rear != 0 && queue->rear % 2 == 1) queue->array[(queue->rear-1)/2]->heap_left = NULL;

	queue->array[0] = queue->array[queue->rear];

    queue->rear--;
    heap_root = order_down(heap_root, queue, table, heap_root);
	return heap_root;
}

struct node* push(struct node *heap_root, struct Queue *queue, struct node *table, struct node *new_node)
{
    struct node *p;
    for (p = table; p != NULL; p = p->next) {
        if (p->next == NULL) {
            p->next = new_node;
            break;
        }
    }
    if (queue->rear == -1) {
        queue->array[0] = new_node;
        queue->rear++;
        heap_root = new_node;
        return heap_root;
    }
    else if (queue->rear == 0) {
        queue->array[1] = new_node;
        queue->rear++;
        heap_root->heap_left = new_node;
    }
    else {
        queue->array[queue->rear+1] = new_node;
        queue->rear++;
        if (queue->rear % 2 == 0) queue->array[(queue->rear-1)/2]->heap_right = new_node;
        else if (queue->rear % 2 == 1) queue->array[(queue->rear-1)/2]->heap_left = new_node;
    }
    heap_root = order_up(heap_root, queue, table, new_node);

    return heap_root;
}

struct node* create_heap(struct node *table, struct Queue *queue)
{
    struct node *p = table, *heap_root = NULL;
    for (p = table; p != NULL; p = p->next) {
        heap_root = insert(heap_root, queue, p);
        heap_root = order_up(heap_root, queue, table, p);
    }
    return heap_root;
}

struct node* insert(struct node *root, struct Queue *queue, struct node *temp)
{
    if (root == NULL) root = temp;
    else
    {
        struct node* front = queue->array[queue->front];
        if (front->heap_left == NULL) front->heap_left = temp;
        else if (front->heap_right == NULL) front->heap_right = temp;
        if (front != NULL && front->heap_left != NULL && front->heap_right != NULL) Delete(queue);
    }
    Add(temp, queue);
    return root;
}


struct node* order_up(struct node *root, struct Queue *queue, struct node *table, struct node *temp)
{
    struct node *p, *pp, *var, *val1, *val2;
    val1 = (struct node *)calloc(1, sizeof(struct node));
    val2 = (struct node *)calloc(1, sizeof(struct node));
    int i, index_p;
    if (root == temp) return root;
    for (p = table; p != NULL; p = p->next) {
        if (p->heap_left == temp) {
            if (p->count > temp->count) {
                if (p == root) {
                    root = temp;
                    val1->heap_left = temp->heap_left;
                    val1->heap_right = temp->heap_right;
                    val2->heap_right = p->heap_right;
                    temp->heap_right = val2->heap_right;
                    temp->heap_left = p;
                    p->heap_left = val1->heap_left;
                    p->heap_right = val1->heap_right;
                }
                else {
                    for (pp = table; pp != NULL; pp = pp->next) {
                         if (pp->heap_left == p) {
                             val1->heap_left = temp->heap_left;
                             val1->heap_right = temp->heap_right;
                             val2->heap_right = p->heap_right;
                             pp->heap_left = temp;
                             temp->heap_right = val2->heap_right;
                             temp->heap_left = p;
                             p->heap_left = val1->heap_left;
                             p->heap_right = val1->heap_right;
                             break;
                         }
                         else if (pp->heap_right == p) {
                             val1->heap_left = temp->heap_left;
                             val1->heap_right = temp->heap_right;
                             val2->heap_right = p->heap_right;
                             pp->heap_right = temp;
                             temp->heap_right = val2->heap_right;
                             temp->heap_left = p;
                             p->heap_left = val1->heap_left;
                             p->heap_right = val1->heap_right;
                             break;
                         }
                    }
                }
                for (i=0; i<queue->rear+1; i++) {
                    if (queue->array[i] == p) {
                        var = queue->array[i];
                        index_p = i;
                    }
                    else if (queue->array[i] == temp) {
                        queue->array[index_p] = queue->array[i];
                        queue->array[i] = var;
                        break;
                    }
                }
                root = order_up(root, queue, table, p);
            }
            free(val1); free(val2);
            return root;
        }
        else if (p->heap_right == temp) {
            if (p->count > temp->count) {
                if (p == root) {
                    root = temp;
                    val1->heap_left = temp->heap_left;
                    val1->heap_right = temp->heap_right;
                    val2->heap_left = p->heap_left;
                    temp->heap_left = val2->heap_left;
                    temp->heap_right = p;
                    p->heap_left = val1->heap_left;
                    p->heap_right = val1->heap_right;
                }
                else {
                    for (pp = table; pp != NULL; pp = pp->next) {
                         if (pp->heap_left == p) {
                             val1->heap_left = temp->heap_left;
                             val1->heap_right = temp->heap_right;
                             val2->heap_left = p->heap_left;
                             pp->heap_left = temp;
                             temp->heap_left = val2->heap_left;
                             temp->heap_right = p;
                             p->heap_left = val1->heap_left;
                             p->heap_right = val1->heap_right;
                             break;
                         }
                         else if (pp->heap_right == p) {
                             val1->heap_left = temp->heap_left;
                             val1->heap_right = temp->heap_right;
                             val2->heap_left = p->heap_left;
                             pp->heap_right = temp;
                             temp->heap_left = val2->heap_left;
                             temp->heap_right = p;
                             p->heap_left = val1->heap_left;
                             p->heap_right = val1->heap_right;
                             break;
                         }
                    }
                }
                for (i=0; i<queue->rear+1; i++) {
                    if (queue->array[i] == p) {
                        var = queue->array[i];
                        index_p = i;
                    }
                    else if (queue->array[i] == temp) {
                        queue->array[index_p] = queue->array[i];
                        queue->array[i] = var;
                        break;
                    }
                }
                root = order_up(root, queue, table, p);
            }
            free(val1); free(val2);
            return root;
        }
    }
    return root;
}

void Add(struct node *temp, struct Queue* queue)
{
    if (queue->rear == queue->size - 1) return ;
    queue->array[++queue->rear] = temp;
    if (queue->front == -1) ++queue->front;
}

void Delete(struct Queue* queue)
{
    if (queue->front == -1) return ;

    if (queue->front == queue->rear) queue->front = queue->rear = -1;
    else ++queue->front;
}

struct node* combine(struct node *node1, struct node *node2)
{
	struct node *temp;
	temp = (struct node *)calloc(1, sizeof(struct node));
	temp->letter = -1;
	temp->left = node1;
	temp->right = node2;
	temp->heap_left = NULL;
	temp->heap_right = NULL;
	temp->count = node1->count + node2->count;
	temp->next = NULL;
    return temp;
}

struct node* create_tree(struct node *heap_root, struct Queue *queue, struct node *table)
{
	struct node *min1, *min2, *new_node, *p;

	while( heap_root->heap_left != NULL || heap_root->heap_right != NULL)
	{
		min1 = heap_root;
		heap_root = pop(heap_root, queue, table);
		min2 = heap_root;
		heap_root = pop(heap_root, queue, table);
		new_node = combine(min1, min2);
		heap_root = push(heap_root, queue, table, new_node);
	}

	return heap_root;
}

void left_coding(struct node *prev_root, struct node *root)
{
    int i;
    for (i=0; i<50; i++) {
        root->code[i] = prev_root->code[i];
        if (prev_root->code[i] == '\0') {
            root->code[i] = '0';
            root->code[i+1] = '\0';
            break;
        }
    }
    if (root->left != NULL) left_coding(root, root->left);
    if (root->right != NULL) right_coding(root, root->right);
}

void right_coding(struct node *prev_root, struct node *root)
{
    int i;
    for (i=0; i<50; i++) {
        root->code[i] = prev_root->code[i];
        if (prev_root->code[i] == '\0') {
            root->code[i] = '1';
            root->code[i+1] = '\0';
            break;
        }
    }
    if (root->left != NULL) left_coding(root, root->left);
    if (root->right != NULL) right_coding(root, root->right);
}

struct node* coding(struct node *root)
{
    root->code[0] = '\0';
    if (root->left != NULL) left_coding(root, root->left);
    if (root->right != NULL) right_coding(root, root->right);

    return root;
}

struct node* create_decode_node(struct node *root, char *str, int letter)
{
    int i;
    struct node *temp;
    if (root == NULL) {
        root = (struct node *)calloc(1, sizeof(struct node));
        root->letter = -1;
        root->right = NULL;
        root->left = NULL;
    }
    temp = root;
    for (i = 0; str[i] != '\n'; i++) {
        if (str[i] == '0') {
            if (temp->left == NULL) {
                temp->left = (struct node *)calloc(1, sizeof(struct node));
                temp->left->letter = -1;
                temp->left->left = NULL;
                temp->left->right = NULL;
            }
            temp = temp->left;
        }
        else if (str[i] == '1') {
            if (temp->right == NULL) {
                temp->right = (struct node *)calloc(1, sizeof(struct node));
                temp->right->letter = -1;
                temp->right->left = NULL;
                temp->right->right = NULL;
            }
            temp = temp->right;
        }
        else printf("ERROR: There is a number not '0' or '1'\n");
    }
    temp->letter = letter;
    return root;
}

FILE* reading_point(char *file, FILE *fp)
{
    fp = fopen(file, "r");
    char str[128];
    int count;

    while (fgets(str, 128, fp)) {
        if (str[0] != ' ' && str[1] != ' ' && str[2] != ' ' && str[3] != ' ') {
            for (count = 0; count < 128; count++) {
                if (str[count] == '\0') break;
            }
            fseek(fp, -count, SEEK_CUR);
            break;
        }
    }
    return fp;
}

struct node* create_decode_tree(char *file, FILE *fp)
{
    fp = fopen(file, "r");
    char str[128];
    int letter, start, i;
    struct node *root = NULL;

    while (fgets(str, 128, fp)) {
        if (str[0] != ' ' && str[1] != ' ' && str[2] != ' ' && str[3] != ' ') break;
        for (start = 0; start < 128; start++) {
            if (str[start] == ' ') {
                str[start] = '\0';
                break;
            }
        }
        letter = atoi(str);
        root = create_decode_node(root, str+start+1, letter);
	}
	fclose(fp);
	return root;
}

void free_tree(struct node *root)
{
    if (root->left != NULL) free_tree(root->left);
    if (root->right != NULL) free_tree(root->right);
    free(root);
}

int main(int argc, char **argv)
{
	struct node *root, *heap_root, *table, *order_table;
	struct Queue *queue;
	int i;
	char *letters = "letter";
	char *trees = "tree";
	char *codes = "code";
	char *decodes = "decode";

	if(!strcmp(argv[2], letters) || !strcmp(argv[2], trees) || !strcmp(argv[2], codes))
	{
		table = init_table(argv[1]);
		order_table = init_order_table(table);
		queue = create_queue(table);
		heap_root = create_heap(table, queue);
		root = create_tree(heap_root, queue, table);

		if(!strcmp(argv[2], letters)) {
            coding(root);
			letter(root, order_table);
		}
		if(!strcmp(argv[2], trees)) {
			tree(root);
		}
		if(!strcmp(argv[2], codes)) {
            coding(root);
			code(root, order_table, argv[1]);
		}

        free_tree(root);
        free(queue);
    }

	else if(!strcmp(argv[2], decodes))
	{
	    FILE *fp;
		root = create_decode_tree(argv[1], fp);
		fp = reading_point(argv[1], fp);
		decode(root, fp);

		free_tree(root);
	}

	return 0;
}
