#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "akinator_struct.h"
#include "akinator_dump.h"
#include "lib/stack/stack.h"

Tree_t *TreeCtor() {

    Tree_t *tree = (Tree_t *) calloc(1, sizeof(Tree_t));
    if (tree == NULL) {
        fprintf(stderr, "ERROR with creating tree\n");
        return NULL;
    }

    TreeNode_t *root = (TreeNode_t *) calloc(1, sizeof(TreeNode_t));
    if (root == NULL) {
        fprintf(stderr, "ERROR with creating root\n");
        return tree;
    }

    root->data = "Don't know who";
    root->data_hash = -1;

    tree->root = root;
    tree->nodes_count = 1;

    return tree;
}

void DeleteNode(Tree_t *tree, TreeNode_t *node) {

    assert(node);
    assert(tree);

    if (node->left != NULL)
        DeleteNode(tree, node->left);

    if (node->right != NULL)
        DeleteNode(tree, node->right);

    free(node);
    tree->nodes_count--;
}

void TreeNodeDtor(Tree_t *tree, TreeNode_t *node) {

    if (tree == NULL || node == NULL)
        return;

    DeleteNode(tree, node);
}

char *GetAnswerString() {

    int curr_size = 16, curr_pos = 0;
    char *answer = (char *) calloc(curr_size, sizeof(char));
    if (answer == NULL)
        return answer;
    int curr_char = 0;
    scanf(" %c", &curr_char);

    while (curr_char != '\0' && curr_char != '\n') {
        answer[curr_pos++] = curr_char;

        if (curr_pos >= curr_size) {
            curr_size += 16;
            answer = (char *) realloc(answer, sizeof(char) * curr_size);

            if (answer == NULL)
                return answer;
        }

        curr_char = getchar();
    }

    answer[curr_pos] = '\0';

    return answer;
}

int InsertNewVariant(Tree_t *tree, TreeNode_t *node) {

    Return_If_Tree_Error(tree);
    if (node == NULL)
        return InsertAkinatorError;

    TreeNode_t *new_node = (TreeNode_t *) calloc(1, sizeof(TreeNode_t));
    TreeNode_t *old_node = (TreeNode_t *) calloc(1, sizeof(TreeNode_t));

    old_node->data = node->data;
    old_node->data_hash = CountStringHashDJB2(node->data);

    node->right = old_node;
    node->left = new_node;

    fprintf(OUTPUT_FILE, "What it was?\n");
    new_node->data = GetAnswerString();

    if (new_node->data == NULL) {
        fprintf(OUTPUT_FILE, "ERROR: Answer error.\n");
        return AnswerAkinatorError;
    }

    new_node->data_hash = CountStringHashDJB2(new_node->data);

    fprintf(OUTPUT_FILE, "What's the difference between " TREE_TYPE_OUTPUT " and " TREE_TYPE_OUTPUT "?"
                          TREE_TYPE_OUTPUT " is...\n",
                          new_node->data, old_node->data, new_node->data);
    node->data = GetAnswerString();

    if (node->data == NULL) {
        fprintf(OUTPUT_FILE, "ERROR: Answer error.\n");
        return AnswerAkinatorError;
    }

    node->data_hash = CountStringHashDJB2(node->data);

    tree->nodes_count += 2;
    Return_Tree_Error(tree);
}

int FindObject(Tree_t *tree) {

    Return_If_Tree_Error(tree);

    TreeNode_t *curr_node = tree->root;

    while (curr_node != NULL) {

        fprintf(OUTPUT_FILE, "Is your object " TREE_TYPE_OUTPUT "? y/n\n", curr_node->data);

        int curr_answer = 0;
        scanf(" %c", &curr_answer);

        if (curr_answer == 'y') {
            if (curr_node->left == NULL)
                fprintf(OUTPUT_FILE, "It is" TREE_TYPE_OUTPUT "\n", curr_node->data);

            curr_node = curr_node->left;

        } else if (curr_answer == 'n') {
            if (curr_node->right == NULL) {
                int insert_error = InsertNewVariant(tree, curr_node);

                if (insert_error != NoTreeError) {
                    fprintf(OUTPUT_FILE, "Error with new value.\n");
                    break;
                }

                break;
            }

            curr_node = curr_node->right;

        } else
            fprintf(OUTPUT_FILE, "Wrong answer. Try arain\n");
    }

    Return_Tree_Error(tree);
}

int GiveDefinitionForObject(Tree_t *tree) {

    Return_If_Tree_Error(tree);

    fprintf(OUTPUT_FILE, "Print name of the object\n");
    char *object_name = GetAnswerString();

    Stack_t object_path = {};
    StackCtor(&object_path, 5);

ON_DEBUG(fprintf(stderr, "stack ptr [%p]\n", &object_path));

    TreeNode_t *search_result = FindObjectByName(tree, tree->root, object_name, &object_path);

    if (search_result == NULL) {
        fprintf(OUTPUT_FILE, "Object didn't find\n");

        Return_Tree_Error(tree);
    }

    return PrintObjectCharacteristics(tree, &object_path);
}

int PrintObjectCharacteristics(Tree_t *tree, Stack_t *object_path) {

    Return_If_Tree_Error(tree);
    assert(tree);

    TreeNode_t *curr_node = tree->root;
    fprintf(OUTPUT_FILE, "\nObject ");

    while (curr_node != NULL) {
        int curr_path = 0;
        StackPop(object_path, &curr_path);

        if (curr_path != 'l' && curr_path != 'r')
            break;

        if (curr_path == 'l') {
            fprintf(OUTPUT_FILE, TREE_TYPE_OUTPUT "; ", curr_node->data);
            curr_node = curr_node->left;

        } else {
            fprintf(OUTPUT_FILE, "not " TREE_TYPE_OUTPUT "; ", curr_node->data);
            curr_node = curr_node->right;

        }
    }
    fprintf(OUTPUT_FILE, "\n\n");

    Return_Tree_Error(tree);
}

TreeNode_t *FindObjectByName(Tree_t *tree, TreeNode_t *curr_node,
                             const char *object_name, Stack_t *object_path) {

    assert(tree);
    assert(curr_node);
    assert(object_name); // в рекурсии ассерты фигачу

    // 'l' - лево, 'r' - право
    int object_hash = CountStringHashDJB2(object_name);

    if (curr_node->data_hash == object_hash)
        if (strcmp(curr_node->data, object_name) == 0)
            return curr_node;

    if (curr_node->left != NULL) {

        TreeNode_t *search_result = FindObjectByName(tree, curr_node->left, object_name, object_path);

        if (search_result != NULL) {
            StackPush(object_path, 'l');
            return search_result;
        }
    }

    if (curr_node->right != NULL) {

        TreeNode_t *search_result = FindObjectByName(tree, curr_node->right, object_name, object_path);

        if (search_result != NULL) {
            StackPush(object_path, 'r');
            return search_result;
        }
    }

    return NULL;
}

int CountStringHashDJB2(const char *curr_string) {

    assert(curr_string);

    int hash = 5381, curr_num = 0;    // магическое число

    while (curr_string[curr_num]) {
        hash = ((hash << 5) + hash) + curr_string[curr_num];   // *33

        curr_num++;
    }

    return hash;
}
