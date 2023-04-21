#include <cstring>

#include "comm.h"
#include "../main.h"
#include "../list.h"
#include "../hero.h"
#include "../game.h"

extern game *cur_game;

int mh() {
    // объект героя
    hero *main_hero;

    // сообщения функции
    const char *mess[] = {
            "Пиво прибавляет %iз. Здоровье: %i/%i. Осталось %.1f л. пива\n",
            "Пива нету\n",
            "Чувак, у тебя и так дохрена здоровья\n",
            "Ты не можешь пить из-за сломанной челюсти\n",
            "Ты выпил слишком много пива за раз и опьянел. Лв -1\n",
            "Куда те ещё пить? И так пьяный!\n"};

    int
    // кол-во пива, которое должно быть использовано
    d;

    int
            i,
            j;

    main_hero = cur_game->main_hero;

    if (main_hero->get_health() < main_hero->get_max_health()) {
        if (main_hero->broken_jaw) {
            settextattr(RED);
            PRINTF("%s", mess[3]);
        } else {
            if (main_hero->get_beer() > 0) {
                if (cur_game->stay_mh > 0) {
                    settextattr(YELLOW);
                    PRINTF("%s", mess[5]);
                } else {
                    if (strcmp(cur_game->active_cmd, "mh") == 0) {
                        d = main_hero->get_beer();
                    } else {
                        d = 1;
                    }

                    if ((d * 5) >= (main_hero->get_max_health() - main_hero->get_health())) {
                        i = main_hero->get_max_health() - main_hero->get_health();
                        j = i;

                        while ((j % 5) != 0) {
                            j++;
                        }

                        main_hero->sub_beer(d = j / 5);
                        main_hero->add_health(main_hero->get_max_health() - main_hero->get_health());
                    } else {
                        i = d * 5;

                        main_hero->sub_beer(d);
                        main_hero->add_health(i);
                    }

                    settextattr(GREEN);
                    PRINTF(mess[0], i, main_hero->get_health(), main_hero->get_max_health(), main_hero->get_beer() * .5);

                    int luck;
                    if (cur_game->stay_mh > 0) {
                        luck = CHANCE(75, 100);
                    } else {
                        // stay_mh отрицательная, поэтому сложение
                        auto bad_luck = 90 + 15 * cur_game->stay_mh--;
                        luck = bad_luck > 0 ? CHANCE(bad_luck, 100) : 0;
                    }

                    // больше лимита опьянения
                    if (d > main_hero->district + luck) {
                        cur_game->stay_mh = 10;
                        main_hero->drunk = true;

                        settextattr(YELLOW);
                        PRINTF("%s", mess[4]);
                    }
                }
            } else {
                settextattr(RED);
                PRINTF("%s", mess[1]);
            }
        }
    } else {
        settextattr(YELLOW);
        PRINTF("%s", mess[2]);
    }

    return 0;
}
