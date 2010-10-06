#include "comm.h"
#include "../main.h"
#include "../list.h"
#include "../hero.h"
#include "../game.h"
#include "../utils.h"

extern game *cur_game;

extern const char *level_type[43];

int s ()
{
  // объект героя
  hero *main_hero;
  
  // сообщения функции
  const char *mess [30] = {
                     "Ты %s %i уровня - %s ",
                     "Твоё погоняло: ",
                     "Сейчас у тебя %i опыта, а для прокачки надо %i\n",
                     "Сл:",
                     "Лв:",
                     "Жв:",
                     "Уд:",
                     "Феньки:\n",
                     " (Всё +%i)",
                     "Сила +%i, ",
                     "Ловкость +%i, ",
                     "Живучесть +%i, ",
                     "Удача +%i, ",
                     "У тебя есть:\n",
                     "Урон ",
                     "Здоровье %i/%i",
                     "Точность %i%%\n",
                     "Точность 90%% - %i ударов, Точность %i удара %i%%\n",
                     "Броня ",
                     "Пиво %d бутылок\n",
                     "Пива нету\n",
                     "Бабки %i руб.\n",
                     "Бабла нету\n",
                     "Косяки %i\n",
                     "Хлам %i\n",
                     " Сломана челюсть",
                     " Сломана нога",
                     "Обдолбанный ",
                     "Бухой",
                     " (%i шт.)"
                   };
  
  const char *
    // тип обозначения героя
    cur_level_type;
  
  int
    // количество возможных ударов
    kick_count;
  
  int
    // флаги на инкременты
    force = 0, smart = 0, vita = 0, luck = 0,
    flag, i;
  
  float
    fi;

  if (cur_game->enemy_desc)
  {
    main_hero = cur_game->enemy;
  }
  else
  {
    main_hero = cur_game->main_hero;            
  }
  
  // уровень и тип героя
  
  if (main_hero->get_level () > 42)
  {
    cur_level_type = level_type [42];
  }
  else
  {
    cur_level_type = level_type [main_hero->get_level ()];
  }

  settextattr (10);
  printw (mess [0], main_hero->get_type (), main_hero->get_level (), cur_level_type);

  settextattr (12);
  
  if (main_hero->stoned)
  {
    printw ("%s",mess [27]);
  }

  if (main_hero->drunk)
  {
    printw ("%s",mess [28]);
  }

  printw ("\n");

  settextattr (10);
  printw ("%s",mess [1]);

  // имя
  settextattr (15);
  printw ("%s\n",main_hero->get_name ());

  // опыт
  settextattr (14);
  printw (mess [2], main_hero->get_exp (), main_hero->get_max_exp ());

  // навыки (сила, ловкость, живучесть, удача)
  
  for (i = 0; i < main_hero->inv_amount; i++)
  {
    if (main_hero->inv [i].have)
    {
      force += main_hero->inv [i].force;
      smart += main_hero->inv [i].smart;
      vita  += main_hero->inv [i].vita;
      luck  += main_hero->inv [i].luck;
    }
  }         

  settextattr (15);
  printw ("%s",mess [3]);

  if (
       (force > 0)
         ||
       (main_hero->stoned)
     )
  {
    settextattr (9);
  }

  printw ("%i ", main_hero->get_force ());
  
  settextattr (15);
  printw ("%s",mess [4]);          
  
  if (
       (smart > 0)
         ||
       (main_hero->stoned)
     )
  {
    settextattr (9);
  }

  if (main_hero->drunk)
  {
    settextattr (12);
  }
  
  printw ("%i ", main_hero->get_smart ());
  
  settextattr (15);
  printw ("%s",mess [5]);

  if (
       (vita > 0)
         ||
       (main_hero->stoned)
     )
  {
    settextattr (9);
  }

  printw ("%i ", main_hero->get_vita ());
  
  settextattr (15);
  printw ("%s",mess [6]);

  if (
       (luck > 0)
         ||
       (main_hero->stoned)
     )
  {
    settextattr (9);
  }

  printw ("%i\n", main_hero->get_luck ());         

  // феньки

  flag = 0;

  for (i = 0; i < main_hero->inv_amount; i++)
  {
    if (main_hero->inv [i].have)
    {             
      force = main_hero->inv [i].force;
      smart = main_hero->inv [i].smart;
      vita  = main_hero->inv [i].vita;
      luck  = main_hero->inv [i].luck;                

      if ((force > 0) || (smart > 0) || (vita > 0) || (luck > 0))
      {
        if (flag == 0)
        {
          settextattr (15);
          printw ("%s",mess [7]);

          settextattr (9);

          flag = 1;
        }

        printw ("%s", main_hero->inv [i].name);
        
        if ((force == smart) && (smart == vita) && (vita == luck))
        {
          printw (mess [8], force);
        }   
        else
        {  
          printw (" (");

          if (force > 0)
          {
            printw (mess [9], force);
          }

          if (smart > 0)
          {
            printw (mess [10], smart);
          }

          if (vita > 0)
          {
            printw (mess [11], vita);
          }

          if (luck > 0)
          {
            printw (mess [12], luck);
          }

          printw ("%c%c)",8,8);
        }

        if (main_hero->inv [i].have > 1)
        {
          printw (mess [29], main_hero->inv [i].have);
        }

        printw ("%s","\n");
      }
    }
  }         

  // у тебя есть

  flag = 0;

  for (i = 0; i < main_hero->inv_amount; i++)
  {
    if (
         (main_hero->inv [i].have)
           &&
         (main_hero->inv [i].force == 0) 
           && 
         (main_hero->inv [i].smart == 0) 
           && 
         (main_hero->inv [i].vita  == 0) 
           && 
         (main_hero->inv [i].luck  == 0) 
           && 
         (main_hero->inv [i].armo  == 0)
           &&
         (main_hero->inv [i].loss  == 0)
       )
    {
      if (flag == 0)
      {
        settextattr (15);
        printw ("%s",mess [13]);                

        flag = 1;
      }
      
      if (main_hero->inv [i].active)
      {
        settextattr (9);
      }
      else
      {
        settextattr (12);
      }

      printw ("%s", main_hero->inv [i].name);

      if (main_hero->inv [i].have > 1)
      {
        printw (mess [29], main_hero->inv [i].have);
      }

      printw ("%s","\n");
    }
  }

  // урон

  settextattr (15);
  printw ("%s",mess [14]);

  flag = 0;

  for (i = 0; i < main_hero->inv_amount; i++)
  {
    if ((main_hero->inv [i].have) && (main_hero->inv [i].loss > 0))
    {              
      if (flag == 0)
      {
        settextattr (9);
        printw ("%i-%i", main_hero->get_min_loss (), main_hero->get_max_loss ());
        flag = 1;
      }

      if (main_hero->inv [i].active == 1)
      {
        settextattr (9);                
        printw (" %s(+%i)", main_hero->inv [i].name, main_hero->inv [i].loss);
      }
      else
      {
        settextattr (12);
        printw (" %s", main_hero->inv [i].name);
      }

      if (main_hero->inv [i].have > 1)
      {
        printw (mess [29], main_hero->inv [i].have);
      }
    }
  }

  if (flag == 0)
  {
    printw ("%i-%i", main_hero->get_min_loss (), main_hero->get_max_loss ());
  }

  printw ("%s","\n");

  // здоровье

  fi = (float) main_hero->get_health () / main_hero->get_max_health ();

  if (fi > 0.5)
  {
    settextattr (10);
  }
  else
  {
    if (fi > 0.25)
    {
      settextattr (14);
    }
    else
    {
      settextattr (12);
    }
  }
  
  printw (mess [15], main_hero->get_health (), main_hero->get_max_health ());

  if (main_hero->broken_jaw)
  {
    settextattr (12);
    printw ("%s",mess [25]);
  }

  if (main_hero->broken_foot)
  {
    settextattr (12);
    printw ("%s",mess [26]);
  }

  printw ("%s","\n");

  // точность

  settextattr (15);

  if ((kick_count = main_hero->get_kick_count ()) == 1)
  {
    printw (mess [16], main_hero->get_accuracy_of_last_kick ());
  }
  else
  {
    printw (mess [17], kick_count - 1, kick_count, main_hero->get_accuracy_of_last_kick ());
  }

  // броня
  
  if (main_hero->get_armo () > 0)
  {
    settextattr (10);
    printw ("%s",mess [18]);

    flag = 0;

    for (i = 0; i < main_hero->inv_amount; i++)
    {
      if ((main_hero->inv [i].have) && (main_hero->inv [i].armo > 0))
      {
        if (flag == 0)
        {
          settextattr (9);
          printw ("%i", main_hero->get_armo ());
          flag = 1;
        }

        if (main_hero->inv [i].active == 1)
        {
          settextattr (9);
          printw (" %s(+%i)", main_hero->inv [i].name, main_hero->inv [i].armo);
        }
        else
        {
          settextattr (12);
          printw (" %s", main_hero->inv [i].name);
        }

        if (main_hero->inv [i].have > 1)
        {
          printw (mess [29], main_hero->inv [i].have);
        }
      }
    }

    if (flag == 0)
    {
      printw ("%i", main_hero->get_armo ());
    }

    printw ("%s","\n");
  }

  // пиво
  
  if (main_hero->get_beer () > 0)
  {
    settextattr (15);
    printw (mess [19], main_hero->get_beer ());
  }
  else
  {
    settextattr (12);
    printw ("%s",mess [20]);
  }

  // бабки

  if (main_hero->get_money () > 0)
  {
    settextattr (15);
    printw (mess [21], main_hero->get_money ());
  }
  else
  {
    settextattr (12);
    printw ("%s",mess [22]);
  }

  // косяки

  if (main_hero->get_ciga () > 0)
  {
    settextattr (15);
    printw (mess [23], main_hero->get_ciga ());
  }

  // хлам

  if (main_hero->get_stuff () > 0)
  {
    settextattr (15);
    printw (mess [24], main_hero->get_stuff ());
  }

  return 0;
}
