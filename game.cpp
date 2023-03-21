#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "main.h"
#include "hero.h"
#include "list.h"
#include "comm/comm.h"
#include "game.h"
#include "utils.h"

extern game *cur_game;

template <class ap>
ap add_new_element(
    ap cur_ap,  // указатель на текущий массив
    int amount, // количество элементов в массиве
    size_t size // размер каждого элемента в массиве
)
{
  if (amount == 0)
  {
    cur_ap = (ap)malloc(size);
  }
  else
  {
    cur_ap = (ap)realloc(cur_ap, (amount + 1) * size);
  }

  return cur_ap;
}

game::game()
{
  strcpy(file_name, "gop2_1.sav");
  strcpy(st_file_name, "gop2.st");
  load_game = 0;

  hero_init = 0;
  enemy_init = 0;
  lads_init = 0;
  cmd_list_init = 0;

  ht_amount = 0;
  loc_amount = 0;
  we_amount = 0;
  pl_amount = 0;
  stn_amount = 0;
  pltl_amount = 0;
  str_hero_amount = 0;
  str_enemy_amount = 0;

  active_pltl = -1;

  num_comm = 0;

  new_district_norealiz = 0;
  end_of_game = 0;

  open_mar = 1;
  open_rep = 1;
  open_pr1 = 0;
  open_pr2 = 0;
  open_hp1 = 0;
  open_hp2 = 0;
  open_d1 = 0;
  open_d2 = 0;
  open_trn1 = 0;
  open_trn2 = 0;
  open_bmar1 = 0;
  open_bmar2 = 0;
  open_a1 = 0;
  open_a2 = 0;
  open_kl1 = 0;
  open_kl2 = 1;
  open_girl = 0;
  open_str = 0;
  open_raid = 0;
  open_ob = 0;

  num_w = 0;
  num_empty_w = 0;

  num_k = 0;
  str_dead_mess = 0;

  num_t = 0;

  enemy_desc = 0;

  stay_mar1 = 0;
  stay_mar2 = 0;
  stay_kos = 0;
  stay_mh = 0;
  stay_v = 0;
  stay_kl1 = 0;
  stay_kl2 = 0;
  stay_met = 0;
  stay_str = 0;
  stay_eog = 0;

  st_amount = 0;
} // end game::game ()

game::~game()
{
  clean_mem();

  if (hero_init)
  {
    delete main_hero;
  }

  if (enemy_init)
  {
    delete enemy;
  }

  if (lads_init)
  {
    delete lads;
  }

  if (cmd_list_init)
  {
    delete cmd_list;
  }
} // end game::~game ()

int game::clean_mem()
{
  int i, j, k;

  // **ht**
  if (ht_amount > 0)
  {
    for (i = 0; i < ht_amount; i++)
    {
      if (ht[i].hero_phrase_amount > 0)
      {
        for (j = 0; j < ht[i].hero_phrase_amount; j++)
        {
          free(ht[i].hero_addr[j]);
          free(ht[i].hero_reply[j]);
        }

        free(ht[i].hero_addr);
        free(ht[i].hero_reply);
      }

      if (ht[i].enemy_phrase_amount > 0)
      {
        for (j = 0; j < ht[i].enemy_phrase_amount; j++)
        {
          free(ht[i].enemy_addr[j]);
          free(ht[i].enemy_reply[j]);
        }

        free(ht[i].enemy_addr);
        free(ht[i].enemy_reply);
      }

      free(ht[i].type);
    }

    free(ht);
  }
  // ******

  // **loc**
  if (loc_amount > 0)
  {
    for (i = 0; i < loc_amount; i++)
    {
      for (j = 0; j < loc[i].comm_amount; j++)
      {
        free(loc[i].command[j]);
        free(loc[i].command_desc[j]);
      }

      free(loc[i].command_func);
      free(loc[i].command_active);
      free(loc[i].command);
      free(loc[i].command_desc);
      free(loc[i].name);
    }

    free(loc);
  }
  // *******

  // **we**
  if (we_amount > 0)
  {
    for (i = 0; i < we_amount; i++)
    {
      free(we[i].event);
    }

    free(we);
  }
  // ******

  // **pl**
  if (pl_amount > 0)
  {
    for (i = 0; i < pl_amount; i++)
    {
      for (j = 0; j < pl[i].member_amount; j++)
      {
        if (pl[i].members[j].buy_phrase_amount > 0)
        {
          for (k = 0; k < pl[i].members[j].buy_phrase_amount; k++)
          {
            free(pl[i].members[j].buy_phrase[k]);
          }

          free(pl[i].members[j].buy_phrase);
        }

        free(pl[i].members[j].name);
        free(pl[i].members[j].comment);
      }

      free(pl[i].members);
    }

    free(pl);
  }
  // ******

  // **stn**
  if (stn_amount > 0)
  {
    for (i = 0; i < stn_amount; i++)
    {
      free(stn[i].name);
      free(stn[i].unavail_reason);
    }

    free(stn);
  }
  // *********

  // **str_enemy**
  if (str_enemy_amount > 0)
  {
    delete[] str_enemy;
  }
  // *************

  // **str_hero**
  if (str_hero_amount > 1)
  {
    delete[] str_hero;
  }
  // ************

  // **st**
  if (st_amount > 0)
  {
    for (i = 0; i < st_amount; i++)
    {
      free(st[i]);
    }

    free(st);
  }
  // ******

  // **pltl**
  if (pltl_amount > 0)
  {
    free(pltl);
  }
  // ********

  return 0;
} // end int game::clean_mem ()

int game::save()
{
  printw("Сохранение в файл не реализовано\n");
  return 1;
  int i;
  FILE *sav_file;
  fwrite(&num_w, sizeof(num_w), 1, sav_file);
  fwrite(&num_empty_w, sizeof(num_empty_w), 1, sav_file);
  fwrite(&num_t, sizeof(num_t), 1, sav_file);
  fwrite(&stay_mar1, sizeof(stay_mar1), 1, sav_file);
  fwrite(&stay_mar2, sizeof(stay_mar2), 1, sav_file);
  fwrite(&stay_kos, sizeof(stay_kos), 1, sav_file);
  fwrite(&stay_mh, sizeof(stay_mh), 1, sav_file);
  fwrite(&open_mar, sizeof(open_mar), 1, sav_file);
  fwrite(&open_rep, sizeof(open_rep), 1, sav_file);
  fwrite(&open_pr1, sizeof(open_pr1), 1, sav_file);
  fwrite(&open_pr2, sizeof(open_pr2), 1, sav_file);
  fwrite(&open_hp1, sizeof(open_hp1), 1, sav_file);
  fwrite(&open_hp2, sizeof(open_hp2), 1, sav_file);
  fwrite(&open_d1, sizeof(open_d1), 1, sav_file);
  fwrite(&open_d2, sizeof(open_d2), 1, sav_file);
  fwrite(&open_trn1, sizeof(open_trn1), 1, sav_file);
  fwrite(&open_trn2, sizeof(open_trn2), 1, sav_file);
  fwrite(&open_bmar1, sizeof(open_bmar1), 1, sav_file);
  fwrite(&open_bmar2, sizeof(open_bmar2), 1, sav_file);
  fwrite(&open_kl1, sizeof(open_kl1), 1, sav_file);
  fwrite(&open_kl2, sizeof(open_kl2), 1, sav_file);
  fwrite(&stay_kl1, sizeof(stay_kl1), 1, sav_file);
  fwrite(&stay_kl2, sizeof(stay_kl2), 1, sav_file);
  fwrite(&open_girl, sizeof(open_girl), 1, sav_file);
  fwrite(&stay_met, sizeof(stay_met), 1, sav_file);
  fwrite(&new_district_norealiz, sizeof(new_district_norealiz), 1, sav_file);
  fwrite(&open_raid, sizeof(open_raid), 1, sav_file);
  fwrite(&open_str, sizeof(open_str), 1, sav_file);
  fwrite(&stay_str, sizeof(stay_str), 1, sav_file);
  fwrite(&open_ob, sizeof(open_ob), 1, sav_file);
  fwrite(&end_of_game, sizeof(end_of_game), 1, sav_file);
  fwrite(&stay_eog, sizeof(stay_eog), 1, sav_file);

  fwrite(&pltl_amount, sizeof(pltl_amount), 1, sav_file);

  for (i = 0; i < pltl_amount; i++)
  {
    fwrite(&pltl[i].active, sizeof(pltl[i].active), 1, sav_file);
  }

  return 0;
} // end int game::save (FILE *)

int game::load()
{
  printw("Загрузка из файла не реализована\n");
  return 1;
  int i;
  FILE *load_file;
  fread(&num_w, sizeof(num_w), 1, load_file);
  fread(&num_empty_w, sizeof(num_empty_w), 1, load_file);
  fread(&num_t, sizeof(num_t), 1, load_file);
  fread(&stay_mar1, sizeof(stay_mar1), 1, load_file);
  fread(&stay_mar2, sizeof(stay_mar2), 1, load_file);
  fread(&stay_kos, sizeof(stay_kos), 1, load_file);
  fread(&stay_mh, sizeof(stay_mh), 1, load_file);
  fread(&open_mar, sizeof(open_mar), 1, load_file);
  fread(&open_rep, sizeof(open_rep), 1, load_file);
  fread(&open_pr1, sizeof(open_pr1), 1, load_file);
  fread(&open_pr2, sizeof(open_pr2), 1, load_file);
  fread(&open_hp1, sizeof(open_hp1), 1, load_file);
  fread(&open_hp2, sizeof(open_hp2), 1, load_file);
  fread(&open_d1, sizeof(open_d1), 1, load_file);
  fread(&open_d2, sizeof(open_d2), 1, load_file);
  fread(&open_trn1, sizeof(open_trn1), 1, load_file);
  fread(&open_trn2, sizeof(open_trn2), 1, load_file);
  fread(&open_bmar1, sizeof(open_bmar1), 1, load_file);
  fread(&open_bmar2, sizeof(open_bmar2), 1, load_file);
  fread(&open_kl1, sizeof(open_kl1), 1, load_file);
  fread(&open_kl2, sizeof(open_kl2), 1, load_file);
  fread(&stay_kl1, sizeof(stay_kl1), 1, load_file);
  fread(&stay_kl2, sizeof(stay_kl2), 1, load_file);
  fread(&open_girl, sizeof(open_girl), 1, load_file);
  fread(&stay_met, sizeof(stay_met), 1, load_file);
  fread(&new_district_norealiz, sizeof(new_district_norealiz), 1, load_file);
  fread(&open_raid, sizeof(open_raid), 1, load_file);
  fread(&open_str, sizeof(open_str), 1, load_file);
  fread(&stay_str, sizeof(stay_str), 1, load_file);
  fread(&open_ob, sizeof(open_ob), 1, load_file);
  fread(&end_of_game, sizeof(end_of_game), 1, load_file);
  fread(&stay_eog, sizeof(stay_eog), 1, load_file);

  fread(&pltl_amount, sizeof(pltl_amount), 1, load_file);

  for (i = 0; i < pltl_amount; i++)
  {
    fread(&pltl[i].active, sizeof(pltl[i].active), 1, load_file);
  }

  return 0;
} // end int game::load (FILE *, float)

int game::wait_command()
{
  //  printw(" ... in game::wait_command() ... ");
  // функция, обрабатывающая вызываемую команду
  FP comm_func;

  int old_attr,
      // индекс команды
      comm_index,
      // значение, возвращённое функцией обработки команды
      ret,
      pltl_ret,
      i, j, n, q;

  char
      // буфер для команды пользователя
      cmd[20];
  // код нажатой клавиши

  /*  if (load_game)
    {
      if (loa () == -1)
      {
  //      clean_mem ();
  //      delete main_hero;
        exit (1);
      }

      load_game = 0;
      strcpy (file_name, "gop2_1.sav");
    } */

  cmd_list = new list();
  cmd_list_init = 1;

  n = 0;

  while (1)
  {
    old_attr = settextattr(8);
    printw("%s", loc[active_loc].name);

    settextattr(15);
    printw("\\");

    for (i = 0; i < 10; i++)
    {
      cmd[i] = 0;
    }

    i = 0;

    while (1) // считывание команды пользователя
    {
      q = getch(0);
      if ((q >= 32) && (q <= 126) && (i < 10)) // печатные символы ^_^
      {
        printw("%c", q);
        cmd[i] = q;
        i++;
      }
      if (q == 10) // UNIX же!
      {
        printw("\n");
        break;
      }

      if ((q == 127) && (i > 0)) // из-за непонятных мне проблем с совместимостью BS==127
      {
        backspace();
        cmd[i--] = 0;
      }

      if (q >= 0xFF)
      {
        q -= 0xFF;
        if ((n > 0) && ((q == 65) || (q == 66)))
        {
          while (i--)
            backspace();
          for (j = 0; j < 10; j++)
            cmd[j] = 0;
          if (q == 65)
          {
            strcpy(cmd, cmd_list->up());
          }
          else
          {
            strcpy(cmd, cmd_list->down());
          }
          i = strlen(cmd);
          printw("%s", cmd);
        }
      }
    }

    cmd_list->add(cmd);
    strcpy(active_cmd, cmd);
    n++;

    comm_index = is_active_location_command(cmd);
    comm_func = loc[active_loc].command_func[comm_index];

    if (comm_index != -1)
    {
      // вызываем функцию по отслеживанию команд процесса
      sstay();
      pltl_ret = supple_pltl_run_over(); // -- test
      ret = 0;
      // вызываем функцию необходимую для выполнения команды
      if (
          (comm_func != NULL) &&
          (pltl[0].active != -1) &&
          (pltl_ret != 2))
      {
        ret = comm_func();
      }
      num_comm++;
      strcpy(active_cmd, "");

      if (pltl_ret == 0)
      {
        supple_pltl_run_over(); // -- test
      }

      if ((ret == 1) || (pltl[0].active == -1))
      {
        break;
      }
    }
    else
    {
      if (isdigitstr(cmd))
      {
        buy_realiz();
      }
    }
    //    cmd_list->scrolldown();
  }

  settextattr(old_attr);
  delete cmd_list;
  cmd_list_init = 0;

  return 0;
} // end int game::wait_command ()

int game::wait_answ()
{
  //  printf(" ... in game::wait_answ() ... ");
  int
      //    old_attr,
      // ответ пользователя
      answ;

  //  old_attr = settextattr (12);

  while (1)
  {
    answ = getch(0);
    if (answ == 'y')
    {
      //      settextattr(old_attr);
      return 1;
    }

    if (answ == 'n')
    {
      //      settextattr(old_attr);
      return 0;
    }

    printw("чё-то ты не то жмёшь\n");
  }
} // end int game::wait_answ ()

int game::add_hero_type(
    // название типа героя
    const char *type,
    // является ли геймерским типом
    int gamer,
    // начальная сила
    int force,
    // начальная ловкость
    int smart,
    // начальная живучесть
    int vita,
    // начальная удача
    int luck,
    // минимальный инкремент к текущему уровню
    // главного героя при формировании уровня
    int min_level_inc,
    // кол-во событий для вероятности появления
    int events,
    // доступность использования
    int active,
    // кол-во событий для вероятности появления:
    // инвентаря, повышающего характеристики героя
    int inv_desc_events,
    // инвентаря, повышающего броню
    int inv_armo_events,
    // инвентаря, повышающего урон
    int inv_loss_events,
    // остального инвентаря
    int inv_other_events,
    // денег
    int money_events,
    // хлама
    int stuff_events,
    // косяков
    int ciga_events)
{
  ht = add_new_element(ht, ht_amount, sizeof(hero_type));

  ht[ht_amount].type = strdup(type);
  ht[ht_amount].gamer = gamer;

  ht[ht_amount].desc[0] = force;
  ht[ht_amount].desc[1] = smart;
  ht[ht_amount].desc[2] = vita;
  ht[ht_amount].desc[3] = luck;

  ht[ht_amount].min_level_inc = min_level_inc;
  ht[ht_amount].events = events;
  ht[ht_amount].active = active;
  ht[ht_amount].inv_desc_events = inv_desc_events;
  ht[ht_amount].inv_armo_events = inv_armo_events;
  ht[ht_amount].inv_loss_events = inv_loss_events;
  ht[ht_amount].inv_other_events = inv_other_events;
  ht[ht_amount].money_events = money_events;
  ht[ht_amount].stuff_events = stuff_events;
  ht[ht_amount].ciga_events = ciga_events;
  ht[ht_amount].hero_phrase_amount = 0;
  ht[ht_amount].enemy_phrase_amount = 0;

  ht_amount++;

  return (ht_amount - 1);
} // end int game::add_hero_type (TEXT, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)

int game::add_hero_phrase(
    // индекс типа героя
    int ht_index,
    // обращение героя к противнику
    const char *hero_addr,
    // ответ противника на обращения героя
    const char *hero_reply)
{
  ht[ht_index].hero_addr = add_new_element(
      ht[ht_index].hero_addr,
      ht[ht_index].hero_phrase_amount,
      sizeof(char *));
  ht[ht_index].hero_reply = add_new_element(
      ht[ht_index].hero_reply,
      ht[ht_index].hero_phrase_amount,
      sizeof(char *));

  ht[ht_index].hero_addr[ht[ht_index].hero_phrase_amount] = strdup(hero_addr);
  ht[ht_index].hero_reply[ht[ht_index].hero_phrase_amount] = strdup(hero_reply);

  ht[ht_index].hero_phrase_amount++;

  return (ht[ht_index].hero_phrase_amount - 1);
} // end int game::add_hero_phrase  (int, TEXT, TEXT)

int game::add_enemy_phrase(
    // индекс типа героя
    int ht_index,
    // обращение противника к герою
    const char *enemy_addr,
    // ответ героя на обращение противника
    const char *enemy_reply)
{
  ht[ht_index].enemy_addr = add_new_element(
      ht[ht_index].enemy_addr,
      ht[ht_index].enemy_phrase_amount,
      sizeof(char *));
  ht[ht_index].enemy_reply = add_new_element(
      ht[ht_index].enemy_reply,
      ht[ht_index].enemy_phrase_amount,
      sizeof(char *));

  ht[ht_index].enemy_addr[ht[ht_index].enemy_phrase_amount] = strdup(enemy_addr);
  ht[ht_index].enemy_reply[ht[ht_index].enemy_phrase_amount] = strdup(enemy_reply);

  ht[ht_index].enemy_phrase_amount++;

  return (ht[ht_index].enemy_phrase_amount - 1);
} // end int game::add_enemy_phrase (int, TEXT, TEXT)

int game::add_location(
    // название локации
    const char *name)
{
  loc = add_new_element(loc, loc_amount, sizeof(location));

  loc[loc_amount].name = strdup(name);
  loc[loc_amount].comm_amount = 0;
  loc[loc_amount].num_func = NULL;

  loc_amount++;

  return (loc_amount - 1);
} // end int game::add_location (TEXT)

int game::add_location_command(
    // индекс локации
    int loc_index,
    // функция обработки комманды
    FP command_func,
    // активность комманды
    int command_active,
    // команда
    const char *command,
    // описание команды
    const char *command_desc)
{
  loc[loc_index].command_func = add_new_element(
      loc[loc_index].command_func,
      loc[loc_index].comm_amount,
      sizeof(FP));
  loc[loc_index].command_active = add_new_element(
      loc[loc_index].command_active,
      loc[loc_index].comm_amount,
      sizeof(int));
  loc[loc_index].command = add_new_element(
      loc[loc_index].command,
      loc[loc_index].comm_amount,
      sizeof(char *));
  loc[loc_index].command_desc = add_new_element(
      loc[loc_index].command_desc,
      loc[loc_index].comm_amount,
      sizeof(char *));

  loc[loc_index].command_func[loc[loc_index].comm_amount] = command_func;
  loc[loc_index].command_active[loc[loc_index].comm_amount] = command_active;
  loc[loc_index].command[loc[loc_index].comm_amount] = strdup(command);
  loc[loc_index].command_desc[loc[loc_index].comm_amount] = strdup(command_desc);

  loc[loc_index].comm_amount++;

  return (loc[loc_index].comm_amount - 1);
} // end int game::add_location_command (int, FP, int, TEXT, TEXT)

int game::add_inventory(
    // объект героя
    hero *cur_hero,
    // название инвентаря
    const char *name,
    // район, в котором появляется инвентарь
    int district,
    // кол-во событий для вероятности появления
    int events,
    // герой владеет данным инвентарём
    int have,
    // инкремент к характеристикам героя
    int force, // сила
    int smart, // ловкость
    int vita,  // живучесть
    int luck,  // удача
    int armo,  // броня
    int loss   // урон
)
{
  cur_hero->inv = add_new_element(
      cur_hero->inv,
      cur_hero->inv_amount,
      sizeof(inventory));

  cur_hero->inv[cur_hero->inv_amount].name = strdup(name);
  cur_hero->inv[cur_hero->inv_amount].district = district;
  cur_hero->inv[cur_hero->inv_amount].events = events;
  cur_hero->inv[cur_hero->inv_amount].have = have;
  cur_hero->inv[cur_hero->inv_amount].force = force;
  cur_hero->inv[cur_hero->inv_amount].smart = smart;
  cur_hero->inv[cur_hero->inv_amount].vita = vita;
  cur_hero->inv[cur_hero->inv_amount].luck = luck;
  cur_hero->inv[cur_hero->inv_amount].armo = armo;
  cur_hero->inv[cur_hero->inv_amount].loss = loss;

  cur_hero->inv[cur_hero->inv_amount].att = 1;

  // активность инвентаря ставим по умолчанию
  cur_hero->inv[cur_hero->inv_amount].active = 1;

  cur_hero->inv_amount++;

  return (cur_hero->inv_amount - 1);
} // end int game::add_inventory (hero *, TEXT, int, int, int, int, int, int, int, int, int)

int game::add_w_event(
    // тип события
    int type,
    // кол-во событий для вероятности появления
    int events,
    // текст события
    const char *event)
{
  we = add_new_element(we, we_amount, sizeof(w_event));

  we[we_amount].type = type;
  we[we_amount].events = events;
  we[we_amount].event = strdup(event);

  // активность ставим по умолчанию
  we[we_amount].active = 1;

  we_amount++;

  return (we_amount - 1);
} // end int game::add_w_event (int, int, TEXT)

int game::add_price_list(
    // индекс локации, для которой определён данный прайс-лист
    int loc_index,
    // функция, вызываемая при покупке товара
    FP2 buy_func)
{
  pl = add_new_element(pl, pl_amount, sizeof(price_list));

  pl[pl_amount].loc_index = loc_index;
  pl[pl_amount].buy_func = buy_func;
  pl[pl_amount].member_amount = 0;

  pl_amount++;

  return (pl_amount - 1);
} // end int game::add_price_list (int, FP2)

int game::add_price_list_memb(
    // индекс прайс-листа
    int pl_index,
    // название товара
    const char *name,
    // комментарий к товару
    const char *comment,
    // цена на товар
    int price)
{
  pl[pl_index].members = add_new_element(
      pl[pl_index].members,
      pl[pl_index].member_amount,
      sizeof(price_list_memb));

  pl[pl_index].members[pl[pl_index].member_amount].name = strdup(name);
  pl[pl_index].members[pl[pl_index].member_amount].comment = strdup(comment);
  pl[pl_index].members[pl[pl_index].member_amount].price = price;
  pl[pl_index].members[pl[pl_index].member_amount].buy_phrase_amount = 0;

  pl[pl_index].members[pl[pl_index].member_amount].no_repeat = 1; // !!!

  // режим отображения ставим по умолчанию
  pl[pl_index].members[pl[pl_index].member_amount].buy_phrase_print_mode = 1;

  pl[pl_index].member_amount++;

  return (pl[pl_index].member_amount - 1);
} // end int game::add_price_list_memb (int, TEXT, TEXT, int)

int game::add_buy_phrase(
    // индекс прайс-листа
    int pl_index,
    // индекс элемента прайс-листа
    int plm_index,
    // фраза при покупке
    const char *buy_phrase)
{
  pl[pl_index].members[plm_index].buy_phrase = add_new_element(
      pl[pl_index].members[plm_index].buy_phrase,
      pl[pl_index].members[plm_index].buy_phrase_amount,
      sizeof(char *));

  pl[pl_index].members[plm_index].buy_phrase[pl[pl_index].members[plm_index].buy_phrase_amount] = strdup(buy_phrase);

  pl[pl_index].members[plm_index].buy_phrase_amount++;

  return (pl[pl_index].members[plm_index].buy_phrase_amount - 1);
} // end int game::add_buy_phrase (int, int, TEXT)

int game::add_station(
    // название станции
    const char *name,
    // является ли доступной
    int active,
    // сообщение о причине по которой станция не доступна
    const char *unavail_reason)
{
  stn = add_new_element(stn, stn_amount, sizeof(station));

  stn[stn_amount].name = strdup(name);
  stn[stn_amount].avail = active;
  stn[stn_amount].unavail_reason = strdup(unavail_reason);

  stn_amount++;

  return (stn_amount - 1);
} // end int game::add_stantion (TEXT, int, TEXT)

int game::add_plot_line(
    // функция обработки сюжетной линии
    FP3 plot_line_func)
{
  pltl = add_new_element(pltl, pltl_amount, sizeof(plot_line));

  pltl[pltl_amount].active = 0;
  pltl[pltl_amount].plot_line_func = plot_line_func;

  pltl_amount++;

  return (pltl_amount - 1);
} // end int game::add_plot_line (FP3)

int game::is_gamer_hero_type(
    // индекс типа героя
    int ht_index)
{
  if (ht_index < ht_amount) // !!!
  {
    return ht[ht_index].gamer;
  }
  else
  {
    return 0;
  }
} // end int game::is_gamer_hero_type (int)

int game::is_active_location_command(
    // команда
    const char *cmd)
{
  int i;

  for (i = 0; i < loc[active_loc].comm_amount; i++)
  {
    if (strcmp(cmd, loc[active_loc].command[i]) == 0)
    {
      return i;
    }
  }

  return -1;
} // end int game::is_active_location_command (TEXT)

int game::headband()
{
  int old_attr, ch;
  _clsc();
  hidecursor();
  old_attr = settextattr(15);
  printw("\n\n\n\n\n");
  printw("                                               ██\n");
  printw("                                              █  █\n");
  printw("                ███ ████ ████ █  █ █  █ █  █      █\n");
  printw("                █   █  █ █  █ █  █ █  █ █ █      █\n");
  printw("                █   █  █ █  █ ████ █ ██ ██  ██  █\n");
  printw("                █   █  █ █  █ █  █ ██ █ █ █    █\n");
  printw("                █   ████ █  █ █  █ █  █ █  █  █\n");
  printw("                                              █████\n");
  printw("\n\n\n");
  printw("                     ███ ОКОРЕНИЕ\n");
  printw("                     █ █\n");
  printw("                     █ █    █ █ УЛЬТУРНОЙ\n");
  printw("                            ██\n");
  printw("                            █ █     ██ ТОЛИЦЫ\n");
  printw("                                    █\n");
  printw("                                    ██\n");
  printw("\n\n\n\n\n\n\n");
  settextattr(7);
  printw("                     Version %4.2f Release\n\n", VERSION);
  printw("Концепция. . . . . . . . . . . . . . . . . . . . . . . . . ");
  settextattr(14);
  printw("V.P.\n");
  settextattr(7);
  printw("Программирование . . . . . . . . . ");
  settextattr(14);
  printw("d-maxx, tiandrey & bolknote.\n");

  ch = getch();

  showcursor();

  if (ch == 27)
  {
    return 1;
  }
  else
  {
    if (ch == 0)
    {
      getch();
    }
    _clsc();

    return 0;
  }
} // end int game::headband ()

int game::create_hero(
    // индекс типа героя
    int ht_index,
    // имя героя
    const char *name,
    // уровень сложности
    int level_of_complexity = 0)
{
  main_hero = new hero(ht[ht_index], ht_index);
  hero_init = 1;

  main_hero->set_name(name);
  main_hero->set_level_of_complexity(level_of_complexity);

  return 0;
} // end int game::create_hero (int, TEXT, int)

/*int game::get_number_of_sav_file (
                           // параметр начала нового поиска
                           int find_first = 0
                         )
{

  int
    done;

  while (1)
  {
    if (find_first)
    {
      done = findfirst ("gop2_?.sav", &ffblk, 0);
      find_first = 0;
    }
    else
    {
      done = findnext (&ffblk);
    }

    if (done == 0)
    {
      if (
   (ffblk.ff_name [5] >= 49)
     &&
   (ffblk.ff_name [5] <= 52)
 )
      {

return (int) (ffblk.ff_name [5] - 48);
      }
      else
      {
continue;
      }
    }
    else
    {
      return 0;
    }
  }
  return 0;
#warning Deprecate get_number_of_sav_file
} // end int game::get_number_of_sav_file (int)
*/

int game::set_loc(
    // индекс локации
    int loc_index)
{
  active_loc = loc_index;

  return 0;
} // end int game::set_loc (int)

int game::get_min_exp_for_level(
    // уровень, который нужно достичь
    int level)
{
  int
      // минимальное кол-во опыта, которое нужно добавить,
      // чтобы достичь заданного уровня
      min_exp_for_level;

  int i;

  min_exp_for_level = 0;

  for (i = 0; i < level; i++)
  {
    min_exp_for_level += i * 10 + 10;
  }

  return min_exp_for_level;
} // end int game::get_min_exp_for_level (int)

int game::search_inv(
    // объект героя
    hero *cur_hero,
    // имя, по которому производится поиск
    const char *name)
{
  int
      // индекс инвентаря
      inv_index;

  int i;

  inv_index = -1;

  for (i = 0; i < cur_hero->inv_amount; i++)
  {
    if (strcmp(cur_hero->inv[i].name, name) == 0)
    {
      inv_index = i;

      break;
    }
  }

  return inv_index;
} // end int game::search_inv (hero *, TEXT)

int game::search_pl(
    // индекс локации
    int loc_index)
{
  int
      // индекс прайс-листа
      pl_index;

  int i;

  pl_index = -1;

  for (i = 0; i < pl_amount; i++)
  {
    if (pl[i].loc_index == loc_index)
    {
      pl_index = i;
    }
  }

  return pl_index;
} // end int game::search_pl (int)

int game::search_ht(
    // название типа героя
    const char *type)
{
  int
      // индекс типа героя
      ht_index;

  int i;

  ht_index = -1;

  for (i = 0; i < ht_amount; i++)
  {
    if (strcmp(ht[i].type, type) == 0)
    {
      ht_index = i;

      break;
    }
  }

  return ht_index;
} // end int game::search_ht (TEXT)

int game::search_plm_price(
    // индекс прайс-листа
    int *pl_index,
    // индекс элемента прайс-листа
    int *plm_index,
    // название товара
    const char *name)
{
  int
      // цена на товар
      price;

  int i, j, j_start;

  price = -1;

  for (i = (*pl_index); i < pl_amount; i++)
  {
    if (i == (*pl_index))
    {
      j_start = *plm_index;
    }
    else
    {
      j_start = 0;
    }

    for (j = j_start; j < pl[i].member_amount; j++)
    {
      if (strcmp(pl[i].members[j].name, name) == 0)
      {
        price = pl[i].members[j].price;

        *pl_index = i;
        *plm_index = j + 1;

        break;
      }
    }

    if (price != -1)
    {
      break;
    }
  }

  return price;
} // end int game::search_plm_price (int *, int *, TEXT)

int game::supple_inv_run_over(
    // индекс инвентаря
    int inv_index)
{
  int j;

  for (j = 0; j < main_hero->inv_amount; j++)
  {
    if (
        (j != inv_index) &&
        (main_hero->inv[j].have))
    {
      // учёт по урону
      if ((main_hero->inv[j].loss > 0) && (main_hero->inv[inv_index].loss > 0))
      {
        if (
            (
                (main_hero->inv[j].loss > 2) &&
                (main_hero->inv[inv_index].loss > 2)) ||
            ((main_hero->inv[j].loss <= 2) &&
             (main_hero->inv[inv_index].loss <= 2)))
        {
          if (main_hero->inv[j].loss > main_hero->inv[inv_index].loss)
          {
            main_hero->inv[inv_index].active = (main_hero->inv[inv_index].have) ? (0) : (1);

            break;
          }
          else
          {
            main_hero->inv[j].active = (main_hero->inv[inv_index].have) ? (0) : (1);

            break;
          }
        }
      }

      // учёт по броне:
      if ((main_hero->inv[j].armo > 0) && (main_hero->inv[inv_index].armo > 0))
      {
        if (
            (
                (main_hero->inv[j].armo > 2) &&
                (main_hero->inv[inv_index].armo > 2)) ||
            ((main_hero->inv[j].armo <= 2) &&
             (main_hero->inv[inv_index].armo <= 2)))
        {
          if (main_hero->inv[j].armo > main_hero->inv[inv_index].armo)
          {
            main_hero->inv[inv_index].active = (main_hero->inv[inv_index].have) ? (0) : (1);

            break;
          }
          else
          {
            main_hero->inv[j].active = (main_hero->inv[inv_index].have) ? (0) : (1);

            break;
          }
        }
      }
    }
  }

  return 0;
} // end int game::supple_inv_run_over (int)

int game::supple_loc_run_over()
{
  // текущая активная локация
  int cur_active_loc;

  cur_active_loc = active_loc;

  active_loc = 0;

  loc[active_loc].command_active[is_active_location_command("rep")] = (main_hero->station) ? (0) : (open_rep);
  loc[active_loc].command_active[is_active_location_command("mar")] = (main_hero->station == 2) ? (1) : ((main_hero->station) ? (0) : (open_mar));
  loc[active_loc].command_active[is_active_location_command("pr")] = (main_hero->station == 4) ? (1) : ((main_hero->station) ? (0) : (get_open_pr()));
  loc[active_loc].command_active[is_active_location_command("trn")] = ((main_hero->station == 4) || (main_hero->station == 0)) ? (get_open_trn()) : (0);
  loc[active_loc].command_active[is_active_location_command("bmar")] = ((main_hero->station == 3) || (main_hero->station == 0)) ? (get_open_bmar()) : (0);
  loc[active_loc].command_active[is_active_location_command("kl")] = (main_hero->station == 2) ? (1) : ((main_hero->station) ? (0) : (get_open_kl()));
  loc[active_loc].command_active[is_active_location_command("girl")] = (main_hero->station == (open_girl - 1)) ? (1) : (0);
  loc[active_loc].command_active[is_active_location_command("str")] = (main_hero->station) ? (0) : (open_str);
  loc[active_loc].command_active[is_active_location_command("ob")] = open_ob;
  loc[active_loc].command_active[is_active_location_command("eog")] = (main_hero->station == 1) ? ((stay_eog) ? (1) : (0)) : (0);

  active_loc = 1;

  loc[active_loc].command_active[is_active_location_command("v")] = get_open_pr();
  loc[active_loc].command_active[is_active_location_command("f")] = (main_hero->inv[search_inv(main_hero, "Пистолет")].have) ? (1) : (0);

  active_loc = 4;

  loc[active_loc].command_active[is_active_location_command("a")] = get_open_a();
  loc[active_loc].command_active[is_active_location_command("hp")] = get_open_hp();
  loc[active_loc].command_active[is_active_location_command("d")] = get_open_d();
  loc[active_loc].command_active[is_active_location_command("raid")] = (main_hero->station) ? (open_raid) : (0);

  active_loc = 8;

  loc[active_loc].command_active[is_active_location_command("meet")] = ((main_hero->station == 2) && (open_girl == 0)) ? (1) : (0);
  loc[active_loc].command_active[is_active_location_command("tus")] = (strcmp(main_hero->get_type(), "Нефор") == 0) ? (0) : (1);
  loc[active_loc].command_active[is_active_location_command("sl")] = (strcmp(main_hero->get_type(), "Нефор") == 0) ? (1) : (0);

  active_loc = cur_active_loc;

  return 0;
} // end int game::supple_loc_run_over ()

int game::supple_pl_run_over()
{
  // -- выглядит тупо, а куда деваться? что-то совершенствовать уже влом...

  int
      pl_index;

  if (main_hero->district >= 1)
  {
    pl_index = search_pl(7);

    pl[pl_index].members[2].price = 40 * ((main_hero->station) ? (2) : (1));
  }

  if (main_hero->district >= 2)
  {
    pl_index = search_pl(7);

    pl[pl_index].members[3].price = 40 * ((main_hero->station) ? (2) : (1));
  }

  if (main_hero->district >= 3)
  {
    pl_index = search_pl(7);

    pl[pl_index].members[4].price = 60 * ((main_hero->station) ? (2) : (1));
  }

  return 0;
} // end int game::supple_pl_run_over ()

int game::supple_pltl_run_over()
{
  int i, ret;

  if (active_pltl > -1)
  {
    pltl[active_pltl].plot_line_func(active_pltl);
  }
  else
  {
    for (i = 0; i < pltl_amount; i++)
    {
      if (pltl[i].active > -1)
      {
        if ((ret = pltl[i].plot_line_func(i)) != 0)
        {
          return ret;
        }
      }
    }
  }

  return 0;
} // int game::supple_pltl_run_over ()

int game::gen_enemy(
    // индекс типа героя
    int *ht_index)
{
  int
      // уровеь врага
      level,
      // величина, обратно пропорциональная вероятности выпадения
      // максимального приращения
      dN,
      i;

  // dN = 10 - (main_hero->get_level () % 10);
  dN = 5;

  level = superrandom(10, dN, 12 + main_hero->district * 3, main_hero->get_level() % 10 + main_hero->district * 3);

  if (level == -1)
  {
    *ht_index = 0;

    return -1;
  }
  else
  {
    level += main_hero->district * 10;
  }

  i = 0;

  while (1)
  {
    i = getrandom(-1, ht_amount - 1);

    if (ht[i].active == 0)
    {
      continue;
    }

    if (chance(1, ht[i].events))
    {
      *ht_index = i;

      if (ht[i].min_level_inc > 0)
      {
        if (
            (level > main_hero->get_level()) &&
            (ht[i].min_level_inc > (level - main_hero->get_level())))
        {
          level += ht[i].min_level_inc;
        }
        else
        {
          if ((level <= main_hero->get_level()))
          {
            level = main_hero->get_level() + ht[i].min_level_inc;
          }
        }
      }

      break;
    }
  }

  return level;
} // end int game::gen_enemy (int *)

int game::gen_enemy_obj(
    // индекс типа героя
    int ht_index,
    // уровень генерируемого героя
    int level,
    int gkc,
    int balance)
{
  // инвентарь героя
  inventory *inv;

  int i;

  float fi;

  if (enemy_init)
  {
    delete enemy;
    enemy_init = 0;
  }

  if (!(enemy = new hero(ht[ht_index], ht_index)))
  {
    clean_mem();

    fprintf(stderr, "debug: game.cpp:1921: epic fail\n");
    exit(1);
  }

  enemy_init = 1;

  enemy->print_mode = 0;

  enemy->set_name("Противник");
  enemy->add_exp(get_min_exp_for_level(level));

  // уравновешивание характеристик противника и героя
  if (balance)
  {
    // сила
    if (
        ((main_hero->get_force() - enemy->get_force()) >= 10) ||
        ((main_hero->get_armo() > 5) &&
         (main_hero->get_force() >= enemy->get_force())))
    {
      enemy->add_force(main_hero->get_armo());

      while (chance(3, 5))
      {
        enemy->add_force(getrandom(3, 5));
      }
    }

    // ловкость
    if (
        (main_hero->get_kick_count() > 1) &&
        (chance(1, 2)))
    {
      while (enemy->get_kick_count() < main_hero->get_kick_count())
      {
        enemy->add_smart(getrandom(5, 10));
      }
    }

    // броня
    if ((main_hero->get_max_health() - enemy->get_max_health()) >= 20)
    {
      fi = 1 - (float)enemy->get_max_health() / main_hero->get_max_health();

      enemy->add_armo((int)main_hero->get_max_loss() * fi);
    }
    else
    {
      if (
          (((float)enemy->get_force() / main_hero->get_force()) < 2) &&
          (main_hero->get_armo() > 5))
      {
        enemy->add_armo(getrandom((int)main_hero->get_armo() / 2, main_hero->get_armo()));
      }
    }

    enemy->add_health(enemy->get_max_health() - enemy->get_health());
  }

  // генерируем инвентарь противника

  inv = main_hero->inv;

  for (i = 0; i < main_hero->inv_amount; i++)
  {
    if (
        (inv[i].district <= main_hero->district) &&
        (inv[i].events > 0) &&
        (chance(1, inv[i].events)))
    {
      if (
          (
              (inv[i].force > 0) ||
              (inv[i].smart > 0) ||
              (inv[i].vita > 0) ||
              (inv[i].luck > 0)) &&
          (ht[ht_index].inv_desc_events > 0) &&
          (chance(1, ht[ht_index].inv_desc_events)))
      {
        break;
      }

      if (
          (inv[i].armo > 0) &&
          (ht[ht_index].inv_armo_events > 0) &&
          (chance(1, ht[ht_index].inv_armo_events)))
      {
        break;
      }

      if (
          (inv[i].loss > 0) &&
          (ht[ht_index].inv_loss_events > 0) &&
          (chance(1, ht[ht_index].inv_loss_events)))
      {
        break;
      }

      if (
          (inv[i].force == 0) &&
          (inv[i].smart == 0) &&
          (inv[i].vita == 0) &&
          (inv[i].luck == 0) &&
          (inv[i].armo == 0) &&
          (inv[i].loss == 0) &&
          (ht[ht_index].inv_other_events > 0) &&
          (chance(1, ht[ht_index].inv_other_events)))
      {
        break;
      }
    }
  }

  // i = 0;

  if (i < main_hero->inv_amount)
  {
    cur_game->add_inventory(
        enemy,
        inv[i].name,
        inv[i].district,
        inv[i].events,
        1,
        inv[i].force,
        inv[i].smart,
        inv[i].vita,
        inv[i].luck,
        inv[i].armo,
        inv[i].loss);

    if (enemy->get_health() < enemy->get_max_health())
    {
      enemy->add_health(enemy->get_max_health() - enemy->get_health());
    }
  }

  // генерируем деньги противника
  if ((ht[ht_index].money_events > 0) && (chance(1, ht[ht_index].money_events)))
  {
    enemy->add_money(getrandom(0, 20 + 20 * main_hero->district));
  }

  // генерируем хлам противника
  if ((ht[ht_index].stuff_events > 0) && (chance(1, ht[ht_index].stuff_events)))
  {
    enemy->add_stuff(getrandom(0, 10 + 10 * main_hero->district));
  }

  // генерируем самокрутки противника
  if ((ht[ht_index].ciga_events > 0) && (chance(1, ht[ht_index].ciga_events)))
  {
    enemy->add_ciga(1);
  }

  if (gkc)
  {
    gen_kick_count();
  }

  return 0;
} // end int game::gen_enemy_obj (int, int, int)

int game::gen_kick_count()
{
  if (
      (main_hero->get_kick_count() > 1) &&
      (enemy->get_kick_count() > 1))
  {
    if (main_hero->get_kick_count() <= enemy->get_kick_count())
    {
      main_hero->kick_count = 1;
      enemy->kick_count = enemy->get_kick_count() - main_hero->get_kick_count() + 1;
    }
    else
    {
      enemy->kick_count = 1;
      main_hero->kick_count = main_hero->get_kick_count() - enemy->get_kick_count() + 1;
    }

    settextattr(10);
    printw(
        "Из-за твоей хорошей ловкости враг сможет пнуть тебя %i раз(а) вместо %i\n",
        enemy->kick_count, enemy->get_kick_count());

    settextattr(12);
    printw(
        "Из-за хорошей ловкости врага ты сможешь пнуть его %i раз(а) вместо %i\n",
        main_hero->kick_count, main_hero->get_kick_count());
  }
  else
  {
    enemy->kick_count = enemy->get_kick_count();
    main_hero->kick_count = main_hero->get_kick_count();
  }

  return 0;
} // int game::gen_kick_count ()

int game::kick(
    // герой, производящий удар
    hero *hero1,
    // герой под ударом
    hero *hero2,
    // точность (вероятность) удара
    int exact,
    // двойной удар
    int *double_loss)
{
  int
      // урон, нанесённый ударом
      loss;

  loss = 0;

  if (
      (!(chance(1, hero1->get_luck() + 1))) &&
      (chance(exact, 100)))
  {
    // что б не было зацикливаний
    if (hero1->get_max_loss() > hero2->get_armo())
    {
      loss = sub(getrandom(hero1->get_min_loss() - 1, hero1->get_max_loss()), hero2->get_armo());
    }
    else
    {
      loss = getrandom(hero1->get_min_loss() - 1, hero1->get_max_loss());
    }

    // -- блин, вообще не втыкаю, как такое может получаться?
    if (loss < 0) // !!!
    {
      loss = 0;
    }

    if (
        (!(hero1->broken_foot)) &&
        (!(chance(1, hero1->get_luck() + 1))) &&
        (chance(exact, 100)))
    {
      loss *= 2;
      *double_loss = 1;
    }

    if (loss > 0)
    {
      if (
          (!(hero2->broken_jaw)) &&
          (!(hero1->broken_foot)) &&
          (!(chance(1, hero1->get_luck() + 1))) &&
          (chance(1, hero2->get_luck() + 1)) &&
          (chance(exact, 100)) // !!!
      )
      {
        // если есть зубная защита
        if (
            (search_inv(hero2, "Зубная защита боксёров") != -1) &&
            (hero2->inv[search_inv(hero2, "Зубная защита боксёров")].have))
        {
          if (chance(1, 2))
          {
            hero2->broken_jaw = 1;
          }
        }
        else
        {
          hero2->broken_jaw = 1;
        }
      }
      else
      {
        if (
            (!(hero2->broken_foot)) &&
            (!(hero1->broken_foot)) &&
            (!(chance(1, hero1->get_luck() + 1))) &&
            (chance(1, hero2->get_luck() + 1 + hero2->trn_foot)) &&
            (chance(exact, 100)))
        {
          hero2->broken_foot = 1;
        }
      }
    }

    hero2->sub_health(loss);
  }

  return loss;
} // end int game::kick (hero *, hero *, int, int)

int game::kick_realiz(
    // герой, производящий удар
    hero *hero1,
    // герой под ударом
    hero *hero2,
    // максимальное кол-во пустых ударов
    int empty_k_count,
    // сообщения для вывода
    const char **mess,
    // видеоатрибуты
    int attr1,
    int attr2)
{
  int
      // урон
      loss,
      // общий урон
      loss_amount,
      // состояние героя до удара
      // состояние челюсти
      broken_jaw,
      // состояние ноги
      broken_foot;

  int
      // двойной урон
      double_loss,
      i;

  if (hero1->get_health() == 0)
  {
    return 0;
  }

  i = 0;
  loss_amount = 0;

  while (
      (i < hero1->kick_count) &&
      (hero2->get_health() > 0))
  {
    i++;

    broken_jaw = hero2->broken_jaw;
    broken_foot = hero2->broken_foot;

    double_loss = 0;

    do
    {
      if (i == hero1->kick_count)
      {
        loss = kick(hero1, hero2, hero1->get_accuracy_of_last_kick(), &double_loss);
      }
      else
      {
        loss = kick(hero1, hero2, 90, &double_loss);
      }

      /*
            if (strcmp ("Братва с общаги", hero1->get_type ()) == 0)
            {
      printf ( "Урон: %i / %i %i \n", loss, hero1->empty_kick_count, empty_k_count);
            }
      */
    } while ((loss == 0) && (hero1->empty_kick_count == empty_k_count));

    loss_amount += loss;

    if (loss > 0)
    {
      hero1->empty_kick_count = 0;
    }
    else
    {
      hero1->empty_kick_count++;
    }

    if (mess != NULL)
    {
      settextattr(attr1);

      if (i > 1)
      {
        printw("%s", mess[0]);
      }

      if (broken_jaw != hero2->broken_jaw)
      {
        printf("%s", mess[1]);

        // если есть зубная защита
        if (
            (search_inv(hero2, "Зубная защита боксёров") != -1) &&
            (hero2->inv[search_inv(hero2, "Зубная защита боксёров")].have))
        {
          printw("%s", mess[6]);
        }
      }

      if (broken_foot != hero2->broken_foot)
      {
        printf("%s", mess[2]);
      }

      if (loss > 0)
      {
        if (double_loss)
        {
          printw("%s", mess[3]);
        }

        printw(mess[4], loss, hero2->get_health());
      }
      else
      {
        settextattr(attr2);
        printw("%s", mess[5]);
      }
    }
  }

  return loss_amount;
} // end int game::kick_realiz (hero *, hero *, int, char **, int, int)

int game::new_district()
{
  if (main_hero->district == DISTRICT_AMOUNT)
  {
    main_hero->district--;

    if (!end_of_game)
    {
      settextattr(9);
      printw("Ты запинал кучу мудаков и стал по-жестокому крутым\n");
      printw("Самое время отправится в общагу и отомстить коменде!\n");

      end_of_game = 1;

      stay_eog = 50;
      stn[1].avail = 1;
    }

    return 0;
  }

  if (main_hero->station != 0)
  {
    new_district_norealiz = 1;

    return 0;
  }

  new_district_norealiz = 0;

  // обновляем некоторые переменные

  open_mar = 0;
  open_rep = 0;
  open_pr1 = 0;
  open_hp1 = 0;
  open_d1 = 0;
  open_bmar1 = 0;
  open_trn1 = 0;
  open_kl1 = 0;

  num_w = 0;
  num_empty_w = 0;

  num_t = 0;

  stay_mar1 = 0;
  stay_kl1 = 0;

  /*
    main_hero->sub_att (main_hero->get_att ());
    main_hero->add_att ((main_hero->district + 1) * 100);
  */

  main_hero->set_att((main_hero->district + 1) * 100);

  settextattr(9);
  printw("Ты стал самым крутым в этом районе. Отправляйся в следующий.\n");

  settextattr(14);
  printw("Хочешь сохранить свои достижения? (y/n)\n");

  if (wait_answ())
  {
    sav();
  }

  return 0;
} // end int game::new_district ()

int game::new_station()
{
  int
      pl_index;

  int
      i,
      j;

  if (main_hero->station != 0)
  {
    ht[search_ht("Студент")].active = 1;
    ht[search_ht("Эмокид")].active = 1;
    ht[search_ht("Интеллигент")].active = 1;

    pl_index = search_pl(5);
    main_hero->inv[search_inv(main_hero, pl[pl_index].members[3].name)].district = 0;
    main_hero->inv[search_inv(main_hero, pl[pl_index].members[4].name)].district = 0;
    main_hero->inv[search_inv(main_hero, pl[pl_index].members[5].name)].district = 0;
    /*
        pl [pl_index].members [3].district = 0;
        pl [pl_index].members [4].district = 0;
        pl [pl_index].members [5].district = 0;
    */
    pl[pl_index].members[6].price = 150;
    pl[pl_index].members[7].price = 30;

    if (main_hero->get_level() >= 35)
    {
      pl[pl_index].members[8].price = 50;
    }

    pl_index = search_pl(8);
    pl[pl_index].members[2].price = 10;

    if (strcmp(main_hero->get_type(), "Нефор") == 0)
    {
      pl[pl_index].members[3].price = 15;
    }

    for (i = 0; i < pl_amount; i++)
    {
      if (pl[i].loc_index != 9)
      {
        for (j = 0; j < pl[i].member_amount; j++)
        {
          pl[i].members[j].price *= 2;
        }
      }
    }
  }
  else
  {
    ht[search_ht("Студент")].active = 0;
    ht[search_ht("Эмокид")].active = 0;
    ht[search_ht("Интеллигент")].active = 0;

    pl_index = search_pl(5);
    main_hero->inv[search_inv(main_hero, pl[pl_index].members[3].name)].district = 1;
    main_hero->inv[search_inv(main_hero, pl[pl_index].members[4].name)].district = 2;
    main_hero->inv[search_inv(main_hero, pl[pl_index].members[5].name)].district = 3;
    pl[pl_index].members[6].price = 0;
    pl[pl_index].members[7].price = 0;
    pl[pl_index].members[8].price = 0;

    pl_index = search_pl(8);
    pl[pl_index].members[2].price = 0;
    pl[pl_index].members[3].price = 0;

    for (i = 0; i < pl_amount; i++)
    {
      if (pl[i].loc_index != 9)
      {
        for (j = 0; j < pl[i].member_amount; j++)
        {
          pl[i].members[j].price /= 2;
        }
      }
    }
  }

  return 0;
} // end int game::new_station ()

int game::buy_realiz()
{
  // элемент прайс-листа
  price_list_memb plm;

  int
      // индекс прайс-листа
      pl_index,
      // индекс элемента прайс-листа
      plm_index,
      // индекс инвентаря
      inv_index;

  int i, flag, flag2;

  supple_pl_run_over();

  pl_index = search_pl(active_loc);
  plm_index = (int)strtod(active_cmd, NULL) - 1;

  if (
      (pl_index != -1) &&
      (plm_index >= 0) &&
      (plm_index < pl[pl_index].member_amount))
  {
    plm = pl[pl_index].members[plm_index];

    if (
        (
            ((inv_index = search_inv(main_hero, plm.name)) != -1) &&
            (main_hero->inv[inv_index].district > main_hero->district)) ||
        (plm.price == 0))
    {
      return 0;
    }

    if (main_hero->get_money() >= plm.price)
    {
      flag = 0;

      // рассматриварием товар в качестве инвентаря
      if (inv_index != -1)
      {
        flag2 = 1;

        if (
            (plm.no_repeat) &&
            (main_hero->inv[inv_index].have))
        {
          settextattr(14);
          printw("У тебя уже это есть. Точно хочешь купить ещё раз? (y/n)\n");

          flag2 = wait_answ();

          plm.buy_phrase_print_mode = 0;
        }

        if (flag2)
        {
          main_hero->inv[inv_index].have++;
          supple_inv_run_over(inv_index);

          flag = 1;
        }
        else
        {
          plm.buy_phrase_print_mode = 0;
        }
      }

      if (
          (plm.buy_phrase_amount > 0) &&
          (plm.buy_phrase_print_mode))
      {
        i = getrandom(-1, plm.buy_phrase_amount - 1);

        settextattr(14);
        printf("%s\n", plm.buy_phrase[i]);
      }

      // вызываем функцию по обработке прайс-листа
      if (pl[pl_index].buy_func != NULL)
      {
        pl[pl_index].buy_func(pl_index, plm_index, &flag);
      }

      // если товар приобретён
      if (flag)
      {
        main_hero->sub_money(plm.price);
      }
    }
    else
    {
      settextattr(12);
      printw("не хватает бабла\n");
    }
  }
  else
  {
    if (loc[active_loc].num_func != NULL)
    {
      loc[active_loc].num_func(plm_index);
    }
  }

  return 0;
} // end int game::buy_realiz ()

int game::fire(
    // герой, производящий выстел
    hero *hero1,
    // герой, подверженный выстрелу
    hero *hero2)
{
  int
      // урон
      loss;

  loss = 0;

  if (
      (!chance(1, hero1->get_luck() + 1)) &&
      (chance(1, (hero2->get_luck() < 10) ? (hero2->get_luck()) : (10))))
  {
    loss = getrandom(20, 80);
  }

  hero2->sub_health(loss);

  return loss;
} // end int game::fire (hero *, hero *)

int game::fire_realiz(
    // герой, производящий выстрел
    hero *hero1,
    // герой, подверженный выстрелу
    hero *hero2,
    // максимальное кол-во пустых выстрелов
    int empty_k_count,
    // сообщения для вывода
    const char **mess,
    // видеоатрибуты
    int attr1,
    int attr2)
{
  int
      // индекс инвентаря
      inv_index,
      // урон
      loss;

  if (hero1->get_health() == 0)
  {
    return 0;
  }

  inv_index = search_inv(hero1, "Патроны");

  if (hero1->inv[inv_index].have > 0)
  {
    do
    {
      loss = fire(hero1, hero2);
    } while ((loss == 0) && (hero1->empty_kick_count == empty_k_count));

    if (loss > 0)
    {
      hero1->empty_kick_count = 0;
    }
    else
    {
      hero1->empty_kick_count++;
    }
  }

  if (mess != NULL)
  {
    if (hero1->inv[inv_index].have > 0)
    {
      if (loss == 0)
      {
        settextattr(attr2);
        printf("%s", mess[0]);
      }
      else
      {
        settextattr(attr1);
        printf(mess[1], loss, hero2->get_health());
      }

      hero1->inv[inv_index].have--;

      printw(mess[2], hero1->inv[inv_index].have);
    }
    else
    {
      settextattr(attr2);
      printw("%s", mess[3]);
    }
  }

  return 0;
} // int game::fire_realize (hero *, hero *, int, char **, int, int)

bool game::get_open_pr()
{
  return (main_hero->station) ? (open_pr2) : (open_pr1);
} // end int game::get_open_pr ()

bool game::get_open_hp()
{
  return (main_hero->station) ? (open_hp2) : (open_hp1);
} // end int game::get_open_hp ()

bool game::get_open_d()
{
  return (main_hero->station) ? (open_d2) : (open_d1);
} // end int game::get_open_d ()

bool game::get_open_trn()
{
  return (main_hero->station) ? (open_trn2) : (open_trn1);
} // end int game::get_open_trn ()

bool game::get_open_bmar()
{
  return (main_hero->station) ? (open_bmar2) : (open_bmar1);
} // end int game::get_open_bmar ()

bool game::get_open_a()
{
  return (main_hero->station) ? (open_a2) : (open_a1);
} // end int game::get_open_a ()

bool game::get_open_kl()
{
  return (main_hero->station) ? (open_kl2) : (open_kl1);
} // end int game::get_open_kl ()

int game::set_open_pr(int d)
{
  (main_hero->station) ? (open_pr2 = d) : (open_pr1 = d);

  return 0;
} // end int game::set_open_pr (int)

int game::set_open_hp(int d)
{
  (main_hero->station) ? (open_hp2 = d) : (open_hp1 = d);

  return 0;
} // end int game::set_open_hp (int)

int game::set_open_d(int d)
{
  (main_hero->station) ? (open_d2 = d) : (open_d1 = d);

  return 0;
} // end int game::set_open_d (int)

int game::set_open_trn(int d)
{
  (main_hero->station) ? (open_trn2 = d) : (open_trn1 = d);

  return 0;
} // end int game::set_open_trn (int)

int game::set_open_bmar(int d)
{
  (main_hero->station) ? (open_bmar2 = d) : (open_bmar1 = d);

  return 0;
} // end int game::set_open_bmar (int)

int game::set_open_a(int d)
{
  (main_hero->station) ? (open_a2 = d) : (open_a1 = d);

  return 0;
} // end int game::set_open_a (int)

int game::set_open_kl(int d)
{
  (main_hero->station) ? (open_kl2 = d) : (open_kl1 = d);

  return 0;
} // end int game::set_open_kl (int)

int game::get_stay_mar()
{
  return (main_hero->station) ? (stay_mar2) : (stay_mar1);
} // end int game::get_stay_mar ()

int game::get_stay_kl()
{
  return (main_hero->station) ? (stay_kl2) : (stay_kl1);
} // end int game::get_stay_kl ()

int game::set_stay_mar(
    int d)
{
  (main_hero->station) ? (stay_mar2 = d) : (stay_mar1 = d);

  return 0;
} // end int game::set_stay_mar (int)

int game::set_stay_kl(
    int d)
{
  (main_hero->station) ? (stay_kl2 = d) : (stay_kl1 = d);

  return 0;
} // end int game::set_stay_mar (int)

int game::start()
{
  hidecursor();
  const char *mess[21] = {
      "Выбери, чё те надо:\n",
      "1-Начать новую игру\n",
      "2-Загрузить игру (не реализовано)\n",
      "3-Выйти\n",
      "Ты можешь начать...\n",
      "%i-...с места быстрого сохранения\n",
      "%i-...с %iого района\n",
      "чё-то ты не то жмёшь\n",
      "Выбери, кем ты будешь:\n", // 8
      "Выбери уровень сложности:\n",
      "1-Простой\n",
      "2-Нормальный\n",
      "3-Сложный\n",
      "Твоё погоняло: ",
      "чё типа, свой зоновский номер написал?\n",
      "те чё, сложно пару букв написать?\n",
      "Чё за нах?",
      "Подтсан   - нормальный чувак, зря не понтуется, беспредела не мутит.\n            бонус - гёрлфренд\n",
      "Отморозок - тупой корявый мудак, но если врежет, мало не покажется.\n            бонус - качалка на Петроградской\n",
      "Гопник    - гоп он и есть гоп, тут вопросов быть не должно.\n            бонус - возможность разводить лохов на деньги\n",
      "Нефор     - грёбанный позер, по сути, такой же урод, как и все остальные.\n            бонус - бесплатная проходка в клуб на Сенной\n"};

  int
      // старый видеоатрибут
      old_attr,
      // выбор пользователя в меню
      user_choice,
      // тип героя пользователя
      user_ht_index,
      // уровень сложности пользователя
      user_level_of_complexity,
      j, i;

  char
      buf[100],
      *user_name,
      *tmp;
  //    ,*cmd;

  old_attr = settextattr(15);
  printw("%s", mess[0]);
  settextattr(14);
  printw("%s", mess[1]);
  printw("%s", mess[2]);
  printw("%s", mess[3]);

  do
  {
    i = 0;
    user_choice = getch(0);
    //      printf("\n");
    switch (user_choice)
    {
    case '1':
      break;

      /*        case '2' :
              load_game=load();
              break;*/

    case '3':
      clean_mem();
      exit(1);
      break;

    default:
      i = 1;
      settextattr(12);
      printw("%s", mess[7]);
    }
  } while (i);

  if (load_game)
  {
    goto exit;
  }

  // завязка сюжета #1
  if (pltl_amount > 0)
  {
    pltl[0].plot_line_func(0);
  }
  settextattr(15);
  printw("%s", mess[8]);

  settextattr(14);
  j = 0;

  for (i = 0; i < ht_amount; i++)
  {
    if (is_gamer_hero_type(i))
    {
      printw("%i-%s\n", i + 1, ht[i].type);
      j++;
    }
  }

  printw("%i-%s\n", j + 1, mess[16]);

  // определение типа героя пользователя
  while (1)
  {
    user_ht_index = getch(0);
    //    printf("\n");

    if ((user_ht_index - '1') == j)
    {
      settextattr(9);
      printw("%s", mess[17]);
      printw("%s", mess[18]);
      printw("%s", mess[19]);
      printw("%s", mess[20]);
      continue;
    }

    if (
        (isdigit(user_ht_index)) &&
        (is_gamer_hero_type(user_ht_index - '1')))
    {
      user_ht_index -= '1';
      break;
    }
    else
    {
      settextattr(12);
      printw("%s", mess[7]);
    }
  }

  switch (user_ht_index)
  {
  case 0:
    open_girl = 1;
    break;

  case 1:
    open_trn2 = 1;
    break;

  case 2:
    open_ob = 1;
    break;
  }

  settextattr(15);
  printw("%s", mess[9]);

  settextattr(14);
  printw("%s", mess[10]);
  printw("%s", mess[11]);
  printw("%s", mess[12]);

  while (1)
  {
    user_level_of_complexity = getch(0);
    //    printf("\n");

    if (
        (user_level_of_complexity >= '1') &&
        (user_level_of_complexity <= '3'))
    {
      user_level_of_complexity -= '1';
      break;
    }
    else
    {
      settextattr(12);
      printw("%s", mess[7]);
    }
  }

  // определение имени героя пользователя
  showcursor();
  while (1)
  {
    //    buf [0] = 19;

    settextattr(10);
    printw("%s", mess[13]);

    settextattr(15);
    user_name = fgets(buf, 99, stdin);
    tmp = user_name + strlen(user_name);
    while ((tmp >= user_name) && ((*tmp == 10) || (*tmp == 0)))
    {
      *tmp = 0;
      tmp--;
    }
    if (strlen(user_name) != 0)
    {
      //      printw ( "\n");

      if (isdigitstr(user_name))
      {
        settextattr(12);
        printw("%s", mess[14]);
      }
      else
      {
        break;
      }
    }
    else
    {
      settextattr(12);
      printw("%s", mess[15]);
    }
  }

  // создание героя
  create_hero(user_ht_index, user_name, user_level_of_complexity);

  // завязка сюжета #2
  if (pltl_amount > 0)
  {
    pltl[0].plot_line_func(0);
  }

exit:

  active_loc = 0;

  settextattr(old_attr);

  return 0;
} // end int game::start ()
