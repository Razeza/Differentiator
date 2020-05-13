#include <zconf.h>
#include <locale>
#include "stdio.h"
#include "MyLib.h"
#include "tree_.h"

const char namein[] = "formula.txt";

struct element* Make_Tree ();
void WriteTree (struct element* head, FILE* out);
void WriteElem (struct element* node, FILE* out, int deep);

struct element* Differencial (struct element* head);
void Simplify (struct element* head);
void CopyNode (struct element* node, struct element* where);

int main ()
{
    struct element* head = Make_Tree ();

    Dump (head);

    Dump (head);

    struct element* head_dif = Differencial (head);


    ShowTree (head_dif);
    Dump (head_dif);


    Dump (head_dif);
    ShowTree (head_dif);
    return 0;
}

void WriteElem (struct element* node, FILE* out)
{
    ASSERT (node)
    ASSERT (out)

    if ((node->what >= 8) || (node->what == 3) || (node->what == 4))
        node->weight = VERY_STRONG_WEIGHT;
    else
        node->weight = LOW_WEIGHT;

    if ((node->prev != nullptr) && ((node->prev->what >= 8) || (node->prev->what == 3) || (node->prev->what == 4)))
        node->prev->weight = VERY_STRONG_WEIGHT;
    else
    {
        if (node->prev != nullptr)
            node->prev->weight = LOW_WEIGHT;
    }

    if ((node->left != nullptr) && ((node->left->what >= 8) || (node->left->what == 3) || (node->left->what == 4)))
        node->left->weight = VERY_STRONG_WEIGHT;
    else
    {
        if (node->left != nullptr)
            node->left->weight = LOW_WEIGHT;
    }

    if ((node->right != nullptr) && ((node->right->what >= 8) || (node->right->what == 3) || (node->right->what == 4)))
        node->right->weight = VERY_STRONG_WEIGHT;
    else
    {
        if (node->right != nullptr)
            node->right->weight = LOW_WEIGHT;
    }

//    if (node->prev != nullptr)
//        printf ("node - %d %lg weight - %d prev - %d %lg its weight - %d\n", node->what, node->number, node->weight, node->prev->what, node->prev->number, node->prev->weight);
//    else
//        printf ("node - %d %lg weight - %d prev - nullptr\n", node->what, node->number, node->weight);
//
//    if (node->left != nullptr)
//        printf ("%d\n", node->left->weight);

    if  (((node->left != nullptr) && (node->weight > node->left->weight) && (node->left->what != NUMBER) && ((node->left->what < 5) || (node->left->what > 7))) || (node->what == DIV))
    {

        if (node->what == DIV)
            fprintf (out, "\\frac { ");
        else
        {fprintf (out, "( ");/*printf ("                   zdec1 (\n");*/}
    }

    if (node->left != nullptr)
    {
        WriteElem (node->left, out);
    }

    if (((node->left != nullptr) && (node->weight > node->left->weight) && (node->left->what != NUMBER) && ((node->left->what < 5) || (node->left->what > 7))) || (node->what == DIV))
    {
        if (node->what == DIV)
            fprintf (out, "} ");
        else
            fprintf (out, ") ");
    }


#define DEF_DIF(what, code, weight, read, write, show, diff) case what:         \
                                                                  {             \
                                                                      write     \
                                                                      break;    \
                                                                  }
    switch (node->what)
    {
        #include "DSL.h"
    }

#undef DEF_DIF


    if (((node->right != nullptr) && (node->weight > node->right->weight) && (node->right->what != NUMBER) && ((node->right->what < 5) || (node->right->what > 7))) || (node->what == DIV)  || (node->what == POWER) || ((node->what >= 8) && (node->what <= 21)))
    {
        if ((node->what == POWER) || (node->what == DIV))
            fprintf (out, "{");
        else
        {fprintf (out, "(");/*printf ("                   zdec2 (\n");*/}
    }

    if (node->right != nullptr)
    {
        WriteElem (node->right, out);
    }

    if (((node->right != nullptr) && (node->weight > node->right->weight) && (node->right->what != NUMBER) && ((node->right->what < 5) || (node->right->what > 7))) || (node->what == DIV) || (node->what == POWER) || ((node->what >= 8) && (node->what <= 21)))
    {
        if ((node->what == POWER) || (node->what == DIV))
            fprintf (out, "}");
        else
            fprintf (out, ")");
    }

}

void WriteTree (struct element* head, FILE* out)
{
    ASSERT (head)

    fprintf (out, "\\begin{math}\n$$");
    WriteElem (head, out);
    fprintf (out, "$$\\end{math}\n");
}

int MakeElement (struct element* node, char* buff, int place, int deep, struct element** head, size_t i)
{
    char* check = (char*) calloc (MAX_SIZE_LINE, sizeof (char));
    int skip = 0;
    sscanf (buff + place, "%[^  ]%*[ ]%n", check, &skip);
    place += skip;

    if (check[0] == '(')
    {
        char what_next[MAX_LENGTH] = "";
        deep++;

        sscanf (buff + place, "%[^ ]", what_next);

        if (what_next[0] == '(')
        {
            struct element* left_node = CreateElement (NOTHING, 0);
            Merge (left_node, node, LEFT);
            MakeElement (left_node, buff, place, deep, head, i);
        }
        else
        {
            MakeElement (node, buff, place, deep, head, i);
        }
        return 0;
    }

    if (check[0] == ')')
    {
        deep--;
        if (deep == 0)
            return 0;

        char what_next[MAX_LENGTH] = "";
        sscanf (buff + place, "%[^ ]", what_next);
        if ( (what_next[0] == ')') && (node->prev != nullptr) )
            MakeElement (node->prev, buff, place, deep, head, i);
        else
            MakeElement (node, buff, place, deep, head, i);
        return 3;
    }

#define DEF_DIF(what, code, weight, read, write, show, diff) read
    while (true)
    {
        #include "DSL.h"
    }
#undef DEF_DIF
}

struct element* Make_Tree ()
{
    char* buff = BufferMaker (namein);

    struct element* beginning = nullptr;

    int deep  = 0;
    int place = 0;

    struct element* head = CreateElement (NOTHING);
    MakeElement (head, buff, place, deep, &head, 0);

    return head;
}

void CopyNode (struct element* node, struct element* where)
{
    ASSERT (node)
    ASSERT (where)

    memcpy (where, node, sizeof (struct element));

    if (node->right != nullptr)
    {
        struct element* copy_right = CreateElement (NOTHING);
        Merge (copy_right, where, RIGHT);
        CopyNode (node->right, copy_right);
        copy_right->prev = where;
    }

    if (node->left != nullptr)
    {
        struct element* copy_left = CreateElement (NOTHING);
        Merge (copy_left, where, LEFT);
        CopyNode (node->left, copy_left);
        copy_left->prev = where;
    }
}


const char say1[]  = {"Легко заметить что:\n\\newline\n\\newline\n\\newline\n"};
const char say2[]  = {"Тривиальное преобразование:\n\\newline\n\\newline\n\\newline\n"};
const char say3[]  = {"И ежу ясно:\n\\newline\n\\newline\n\\newline\n"};
const char say4[]  = {"Даже Максиму ясно\n\\newline\n\\newline\n\\newline\n"};
const char say5[]  = {"Любой 5-тиклассник знает:\n\\newline\n\\newline\n\\newline\n"};
const char say6[]  = {"Сам думай, как я это получил:\n\\newline\n\\newline\n\\newline\n"};
const char say7[]  = {"Даже моя бабушка это посчитала:\n\\newline\n\\newline\n\\newline\n"};
const char say8[]  = {"Мне это приснилось во сне:\n\\newline\n\\newline\n\\newline\n"};
const char say9[]  = {"Сам думай, как я это получил:\n\\newline\n\\newline\n\\newline\n"};
const char say10[] = {"Даже пояснять не буду:\n\\newline\n\\newline\n\\newline\n"};
const char say11[] = {"Ну это же очевино:\n\\newline\n\\newline\n\\newline\n"};
const char say12[] = {"Очевидно:\n\\newline\n\\newline\n\\newline\n"};
const char say13[] = {"Не трудно заметить:\n\\newline\n\\newline\n\\newline\n"};
const char say14[] = {"На лекции доказывалось:\n\\newline\n\\newline\n\\newline\n"};
const char say15[] = {"Аналогично:\n\\newline\n\\newline\n\\newline\n"};
const char say16[] = {"Понятно:\n\\newline\n\\newline\n\\newline\n"};

void Differ_smth (struct element* diff_node, struct element* node, FILE* out)
{
#define DEF_DIF(what, code, weight, read, write, show, diff) case what:         \
                                                                  {             \
                                                                      diff      \
                                                                      break;    \
                                                                  }



    switch (node->what)
    {
        #include "DSL.h"
    }

#undef DEF_DIF

    struct element* prev = diff_node->prev;
    diff_node->prev = nullptr;

    if (rand () % 4 + 1 == 1)
    {
        if (diff_node->right != nullptr)
            diff_node->right->prev = diff_node;

        if (diff_node->left != nullptr)
            diff_node->left->prev = diff_node;

        int b = rand () % 16 + 1;
        char say[150] = "";

#define RAND(number) if (b == number) memcpy (say, say##number, strlen(say##number));

        RAND (1)
        RAND (2)
        RAND (3)
        RAND (4)
        RAND (5)
        RAND (6)
        RAND (7)
        RAND (8)
        RAND (9)
        RAND (10)
        RAND (11)
        RAND (12)
        RAND (13)
        RAND (14)
        RAND (15)
        RAND (16)

        fprintf (out, "%s(", say);
    }
    else
    {
        fprintf (out, "(");
    }

    Simplify (diff_node);
    WriteTree (node, out);

    fprintf (out, ")' = ");
    WriteTree (diff_node, out);
    fprintf (out, "$$\n\\newline\n\\newline\n\\newline\n $$");

    diff_node->prev = prev;
}

struct element* Differencial (struct element* head)
{
    struct element* head_dif = CreateElement (NOTHING);

    printf ("tut");

    FILE* out = fopen ("tex_it.tex", "w");

    fprintf (out, "\n"
                  "\\documentclass{article}\n"
                  "\\usepackage[russian]{babel}\n"
                  "\\begin{document}\n"
                  "\\fontsize{15pt}{10pt}\\selectfont\n"
                  "\\part{Выкладки неизвестного шизофреника из ФизКеКа}\n"
                  "\\section{Предисловие}\n"
                  "\\begin{center}\n"
                  "Мне казалось, АлГем меня накроет\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Так и ПрОиЗоШлО\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Но вдруг мне задали написать дифФеренциатор, и тут меня КоКнУлО\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Казалось, что это длится вечно, но я ошибался, рошло всего 8 часов \n"
                  "и до сих пор не был написан ввод из файла\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Надо было выпить\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Оказалось, пить было нечего\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Лёг спать\n"
                  "\\newline\n"
                  "\\newline\n"
                  "С утра пошёл прогуляться\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Был раздосадован вчерашней неудачей\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Пришёл домой\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Помылся\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Пока намыливал придумал, как реализовать ввод\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Написал, заработало на маленьких формулах\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Ввёл длинную, перестало работать\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Поехал в общагу расстроенный\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Приехал, смог написать\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Поиграл в настольный теннис\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Сел смог задиффурить\n"
                  "\\newline\n"
                  "\\newline\n"
                  "Ну что? Погнали?\n"
                  "\\newpage\n"
                  "\\part{А давайте ДиФфЕрЕнЦиРоВаТь, гЫ Гы гЫ Гы}\n\\newline");
    fprintf (out, "Попробуем продифференцировать эту срань:\n\\newline");
    WriteTree (head, out);
    fprintf (out, "\n\\newline");
    //Dump (head);
    Differ_smth (head_dif, head, out);

    fprintf (out, "Я мог это написать в самом начале, но тебе критину объяснил:\n\\newline\n\\newline\n\\newline(");
    WriteTree (head, out);

    fprintf (out, ")' = ");
    WriteTree (head_dif, out);

    fprintf (out, "\\end{document}\n");

    fclose (out);
    system ("D:\\MiKTeX\\miktex\\bin\\x64\\pdflatex.exe -synctex=1 -interaction=nonstopmode \"tex_it\".tex");
    system ("C:\\Users\\dunka\\CLionProjects\\proizvodnaya\\cmake-build-debug\\tex_it.pdf");



    return head_dif;
}

void MulDivZero (struct element* node, bool* Exit)
{
    if ((node->what != NUMBER) && (node->what != x) && (node->what != y) && (node->what != z) && (node->right != nullptr))
        MulDivZero (node->right, Exit);

    if ((node->what != NUMBER) && (node->what != x) && (node->what != y) && (node->what != z) && (node->left != nullptr))
        MulDivZero (node->left, Exit);


    if (node->prev == nullptr) return;
    if ((node->what == NUMBER) && (node->number == 0) && ((node->prev->what == MUL) || (node->prev->what == DIV)))
    {
        if ((node->prev->what == DIV) && (node->prev->right->what == NUMBER) && (node->prev->right->number == 0))
        {
            printf ("No such derivative");

#define NO_DER 22

            exit (NO_DER);

#undef NO_DER

        }
        node->prev->what   = NUMBER;
        node->prev->number = 0;
        DeleteUnderNode (node->prev);
        *Exit = false;
    }
}

bool CheckFor_PlusZero_MulDivOne (struct element* node)
{
    return ((node->what == NUMBER) && (node->number == 0) && (node->prev->what == PLUS)) || ((node->what == NUMBER) && (node->number == 1) && ((node->prev->what == MUL) || ((node->prev->what == DIV) && (node->prev->right == node))));
}

void PlusZero_MulDivOne (struct element* node, bool* Exit)
{

    if ((node->what != NUMBER) && (node->what != x) && (node->what != y) && (node->what != z) && (node->right != nullptr))
        PlusZero_MulDivOne (node->right, Exit);

    if ((node->what != NUMBER) && (node->what != x) && (node->what != y) && (node->what != z) && (node->left != nullptr))
        PlusZero_MulDivOne (node->left, Exit);

    if (node->prev == nullptr) return;
    if (CheckFor_PlusZero_MulDivOne (node))
    {
        struct element* copy_sum = CreateElement (NOTHING);

        if (node->prev->right != node)
            CopyNode (node->prev->right, copy_sum);
        else
            CopyNode (node->prev->left, copy_sum);

        if (node->prev->prev == nullptr)
        {
            CopyNode (copy_sum, node->prev);
            DeleteTree (node);
            return;
        }

        if (node->prev->prev->right == node->prev)
            Merge (copy_sum, node->prev->prev, RIGHT);
        else
            Merge (copy_sum, node->prev->prev, LEFT);

        DeleteTree (node->prev);
        *Exit = false;
        return;;
    }
}

void IfConstant (struct element* node, bool* Exit)
{

    if ((node->what != NUMBER) && (node->what != x) && (node->what != y) && (node->what != z)) {

        if (node->left != nullptr)  {
            IfConstant (node->left, Exit);
        }
        if ((node->right != nullptr)) {
            IfConstant (node->right, Exit);
        }
    }

    if (node->prev == nullptr) return;

    if ((node->prev->right != nullptr) && (node->prev->left != nullptr) &&
        (node->prev->right->what == NUMBER) && (node->prev->left->what == NUMBER))
    {


#define cases(what, sign)                                                                       \
        case what:                                                                              \
       {                                                        \
           node->prev->number = node->prev->left->number sign node->prev->right->number;        \
       } break;

        switch (node->prev->what)
        {
            cases (PLUS, +)
            cases (MINUS, -)
            cases (MUL, *)
            cases (DIV, /)
            default:
                break;
        }


        node->prev->what = NUMBER;
        DeleteUnderNode (node->prev);
        *Exit = false;
        return;
    }
}

void Simplify (struct element* head)
{
    while (true)
    {
        bool Exit = true;

        MulDivZero            (head, &Exit);
        PlusZero_MulDivOne    (head, &Exit);
        IfConstant            (head, &Exit);

        if (Exit)
            break;
    }
}
