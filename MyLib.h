//
// Created by dunkan on 27.09.2019.
//
#include <cstdio>
#include <cstdlib>
#include "stdio.h"
#include "Assert.h"
#include <sys/stat.h>
#include "ctype.h"
//#define DEBUG
#ifdef DEBUG
#define ON_DEBUG( code ) code
#else
#define ON_DEBUG( code )
#endif

/*!
 * Function compares words
 * @param[in] compare1 First word to compare with
 * @param[in] compare2 Second word to compare with
 * @return 0 if compare 1 != compare 2
 * @return 1 if compare 1 == compare 2
 */
bool WordCmp (const char* compare1, const char* compare2 );

/*!
 * Input text into string
 * @param[in] name - name of file
 * @param[in] text - string to input text
 * @param[in] size - size of text
 * @return massive of structures with pointers
*/
struct Text * Input ( const char name );

/*!
 * Function for Input to compare pointers with their place in structure
 * @param[in] text
 * @param[in] size of text
 * @param[in] NLines number of Lines
 * @return massive of structures with pointers
 */
struct Text * FillingThePointers ( char *text,  int size, int NLines );

/*!
 * If has define ASSEMBLER counts size of massive, which includes machine code,
 * else counts number of words in the text
 * @param[in] str buffer with text
 * @param[in] length length of the text
 * @return size of massive, which includes machine code(ifdef ASSEMBER)
 * @return number of words(undef ASSEMBLER)
 */
int WordCount ( char* str, int length );

/*!
 * Counts size of file
 * @param fname name of file
 * @return size of file
 */
int Sizecount ( const char *fname );

/*! Makes a buffer
 * @param[in] name name of file
 * @param[in] length length of file
 * @return dynamic buffer with text from file
 */
char* Buf ( const char* name, int length );

/*! Makes a buffer
 * @param[in] name name of file
 * @return dynamic buffer with text from file
 */
char* BufferMaker ( const char* name);

/*!
 * Upper the string
 * @param string
 * @param length
 * @return Upperd string
 */
char* StringUpper(char* string, int length);

/*!
 * Say the string
 * @param str
 */
void Say (char* str);

/*!
 * Comparator for numbers from The biggest number to lowest
 * @param first number
 * @param second number
 * @return
 */
int IntFromHighToLow (int first, int second);

int IntFromLowToHigh (int first, int second);

void Say (char* str)
{
    ASSERT (str)

    char* call = "D:\\eSpeak\\command_line\\espeak.exe \"";
    char* end = "\" -s500";

    char say[strlen (call) + strlen (str) + +strlen (end) + 1] = "D:\\eSpeak\\command_line\\espeak.exe \"";



    strcat (say, str);
    strcat (say, end);

    system (say);

}

bool WordCmp ( const char* compare1, const char* compare2 )
{
    ASSERT ( compare1 )
    ASSERT ( compare2 )
    for ( int i = 0; i < 4; i++ )
    {
        if ( compare1[i] != compare2[i])
        {
            return false;
        }
    }
    return true;
}


int WordCount ( char* str, int length )
{
    int i = 0;
    int number = 0;
    str[length + 1] = '\n';
    for ( i; i < length + 1 ; i++ )
    {
        if ( ( ( str[i] == ' ' ) && ( str[i+1] != ' ' ) ) || ( ( str[i] != ' ' ) && ( str[i+1] == '\n'  ) ) )
        {
            number++;
            ON_DEBUG(printf("\n------%d--------\n", number);)

        }
    }

    if ( ( str[i] != ' ' ) && ( str[i - 1] == ' ' ) )
    {
        ON_DEBUG(printf("$$-%c-$$", str[i]);)
        number++;
    }
    return number;
}

/*!
 * Contains pointers on the lines of Code
 */
struct Text
{
    char* begin = nullptr;   //<pointer on the beginning of the line
    char* end = nullptr;     //<pointer on the end of the line
};

struct Text * Input ( const char* name )
{
    ASSERT ( name )

    char* text = BufferMaker ( name );

    FILE * in = fopen ( name, "r");

    int NLines = 0;
    int size = Sizecount ( name );
    ON_DEBUG(printf("^%d^", size);)

    ON_DEBUG (printf ("%s", text);)

    for ( int i = 0; i < size; i++ )
    {
        if ( text[i] == '\n')
        {
            NLines++;
            i++;
        }
    }
    text[size] = '\n';


    fclose ( in );

    struct Text * Code = FillingThePointers ( text, size, NLines );
    return Code;
}

double MakeInt (char* numb)
{
    double  number = 0;
    int k = 1;
    int i = 0;
    bool under_zero = false;
    if (numb[i] == '-')
    {
        under_zero = true;
        i++;
    }
    while ((numb[i] >= '0') && (numb[i] <= '9'))
    {
        printf ("--%s--", numb);
        number += (numb[i] - 48) * k;
        k *= 10;
        i++;
    }

    i++;

    double ostatok = 0;
    k = 1;
    while ((numb[i] != ' ') && (numb[i] != '0') && (strlen (numb) > i))
    {
        ostatok += (double)(numb[i - 1] - 48) / k;
        k *= 10;
        i++;
    }
    if (under_zero)
        number *= -1;
    return number + ostatok;
}

struct Text * FillingThePointers ( char *text,  int size, int NLines )
{
    ASSERT (text);
    ASSERT (size > 0);
    ASSERT (NLines >= 0);

    struct Text * Code = (struct Text *) calloc ( NLines + 2, sizeof (struct Text) );
    Code[0].begin = text;

    int Line = 0;
    int i = 0;
    ON_DEBUG (printf ("fill pointers\n");)
    while ( i < size)
    {
        if (text[i] == '\n')
        {
            //text[i + 1] = '\0';
            text[i] = '\0';
            Code[Line].end = &text[i];
            Line++;
            ON_DEBUG (printf("\n%s", Code[Line - 1].begin);)
            i++;

            if (i < size - 1)
            {
                Code[Line].begin = &text[i];
            }
            continue;
        }
        //ON_DEBUG (printf("\n%c", text[i]);)
        i++;
    }
    return Code;
}

int Sizecount ( const char *fname )
{
    ASSERT ( fname );
    //fname[0] = 'u'; /* поднагадить с имечком */
    struct stat sizeknowing = {};

    stat ( fname, &sizeknowing );

    ASSERT ( sizeknowing.st_size >= 0 );

    return sizeknowing.st_size;
}

char* Buf ( const char* name, int length )
{
    ASSERT ( name )
    ASSERT (length > 0 )
    char* buffer = ( char* )  calloc ( length + 1, sizeof (char) );

    FILE *fp = fopen(name, "r");
    ASSERT ( fp )

    fread ( buffer, sizeof(char), length, fp);
    ASSERT ( buffer )

    fclose ( fp );

    return buffer;
}


char* BufferMaker (const char* name)
{
    int length = Sizecount ( name );

    char* buffer = Buf ( name, length );

    return buffer;
}


char* StringUpper (char* string, int length)
{
    for (int i = 0; i < length; i++)
    {
        string[i] = toupper(string[i]);
    }
    return string;
}

int IntFromHighToLow (int first, int second)
{
    return second - first;
}

int IntFromLowToHigh (int first, int second)
{
    return first - second;
}

