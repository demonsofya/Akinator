#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TX_USE_SPEAK
#include "TXLib.h"

#include "akinator_struct.h"
#include "akinator_dump.h"
#include "lib/stack/stack.h"
#include "lib\onegin\buffer.h"


//=============================================================================

const HDC boy_image_dissapoint    = txLoadImage("images/akinator_boy_1.bmp");
const HDC boy_image_cringe_1      = txLoadImage("images/akinator_boy_2.bmp");
const HDC boy_image_what_the_f    = txLoadImage("images/akinator_boy_3.bmp");
const HDC boy_image_ok            = txLoadImage("images/akinator_boy_4.bmp");
const HDC boy_image_train         = txLoadImage("images/akinator_boy_5.bmp");
const HDC boy_image_cringe_2      = txLoadImage("images/akinator_boy_6.bmp");
const HDC boy_image_fuck_you      = txLoadImage("images/akinator_boy_7.bmp");

//=============================================================================

FILE *AKINATOR_BASE_FILE = OpenOutputFile();
const char *DEFAULT_OUTPUT_TREE_FILE_NAME = "Akinator_output_file.txt";

FILE *INPUT_LOGFILE = OpenInputLogFile();
const char *DEFAULT_INPUT_LOGFILE_NAME = "ImputLogfileAkinator.html";


//=============================================================================

FILE* OpenOutputFile() {

    FILE *file_ptr = fopen(DEFAULT_OUTPUT_TREE_FILE_NAME, "w");
    atexit(CloseOutputFile);

    txCreateWindow(600, 800);

    txSetColor (TX_BROWN);
    txSelectFont("Comic Sans MS", 40);

    txSpeak ("\a <speak version=\"1.0\" xmlns='http://www.w3.org/2001/10/synthesis' xml:lang=\"ru\">"
            "<p> Thank you for choosing my akinator programm.</p>"
            "<p> To guess you character push g. </p>"
            "<p> To print object definition push p. </p>"
            "<p> To save information base in file push s. </p>"
            "<p> To load information base from file push l. </p>"
            "<p> To compare objects push d. </p>"
            "<p> To end the programm push f. На русском кстати тоже шпарю</p></speak>");

    txBitBlt ( 0, 0, boy_image_cringe_1);
    txTextOut(40, 200, "HELLO!",  txDC() );

    return file_ptr;
}

void CloseOutputFile() {

    fclose(OUTPUT_FILE_FOR_TREE);
    txDestroyWindow();
    printf("Output file closed succesfully\n");
}

//-----------------------------------------------------------------------------

FILE* OpenInputLogFile() {

    FILE *file_ptr = fopen(DEFAULT_INPUT_LOGFILE_NAME, "w");
    atexit(CloseInputLogFile);

    return file_ptr;
}

void CloseInputLogFile() {

    fclose(INPUT_LOGFILE);
    printf("Input logfile closed succesfully\n");
}

//=============================================================================

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

//=============================================================================

bool CheckIfAnswerCorrect() {

    char symbol = getchar();
    bool result = true;

    while (symbol != '\n' && symbol != EOF && symbol != '\0') {
        if (!isspace(symbol))
            result = false;
        symbol = getchar();
    }

    return result;
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

    txBitBlt(0, 0, boy_image_what_the_f);
    txTextOut(40, 200, "What did you guessed?",  txDC() );

    txSpeak("\v What it was?\n");
    new_node->data = GetAnswerString();

    if (new_node->data == NULL) {
        txSpeak("\vERROR: Answer error.\n");
        txBitBlt(0, 0, boy_image_fuck_you);

        return AnswerAkinatorError;
    }

    new_node->data_hash = CountStringHashDJB2(new_node->data);

    txSpeak("\v What's the difference between " TREE_TYPE_OUTPUT " and " TREE_TYPE_OUTPUT "?"
                          TREE_TYPE_OUTPUT " is...\n",
                          new_node->data, old_node->data, new_node->data);

    node->data = GetAnswerString();

    if (node->data == NULL) {
        txSpeak("\vERROR: Answer error.\n");

        txSpeak("Что за бред ты ввел");
        txBitBlt(0, 0, boy_image_fuck_you);

        return AnswerAkinatorError;
    }

    node->data_hash = CountStringHashDJB2(node->data);

    tree->nodes_count += 2;
    Return_Tree_Error(tree);
}

int FindObject(Tree_t *tree) {

    Return_If_Tree_Error(tree);

    TreeNode_t *curr_node = tree->root;

//txSpeak ("TX Library is cool!");

    while (curr_node != NULL) {

        txBitBlt(0, 0, boy_image_train);
        txTextOut(0, 200, "Is your object?...",  txDC() );
        txSpeak("\v Is your object " TREE_TYPE_OUTPUT "? [y]/[n]\n", curr_node->data);

        int curr_answer = 0;
        scanf(" %c", &curr_answer);

        Continue_If_Answer_Error();

        if (curr_answer == 'y') {
            if (curr_node->left == NULL) {
                txBitBlt(0, 0, boy_image_ok);
                txTextOut(40, 200, "I'd figured out your object!",  txDC() );

                txSpeak("\v Your object is " TREE_TYPE_OUTPUT "\n", curr_node->data);
                txSpeak("Еще одно доказательство, что машина лучше человека");
            }

            curr_node = curr_node->left;

        } else if (curr_answer == 'n') {
            if (curr_node->right == NULL) {
                int insert_error = InsertNewVariant(tree, curr_node);

                if (insert_error != NoTreeError) {
                    txSpeak("\v Error with new value.\n");
                    break;
                }

                break;
            }

            curr_node = curr_node->right;

        } else {
            txBitBlt(0, 0, boy_image_fuck_you);
            txSpeak("\vNo such vatiant. Try arain\n");
            txSpeak("За базар отвечай");
        }
    }

    Return_Tree_Error(tree);
}


//=============================================================================


int GiveDefinitionForObject(Tree_t *tree) {

    Return_If_Tree_Error(tree);

    txBitBlt ( 0, 0, boy_image_cringe_2);
    txTextOut(40, 200, "Print your object",  txDC() );

    txSpeak("\vPrint name of the object\n");
    char *object_name = GetAnswerString();

    Stack_t object_path = {};
    StackCtor(&object_path, 5);

    TreeNode_t *search_result = FindObjectByName(tree, tree->root, object_name, &object_path);

    if (search_result == NULL) {
        txSpeak("\vObject didn't find\n");

        Return_Tree_Error(tree);
    }

    return PrintObjectCharacteristicsSinceNode(tree, tree->root, &object_path);
}

int PrintObjectCharacteristicsSinceNode(Tree_t *tree, TreeNode_t *curr_node, Stack_t *object_path) {

    Return_If_Tree_Error(tree);
    assert(object_path);
    assert(curr_node);

    txTextOut(20, 200, "Your object is...",  txDC() );

    while (curr_node != NULL) {
        int curr_path = 0;
        StackPop(object_path, &curr_path);

        if (curr_path != 'l' && curr_path != 'r')
            break;

        if (curr_path == 'l') {
            fprintf(OUTPUT_FILE, TREE_TYPE_OUTPUT "; ", curr_node->data);
            curr_node = curr_node->left;

        } else {
            txSpeak("\vnot " TREE_TYPE_OUTPUT "; ", curr_node->data);
            curr_node = curr_node->right;

        }
    }
    fprintf(OUTPUT_FILE, "\n\n");

    Return_Tree_Error(tree);
}

TreeNode_t *FindObjectByName(Tree_t *tree, TreeNode_t *curr_node,
                             const char *object_name, Stack_t *object_path) {

ON_DEBUG(TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "dump before searcing object", NoTreeError));

    assert(tree);
    assert(curr_node);
    assert(object_name);
    assert(object_path);

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

//=============================================================================

int CountStringHashDJB2(const char *curr_string) {

    assert(curr_string);

    int curr_hash = 5381, curr_num = 0;    // пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ

    while (curr_string[curr_num]) {
        curr_hash = ((curr_hash << 5) + curr_hash) + curr_string[curr_num];   // *33

        curr_num++;
    }

    return curr_hash;
}


//=============================================================================

int CompareTwoObjects(Tree_t *tree) {

    Return_If_Tree_Error(tree);

    txSpeak("\vPrint first object name\n");
    char *first_object_name = GetAnswerString();

    txSpeak("\vPrint second object name\n");
    char *second_object_name = GetAnswerString();

    Stack_t first_object_path = {};
    StackCtor(&first_object_path, 5);

    Stack_t second_object_path = {};
    StackCtor(&second_object_path, 5);

    TreeNode_t *first_search_result = FindObjectByName(tree, tree->root, first_object_name, &first_object_path);
    TreeNode_t *second_search_result = FindObjectByName(tree, tree->root, second_object_name, &second_object_path);


    if (first_search_result == NULL || second_search_result == NULL) {
        txSpeak("\vObject didn't find\n");

        Return_Tree_Error(tree);
    }

    return PrintObjectsDifferent(tree, &first_object_path, &second_object_path);
}

int PrintObjectsDifferent(Tree_t *tree, Stack_t *first_object_path, Stack_t *second_object_path) {

    Return_If_Tree_Error(tree);
    assert(first_object_path);
    assert(second_object_path);

    TreeNode_t *curr_node = tree->root;

    while (curr_node != NULL) {
        int first_path = 0, second_path = 0;
        StackPop(first_object_path, &first_path);
        StackPop(second_object_path, &second_path);

        if (first_path != 'l' && first_path != 'r' || second_path != 'l' && second_path != 'r') {
            txSpeak("\vComparing objects error\n");
            break;
        }

        if (first_path != second_path) {
            txBitBlt ( 0, 0, boy_image_train);

            txTextOut(40, 200, "First object characteristics...",  txDC() );
            txSpeak("\v\nFirst object is ");
            PrintObjectCharacteristicsSinceNode(tree, curr_node, first_object_path);

            txTextOut(40, 200, "Second object characteristics...",  txDC() );
            txSpeak("\v\nSecond object is ");
            PrintObjectCharacteristicsSinceNode(tree, curr_node, second_object_path);

            Return_Tree_Error(tree);
        }

        if (first_path == 'l')
            curr_node = curr_node->left;

        else
            curr_node = curr_node->right;

    }

    txSpeak("\vComparing objects error\n");

    Return_Tree_Error(tree);
}

//=============================================================================

void PrintTreeNode(TreeNode_t *node) {

    assert(node);

    fprintf(OUTPUT_FILE_FOR_TREE, " ( ");

    fprintf(OUTPUT_FILE_FOR_TREE, " \"" TREE_TYPE_OUTPUT "\" ", node->data);

    if (node->left != NULL)
        PrintTreeNode(node->left);
    else
        fprintf(OUTPUT_FILE_FOR_TREE, " nil ");

    if (node->right != NULL)
        PrintTreeNode(node->right);
    else
        fprintf(OUTPUT_FILE_FOR_TREE, " nil ");

    fprintf(OUTPUT_FILE_FOR_TREE, " ) ");
}

int PrintAkinatorBaseInOutputFile(Tree_t *tree) {

    Return_If_Tree_Error(tree)

    PrintTreeNode(tree->root);

    Return_Tree_Error(tree);
}

//=============================================================================

TreeNode_t *GetAkinatorBaseFromFile() {

    size_t buffer_size = 0;
    char *buffer = GetBuffer(INPUT_FILE_FOR_TREE, &buffer_size);

    if (buffer == NULL) {
        txSpeak("\vInput file error\n");
        return NULL;
    }

//ON_DEBUG(txSpeak("\v\n\n%s\n\n", buffer));
ON_DEBUG(buffer_begin = buffer);

    return ReadCurrNode(&buffer);
}

void SkipSpaces(char **curr_pos) {

    assert(curr_pos);

    while (isspace(**curr_pos) && **curr_pos != '\0')
        (*curr_pos)++;

ON_DEBUG(Print_Curr_Input_Information(*curr_pos, "Dump after skipping spaces"));

    //return curr_pos;

}

TreeNode_t *ReadCurrNode(char **curr_symbol) {

    assert(curr_symbol);

    SkipSpaces(curr_symbol);

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump in the begin of recursion"));

    if (**curr_symbol == '(') {

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump in the begin of reading new node"));

        TreeNode_t *new_node = (TreeNode_t *) calloc(1, sizeof(TreeNode_t));
        (*curr_symbol)++;

        size_t data_len = 0;
        sscanf(*curr_symbol, " \"%*[^\"]\"%n", &data_len);

        new_node->data = (char *) calloc(data_len, sizeof(char));
        sscanf(*curr_symbol, " \"%[^\"]", new_node->data);
        new_node->data_hash = CountStringHashDJB2(new_node->data);

        (*curr_symbol) += data_len;
ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Tree dump after creating new node without sons"));
ON_DEBUG(TreeDumpCreateGraphFile(new_node));

ON_DEBUG(fprintf(INPUT_LOGFILE, "data size is %d, data is %s[%p]\n", data_len, new_node->data, &new_node->data));
        new_node->left = ReadCurrNode(curr_symbol);

        new_node->right = ReadCurrNode(curr_symbol);

        SkipSpaces(curr_symbol);

        if (**curr_symbol != ')') {
            txSpeak("\vRead from input file error: SYNTAX ERROR\n");
            return NULL;
        }
        (*curr_symbol)++;

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump after reading new node"));
ON_DEBUG(TreeDumpCreateGraphFile(new_node));

        return new_node;
    }

    else {
        /*
        int word_size;
        sscanf(curr_symbol, " nil%n", &word_size);  //sscanf_s


        if (word_size >= 3)
            curr_symbol += word_size;
        */
ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump before nil"));

        if (strncmp(*curr_symbol, "nil", strlen("nil")) == 0)
            *curr_symbol += strlen("nil");

        else
            txSpeak("\vRead from input file error: SYNTAX ERROR\n");

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump after nil"));

        return NULL;
    }

    return NULL;
}
