#include "Othello.h"
#include <DxLib.h>

Othello::Othello() :
	cell{},
	width(8),
	height(6)
{
	cell.reserve(static_cast<size_t>(width * height));
}

void Othello::Init()
{
	cell.resize(static_cast<size_t>(width * height));

	cell[27] = Color::BLACK;
	cell[36] = Color::BLACK;

	cell[28] = Color::WHITE;
	cell[35] = Color::WHITE;

	cell[37] = Color::HOLE;
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

		unsigned int color = GetColor(0x00, 0xFF, 0x00);
		if ((x + y) % 2) color = GetColor(0x00, 0xCC, 0x00);

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

int Othello::Put(int x, int y, Color color)
{
	int index = y * width + x;
	if (cell[index] == Color::HOLE)
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
			if (x + i < 0 || y + j < 0 || x + i >= width || y + j >= height)
			{
				continue;
			}
			index = (y + j) * width + (x + i);
			if (cell[index] != other)
			{
				continue;
			}

			const int size = 8;
			for (int s = 2; s < size; s++)
			{
				index += j * width + i;
				if (index >= 0 && index < cell.size())
				{
					if (cell[index] == Color::EMPTY)
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
							index += j * width + i;
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
