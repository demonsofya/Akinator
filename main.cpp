#include <stdio.h>

#include "akinator_struct.h"
#include "akinator_dump.h"

int main() {

    Tree_t *tree = TreeCtor();

    while (true) { // TODO: AkinatorMenu
        // if (...)
        //     break;

        // getUserAnswer
        //
        // int answer = getchar();
        //
        // while (!CheckIfAnswerCorrect() )
        //     PrintHelp();
        //     print("huinu napisal");
        //     answer = getchar();
        //
        // switch ...  - преобразование чаровского значения в енамовское
        // return answer;

        AkinatorAnswers answer = GetUserAnswer();

        if (answer == EndProgrammAnswer)
            break;

        AkinatorRun(answer, tree);
    }

    TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "Tree dump after ending the programm", NoTreeError);
    return 0;
}
