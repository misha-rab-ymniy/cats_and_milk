#include <SFML/Graphics.hpp>
#include <ctime>
#include <chrono>
#include <windows.h>
#include <vector>
#include <iostream>
#include "map.h"

using namespace sf;

struct bullet {
	int type_of_movement = 1;
	int x_length = 10;
	int y_length = 25;
	int x, y;
	int speedY, speedX, speed = 25;
	Sprite sbullet;
	bool is_draw;
	int team;
	/*type_of_movement:
	вверх - 1
	вниз - 2
	влево - 3
	вправо - 4*/
};

struct tanks {
	bool alive = true;
	int hp = 3;
	int orientation;
	int default_orientation;
	double x, y;
	double shoot_time = -100;
	double dead_time = 0;
	int raise_time = 2;
	double reload = 0.35;
	double speed = 5;
	int spawn_x, spawn_y;
	bool move_ability = true;
	Sprite stank;
	int team;
	int size = 90;
	bool immortality = false;
};
struct buff {
	double start = 100;
	double life_buff = 5;
	double boost_time = 2;
	int reload_degree = 5;
	int boost_degree = 2;
	Sprite sbuff;
};
double game_time = 100;

void check_colision(bullet& bull, int height, int width);
void check_shooting(bullet& bull, tanks& t);
void check_blocks(bullet& bull);
void death(tanks& tank);
void raise(tanks& tank);
void check_shooting(bullet& bull, std::vector<tanks>& tanks_arr);
void create_buff(int& buff_x, int& buff_y);
void buffs_choose(tanks& tank, int& buff_choose);

int main() {
	int window_height = 1080, window_width = 1920;
	int heart_width = 42;
	std::vector<bullet> bullets = {};
	//std::vector<buff_time> times = {};
	std::vector<buff> buffs = {};
	
	const int fps = 200;

	int block_size = 100;
	RenderWindow window(VideoMode(window_width, window_height), "tanks", Style::Fullscreen);
	window.setMouseCursorVisible(false);

	Image icon;
	icon.loadFromFile("resource\\tank_up.png");

	window.setIcon(90, 90, icon.getPixelsPtr());
	window.setMouseCursorVisible(false);
	Texture ttank_up, ttank_right, ttank_left, ttank_down, enemy_ttank_up, enemy_ttank_down, enemy_ttank_left, enemy_ttank_right;
	ttank_up.loadFromFile("resource\\tank_up.png");
	ttank_right.loadFromFile("resource\\tank_right.png");
	ttank_left.loadFromFile("resource\\tank_left.png");
	ttank_down.loadFromFile("resource\\tank_down.png");

	enemy_ttank_up.loadFromFile("resource\\enemy_tank_up.png");
	enemy_ttank_right.loadFromFile("resource\\enemy_tank_right.png"); 
	enemy_ttank_left.loadFromFile("resource\\enemy_tank_left.png");
	enemy_ttank_down.loadFromFile("resource\\enemy_tank_down.png");

	Texture tbullet_up, tbullet_right, tbullet_left, tbullet_down;
	tbullet_up.loadFromFile("resource\\bullet_up.png");
	tbullet_right.loadFromFile("resource\\bullet_right.png");
	tbullet_left.loadFromFile("resource\\bullet_left.png");
	tbullet_down.loadFromFile("resource\\bullet_down.png");

	Texture wood_block, wood_broken1, wood_broken2, dark_block;
	dark_block.loadFromFile("resource\\dark_block.png");
	wood_block.loadFromFile("resource\\wood_block.png");
	wood_broken1.loadFromFile("resource\\wood_broken1.png");
	wood_broken2.loadFromFile("resource\\wood_broken2.png");

	Texture buff_speed, buff_shild, buff_reload;
	buff_speed.loadFromFile("resource\\buff_speed.png");
	buff_shild.loadFromFile("resource\\buff_shild.png");
	buff_reload.loadFromFile("resource\\buff_reload.png");

	Sprite smap;
	Sprite hearth;
	Sprite enemy_hearth;

	Texture heart, enemy_heart;
	heart.loadFromFile("resource\\enemy_hearth.png");
	enemy_heart.loadFromFile("resource\\hearth.png");
	hearth.setTexture(heart);
	enemy_hearth.setTexture(enemy_heart);

	tanks tank, enemy_tank;
	tank.stank.setTexture(enemy_ttank_left);
	enemy_tank.stank.setTexture(ttank_right);


	/*orientation:
	вверх - 1
	вниз - 2
	влево - 3
	вправо - 4*/
	tank.orientation = 3;
	tank.default_orientation = 3;
	enemy_tank.orientation = 4;
	enemy_tank.default_orientation = 4;
	enemy_tank.team = 1;
	enemy_tank.x = enemy_tank.spawn_x = 0, enemy_tank.y = enemy_tank.spawn_y = 500, tank.x = tank.spawn_x = 1820, tank.y = tank.spawn_y = 500;
	enemy_tank.stank.setPosition(enemy_tank.x, enemy_tank.y);
	tank.stank.setPosition(tank.x, enemy_tank.y);
	tank.team = 2;


	buff buff1, buff2, buff3;
	buff1.sbuff.setTexture(buff_speed);
	buff2.sbuff.setTexture(buff_shild);
	buff3.sbuff.setTexture(buff_reload);
	buffs.push_back(buff1);
	buffs.push_back(buff2);
	buffs.push_back(buff3);

	srand(time(NULL));
	int buff_y = 0, buff_x = 0;
	int buff_choose = 0;

	while (window.isOpen()) {
		//проверка на закрытие
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();
		}
		//tank2 movement

		if (!tank.alive) {
			if (game_time - tank.dead_time > tank.raise_time) {
				tank.hp = 3;
				tank.alive = true;
				tank.x = tank.spawn_x;
				tank.y = tank.spawn_y;
				switch (tank.orientation) {
				case 1:
					tank.stank.setTexture(enemy_ttank_up);
					break;
				case 2:
					tank.stank.setTexture(enemy_ttank_down);
					break;
				case 3:
					tank.stank.setTexture(enemy_ttank_left);
					break;
				case 4:
					tank.stank.setTexture(enemy_ttank_right);
				}
				tank.stank.setPosition(tank.x, tank.y);
				window.draw(tank.stank);
			}
		}
		if (!enemy_tank.alive) {
			if (game_time - enemy_tank.dead_time > enemy_tank.raise_time) {
				enemy_tank.hp = 3;
				enemy_tank.alive = true;
				enemy_tank.x = enemy_tank.spawn_x;
				enemy_tank.y = enemy_tank.spawn_y;
				switch (enemy_tank.orientation) {
				case 1:
					enemy_tank.stank.setTexture(ttank_up);
					break;
				case 2:
					enemy_tank.stank.setTexture(ttank_down);
					break;
				case 3:
					enemy_tank.stank.setTexture(ttank_left);
					break;
				case 4:
					enemy_tank.stank.setTexture(ttank_right);
				}
				enemy_tank.stank.setPosition(enemy_tank.x, enemy_tank.y);
				window.draw(enemy_tank.stank);
			}
		}


		if (game_time - buff1.start > buff1.life_buff) {
			buff_choose = rand() % 3;
			create_buff(buff_x, buff_y);
			buff1.start = game_time;
		}
		if (TileMap[buff_x][buff_y] == 'l' && game_time - buff1.start > buff2.life_buff / buff2.boost_time) {
			TileMap[buff_x][buff_y] = ' ';
		}
		if (!tank.alive || (tank.immortality = true || tank.speed == 10 || tank. reload == 0.1) && game_time - buff3.start > buff3.life_buff / buff3.boost_time) {
			tank.speed = 5;
			tank.reload = 0.35;
			tank.immortality = false;
		}
		if (!enemy_tank.alive || (enemy_tank.immortality || enemy_tank.speed == 10 || enemy_tank.reload == 0.1) && game_time - buff3.start > buff3.life_buff / buff3.boost_time) {
			enemy_tank.speed = 5;
			enemy_tank.reload = 0.35;
			enemy_tank.immortality = false;
		}


		if (tank.alive) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				tank.orientation = 3;
				tank.stank.setTexture(enemy_ttank_left);

				if (tank.x > 0 && (tank.x > enemy_tank.x + tank.size + tank.speed || tank.x + tank.size < enemy_tank.x || tank.y + tank.size < enemy_tank.y || tank.y > enemy_tank.y + tank.size)) {
					
					for (int i = 0; i < HEIGHT_MAP; i++)
						for (int j = 0; j < WIDTH_MAP; j++) {
							if (TileMap[i][j] == ' ') {
								continue;
							}
							int block_x = j * block_size, block_y = i * block_size;
							if (!(tank.x > block_x + block_size + tank.speed || tank.x + tank.size < block_x || tank.y + tank.size < block_y || tank.y > block_y + block_size)) {
								if (TileMap[i][j] == 'l') {
									TileMap[i][j] = ' ';
									buffs_choose(tank, buff_choose);
									buff3.start = game_time;
									continue;
								}
								if (!(tank.x > block_x + block_size + tank.speed)) {
									tank.x = block_x + block_size + 1;
								}
								tank.move_ability = false;
							}
						}
					if (tank.move_ability) {
						tank.x -= tank.speed;
					}
					tank.stank.setPosition(tank.x, tank.y);
					tank.move_ability = true;
				}


			}

			else if (Keyboard::isKeyPressed(Keyboard::Right)) {
				tank.orientation = 4;
				tank.stank.setTexture(enemy_ttank_right);

				if (window_width - tank.size > tank.x && (enemy_tank.x + tank.size < tank.x || tank.x + tank.size + tank.speed < enemy_tank.x || tank.y + tank.size < enemy_tank.y || tank.y > enemy_tank.y + tank.size)) {
					for (int i = 0; i < HEIGHT_MAP; i++)
						for (int j = 0; j < WIDTH_MAP; j++) {
							if (TileMap[i][j] == ' ') {
								continue;
							}
							int block_x = j * 100, block_y = i * 100;
							if (!(block_x + block_size < tank.x || tank.x + tank.size + tank.speed < block_x || tank.y + tank.size < block_y || tank.y > block_y + block_size)) {
								if (TileMap[i][j] == 'l') {
									TileMap[i][j] = ' ';
									buffs_choose(tank, buff_choose);
									buff3.start = game_time;
									continue;
								}
								if (!(tank.x + tank.size + tank.speed < block_x)) {
									tank.x = block_x - tank.size - 1;
								}
								tank.move_ability = false;
							}
						}
					if (tank.move_ability) {
						tank.x += tank.speed;
					}
					tank.stank.setPosition(tank.x, tank.y);
					tank.move_ability = true;
				}
			}

			else if (Keyboard::isKeyPressed(Keyboard::Up)) {
				tank.orientation = 1;
				tank.stank.setTexture(enemy_ttank_up);
				if (tank.y > 0 && (tank.y + tank.size < enemy_tank.y || tank.x > enemy_tank.x + tank.size || tank.x + tank.size < enemy_tank.x || tank.y > enemy_tank.y + tank.size + tank.speed)) {

					for (int i = 0; i < HEIGHT_MAP; i++)
						for (int j = 0; j < WIDTH_MAP; j++) {
							if (TileMap[i][j] == ' ') {
								continue;
							}
							int block_x = j * 100, block_y = i * 100;
							if (!(tank.y + tank.size < block_y || tank.x > block_x + block_size || tank.x + tank.size < block_x || tank.y > block_y + block_size + tank.speed)) {
								if (TileMap[i][j] == 'l') {
									TileMap[i][j] = ' ';
									buffs_choose(tank, buff_choose);
									buff3.start = game_time;
									continue;
								}
								if (!(tank.y > block_y + block_size + tank.speed)) {
									tank.y = block_y + block_size + 1;
								}
								tank.move_ability = false;
							}
						}
					if (tank.move_ability) {
						tank.y -= tank.speed;
					}
					tank.stank.setPosition(tank.x, tank.y);
					tank.move_ability = true;
				}
			}

			else if (Keyboard::isKeyPressed(Keyboard::Down)) {
				tank.orientation = 2;
				tank.stank.setTexture(enemy_ttank_down);
				if (window_height - tank.size > tank.y && (tank.y + tank.size + tank.speed < enemy_tank.y || tank.y > enemy_tank.y + tank.size || tank.x > enemy_tank.x + tank.size || tank.x + tank.size < enemy_tank.x)) {
					for (int i = 0; i < HEIGHT_MAP; i++)
						for (int j = 0; j < WIDTH_MAP; j++) {
							if (TileMap[i][j] == ' ') {
								continue;
							}
							int block_x = j * 100, block_y = i * 100;
							if (!(tank.y + tank.size + tank.speed < block_y || tank.y > block_y + block_size || tank.x > block_x + block_size || tank.x + tank.size < block_x)) {
								if (TileMap[i][j] == 'l') {
									TileMap[i][j] = ' ';
									buffs_choose(tank, buff_choose);
									buff3.start = game_time;
									continue;
								}
								if (!(tank.y + tank.size + tank.speed < block_y)) {
									tank.y = block_y - tank.size - 1;
								}
								tank.move_ability = false;
							}
						}
					if (tank.move_ability) {
						tank.y += tank.speed;
					}
					tank.stank.setPosition(tank.x, tank.y);
					tank.move_ability = true;
				}
			}
		}
		//tank1 movement
		if (enemy_tank.alive) {
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				enemy_tank.orientation = 3;
				enemy_tank.stank.setTexture(ttank_left);
				if (enemy_tank.x > 0 && (enemy_tank.x > tank.x + enemy_tank.size + enemy_tank.speed || enemy_tank.x + enemy_tank.size < tank.x || enemy_tank.y + enemy_tank.size < tank.y || enemy_tank.y > tank.y + enemy_tank.size)) {
					for (int i = 0; i < HEIGHT_MAP; i++)
						for (int j = 0; j < WIDTH_MAP; j++) {
							if (TileMap[i][j] == ' ') {
								continue;
							}
							int block_x = j * block_size, block_y = i * block_size;
							if (!(enemy_tank.x > block_x + block_size + enemy_tank.speed || enemy_tank.x + enemy_tank.size < block_x || enemy_tank.y + enemy_tank.size < block_y || enemy_tank.y > block_y + block_size)) {
								if (TileMap[i][j] == 'l') {
									TileMap[i][j] = ' ';
									buffs_choose(enemy_tank, buff_choose);
									buff3.start = game_time;
									continue;
								}
								if (!(enemy_tank.x > block_x + block_size + enemy_tank.speed)) {
									enemy_tank.x = block_x + block_size + 1;
								}
								enemy_tank.move_ability = false;
							}
						}
					if (enemy_tank.move_ability) {
						enemy_tank.x -= enemy_tank.speed;
					}
					enemy_tank.stank.setPosition(enemy_tank.x, enemy_tank.y);
					enemy_tank.move_ability = true;
				}
			}

			else if (Keyboard::isKeyPressed(Keyboard::D)) {
				enemy_tank.orientation = 4;
				enemy_tank.stank.setTexture(ttank_right);

				if (window_width - enemy_tank.size > enemy_tank.x && (tank.x + enemy_tank.size < enemy_tank.x || enemy_tank.x + enemy_tank.size + enemy_tank.speed < tank.x || enemy_tank.y + enemy_tank.size < tank.y || enemy_tank.y > tank.y + enemy_tank.size)) {
					for (int i = 0; i < HEIGHT_MAP; i++)
						for (int j = 0; j < WIDTH_MAP; j++) {
							if (TileMap[i][j] == ' ') {
								continue;
							}
							int block_x = j * 100, block_y = i * 100;
							if (!(block_x + block_size < enemy_tank.x || enemy_tank.x + enemy_tank.size + enemy_tank.speed < block_x || enemy_tank.y + enemy_tank.size < block_y || enemy_tank.y > block_y + block_size)) {
								if (TileMap[i][j] == 'l') {
									TileMap[i][j] = ' ';
									buffs_choose(enemy_tank, buff_choose);
									buff3.start = game_time;
									continue;
								}
								if (!(enemy_tank.x + enemy_tank.size + enemy_tank.speed < block_x)) {
									enemy_tank.x = block_x - enemy_tank.size - 1;
								}
								enemy_tank.move_ability = false;
							}
						}
					if (enemy_tank.move_ability) {
						enemy_tank.x += enemy_tank.speed;
					}
					enemy_tank.stank.setPosition(enemy_tank.x, enemy_tank.y);
					enemy_tank.move_ability = true;
				}
			}

			else if (Keyboard::isKeyPressed(Keyboard::W)) {
				enemy_tank.orientation = 1;
				enemy_tank.stank.setTexture(ttank_up);
				if (enemy_tank.y > 0 && (enemy_tank.y + enemy_tank.size < tank.y || enemy_tank.x > tank.x + enemy_tank.size || enemy_tank.x + enemy_tank.size < tank.x || enemy_tank.y > tank.y + enemy_tank.size + enemy_tank.speed)) {

					for (int i = 0; i < HEIGHT_MAP; i++)
						for (int j = 0; j < WIDTH_MAP; j++) {
							if (TileMap[i][j] == ' ') {
								continue;
							}
							int block_x = j * 100, block_y = i * 100;
							if (!(enemy_tank.y + enemy_tank.size < block_y || enemy_tank.x > block_x + block_size || enemy_tank.x + enemy_tank.size < block_x || enemy_tank.y > block_y + block_size + enemy_tank.speed)) {
								if (TileMap[i][j] == 'l') {
									TileMap[i][j] = ' ';
									buffs_choose(enemy_tank, buff_choose);
									buff3.start = game_time;
									continue;
								}
								if (!(tank.y > block_y + block_size + tank.speed)) {
									enemy_tank.y = block_y + block_size + 1;
								}
								enemy_tank.move_ability = false;
							}
						}
					if (enemy_tank.move_ability) {
						enemy_tank.y -= enemy_tank.speed;
					}
					enemy_tank.stank.setPosition(enemy_tank.x, enemy_tank.y);
					enemy_tank.move_ability = true;
				}
			}

			else if (Keyboard::isKeyPressed(Keyboard::S)) {
				enemy_tank.orientation = 2;
				enemy_tank.stank.setTexture(ttank_down);
				if (window_height - enemy_tank.size > enemy_tank.y && (enemy_tank.y + enemy_tank.size + enemy_tank.speed < tank.y || enemy_tank.y > tank.y + enemy_tank.size || enemy_tank.x > tank.x + enemy_tank.size || enemy_tank.x + enemy_tank.size < tank.x)) {
					for (int i = 0; i < HEIGHT_MAP; i++)
						for (int j = 0; j < WIDTH_MAP; j++) {
							if (TileMap[i][j] == ' ') {
								continue;
							}
							int block_x = j * 100, block_y = i * 100;
							if (!(enemy_tank.y + enemy_tank.size + enemy_tank.speed < block_y || enemy_tank.y > block_y + block_size || enemy_tank.x > block_x + block_size || enemy_tank.x + enemy_tank.size < block_x)) {
								if (TileMap[i][j] == 'l') {
									TileMap[i][j] = ' ';
									buffs_choose(enemy_tank, buff_choose);
									buff3.start = game_time;
									continue;
								}
								if (!(enemy_tank.y + enemy_tank.size + enemy_tank.speed < block_y)) {
									enemy_tank.y = block_y - tank.size - 1;
								}
								enemy_tank.move_ability = false;
							}
						}
					if (enemy_tank.move_ability) {
						enemy_tank.y += enemy_tank.speed;
					}
					enemy_tank.stank.setPosition(enemy_tank.x, enemy_tank.y);
					enemy_tank.move_ability = true;
				}
			}
		}
		//стрельба второго
		if (tank.alive) {
			if (Keyboard::isKeyPressed(Keyboard::RControl) && game_time - tank.shoot_time > tank.reload) {
				bullet bull;
				bull.type_of_movement = tank.orientation;
				bull.team = tank.team;
				switch (bull.type_of_movement) {
				case 1:
					bull.x = tank.x + 43;
					bull.y = tank.y;
					bull.speedX = 0;
					bull.speedY = -bull.speed;
					bull.sbullet.setTexture(tbullet_up);
					bull.x_length = 10;
					bull.y_length = 25;
					break;
				case 2:
					bull.x = tank.x + 43;
					bull.y = tank.y + 75;
					bull.speedX = 0;
					bull.speedY = bull.speed;
					bull.sbullet.setTexture(tbullet_down);
					bull.x_length = 10;
					bull.y_length = 25;
					break;
				case 3:
					bull.x = tank.x + 25;
					bull.y = tank.y + 45;
					bull.speedX = -bull.speed;
					bull.speedY = 0;
					bull.sbullet.setTexture(tbullet_left);
					bull.x_length = 25;
					bull.y_length = 10;
					break;
				case 4:
					bull.x = tank.x + 50;
					bull.y = tank.y + 45;
					bull.speedX = bull.speed;
					bull.speedY = 0;
					bull.sbullet.setTexture(tbullet_right);
					bull.x_length = 25;
					bull.y_length = 10;
					break;
				}
				bull.is_draw = true;
				bullets.push_back(bull);
				tank.shoot_time = game_time;
			}
		}
		//стрельба первого
		if (enemy_tank.alive) {
			if (Keyboard::isKeyPressed(Keyboard::Space) && game_time - enemy_tank.shoot_time > enemy_tank.reload) {
				bullet bull1;
				bull1.type_of_movement = enemy_tank.orientation;
				bull1.team = enemy_tank.team;
				switch (bull1.type_of_movement) {
				case 1:
					bull1.x = enemy_tank.x + 45;
					bull1.y = enemy_tank.y;
					bull1.speedX = 0;
					bull1.speedY = -bull1.speed;
					bull1.sbullet.setTexture(tbullet_up);
					break;
				case 2:
					bull1.x = enemy_tank.x + 45;
					bull1.y = enemy_tank.y + 75;
					bull1.speedX = 0;
					bull1.speedY = bull1.speed;
					bull1.sbullet.setTexture(tbullet_down);
					break;
				case 3:
					bull1.x = enemy_tank.x + 25;
					bull1.y = enemy_tank.y + 45;
					bull1.speedX = -bull1.speed;
					bull1.speedY = 0;
					bull1.sbullet.setTexture(tbullet_left);
					break;
				case 4:
					bull1.x = enemy_tank.x + 50;
					bull1.y = enemy_tank.y + 45;
					bull1.speedX = bull1.speed;
					bull1.speedY = 0;
					bull1.sbullet.setTexture(tbullet_right);
					break;
				}
				bullets.push_back(bull1);
				enemy_tank.shoot_time = game_time;

			}
		}

		//перерисовка окна(вынести в функцию)
		window.clear();

		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++) {
				switch (TileMap[i][j]) {
				case ' ':
					smap.setTexture(dark_block);
					break;
				case 'l':
					switch (buff_choose) {
					case 0:
						smap.setTexture(buff_speed);
						break;
					case 1:
						smap.setTexture(buff_shild);
						break;
					case 2:
						smap.setTexture(buff_reload);
						break;
					}
					break;
				case 's':
					smap.setTexture(wood_block);
					break;
				case 'r':
					smap.setTexture(wood_broken1);
					break;
				case 'q':
					smap.setTexture(wood_broken2);
					break;
				}

				smap.setPosition(j * block_size, i * block_size);
				window.draw(smap);
			}
		for (bullet& bull : bullets) {
			if (bull.is_draw) {
				bull.x += bull.speedX;
				bull.y += bull.speedY;
				bull.sbullet.setPosition(bull.x, bull.y);
				window.draw(bull.sbullet);

				check_colision(bull, window_height, window_width);
				check_shooting(bull, tank);
				check_shooting(bull, enemy_tank);
				check_blocks(bull);
			}
		}
		window.draw(tank.stank);
		window.draw(enemy_tank.stank);
		for (int i = 0; i < enemy_tank.hp; i++) {
			enemy_hearth.setPosition(i * heart_width, 0);
			window.draw(enemy_hearth);
		}
		for (int i = 0; i < tank.hp; i++) {
			hearth.setPosition(window_width - (i + 1) * heart_width, 0);
			window.draw(hearth);
		}

		window.display();
		//установка частоты обновления
		Sleep(1000 / fps);
		game_time += 1.0 / fps;
	}

	return 0;
}

void check_colision(bullet& bull, int height, int width) {
	if (bull.x > width || bull.x * bull.y < 0 || bull.y > height) {
		bull.is_draw = false;
	}
}

void death(tanks& tank) {
	tank.alive = false;
	tank.dead_time = game_time;
	tank.x = 1920;
	tank.y = 0;
	tank.stank.setPosition(tank.x, tank.y);
	tank.orientation = tank.default_orientation;
}

void check_shooting(bullet& bull, tanks& t) {//ccylka
	if (!t.alive)
		return;
	bool a_inside, b_inside, c_inside, d_inside;
	if (bull.team != t.team && bull.is_draw) {
		a_inside = bull.x > t.x && bull.x < t.x + t.size && bull.y > t.y && bull.y < t.y + t.size;
		b_inside = bull.x + bull.x_length > t.x && bull.x + bull.x_length< t.x + t.size && bull.y > t.y && bull.y < t.y + t.size;
		c_inside = bull.x + bull.x_length > t.x && bull.x + bull.x_length< t.x + t.size && bull.y + bull.y_length > t.y && bull.y + bull.y_length < t.y + t.size;
		d_inside = bull.x > t.x && bull.x < t.x + t.size && bull.y + bull.y_length > t.y && bull.y + bull.y_length < t.y + t.size;
		if (a_inside || b_inside || c_inside || d_inside) {
			if(!t.immortality)
				t.hp--;
			if (t.hp <= 0) {
				death(t);
			}
			bull.is_draw = false;
		}
	}
}

/*type_of_movement:
	вверх - 1
	вниз - 2
	влево - 3
	вправо - 4*/

void check_blocks(bullet& bull) {
	for (int i = 0; i < HEIGHT_MAP; i++) {
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == ' ' || TileMap[i][j] == 'l')
				continue;
			int block_x = j * 100;
			int block_y = i * 100;
			switch (bull.type_of_movement) {
			case 1:
				if (bull.x + bull.x_length > block_x && bull.x < block_x + 100 && bull.y <= block_y + 100 && bull.y + bull.y_length > block_y) {
					bull.is_draw = false;

					switch (TileMap[i][j]) {
					case 's':
						TileMap[i][j] = 'r';
						break;
					case 'r':
						TileMap[i][j] = 'q';
						break;
					case 'q':
						TileMap[i][j] = ' ';
						break;
					}
				}
				break;
			case 2:
				if (bull.x + bull.x_length > block_x && bull.x < block_x + 100 && bull.y + bull.y_length >= block_y && bull.y + bull.y_length < block_y + 100) {
					bull.is_draw = false;

					switch (TileMap[i][j]) {
					case 's':
						TileMap[i][j] = 'r';
						break;
					case 'r':
						TileMap[i][j] = 'q';
						break;
					case 'q':
						TileMap[i][j] = ' ';
						break;
					}
				}
				break;
			case 3:
				if (bull.y + bull.y_length > block_y && bull.y < block_y + 100 && bull.x <= block_x + 100 && bull.x + bull.x_length > block_x) {
					bull.is_draw = false;

					switch (TileMap[i][j]) {
					case 's':
						TileMap[i][j] = 'r';
						break;
					case 'r':
						TileMap[i][j] = 'q';
						break;
					case 'q':
						TileMap[i][j] = ' ';
						break;
					}
				}
				break;
			case 4:
				if (bull.y + bull.y_length > block_y && bull.y < block_y + 100 && bull.x + bull.x_length >= block_x && bull.x + bull.x_length < block_x + 100) {
					bull.is_draw = false;

					switch (TileMap[i][j]) {
					case 's':
						TileMap[i][j] = 'r';
						break;
					case 'r':
						TileMap[i][j] = 'q';
						break;
					case 'q':
						TileMap[i][j] = ' ';
						break;
					}
				}
				break;
			}

		}
	}
}

void create_buff(int& buff_x, int& buff_y) {
	bool void_place = true;
	while (void_place) {
		void_place = false;
		buff_x = rand() % 11;
		buff_y = rand() % 19;
		for (int i = 0; i < HEIGHT_MAP; i++) {
			for (int j = 0; j < WIDTH_MAP; j++) {
				if (TileMap[buff_x][buff_y] == 's' || TileMap[buff_x][buff_y] == 'r' || TileMap[buff_x][buff_y] == 'q') {
					void_place = true;
					break;
				}
			}
			if (TileMap[buff_x][buff_y] == 's' || TileMap[buff_x][buff_y] == 'r' || TileMap[buff_x][buff_y] == 'q') {
				break;
			}
		}
	}
	TileMap[buff_x][buff_y] = 'l';
}
void buffs_choose(tanks& tank, int& buff_choose) {
	switch (buff_choose) {
	case 0:
		tank.speed *= 2;
		break;
	case 1:
		tank.immortality = true;
		break;
	case 2:
		tank.reload = 0.1;
		break;
	}
}