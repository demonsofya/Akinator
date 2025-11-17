#ifndef AKINATOR_DUMP_H_INCLUDED
#define AKINATOR_DUMP_H_INCLUDED


#include "akinator_struct.h"
#include "lib/stack/stack.h"

//=============================================================================


#define Return_If_Tree_Error(tree)                                              \
    {                                                                           \
        int error = TreeVerify(tree);                                           \
        if (error != 0) {                                                       \
            TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "", NoTreeError);  \
            return error;                                                       \
        }                                                                       \
    }

#define Return_Tree_Error(tree)                                                 \
    {                                                                           \
        int error = TreeVerify(tree);                                           \
        if (error != 0)                                                         \
            TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "", NoTreeError);  \
        return error;                                                           \
    }

#define Continue_If_Answer_Error()                                              \
{                                                                               \
    if (!CheckIfAnswerCorrect()) {                                              \
            fprintf(OUTPUT_FILE, "Wrong answer format. Try again\n\n");        \
            continue;                                                           \
    }                                                                           \
}
//-----------------------------------------------------------------------------
//---------DEBUG----------
static char *buffer_begin = NULL;

#define Print_Curr_Input_Information(curr_symbol, dump_information_string)                      \
    {                                                                                           \
        fprintf(SEREGA, "<h3><font color=\"#%s\"><p align=\"center\">Input DUMP"                \
                    "</p></font></h3><h4><p align=\"center\">%s</p></h4>",                \
                    ERROR_RED_COLOR, dump_information_string);                                  \
        fprintf(SEREGA, "<h5>Input dump from %s at %s:%d:</h5>",                       \
        __FILE__, __FUNCTION__, __LINE__);                                             \
        char help_curr_symbol = *curr_symbol;                                                \
        *curr_symbol = '\0';                                                     \
        fprintf(SEREGA, "<h6><font color=\"#%s\"><p align=\"left\">"                            \
                "%s</font>", LAVANDER_BLUE_COLOR, buffer_begin);               \
        *curr_symbol = help_curr_symbol;                                                        \
        fprintf(SEREGA, "<font color=\"#%s\">"                            \
                "%s</font></p></h6>", DEEP_BLUE_COLOR, curr_symbol);                   \
    }

//-----------------------------------------------------------------------------
//-----CONSTANTS--------
extern const char *DARK_RED_COLOR;
extern const char *PURPLE_BLUE_COLOR;
extern const char *LIGHT_PURPLE_PINK_COLOR;
extern const char *SOFT_PINK_COLOR;
extern const char *LIGHT_BLUE_COLOR;
extern const char *DEEP_BLUE_COLOR;
extern const char *ERROR_RED_COLOR;
extern const char *LIGHT_PASTEL_RED_COLOR;
extern const char *HOLY_WIGHT_COLOR;
extern const char *GRASS_GREEN_COLOR;
extern const char *LAVANDER_BLUE_COLOR;
extern const char *COBALITE_COLOR;
extern const char *LIGHT_YELLOW_GREEN_COLOR;
extern const char *PASTEL_PURPLE_PINK_COLOR;
extern const char *LIGHT_GRAY_PURPLE_COLOR;
extern const char *BRIGHT_GREEN_COLOR;
extern const char *LAVANDER_PINK_COLOR;
extern const char *SOFT_PINK_COLOR;

extern const int MAX_DOT_NAME_SIZE;
extern const int MAX_DOT_FILE_NAME_SIZE;
extern const int MAX_DOT_COMMAND_SIZE;
//-----------------------------------------------------------------------------
//-----LOGFILE----------

FILE* OpenLogFile();
void CloseLogFile();

#define SEREGA LOGFILE
extern FILE *LOGFILE;

extern const char *DEFAULT_LOG_FILE_NAME;
//-----------------------------------------------------------------------------

enum TreeErrors {
    NoTreeError         = 0,
    TreePtrError        = 1 << 0,
    RootPtrError        = 1 << 1,
    NodePtrError        = 1 << 2,
    SizeTreeError       = 1 << 3,
    AnswerAkinatorError = 1 << 4,
    InsertAkinatorError = 1 << 5,
    ObjectAkinatorError = 1 << 6
};

int TreeVerify(Tree_t *tree);
void TreeDump(Tree_t *tree, const char *file_name, const char *function_name,
              int line_number, const char *calling_reason_string, int curr_error);

void TreeDumpPrintErrors(Tree_t *tree, const char *file_name, const char *function_name,
              int line_number, const char *calling_reason_string, int curr_error);

void TreeDumpCreateGraphFile(TreeNode_t *root);
char *CreateDotFileName(const char *file_type);

void PrintDotFileHeader(FILE *dot_file_ptr, char *DotFilePngName);
int DrawDotNode(const TreeNode_t *node, FILE *dot_file_ptr, int rank_num, int node_num);
void PrintDotFileEnd(FILE *dot_file_ptr);

void CreateImageFromDotFile(char *DotFileName, char *DotFilePngName);

char *GetNodeName(int node_num);

#endif // AKINATOR_DUMP_H_INCLUDED
