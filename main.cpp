#include <bits/stdc++.h>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <ctime>
 
using namespace std;
 
const int size_of_field = 19, start_size = 4, speed = 900, goal_points = 10;
//размер поля, изначальная длина змеи, скорость (от 1 до 1000)
 
const char empty_cell = '.', apple_character = 'A', head_character = '@', obstacle_character = 'H';
//отображение пустой клетки, яблока, головы змеи, препядствия
 
const char out_direction[5] = {'^', '>', 'v', '<', empty_cell};
//отображение тела змеи
 
int lenght_of_slizer, points = 0, last_plus = 0, speed_c;
char character_direction;
//длина змеи, количества очков, последнее увеличение очков
 
//перевод направления змеи из char в int
int conformity(char character) {
    map <char, int> out;
    out['w'] = out['W'] = 0;
    out['d'] = out['D'] = 1;
    out['s'] = out['S'] = 2;
    out['a'] = out['A'] = 3;
 
    return out[character];
}
 
//Заводим структуры яблока, бонуса, объекта и тела змеи
struct segment {
    int x, y, way;
};
 
struct apple {
    int x, y;
};
 
struct bonus {
    int x, y, inc;
};
 
struct obstacle {
    int x, y;
};
 
//здесь храним сегменты тела и препятствия
vector <segment> body;
vector <obstacle> obstacles;
apple my_apple;
bonus my_bonus;
 
 
//функция возвращает заполнение клетки по её координатам
char filling(int Y, int X) {
    if (my_apple.x == X && my_apple.y == Y) {
        return apple_character;
    }
 
    if (my_bonus.x == X && my_bonus.y == Y) {
        return (my_bonus.inc + '0');
    }
 
    if (body[0].x == X && body[0].y == Y) {
        return head_character;
    }
 
    for (int i = 1; i < lenght_of_slizer; ++i) {
        if (body[i].x == X && body[i].y == Y) {
            return out_direction[body[i].way];
        }
    }
 
    for (int i = 0; i < (int)obstacles.size(); ++i) {
        if (obstacles[i].x == X && obstacles[i].y == Y) {
            return obstacle_character;
        }
    }
 
    //возвращает пустую клетку, если ничего нет
    return out_direction[4];
}
 
//выбирает рандомную пустую клетку и кладёт туда бонус
void new_bonus() {
    my_bonus.x = -1; my_bonus.y = -1;
    srand(time(0));
    int free_cells = size_of_field * size_of_field - lenght_of_slizer - 1 - (int)obstacles.size();
    int random_number = rand() % free_cells;
 
    for (int i = 0; i < size_of_field; ++i) {
        for (int j = 0; j < size_of_field; ++j) {
            if (filling(i, j) == empty_cell) {
                random_number--;
                if (random_number < 0) {
                    my_bonus.x = j; my_bonus.y = i;
                    my_bonus.inc = 9;
                    break;
                }
            }
        }
        if (my_bonus.x != -1) break;
    }
}
 
 
//выбирает рандомную пустую клетку и кладёт туда яблоко
void new_apple() {
    my_apple.x = -1; my_apple.y = -1;
    srand(time(0));
    int free_cells = size_of_field * size_of_field - lenght_of_slizer - (my_bonus.inc > 0) - (int)obstacles.size();
    int random_number = rand() % free_cells;
 
    for (int i = 0; i < size_of_field; ++i) {
        for (int j = 0; j < size_of_field; ++j) {
            if (filling(i, j) == empty_cell) {
                random_number--;
                if (random_number < 0) {
                    my_apple.x = j; my_apple.y = i;
                    break;
                }
            }
        }
        if (my_apple.x != -1) break;
    }
}
 
//если is_y = 1, то возврашает на сколько должна меняться
//координата y при направлении int_way, иначе на сколько меняется
//коорданата x при направлении int_way
int identify_way(int int_way, bool is_y) {
    int out[4][2] = {0, -1, 1, 0, 0, 1, -1, 0};
    return out[int_way][is_y];
}
 
//проверяет является ли клетка препятствием
//то есть совпадает ли нынешняя клетка головы с туловищем или препятствием
bool is_obstacle(int Y, int X) {
    for (int i = 1; i < lenght_of_slizer; ++i) {
        if (X == body[i].x && Y == body[i].y) {
            return 1;
        }
    }
 
    for (int i = 0; i < (int)obstacles.size(); ++i) {
        if (X == obstacles[i].x && Y == obstacles[i].y) {
            return 1;
        }
    }
 
    return 0;
}
 
//Заканчивает игру
//is_win = 1, если игрок достиг цели, иначе is_win = 0
 
void final_screen(bool is_win) {
    system("cls");
 
    exit(0);
}
 
//функция двигает змею на одну клетку в направлении direction
void move_slizer(int direction) {
 
    vector <segment> was_directions = body;
    for (int i = 1; i < lenght_of_slizer; ++i) {
        body[i] = was_directions[i - 1];
    }
 
    body[0].way = direction;
 
    body[0].x += identify_way(body[0].way, 0);
    body[0].y += identify_way(body[0].way, 1);
 
    if(body[0].x < 0) body[0].x = size_of_field - 1;
    if(body[0].x == size_of_field) body[0].x = 0;
 
    if(body[0].y < 0) body[0].y = size_of_field - 1;
    if(body[0].y == size_of_field) body[0].y = 0;
 
    if (is_obstacle(body[0].y, body[0].x)) {
        final_screen(0);
    }
 
    if (body[0].x == my_apple.x && body[0].y == my_apple.y) {
        points++;
        last_plus = 1;
 
        body.push_back(body[lenght_of_slizer - 1]);
        lenght_of_slizer++;
        new_apple();
    }
 
    if (body[0].x == my_bonus.x && body[0].y == my_bonus.y) {
        points += my_bonus.inc;
        last_plus = my_bonus.inc;
 
        my_bonus.x = -1;
        my_bonus.y = -1;
        my_bonus.inc = 0;
    }
}
 
//выводит количество очков и последнее прибавление
void out_points() {
    cout << "     " << points << "  ";
 
    if (last_plus > 0) {
        cout << "+" << last_plus;
    }
 
}
 
//создаёт прямоугольник полностью заполненый препятствиями
//с координатами крайних углов xl, yl и xr, yr
 
void make_obstacle(int xl, int yl, int xr, int yr) {
    if (yl > yr) {
        swap(yl, yr);
    }
 
    if (xl > xr) {
        swap(xl, xr);
    }
 
    for (int i = yl; i <= yr; ++i) {
        for (int j = xl; j <= xr; ++j) {
            obstacle in;
            in.x = j; in.y = i;
 
            obstacles.push_back(in);
        }
    }
}
 
//устанавливает начальные значения игры
void do_start_conditions() {
    speed_c = max(1000 - speed, 0);
    character_direction = 'w';
 
    segment uhead;
    uhead.x = size_of_field / 2;
    uhead.y = size_of_field / 2;
    uhead.way = 0;
 
    my_bonus.x = -1;
    my_bonus.y = -1;
 
    lenght_of_slizer = start_size;
    for (int i = 0; i < start_size; ++i) {
        body.push_back(uhead);
    }
 
    new_apple();
}
 
//функция создания препятствий
void do_obstacles() {
    make_obstacle(0, 0, 0, 18);
    make_obstacle(0, 0, 18, 0);
    make_obstacle(18, 18, 0, 18);
    make_obstacle(18, 18, 18, 0);
 
}
 
//основная функция
void go_slizer() {
 
    do_obstacles();
    do_start_conditions();
 
    while (1) {
        system("cls");
 
        if (my_bonus.inc < 1) {
            my_bonus.x = -1;
            my_bonus.y = -1;
        }
 
        for (int i = 0; i < size_of_field; ++i) {
            for (int j = 0; j < size_of_field; ++j) {
                cout << filling(i, j);
            }
 
            if (i == 0) {
                out_points();
            }
 
            cout << endl;
        }
        last_plus = 0;
 
        my_bonus.inc--;
        int direction;
 
        if (_kbhit()) {//проверяет введено ли число
            character_direction = getch();//кладёт введённый символ в character_direction
        }
        Sleep(speed_c);
 
        direction = conformity(character_direction);
 
        move_slizer(direction);
 
        //бонус появляется с шансом 0.2 на каждом сдвиге змеи
        if (my_bonus.inc < 1) {
            srand(time(0));
            int random_number = rand() % 5;
            if (random_number == 0) {
                new_bonus();
            }
        }
 
        //условия победы
        if (points >= goal_points && lenght_of_slizer >= 15) {
            final_screen(1);
        }
    }
}
 
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    go_slizer();
}
