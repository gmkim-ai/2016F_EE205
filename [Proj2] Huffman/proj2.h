struct node
{
	int letter;
	int count;
	struct node *left, *right, *next, *next_order;
	struct node *heap_left, *heap_right;
	char code[50];

	/* You can add any variables here */
};
struct Queue
{
    int front, rear;
    int size;
    struct node* *array;
};

/* requirements */
void letter(struct node *root, struct node *order_table);
void tree(struct node *root);
void code(struct node *root, struct node *order_table, char *file);
void decode(struct node *root, FILE *fp);

/* count function */
struct node* init_table(const char *file);
struct node* init_order_table(struct node *table);
struct Queue* create_queue(struct node *table);

/* heap functions */
struct node* order_down(struct node *root, struct Queue *queue, struct node *table, struct node *temp);
struct node* pop(struct node *heap_root, struct Queue *queue, struct node *table);
struct node* push(struct node *heap_root, struct Queue *queue, struct node *table, struct node *new_node);
struct node* create_heap(struct node *table, struct Queue *queue);
struct node* insert(struct node *root, struct Queue *queue, struct node *temp);


/* tree functions */
struct node* order_up(struct node *root, struct Queue *queue, struct node *table, struct node *temp);
void Add(struct node *temp, struct Queue* queue);
void Delete(struct Queue* queue);
struct node* combine(struct node *node1, struct node *node2);
struct node* create_tree(struct node *heap_root, struct Queue *queue, struct node *table);
struct node* create_decode_node(struct node *root, char *str, int letter);
FILE* reading_point(char *file, FILE *fp);
struct node* create_decode_tree(char *file, FILE *fp);
void left_coding(struct node *prev_root, struct node *root);
void right_coding(struct node *prev_root, struct node *root);
struct node* coding(struct node *root);
void free_tree(struct node *root);
