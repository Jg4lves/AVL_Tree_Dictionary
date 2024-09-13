// To compile: gcc -o main main.c
// To run: ./main input output

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYNONYMS 10

typedef struct node {
    int balance;
    char key[31];
    struct node* left;
    struct node* right;
    char synonym_list[MAX_SYNONYMS][31];
    int num_synonyms;
} node;

node* search(node* root, const char name[31]) {
    if (root == NULL || strcmp(root->key, name) == 0) {
        return root;
    }

    if (strcmp(name, root->key) < 0) {
        return search(root->left, name);
    } else {
        return search(root->right, name);
    }
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int TreeHeight(node *root) {
    if (root == NULL) {
        return 0;
    } else {
        return root->balance;
    }
}

void update_balance(node** root) {
    if (*root) {
        int left_height = TreeHeight((*root)->left);
        int right_height = TreeHeight((*root)->right);
        (*root)->balance = 1 + max(left_height, right_height);
    }
}

// balance == +2, +1
// Assume the update_balance function is defined correctly

void rotate_L(node** root) {
    if (*root && (*root)->right) {
        node* pivot = (*root)->right;
        (*root)->right = pivot->left;
        pivot->left = *root;
        *root = pivot;
        update_balance(&((*root)->left));
        update_balance(root);
    }
}

void rotate_R(node** root) {
    if (*root && (*root)->left) {
        node* pivot = (*root)->left;
        (*root)->left = pivot->right;
        pivot->right = *root;
        *root = pivot;
        update_balance(&((*root)->right));
        update_balance(root);
    }
}

void rotate_L_R(node** root) {
    if (*root && (*root)->left) {
        rotate_L(&((*root)->left));
        rotate_R(root);
    }
}

void rotate_R_L(node** root) {
    if (*root && (*root)->right) {
        rotate_R(&((*root)->right));
        rotate_L(root);
    }
}

int getBalance(node* root) {
    if (root == NULL)
        return 0;
    return TreeHeight(root->left) - TreeHeight(root->right);
}

node* insert(node** root, const char name[31], const char synonyms[MAX_SYNONYMS][31], int num_synonyms) {
    if ((*root) == NULL) {
        node* new_node = (node*)malloc(sizeof(node));
        if (new_node == NULL) {
            printf("Error: Could not allocate memory for 'node' structure.\n");
            return (*root);
        }

        strcpy(new_node->key, name);
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->balance = 1;
        new_node->num_synonyms = num_synonyms; // Set the number of synonyms

        // Copy the list of synonyms
        for (int i = 0; i < num_synonyms; i++) {
            strcpy(new_node->synonym_list[i], synonyms[i]);
        }

        return new_node;
    }

    if (strcmp(name, (*root)->key) < 0) {
        (*root)->left = insert(&(*root)->left, name, synonyms, num_synonyms);
    } else if (strcmp(name, (*root)->key) > 0) {
        (*root)->right = insert(&(*root)->right, name, synonyms, num_synonyms);
    } else {
        // Already exists, do nothing
        return (*root);
    }

    update_balance(&(*root));
    int balanced = getBalance((*root));

    if (balanced > 1 && strcmp(name, (*root)->left->key) < 0) {
        rotate_R(&(*root));
    } else if (balanced < -1 && strcmp(name, (*root)->right->key) > 0) {
        rotate_L(&(*root));
    } else if (balanced > 1 && strcmp(name, (*root)->left->key) > 0) {
        rotate_L_R(&(*root));
    } else if (balanced < -1 && strcmp(name, (*root)->right->key) < 0) {
        rotate_R_L(&(*root));
    }

    return (*root);
}

int printSearchedElementsAndSynonyms(node* root, FILE* output, const char name[31], uint32_t count) {
    if (root == NULL) {
        fprintf(output, "?]\n");
        fprintf(output, "-\n");
        return 0;
    }

    if (count < 1) {
        fprintf(output, "["); // Start of the element list
    }

    fprintf(output, "%s", root->key);
    if (strcmp(root->key, name) == 0) {
        fprintf(output, "]\n");
        if (root->num_synonyms > 0) {
            for (int i = 0; i < root->num_synonyms; i++) {
                fprintf(output, "%s", root->synonym_list[i]);
                if (i < root->num_synonyms - 1) {
                    fprintf(output, ",");
                }
            }
            fprintf(output, "\n");
        } else {
            fprintf(output, " No synonym found for the word: %s\n", name);
        }
        return 1; // Found the word, return 1
    } else if (strcmp(root->key, name) > 0) {
        fprintf(output, "->"); // Separator for elements to the right
        return printSearchedElementsAndSynonyms(root->left, output, name, 1);
    } else if (strcmp(root->key, name) < 0) {
        fprintf(output, "->"); // Separator for elements to the left
        return printSearchedElementsAndSynonyms(root->right, output, name, 1);
    }
    return 0;
}

void freeTree(node* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        perror("Error opening output file");
        fclose(input);
        return 1;
    }

    uint32_t numbers;
    char word[31];
    char synonyms[MAX_SYNONYMS][31];
    int num_synonyms;
    node* tree = NULL;
    uint32_t query_count;

    if (fscanf(input, "%u", &numbers) == 1) {
        
    }

    uint32_t i = 0;
    while (i < numbers) {
        i++;
        if (fscanf(input, "%30s %d", word, &num_synonyms) == 2) {
            // Read each individual synonym
            for (int j = 0; j < num_synonyms; j++) {
                if (fscanf(input, "%30s", synonyms[j]) != 1) {
                    printf("Error reading synonym.\n");
                    return 1;
                }
            }

            // Add the word and its synonyms to the tree
            tree = insert(&tree, word, synonyms, num_synonyms);
        }
    }

    if (fscanf(input, "%u", &query_count) != 1) {
        if (feof(input)) {
            printf("Error: End of file reached before reading the number of queries.\n");
            fclose(input);
            fclose(output);
            return 1;
        } else if (ferror(input)) {
            perror("Error reading the number of queries");
            fclose(input);
            fclose(output);
            return 1;
        } else {
            printf("Unknown error reading the number of queries.\n");
            fclose(input);
            fclose(output);
            return 1;
        }
    }

    for (uint32_t k = 0; k < query_count; k++) {
        char query[31];
        if (fscanf(input, "%30s", query) != 1) {
            printf("Error reading query.\n");
            fclose(input);
            fclose(output);
            return 1;
        }

        printSearchedElementsAndSynonyms(tree, output, query, 0);
    }

    fclose(input);
    fclose(output);
    freeTree(tree);
    return 0;
}