#include "Othello.h"
#include <DxLib.h>

// ファイルの読み込み
#include <fstream>
#include "../File/LoadCSV.h"

Othello::Othello() :
	cell{},
	initCell{},
	width(8),
	height(8)
{
}

void Othello::Init()
{
	cell.reserve(static_cast<size_t>(width * height));
	initCell.reserve(cell.size());

	//cell.resize(static_cast<size_t>(width * height));

	//cell[9] = BLACK;
	//cell[18] = BLACK;
	//cell[27] = BLACK;
	//cell[36] = BLACK;
	//cell[45] = BLACK;
	//cell[54] = BLACK;
	//cell[63] = BLACK;

	//cell[7] = WHITE;
	//cell[14] = WHITE;
	//cell[21] = WHITE;
	//cell[28] = WHITE;
	//cell[35] = WHITE;
	//cell[42] = WHITE;
	//cell[49] = WHITE;
	//cell[56] = WHITE;

	//initCell = cell;
}

void Othello::Draw(int offsetX, int offsetY)
{
	int circleOffsetX = offsetX + circleSize / 2;
	int circleOffsetY = offsetY + circleSize / 2;

	for (int i = 0; i < width * height; i++)
	{
		if (cell[i] == Color::HOLE)
		{
			continue;
		}

		int x = i % width;
		int y = i / width;

		unsigned int color = GetColor(0x00, 0xF0, 0x00);
		if ((x + y) % 2) color = GetColor(0x00, 0xC0, 0x00);

		DrawBox(circleSize * x + offsetX, circleSize * y + offsetY,
				circleSize * (x + 1) + offsetX, circleSize * (y + 1) + offsetY, color, true);

		if (cell[i] == Color::BLACK)
		{
			color = GetColor(0, 0, 0);
		}
		else if (cell[i] == Color::WHITE)
		{
			color = GetColor(0xFF, 0xFF, 0xFF);
		}
		else
		{
			continue;
		}

		DrawCircle(circleSize * x + circleOffsetX, circleSize * y + circleOffsetY, circleSize / 2, color, true);
	}
}

void Othello::Reset()
{
	for (size_t i = 0; i < cell.size(); i++)
	{
		cell[i] = initCell[i];
	}
}

int Othello::Put(int x, int y, Color color)
{
	int index = y * width + x;
	if (cell[index] != Color::EMPTY)
	{
		return 0;
	}

	int count = 0;
	Color other = Color::EMPTY; //相手の色
	if (color == Color::BLACK)
	{
		other = Color::WHITE;
	}
	else if (color == Color::WHITE)
	{
		other = Color::BLACK;
	}

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}
			if (x + j < 0 || y + i < 0 || x + j >= width || y + i >= height)
			{
				continue;
			}
			index = (y + i) * width + (x + j);
			if (cell[index] != other)
			{
				continue;
			}

			const int size = 8;
			for (int s = 2; s < size; s++)
			{
				if (x + (j * s) < 0 || y + (i * s) < 0 || x + (j * s) >= width || y + (i * s) >= height)
				{
					break;
				}

				index += i * width + j;
				if (index >= 0 && index < cell.size())
				{
					if (cell[index] != Color::BLACK && cell[index] != Color::WHITE)
					{
						break;
					}

					if (cell[index] == color)
					{
						index = y * width + x;
						cell[index] = color;
						count += s;

						for (int n = 1; n < s; n++)
						{
							index += i * width + j;
							cell[index] = color;
						}

						break;
					}
				}
			}
		}
	}

	return count;
}

bool Othello::IsSkip(Color color)
{
	bool result = true;

	for (int i = 0; i < width * height; i++)
	{
		if (result == false)
		{
			break;
		}

		if (cell[i] != Color::EMPTY)
		{
			continue;
		}

		Color other = Color::EMPTY; //相手の色
		if (color == Color::BLACK)
		{
			other = Color::WHITE;
		}
		else if (color == Color::WHITE)
		{
			other = Color::BLACK;
		}

		int index = i;
		int x = i % width;
		int y = i / width;

		for (int dirY = -1; dirY <= 1; dirY++)
		{
			for (int dirX = -1; dirX <= 1; dirX++)
			{
				if (dirY == 0 && dirX == 0)
				{
					continue;
				}
				if (x + dirX < 0 || y + dirY < 0 || x + dirX >= width || y + dirY >= height)
				{
					continue;
				}
				index = (y + dirY) * width + (x + dirX);
				if (cell[index] != other)
				{
					continue;
				}

				const int size = 8;
				for (int s = 2; s < size; s++)
				{
					if (x + (dirX * s) < 0 || y + (dirY * s) < 0 || x + (dirX * s) >= width || y + (dirY * s) >= height)
					{
						break;
					}

					index += dirY * width + dirX;
					if (index >= 0 && index < cell.size())
					{
						if (cell[index] != Color::BLACK && cell[index] != Color::WHITE)
						{
							break;
						}

						if (cell[index] == color)
						{
							result = false;
							break;
						}
					}
				}
			}
		}
	}

	return result;
}

int Othello::Load(const std::string& filePath)
{
	if (filePath.empty())
	{
		return -1;
	}

	cell.clear();

	std::ifstream ifs(filePath);
	std::string str;

	if (ifs.fail())
	{
		OutputDebugStringA("ファイルが開けません。\n");
		return -1;
	}

	getline(ifs, str);
	int num[] = { 0, 0, 0 };
	int i = 0;
	for (auto s : str)
	{
		if (s == ',' || s == '\n')
		{
			i++;
			continue;
		}
		else if (s >= '0' && s <= '9')
		{
			num[i] *= 10;
			num[i] += s - '0';
		}
	}

	width = num[0];
	height = num[1];
	startColor = num[2];

	int* cellArray = new int[(width * height)];
	File::LoadMapChip(ifs, cellArray, width * height);

	for (int i = 0; i < width * height; i++)
	{
		cell.push_back(static_cast<Color>(cellArray[i]));
	}

	initCell = cell;

	delete[] cellArray;
	ifs.close();
	return 0;
}

Color Othello::GetCell(const size_t& index) const
{
	if (index < 0 || index >= cell.size())
	{
		return Color::NONE;
	}

	return cell[index];
}

Color Othello::GetStartColor() const
{
	if (startColor)
	{
		return Color::WHITE;
	}
	else
	{
		return Color::BLACK;
	}
}
