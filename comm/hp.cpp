#include "comm.h"
#include "../main.h"
#include "../list.h"
#include "../hero.h"
#include "../game.h"

extern game *cur_game;

int hp() {
    // объект героя
    hero *main_hero;

    // сообщения функции
    const char *mess[] = {
            "Это %s %i уровня\n"};

    int
    // индекс инвентаря
    inv_index,
    // индекс генерируемого типа героя
    ht_index;

    int
    // уровень генерируемого героя
    level;

    main_hero = cur_game->main_hero;

    if (cur_game->get_open_hp()) {
        cur_game->set_open_hp(2);

        do {
            level = cur_game->gen_enemy(&ht_index);
        } while (level == -1);

        if (
                (main_hero->district > 1) &&
                (!main_hero->inv[game::search_inv(main_hero, "Пропуск в общагу")].have)) {
            ht_index = cur_game->search_ht("Студент");
        }

        settextattr(YELLOW);
        PRINTF(mess[0], cur_game->ht[ht_index].type, level);

        cur_game->gen_enemy_obj(ht_index, level);

        if (
                (ht_index == cur_game->search_ht("Студент")) &&
                (game::search_inv(cur_game->enemy, "Пропуск в общагу") == -1)) {
            inv_index = game::search_inv(main_hero, "Пропуск в общагу");

            if (inv_index != -1) {
                game::add_inventory(
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
        }

        // переход к новой локации
        cur_game->set_loc(1);
    }

    return 0;
}
