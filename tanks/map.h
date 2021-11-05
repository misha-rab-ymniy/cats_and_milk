#pragma once
#include <SFML\Graphics.hpp>
#include <string>

const int HEIGHT_MAP = 11;
const int WIDTH_MAP = 19;

sf::String TieMap[HEIGHT_MAP] = {
	"                   ",
	"     sss    s      ",
	"     s             ",
	"    s     s   s    ",
	"    s    sssssss   ",
	"     sss s     s   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
};
sf::String TileMap[HEIGHT_MAP] = {
	"   ssss   sssss    ",
	"       s  s        ",
	"        ss         ",
	"    sss    sss     ",
	"                   ",
	"    ssss  ssss     ",
	"    s  s  s  s     ",
	"    ssss  ssss     ",
	"                   ",
	"      ssssss       ",
	"                   ",
};