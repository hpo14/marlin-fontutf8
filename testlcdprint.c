
#include <stdio.h>

#include "syslang.h"
#include "sysmarlin.h"

#include "language.h"


#if USE_HD44780

#else // U8GLIB

#endif

// sample translations
#define MSG_SAMPLE1_EN   "Welcome"
#define MSG_SAMPLE1_RU   "добро пожаловать"
#define MSG_SAMPLE1_JP   "歓迎"
#define MSG_SAMPLE1_CNZH "欢迎"
#define MSG_SAMPLE1_CNTW "歡迎"
#define MSG_SAMPLE1_FR   "bienvenue"

#define MSG_SAMPLE2_EN   "You did a great job!"
#define MSG_SAMPLE2_RU   "Вы проделали хорошую работу!"
#define MSG_SAMPLE2_JP   "あなたは素晴らしい仕事をしました！"
#define MSG_SAMPLE2_CNZH "你做得很好！"
#define MSG_SAMPLE2_CNTW "你做得很好！"
#define MSG_SAMPLE2_FR   "Vous avez fait un très bon travail!"

int
main(void)
{
    char * str = WELCOME_MSG;
    printf ("%s\n", str);
    test_show_uchar ();
    return 1;
}

