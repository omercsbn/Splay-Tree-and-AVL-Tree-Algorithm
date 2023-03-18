#include <stdio.h>
#include <stdlib.h>

//definition of node for AVL tree.
typedef struct AVLNode {
    char key;  //node key.
    int value;  //occurence frequency.
    int height;  //height of node.
    struct AVLNode *left;  //left child pointer.
    struct AVLNode *right;  //right child pointer.
} AVLNode;

//definition of node for splay tree.
typedef struct SplayNode {
    char key;  //node key.
    int value;  //occurence frequency.
    struct SplayNode *left;  //left child pointer.
    struct SplayNode *right;  //right child pointer.
    struct SplayNode *parent;  //parent of node pointer.
} SplayNode;

//avl tree function prototypes.
AVLNode* avl_create_node(char key, int value);
int avl_get_height(AVLNode *node);
int avl_get_balance_factor(AVLNode *node);
int max(int a, int b);
AVLNode* avl_rotate_left(AVLNode *root);
AVLNode* avl_rotate_right(AVLNode *root);
void avl_preorder(AVLNode *node);

//splay tree function prototypes.
SplayNode* splay_create_node(char key, int value);
SplayNode* splay_splay(SplayNode *node, char key);
void splay_preorder(SplayNode *node);


// interesting. i get undefined reference to `max' error but i include math.h anyways i define max function and remove math.h.
// ex. a=2, b=3 so a>b is false return b.
int max(int a, int b) {
    return (a > b) ? a : b;
}

// memory allocation for avl node also initialize height, value(occurence freq) and node left and node right is NULL.
// if in text file one key existed 2 times value will be updated.
// also height will be updated new node inserted or node rotated.
AVLNode* create_avl_node(char key, int value) {
    AVLNode *node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->value = 1;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}
// memory allocation for avl node also initialize height, value(occurence freq) and node left and node right is NULL.
// if in text file one key existed 2 times value will be updated.
// ! splay nodes dont need to store height because we dont balance tree or get balance factor of node. !
// ! splay tree use rotations and modify tree and nodes will be closer to the root of tree. !
SplayNode* create_splay_node(char key, int value) {
    SplayNode *node = (SplayNode*)malloc(sizeof(SplayNode));
    node->key = key;
    node->value = 1;
    node->left = node->right = node->parent = NULL;
    return node;
}

//if node exists return node height, if node is null(dont exist) return 0.
int get_avl_height(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

//get avl balance factor, difference from left and right subtrees height of avl tree.
int get_avl_balance_factor(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return get_avl_height(node->left) - get_avl_height(node->right);
}

//rotate left node in avl tree.
AVLNode* rotate_left(AVLNode *root) {
    AVLNode *new_root = root->right;
    AVLNode *temp = new_root->left;

    //rotate tree. left child of new root will be root right child and right child of new root will be left child of root.
    new_root->left = root;
    root->right = temp;

    //update heights of root and new root after left rotation.
    root->height = 1 + max(get_avl_height(root->left), get_avl_height(root->right));
    new_root->height = 1 + max(get_avl_height(new_root->left), get_avl_height(new_root->right));

    //return new root.
    return new_root;
}
    
//rotate node right single time. get pointer of root, new root is after rotation. temp is pointer and store right child of new root(left child of prev root).
AVLNode* rotate_right(AVLNode *root) {
    AVLNode *new_root = root->left;
    AVLNode *temp = new_root->right;

    //rotate tree. right child of new root will be root left child and left child of new root will be left child of root.
    new_root->right = root;
    root->left = temp;

    //update heights of root and new root after left rotation.
    root->height = 1 + max(get_avl_height(root->left), get_avl_height(root->right));
    new_root->height = 1 + max(get_avl_height(new_root->left), get_avl_height(new_root->right));

    //return new root.
    return new_root;
}

//insert a new node to AVL tree. if tree is unbalanced after the insertion perform rotations to restore balance.
AVLNode* avl_insert(AVLNode *root, char key, int *comparisons, int *rotations) {
    //if tree is empty create a new node.
    if (root == NULL) {
        return create_avl_node(key, 1);
    }

    //increment number of comparisons.
    (*comparisons)++;

    //if key is smaller than the current node key, insert the node left.
    if (key < root->key) {
        root->left = avl_insert(root->left, key, comparisons, rotations);
    }
    //if key is larger than the current node key, insert the node right.
	else if (key > root->key) {
        root->right = avl_insert(root->right, key, comparisons, rotations);
    }
    //if keys are equal that means freq will be +1 and return same node.
	else {
        root->value++;
        return root;
    }

    //update height of current node.
    root->height = 1 + max(get_avl_height(root->left), get_avl_height(root->right));

     //check tree is balanced or not. get balance factor and write back to integer variable balance.
    int balance_factor = get_avl_balance_factor(root);

    //if tree is not balanced then there are 4 cases.

    //node left left is not balanced. rotate 1 time. balance factor is greater than 1 and key is less than node left key. rotate right.
    if (balance_factor > 1 && key < root->left->key) {
        (*rotations)++;
        return rotate_right(root);
    }
    //node right right is not balanced. rotate 1 time. balance factor is greater than 1 and key is less than node right key. rotate left.
    if (balance_factor < -1 && key > root->right->key) {
        (*rotations)++;
        return rotate_left(root);
    }
    //node left right is not balanced. rotate 2 times. balance factor is greater than 1 and key is greater than node left key. 
	//first rotate left node left and then rotate right node.
    if (balance_factor > 1 && key > root->left->key) {
        (*rotations) += 2;
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    //node right left is not balanced. rotate 2 times. balance factor is greater than 1 and key is greater than node right key. 
	//first rotate right node right and then rotate left node.
    if (balance_factor < -1 && key < root->right->key) {
        (*rotations) += 2;
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

     //return node.
    return root;
}



//splay operation on given node in splay tree, rotate node and node ancestors, if node is root then finish. 
SplayNode* splay(SplayNode *node, SplayNode **root, int *splay_ops) {
    //node is root, just return node.
    if (node == *root) {
        return node;
    }

    //if node has parent splay operations increments.
    if (node->parent != NULL) {
        (*splay_ops)++;
    }

    SplayNode *parent = node->parent;
    SplayNode *grandparent = (parent == NULL) ? NULL : parent->parent;

    if (parent == NULL) {
        //node dont have parent so it is already root, return node.
        return node;
    } else if (grandparent == NULL) {
        // node have parent but no grandparent so one zig is enough to move node to root.
        if (parent->left == node) {
        	// node is left child of parent.
        	// right zig.
            parent->left = node->right;
            if (node->right != NULL) {
                node->right->parent = parent;
            }
            node->right = parent;
        } else {
        	// node is right child of parent.
        	// left zig.
            parent->right = node->left;
            if (node->left != NULL) {
                node->left->parent = parent;
            }
            node->left = parent;
        }
        node->parent = NULL;
        parent->parent = node;
        *root = node;
        return node;
    } else {
        // node has grandparent, zig-zig or zig-zag.
        SplayNode *ggp = grandparent->parent;
        if (parent->left == node && grandparent->left == parent) {
            // zig-zig left for node.
            grandparent->left = parent->right;
            if (parent->right != NULL)
                        parent->right->parent = grandparent;
            parent->right = grandparent;
            grandparent->parent = parent;
            node->right = parent->left;
            if (parent->left != NULL) {
                parent->left->parent = node;
            }
            parent->left = node;
            node->parent = parent;
        } else if (parent->right == node && grandparent->right == parent) {
            // zig-zig right for node.
            grandparent->right = parent->left;
            if (parent->left != NULL) {
                parent->left->parent = grandparent;
            }
            parent->left = grandparent;
            grandparent->parent = parent;
            node->left = parent->right;
            if (parent->right != NULL) {
                parent->right->parent = node;
            }
            parent->right = node;
            node->parent = parent;
        } else if (parent->right == node && grandparent->left == parent) {
            // zig-zag left for node.
            parent->right = node->left;
            if (node->left != NULL) {
                node->left->parent = parent;
            }
            node->left = parent;
            parent->parent = node;
            grandparent->left = node->right;
            if (node->right != NULL) {
                node->right->parent = grandparent;
            }
            node->right = grandparent;
            grandparent->parent = node;
        } else {
            // zig-zag right for node.
            parent->left = node->right;
            if (node->right != NULL) {
                node->right->parent = parent;
            }
            node->right = parent;
            parent->parent = node;
            grandparent->right = node->left;
            if (node->left != NULL) {
                node->left->parent = grandparent;
            }
            node->left = grandparent;
            grandparent->parent = node;
        }

        // update root of splay tree.
        *root = node;
        return node;
    }
}

SplayNode* splay_insert(SplayNode *node, int key, int *comparisons, int *splay_ops) {
    //tree is empty, create new node.
    if (node == NULL) {
        *comparisons += 1;
        SplayNode *new_node = (SplayNode*)malloc(sizeof(SplayNode));
        new_node->key = key;
        new_node->value = 1;
        new_node->parent = NULL;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }

    //increment number of comparisons.
    *comparisons += 1;

    //if key is smaller than the current node key, insert the node left.
    if (key < node->key) {
        node->left = splay_insert(node->left, key, comparisons, splay_ops);
        node->left->parent = node;
    }
    //if key is greater than the current node key, insert the node right.
    else if (key > node->key) {
        node->right = splay_insert(node->right, key, comparisons, splay_ops);
        node->right->parent = node;
    }
    //if key is equal to the current node key, increment the node value(occurence freq).
    else {
        node->value += 1;
    }

    //splay operations for node.
    return splay(node, &node, splay_ops);
}

//print avl tree in preorder.
void avl_preorder(AVLNode *node) {
	//if root is not null get root go left then right.
	//for example        1
	//                 2  3
	// print in pre-order way 1-2-3.  
    if (node == NULL) {
        return;
    }

    //print root node in avl tree.
    printf("%d ", node->key);

    //print left subtree of root node in avl tree.
    avl_preorder(node->left);

    //print right subtree of root node in avl tree.
    avl_preorder(node->right);
}

//print splay tree in preorder.
void splay_preorder(SplayNode *node) {
	//if root is not null get root go left then right.
	//for example        1
	//                 2  3
	// print in pre-order way 1-2-3.  
    if (node == NULL) {
        return;
    }

    //print root node in splay tree.
    printf("%d ", node->key);

    //print left subtree of root node in splay tree.
    splay_preorder(node->left);

    //print right subtree of root node in splay tree.
    splay_preorder(node->right);
}
//for every key print occurrence frequency in avl tree(value).
void print_avl_frequency(AVLNode *node) {
	//if node is not null visit every existed node and find nodes(keys) and node values(frequency).
    if (node != NULL) {
        print_avl_frequency(node->left);
        printf("%d occurs %d times\n", node->key, node->value);
        print_avl_frequency(node->right);
    }
}
//for every key print occurrence frequency in splay tree(value).
void print_splay_frequency(SplayNode *node) {
	//if node is not null visit every existed node and find nodes(keys) and node values(frequency).
    if (node != NULL) {
        print_splay_frequency(node->left);
        printf("%d occurs %d times\n", node->key, node->value);
        print_splay_frequency(node->right);
    }
}

int main() {
	
	//initialize comparisions and rotations for avl tree and splay comparisions and operations. 
	//duplicated because 2 input files. i tried resetting values of variables but dont work.
    int avl_comparisons = 0;
    int avl_rotations = 0;
    int avl_comparisons2 = 0;
    int avl_rotations2 = 0;
    int splay_comparisons = 0;
    int splay_ops = 0;
    int splay_comparisons2 = 0;
    int splay_ops2 = 0;
    

    //create root node for avl tree.
    AVLNode *avl_root = NULL;
    AVLNode *avl_root2 = NULL;
    //create root node for splay tree.
    SplayNode *splay_root = NULL;
    SplayNode *splay_root2 = NULL;
    
    
	 //open file in read mode.
    FILE* input_file = fopen("input1.txt", "r");

    //read input_file.txt and write buffer.
    //initialize buffer char array(fixed size).
    char buffer[1024];
    //input_file to buffer.
    fread(buffer, 1, sizeof(buffer), input_file);

    //split the buffer and write back to input_list char pointer array. all elements splitted with backspace.
	
	//define input_list char pointer array fixed size. i tried to dynamic size array but i cant do it.
    //so it can be overflow and after project i am research dynamic size arrays in c.
    char* input_list[1024];
    //char pointer token variable use strtok function and gets buffer split by backspace.
    char* token = strtok(buffer, " ");
    //for loop initial declarations are only allowed in C99 or C11 mode so i am initializing i and j.
    int i,j = 0;

    //while loop for every element splitted by whitespace going to input_list index. input_list[0] will be 44 and input_list[2] 3 like that.
    while (token) {
    	//get every element with indexes in input_list array.
        input_list[i++] = token;
        //if token doesnt find character return null.
        token = strtok(NULL, " ");
    }

    //close input file.
    fclose(input_file);
    //initialize x variable its get char pointer input list and convert integer every element for every existed index.
	int x = 0;
    //for loop for converting char elements to integer and put integer elements to avl tree and splay tree.
    for (j ; j < i; j++) {
    	x =	atoi(input_list[j]);
        avl_root = avl_insert(avl_root, x, &avl_comparisons, &avl_rotations);
        splay_root = splay_insert(splay_root, x, &splay_comparisons, &splay_ops);
    }
    printf("Output 1\n");
    //print avl tree in pre-order with total cost(sums of avl comparisons and avl rotations).
    printf("AVL tree:\n");
	avl_preorder(avl_root);
	printf("\nTotal cost: %d\n", avl_comparisons + avl_rotations);
	
	//print splay tree in pre-order with total cost(sums of splay comparisons and splay operations).
	printf("Splay tree:\n");
	splay_preorder(splay_root);
	printf("\nTotal cost: %d\n", splay_comparisons + splay_ops);
	
	
	
	//open file in read mode.
    FILE* input_file2 = fopen("input2.txt", "r");

    //read input_file.txt and write buffer.
    //initialize buffer char array(fixed size).
    char buffer2[1024];
    //input_file to buffer.
    fread(buffer2, 1, sizeof(buffer2), input_file2);

    //split the buffer and write back to input_list char pointer array. all elements splitted with backspace.
	
	//define input_list char pointer array fixed size. i tried to dynamic size array but i cant do it.
    //so it can be overflow and after project i am research dynamic size arrays in c.
    char* input_list2[1024];
    //char pointer token variable use strtok function and gets buffer split by backspace.
    char* token2 = strtok(buffer2, " ");
    //for loop initial declarations are only allowed in C99 or C11 mode so i am initializing i and j.
    i=0;
    j=1;

    //while loop for every element splitted by backspace going to input_list index. input_list[0] will be 44 and input_list[1] 3 like that.
    while (token2) {
    	//get every element with indexes in input_list array.
        input_list2[i++] = token2;
        //if token doesnt find character return null.
        token2 = strtok(NULL, " "); 
    }


    //close input file.
    fclose(input_file2);
    //initialize x variable its get char pointer input list and convert integer every element for every existed index.
	x = 0;
    //for loop for converting char elements to integer and put integer elements to avl tree and splay tree.
    for (j ; j < i; j++) {
    	x =	atoi(input_list2[j]);
        avl_root2 = avl_insert(avl_root2, x, &avl_comparisons2, &avl_rotations2);
        splay_root2 = splay_insert(splay_root2, x, &splay_comparisons2, &splay_ops2);
    }
    printf("\nOutput 2\n");
    //print avl tree in pre-order with total cost(sums of avl comparisons and avl rotations).
    printf("AVL tree:\n");
	avl_preorder(avl_root2);
	printf("\nTotal cost: %d\n", avl_comparisons2 + avl_rotations2);
	
	//print splay tree in pre-order with total cost(sums of splay comparisons and splay operations).
	printf("Splay tree:\n");
	splay_preorder(splay_root2);
	printf("\nTotal cost: %d\n", splay_comparisons2 + splay_ops2);
		
	
    return 0;
}


