#include <gopnik2/comm/comm.h>
#include <gopnik2/main.h>
#include <gopnik2/list.h>
#include <gopnik2/hero.h>
#include <gopnik2/game.h>

extern game *cur_game;

int bsale(
        // индекс временного
        // динамического массива "int *inv_have"
        int index) {
    // объект героя
    hero *main_hero;

    // сообщения функции
    const char *mess[] = {
            "Ты толкаешь следующую вещь - ",
            "Барыги предлагают за неё %i руб. Ты согласен? (y/n)\n",
            "Ты толкнул одну из своих вещей и получил %i руб.\n",
            "Ты эту вещь уже продал\n"};

    int
    // индекс прайс-листа
    pl_index,
    // индекс элемента прайс-листа
    plm_index,
    // цена на товар
    price,
            cur_price;

    int
            i,
            flag;

    main_hero = cur_game->main_hero;

    if (
            (index < 0) ||
            (index >= main_hero->inv_have_amount)) {
        return 0;
    }

    i = main_hero->inv_have[index];

    if (main_hero->inv[i].have) {
        pl_index = 0;
        plm_index = 0;
        flag = 0;
        price = -1;

        while ((cur_price = cur_game->search_plm_price(&pl_index, &plm_index, main_hero->inv[i].name)) != -1) {
            if (flag == 0) {
                price = cur_price;

                flag = 1;
            } else {
                if (cur_price < price) {
                    price = cur_price;
                }
            }
        }

        if (price == -1) {
            if (main_hero->inv[i].events > 0) {
                price = main_hero->inv[i].events;
            } else {
                price = 10;
            }
        }

        // доп. условие
        if (i == game::search_inv(main_hero, "Патроны")) {
            price = (int) price / 5;
        }

        price = (int) (price * 0.5);

        settextattr(WHITE);
        PRINTF("%s", mess[0]);

        settextattr(BLUE);
        PRINTF("%s", main_hero->inv[i].name);

        settextattr(YELLOW);
        PRINTF(mess[1], price);

        if (game::wait_answ()) {
            main_hero->inv[i].have--;
            main_hero->add_money(price);

            cur_game->supple_inv_run_over(i); // !!!

            settextattr(WHITE);
            PRINTF(mess[2], price);
        }
    } else {
        settextattr(RED);
        PRINTF("%s", mess[3]);
    }

    return 0;
}