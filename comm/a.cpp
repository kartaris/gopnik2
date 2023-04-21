#include "comm.h"
#include "../main.h"
#include "../list.h"
#include "../hero.h"
#include "../game.h"

extern game *cur_game;

int a() {
    // сообщения функции
    const char *mess[] = {
            "-Тут есть пара хороших мест, куда можно сходить\n",
            "Ты узнал, где находятся барыги и качалка\n"};

    if (cur_game->get_open_a()) {
        cur_game->set_open_a(0);

        cur_game->set_open_trn(1);
        cur_game->set_open_bmar(1);

        settextattr(YELLOW);
        PRINTF("%s", mess[0]);

        settextattr(GREEN);
        PRINTF("%s", mess[1]);
    }

    return 0;
}
