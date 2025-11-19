#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "speak.h"

//#define TX_USE_SPEAK
//#include "TXLib.h"

#include "akinator_struct.h"
#include "akinator_dump.h"
#include "lib/stack/stack.h"
#include "lib\onegin\buffer.h"

#include <Mmsystem.h>
#include <mciapi.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")


//=============================================================================

HWND AkinatorWindow = CreateAkinatorWindow();

//=============================================================================

HWND CreateAkinatorWindow() {

    //mciSendString("open \"ringtone.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
    //mciSendString("play mp3", NULL, 0, NULL);

    //txWaveData_t ringtone = txWaveLoadWav("ringtone.wav");
    //txWaveOut(1000, 0, 30, ringtone);
    //txWaveOut (txWaveLoadWav ("ringtone.wav"));

    txSpeak ("<speak version=\"1.0\" xmlns='http://www.w3.org/2001/10/synthesis' xml:lang=\"ru\">"
             "<p> Thank you for choosing my akinator programm.</p></speak>");

    HWND AkinatorWindow = txCreateWindow(WINDOW_X_SIZE, WINDOW_Y_SIZE);

    txSetColor(TX_BROWN);
    txSelectFont("Comic Sans MS", TEXT_SIZE);

    ChangeImage(boy_image_cringe_1);

    //txPlaySound("ringtone.wav", SND_NOSTOP);

    atexit(DestroyAkinatorWindow);

    return AkinatorWindow;
}

void DestroyAkinatorWindow() {

    txPlaySound("ringtone.wav", SND_SYNC);
    txDestroyWindow();
    printf("Window closed succesfully\n");
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

AkinatorAnswers GetUserAnswer() {

    ChangeImage(boy_image_cringe_1);

    PrintHelp();

    int answer = getchar();

    while (!CheckIfAnswerCorrect()) {
        printf("Syntaxis error\n\n");
        PrintHelp();
        answer = getchar();
    }

    switch (answer) {
        case 'g':
            return GuessCharacterAnswer;

        case 'p':
            return PrintDefinitionAnswer;

        case 's':
            return SaveProgressAnswer;

        case 'l':
            return LoadBaseAnswer;

        case 'd':
            return PrintDifferenceAnswer;

        case 'f':
            return EndProgrammAnswer;

        default:
            return ErrorAkinatorAnswer;
    }
}

void PrintHelp() {

    txSpeak ("\a <speak version=\"1.0\" xmlns='http://www.w3.org/2001/10/synthesis' xml:lang=\"it\">"
            "<p> To guess you character push g. </p>"
            "<p> To print object definition push p. </p>"
            "<p> To save information base in file push s. </p>"
            "<p> To load information base from file push l. </p>"
            "<p> To compare objects push d. </p>"
            "<p> To end the programm push f.</p></speak>");

    printf("\nGuess your character [g]\n"
            "Print object definition [p]\n"
            "Save progress [s]\n"
            "Load base from file [l]\n"
            "Print difference between objects [d]\n"
            "F to pay respects (end the programm)[f]\n\n");
}

void AkinatorRun(AkinatorAnswers answer, Tree_t *tree) {

    switch(answer) {
        case GuessCharacterAnswer:
            FindObject(tree);
            break;

        case PrintDefinitionAnswer:
            GiveDefinitionForObject(tree);
            break;

        case SaveProgressAnswer:
            PrintAkinatorBaseInOutputFile(tree);
            break;

        case LoadBaseAnswer:
            tree->root = GetAkinatorBaseFromFile();
            break;

        case PrintDifferenceAnswer:
            CompareTwoObjects(tree);
            break;

        default:
            fprintf(OUTPUT_FILE, "\nNo such option.\n\n");
    }
}

//-----------------------------------------------------------------------------

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

    while (curr_node != NULL) {

        ChangeImageAndText(boy_image_train, "Is your object?...");
        txSpeak("\v Is your object " TREE_TYPE_OUTPUT "? [y]/[n]\n", curr_node->data);

        int curr_answer = 0;
        scanf(" %c", &curr_answer);

        Continue_If_Answer_Error();

        if (curr_answer == 'y') {
            if (curr_node->left == NULL) {
                ChangeImageAndText(boy_image_ok, "I'd figured out your object!");

                txSpeak("\v Your object is " TREE_TYPE_OUTPUT "\n", curr_node->data);
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
            ChangeImageAndText(boy_image_fuck_you, "No such vatiant. Try arain");
            txSpeak("\vNo such vatiant. Try arain\n");
        }
    }

    Return_Tree_Error(tree);
}


//=============================================================================

void ChangeImage(const HDC image) {

    txBitBlt ( 0, 0, image);
}

void ChangeText(const char *text) {

    assert(text);

    txTextOut(TEXT_X_POS, TEXT_Y_POS, text,  txDC() );
}

void ChangeImageAndText(const HDC image, const char *text) {

    assert(text);

    ChangeImage(image);
    ChangeText(text);
}

//=============================================================================

int GiveDefinitionForObject(Tree_t *tree) {

    Return_If_Tree_Error(tree);

    // TODO: ChangeImage(), ChangeText()
    // TODO: ChangeImageAndText()

    ChangeImageAndText(boy_image_cringe_2, "Print your object");

    //txBitBlt ( 0, 0, boy_image_cringe_2);
    //txTextOut(40, 200, "Print your object",  txDC() );

    txSpeak("\vPrint name of the object\n");
    char *object_name = GetAnswerString();

    Stack_t object_path = {};
    StackCtor(&object_path, 5);

    TreeNode_t *search_result = FindObjectByName(tree, tree->root, object_name, &object_path);

    if (search_result == NULL) {
        ChangeImageAndText(boy_image_dissapoint, "Object didn't find :(((");
        txSpeak("\vObject didn't find\n");

        Return_Tree_Error(tree);
    }

    ChangeImageAndText(boy_image_ok, "Your object is...");
    txSpeak("\vYour object is ");
    return PrintObjectCharacteristicsSinceNode(tree, tree->root, &object_path);
}

int PrintObjectCharacteristicsSinceNode(Tree_t *tree, TreeNode_t *curr_node, Stack_t *object_path) {

    Return_If_Tree_Error(tree);
    assert(object_path);
    assert(curr_node);

    while (curr_node->left != NULL && curr_node->right != NULL) {
        int curr_path = NoNode;
        StackPop(object_path, &curr_path);

        if (curr_path != LeftNode && curr_path != RightNode)
            break;

        if (curr_path == LeftNode) {
            txSpeak("\v" TREE_TYPE_OUTPUT "; ", curr_node->data);
            curr_node = curr_node->left;

        } else {
            txSpeak("\vnot " TREE_TYPE_OUTPUT "; ", curr_node->data);
            curr_node = curr_node->right;

        }
    }
    fprintf(OUTPUT_FILE, "\n");

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
            StackPush(object_path, LeftNode);
            return search_result;
        }
    }

    if (curr_node->right != NULL) {

        TreeNode_t *search_result = FindObjectByName(tree, curr_node->right, object_name, object_path);

        if (search_result != NULL) {
            StackPush(object_path, RightNode);
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

    ChangeImageAndText(boy_image_cringe_2, "Print your objects...");

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
        txSpeak("\vObject hasn't been found\n");

        Return_Tree_Error(tree);
    }

    return PrintObjectsDifferent(tree, &first_object_path, &second_object_path);
}

int PrintObjectsDifferent(Tree_t *tree, Stack_t *first_object_path, Stack_t *second_object_path) {

    Return_If_Tree_Error(tree);
    assert(first_object_path);  // Вывести общие признаки
    assert(second_object_path);

    TreeNode_t *curr_node = tree->root;

    while (curr_node != NULL) {
        int first_path = NoNode, second_path = NoNode;
        StackPop(first_object_path, &first_path);
        StackPop(second_object_path, &second_path);

        if (first_path != LeftNode && first_path != RightNode || second_path != LeftNode && second_path != RightNode) {
            txSpeak("\vComparing objects error\n");
            break;

        }

        if (first_path != second_path) {

ON_DEBUG(printf("first path is %d, second path is %d, LeftNode is %d, right node is %d\n", first_path, second_path, LeftNode, RightNode));

            txBitBlt ( 0, 0, boy_image_train);

            TreeNode_t *parent_node = curr_node;
            StackPush(first_object_path, first_path);
            StackPush(second_object_path, second_path);

            ChangeImageAndText(boy_image_ok, "First object characteristics...");
            txSpeak("\v\nThe difference, is that First object is ");
            PrintObjectCharacteristicsSinceNode(tree, curr_node, first_object_path);

            ChangeImageAndText(boy_image_ok, "Second object characteristics...");
            txSpeak("\vAnd Second object is ");
            PrintObjectCharacteristicsSinceNode(tree, parent_node, second_object_path);

            Return_Tree_Error(tree);

        } else {
            ChangeImageAndText(boy_image_train, "Both objects are...");
            txSpeak("\v\nBoth objects are ");

            if (first_path == LeftNode)
                txSpeak("\v" TREE_TYPE_OUTPUT "; ", curr_node->data);

            else
                txSpeak("\vnot " TREE_TYPE_OUTPUT "; ", curr_node->data);


        }

        if (first_path == LeftNode)
            curr_node = curr_node->left;

        else
            curr_node = curr_node->right;

    }

    txSpeak("\vComparing objects error\n");

    Return_Tree_Error(tree);
}
