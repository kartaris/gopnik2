#include <cstdlib>

#include "comm.h"
#include "../main.h"
#include "../list.h"
#include "../hero.h"
#include "../game.h"
#include "../utils.h"

extern game *cur_game;

int w() {
    // объект героя
    hero *main_hero;

    // сообщения функции
    const char *mess[14] = {
            "Навстречу идёт %s %i уровня. Хочешь наехать? (y/n)\n",
            "Ничё не происходит\n",
            "Совсем ничё не происходит\n",
            "Слышь, браток, дело есть...",
            "Чё за нах?!",
            "Он тебя заметил...\n",
            "Эй, мудак, ты откуда вылез?",
            "Завали ебало, ублюдок! Мудак у тебя между ног!",
            "Ты смылся...\n",
            "Ты нашёл базар\n",
            "Ты спросил у прохожего, где больница\n",
            "%s хотел наехать, но увидев у тебя чотки, решил не рисковать\n",
            "Ты увидел объявление: \"Типа заходи в наш понтовый клуб\"\n",
            "Мент хотел наехать, но, увидев тебя в очках, решил, что ты не быдло\n"};

    int
    // индекс генерируемого типа героя
    ht_index,
    // индекс фраз противника и героя
    ph_index,
    // индекс инвентаря
    inv_index;

    const char
    // фразы противника и героя
    *ph_addr,
            *ph_reply;

    int
    // уровень генерируемого героя
    level,
            flag, i;

    if (
            (cur_game->active_loc != 0) &&
            (cur_game->active_loc != 1)) {
        // переход к новой локации
        cur_game->set_loc(0);

        return 0;
    }

    main_hero = cur_game->main_hero;

    // вывод событий разного толка

    // если базар пока не найден
    if (
            (cur_game->open_mar == 0) &&
            (cur_game->num_w > 10) &&
            (main_hero->station == 0)) {
        cur_game->open_mar = chance(1, 5);

        if (cur_game->open_mar) {
            settextattr(BLUE);
            printf("%s", mess[9]);
        }
    }

    // если ветеринар пока не найден
    if (
            (cur_game->open_rep == 0) &&
            (cur_game->num_w > 10) &&
            (main_hero->station == 0)) {
        cur_game->open_rep = chance(1, 5);

        if (cur_game->open_rep) {
            settextattr(BLUE);
            printf("%s", mess[10]);
        }
    }

    // если клуб пока не найден
    if (
            (cur_game->get_open_kl() == 0) &&
            (cur_game->num_w > 50) &&
            (main_hero->station == 0)) {
        cur_game->set_open_kl(chance(1, 5));

        if (cur_game->get_open_kl()) {
            settextattr(BLUE);
            printf("%s", mess[12]);
        }
    }

    // вывод событий команды "w"

    flag = 0;

    if (
            (cur_game->num_w > 20) &&
            ((cur_game->num_w % 20) == 0) &&
            (main_hero->station == 0) &&
            (!cur_game->open_str)) {
        for (i = 0; i < cur_game->we_amount; i++) {
            if (cur_game->we[i].active) {
                flag = 1;

                if (chance(1, cur_game->we[i].events)) {
                    // делаем неактивным произосшедшее событие
                    cur_game->we[i].active = 0;

                    if (cur_game->we[i].type == 1) {
                        settextattr(BLUE);
                    } else {
                        settextattr(YELLOW);
                    }

                    printf("%s\n", cur_game->we[i].event);

                    break;
                }
            }
        }
    } else {
        flag = 1;
    }

    // если ни одно из событий не оказалось
    // активным заново обновляем настройки
    if (flag == 0) {
        for (i = 0; i < cur_game->we_amount; i++) {
            cur_game->we[i].active = 1;
        }
    }

    // генерируем тип героя и левел противника
    do {
        level = cur_game->gen_enemy(&ht_index);
    } while ((level == -1) && (cur_game->num_empty_w == 10));

    // дополнительные условия для генерации противеика
    if (level == -1) {
        // если герой пьяный
        if (
                (main_hero->drunk) &&
                ((ht_index = cur_game->search_ht("Ветер")) > -1) &&
                (chance(1, cur_game->ht[ht_index].events))) {
            level = main_hero->get_level();

            goto then;
        }

        // если возможна встреча с ментом
        if (
                ((ht_index = cur_game->search_ht("Мент")) > -1) &&
                (chance(1, cur_game->ht[ht_index].events)) &&
                ((main_hero->get_level() % 10) >= 5)) {
            level = getrandom(main_hero->get_level() + 1, main_hero->get_level() + 5);

            goto then;
        }
    }

    then:

    // если противник имеет место быть
    if (level > -1) {
        settextattr(YELLOW);
        printf(mess[0], cur_game->ht[ht_index].type, level);

        flag = 0;

        // если герой наезжает на противника
        if (cur_game->wait_answ()) {
            flag = 1;

            if (cur_game->ht[ht_index].hero_phrase_amount > 0) {
                ph_index = getrandom(-1, cur_game->ht[ht_index].hero_phrase_amount - 1);

                ph_addr = cur_game->ht[ht_index].hero_addr[ph_index];
                ph_reply = cur_game->ht[ht_index].hero_reply[ph_index];
            } else {
                ph_addr = mess[3];
                ph_reply = mess[4];
            }

            settextattr(GREEN);
            printf("-%s\n", ph_addr);

            settextattr(RED);
            printf("-%s\n", ph_reply);
        } else {
            if (level > main_hero->get_level()) {
                // если противник наезжает на героя

                if (
                        (chance(1, main_hero->get_luck() + 1)) ||
                        (!chance(1, level - main_hero->get_level() + 1))) {
                    // если есть чотки
                    if (
                            (main_hero->inv[cur_game->search_inv(main_hero, "Чотки")].have) &&
                            (chance(1, 2)) &&
                            (ht_index != cur_game->search_ht("Мент"))) {
                        printf(mess[11], cur_game->ht[ht_index].type);

                        goto then1;
                    }

                    // если есть затемнённые очки
                    if (
                            (main_hero->inv[cur_game->search_inv(main_hero, "Затемнённые очки")].have) &&
                            (ht_index == cur_game->search_ht("Мент"))) {
                        printf("%s", mess[13]);

                        goto then1;
                    }

                    flag = 1;

                    printf("%s", mess[5]);

                    if (cur_game->ht[ht_index].enemy_phrase_amount > 0) {
                        ph_index = getrandom(-1, cur_game->ht[ht_index].enemy_phrase_amount - 1);

                        ph_addr = cur_game->ht[ht_index].enemy_addr[ph_index];
                        ph_reply = cur_game->ht[ht_index].enemy_reply[ph_index];
                    } else {
                        ph_addr = mess[6];
                        ph_reply = mess[7];
                    }

                    settextattr(RED);
                    printf("-%s\n", ph_addr);

                    settextattr(GREEN);
                    printf("-%s\n", ph_reply);
                } else {
                    printf("%s", mess[8]);
                }
            }
        }

        then1:

        if (flag == 1) {
            // генерируем объект противника
            cur_game->gen_enemy_obj(ht_index, level);

            // дополнительное добавление инвентаря

            inv_index = -1;

            if (ht_index == cur_game->search_ht("Скинхед")) {
                inv_index = cur_game->search_inv(main_hero, "Кастет");
            }

            if (ht_index == cur_game->search_ht("Мент")) {
                inv_index = cur_game->search_inv(main_hero, "Дубинка");
            }

            if (inv_index != -1) {
                cur_game->add_inventory(
                        cur_game->enemy,
                        main_hero->inv[inv_index].name,
                        main_hero->inv[inv_index].district,
                        main_hero->inv[inv_index].events,
                        1,
                        main_hero->inv[inv_index].force,
                        main_hero->inv[inv_index].smart,
                        main_hero->inv[inv_index].vita,
                        main_hero->inv[inv_index].luck,
                        main_hero->inv[inv_index].armo,
                        main_hero->inv[inv_index].loss);
            }

            // переход к новой локации
            cur_game->set_loc(1);
        }

        cur_game->num_empty_w = 0;
    } else {
        // выводим, что ничё не происходит

        settextattr(WHITE);

        if (
                (cur_game->num_empty_w > 5) &&
                (chance(1, 5))) {
            printf("%s", mess[2]);
        } else {
            printf("%s", mess[1]);
        }

        cur_game->num_empty_w++;
    }

    cur_game->num_w++;

    return 0;
}
