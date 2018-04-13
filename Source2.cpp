// задача нахождения пути от коня до пешки
#include <fstream>
#include <stack>
#include <tuple>
#include <map>
#include <vector>
#include <string>
//#include <iostream>

bool* fieldPtr;

class Coord
{
public:
	bool operator== (Coord e) { return (this->x == e.x) && (this->y == e.y); }
	bool operator!= (Coord e) { return !operator==(e); }

	Coord();
	Coord(int x, int y);
	int x;
	int y;
};

Coord::Coord(int x, int y)
{
	this->x = x;
	this->y = y;
}

Coord::Coord() {}

Coord pawn;

std::string CoordToStr(Coord& obj)
{
	int x = obj.x + 48 + 49;
	int y = obj.y + 49;
	char ch[2]{ x, y };
	return std::string(ch, 2);
}

bool inMatrix(int x, int y)
{
	return (0 <= x) && (x < 8) && (0 <= y) && (y < 8);
}

bool isValidPosition(int x, int y)
{
	return inMatrix(x, y) && !*(fieldPtr + 8 * x + y);
}

// функции, перемещающие коня, если это возможно, и сообщающие об этом true / false
bool stepUpLeft(Coord& horse, Coord& oldHorse)
{
	oldHorse = horse;
	if (isValidPosition(horse.x - 1, horse.y + 2))
	{
		horse.x -= 1;
		horse.y += 2;
		*(fieldPtr + 8 * horse.x + horse.y) = true;
		return true;
	}
	return false;
}
bool stepUpRight(Coord& horse, Coord& oldHorse)
{
	oldHorse = horse;
	if (isValidPosition(horse.x + 1, horse.y + 2))
	{
		horse.x += 1;
		horse.y += 2;
		*(fieldPtr + 8 * horse.x + horse.y) = true;
		return true;
	}
	return false;
}
bool stepRightUp(Coord& horse, Coord& oldHorse)
{
	oldHorse = horse;
	if (isValidPosition(horse.x + 2, horse.y + 1))
	{
		horse.x += 2;
		horse.y += 1;
		*(fieldPtr + 8 * horse.x + horse.y) = true;
		return true;
	}
	return false;
}
bool stepRightDown(Coord& horse, Coord& oldHorse)
{
	oldHorse = horse;
	if (isValidPosition(horse.x + 2, horse.y - 1))
	{
		horse.x += 2;
		horse.y -= 1;
		*(fieldPtr + 8 * horse.x + horse.y) = true;
		return true;
	}
	return false;
}
bool stepDownLeft(Coord& horse, Coord& oldHorse)
{
	oldHorse = horse;
	if (isValidPosition(horse.x - 1, horse.y - 2))
	{
		horse.x -= 1;
		horse.y -= 2;
		*(fieldPtr + 8 * horse.x + horse.y) = true;
		return true;
	}
	return false;
}
bool stepDownRight(Coord& horse, Coord& oldHorse)
{
	oldHorse = horse;
	if (isValidPosition(horse.x + 1, horse.y - 2))
	{
		horse.x += 1;
		horse.y -= 2;
		*(fieldPtr + 8 * horse.x + horse.y) = true;
		return true;
	}
	return false;
}
bool stepLeftUp(Coord& horse, Coord& oldHorse)
{
	oldHorse = horse;
	if (isValidPosition(horse.x - 2, horse.y + 1))
	{
		horse.x -= 2;
		horse.y += 1;
		*(fieldPtr + 8 * horse.x + horse.y) = true;
		return true;
	}
	return false;
}
bool stepLeftDown(Coord& horse, Coord& oldHorse)
{
	oldHorse = horse;
	if (isValidPosition(horse.x - 2, horse.y - 1))
	{
		horse.x -= 2;
		horse.y -= 1;
		*(fieldPtr + 8 * horse.x + horse.y) = true;
		return true;
	}
	return false;
}

bool(*stepFunctions[])(Coord& horse, Coord& oldHorse) {
	stepUpLeft,
		stepUpRight,
		stepRightUp,
		stepRightDown,
		stepDownRight,
		stepDownLeft,
		stepLeftDown,
		stepLeftUp
};

int main()
{
	std::map<std::string, std::string> way;

	std::ifstream fin("in.txt");

	char* buffer = new char[10];

	fin.getline(buffer, 10);
	Coord horse = Coord(buffer[0] - 48 - 49, buffer[1] - 49);

	fin.getline(buffer, 10);
	pawn = Coord(buffer[0] - 48 - 49, buffer[1] - 49);

	std::string end = CoordToStr(pawn);
	std::string start = CoordToStr(horse);

	delete[] buffer;
	fin.close();

	bool map[8][8];
	fieldPtr = &map[0][0];
	int sizeRow = sizeof(map) / sizeof(map[0]),
		sizeCol = sizeof(map[0]) / sizeof(map[0][0]),
		sizeElement = sizeof(bool);

	for (bool *ptr = &map[0][0]; ptr < &map[0][0] + sizeCol * sizeRow * sizeElement; ptr += sizeElement)
		*ptr = false;

	map[horse.x][horse.y] = true;
	map[pawn.x - 1][pawn.y - 1] = true;
	map[pawn.x + 1][pawn.y - 1] = true;

	std::stack<std::tuple<Coord, int>> history;
	std::stack<std::string> result;

	history.push(std::make_tuple(horse, 0));

	if (horse != pawn)
	{
		while (!history.empty())
		{
			Coord horse = std::get<0>(history.top());
			int currentIndex = std::get<1>(history.top());

			history.pop();

			if (horse != pawn)
			{
				bool stepTrue = false;
				Coord oldHorse;

				for (int i = currentIndex; i < 8; i++)
				{
					if (stepFunctions[i](horse, oldHorse))
					{
						history.push(std::make_tuple(oldHorse, i + 1));
						stepTrue = true;
						break;
					}
				}

				if (stepTrue)
				{
					history.push(std::make_tuple(horse, 0));
					way[CoordToStr(horse)] = CoordToStr(oldHorse);
				}
			}
			else
			{
				break;
			}
		}

		while (way[end] != start)
		{
			result.push(end);
			end = way[end];
		}

		result.push(end);
		result.push(start);

		/*
		//-------------------------------
		/*int i = 0;
		for (bool *ptr = &map[0][0]; ptr < &map[0][0] + sizeCol * sizeRow * sizeElement; ptr += sizeElement)
		{
		printf_s("%d ", *ptr);
		if (++i % sizeCol == 0)
		printf_s("\n");
		}

		//std::map<std::string, std::string>::iterator it;

		//for (it = way.begin(); it != way.end(); it++)
		//	std::cout << it->first << ' ' << it->second << '\n';


		while (!result.empty())
		{
		std::cout << result.top() << "\n";
		result.pop();
		}
		//-------------------------------
		*/
	}
	else
	{
		result.push(end);
		result.push(start);
	}

	std::ofstream fout("out.txt");

	while (!result.empty())
	{
		fout << result.top() << "\n";
		result.pop();
	}

	fout.close();
		
	return 0;
}