#include "Othello.h"
#include <DxLib.h>
#include <array>

// ファイルの読み込み
#include <fstream>
#include "../File/LoadCSV.h"

Othello::Othello() :
	cell{},
	width(8),
	height(8),
	startColor(false)
{
}

void Othello::Init()
{
	cell.reserve(static_cast<size_t>(width * height));

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
}

void Othello::Draw(int offsetX, int offsetY)
{
	int circleOffsetX = offsetX + circleSize / 2;
	int circleOffsetY = offsetY + circleSize / 2;

	for (int i = 0; i < width * height; i++)
	{
		if (cell[i].colorFlag == ColorFlag::HOLE)
		{
			continue;
		}

		int x = i % width;
		int y = i / width;

		unsigned int color = GetColor(0x00, 0xE0, 0x00);
		if ((x + y) % 2)
		{
			color = GetColor(0x00, 0xB0, 0x00);
		}
		if (cell[i].GetBigCell() != BigCell::NONE)
		{
			color = GetColor(0x00, 0xC8, 0x00);
		}

		if (cell[i].GetBigCell() == BigCell::NONE)
		{
			DrawBox(circleSize * x + offsetX, circleSize * y + offsetY,
					circleSize * (x + 1) + offsetX, circleSize * (y + 1) + offsetY, color, true);
		}
		else if (cell[i].GetBigCell() == BigCell::LT)
		{
			DrawBox(circleSize * x + offsetX, circleSize * y + offsetY,
					circleSize * (x + 2) + offsetX, circleSize * (y + 2) + offsetY, color, true);
		}

		if ((cell[i].colorFlag == ColorFlag::BIG_B || cell[i].colorFlag == ColorFlag::BIG_W) &&
			cell[i].GetBigCell() != BigCell::RB)
		{
			continue;
		}

		if (cell[i].colorFlag == ColorFlag::BLACK || cell[i].colorFlag == ColorFlag::BIG_B)
		{
			color = GetColor(0, 0, 0);
		}
		else if (cell[i].colorFlag == ColorFlag::WHITE || cell[i].colorFlag == ColorFlag::BIG_W)
		{
			color = GetColor(0xFF, 0xFF, 0xFF);
		}
		else
		{
			continue;
		}

		if (cell[i].colorFlag == ColorFlag::BIG_B || cell[i].colorFlag == ColorFlag::BIG_W)
		{
			DrawCircle(circleSize * x + offsetX, circleSize * y + offsetY, circleSize, color, true);
		}
		else
		{
			DrawCircle(circleSize * x + circleOffsetX, circleSize * y + circleOffsetY, circleSize / 2, color, true);
		}
	}
}

void Othello::Reset()
{
	for (size_t i = 0; i < cell.size(); i++)
	{
		cell[i].Reset();
	}
}

int Othello::Put(int x, int y, ColorFlag color)
{
	int count = 0;

	ColorFlag skipColor[] = {
		ColorFlag::EMPTY,
		ColorFlag::HOLE,
		ColorFlag::BIG_E
	};

	ColorFlag bigColor = ColorFlag::EMPTY; //自分の色
	ColorFlag other = ColorFlag::EMPTY; //相手の色
	ColorFlag bigOther = ColorFlag::EMPTY; //相手の色
	if (color == ColorFlag::BIG_B)
	{
		color = ColorFlag::BLACK;
	}
	else if (color == ColorFlag::BIG_W)
	{
		color = ColorFlag::WHITE;
	}
	if (color == ColorFlag::BLACK)
	{
		bigColor = ColorFlag::BIG_B;
		other = ColorFlag::WHITE;
		bigOther = ColorFlag::BIG_W;
	}
	else if (color == ColorFlag::WHITE)
	{
		bigColor = ColorFlag::BIG_W;
		other = ColorFlag::BLACK;
		bigOther = ColorFlag::BIG_B;
	}

	int index = y * width + x;
	bool isBigB = false; //デカマスを通るかどうか(自分の色)
	bool isBigW = false; //デカマスを通るかどうか(相手の色)

	if (cell[index].colorFlag != ColorFlag::EMPTY && cell[index].colorFlag != ColorFlag::BIG_E)
	{
		return 0;
	}

	if (cell[index].colorFlag == ColorFlag::EMPTY)
	{
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
				isBigW = false;
				if (cell[index].colorFlag == bigOther)
				{
					isBigW = true;
				}
				else if (cell[index].colorFlag != other)
				{
					continue;
				}

				const int size = (width > height) ? width : height;
				for (int s = 2; s < size; s++)
				{
					if (x + (dirX * s) < 0 || y + (dirY * s) < 0 || x + (dirX * s) >= width || y + (dirY * s) >= height)
					{
						break;
					}

					index += dirY * width + dirX;
					if (index >= 0 && index < cell.size())
					{
						bool flag = false;
						for (auto i : skipColor)
						{
							if (cell[index].colorFlag == i)
							{
								flag = true;
								break;
							}
						}
						if (flag)
						{
							break;
						}

						if (cell[index].colorFlag == color)
						{
							if (isBigW || isBigB)
							{
								break;
							}

							index = y * width + x;
							cell[index].colorFlag = color;

							for (int n = 1; n < s; n++)
							{
								index += dirY * width + dirX;
								if (cell[index].colorFlag >= ColorFlag::EMPTY && cell[index].colorFlag < ColorFlag::HOLE)
								{
									cell[index].colorFlag = color;
									count += 1;
								}
								else if (cell[index].colorFlag >= ColorFlag::BIG_E && cell[index].colorFlag <= ColorFlag::BIG_W)
								{
									index += dirY * width + dirX;
									n++;
									BigChange(index, bigColor);
									count += 4;
								}
							}
							break;
						}
						else if (cell[index].colorFlag == bigColor)
						{
							if (isBigW)
							{
								break;
							}

							if (isBigB)
							{
								index = y * width + x;
								cell[index].colorFlag = color;

								for (int n = 1; n < s; n++)
								{
									index += dirY * width + dirX;
									if (cell[index].colorFlag >= ColorFlag::EMPTY && cell[index].colorFlag < ColorFlag::HOLE)
									{
										cell[index].colorFlag = color;
										count += 1;
									}
									else if (cell[index].colorFlag >= ColorFlag::BIG_E && cell[index].colorFlag <= ColorFlag::BIG_W)
									{
										index += dirY * width + dirX;
										n++;
										BigChange(index, bigColor);
										count += 4;
									}
								}
								break;
							}

							isBigB = !isBigB;
						}
						else if (cell[index].colorFlag == other)
						{
							if (isBigW)
							{
								break;
							}
						}
						else if (cell[index].colorFlag == bigOther)
						{
							isBigW = !isBigW;
						}
					}
				}
			}
		}

		if (count > 0)
		{
			count += 1;
		}
	}
	else if (cell[index].colorFlag == ColorFlag::BIG_E)
	{
		int indexLT = index;
		if (cell[index].GetBigCell() % 2 == 1)
		{
			indexLT -= 1;
		}
		if (cell[index].GetBigCell() / 2 == 1)
		{
			indexLT -= width;
		}

		for (int i = 0; i < 4; i++)
		{
			index = indexLT;
			if (i % 2 == 1)
			{
				index += 1;
			}
			if (i / 2 == 1)
			{
				index += width;
			}
			x = index % width;
			y = index / width;

			for (int dir = 0; dir < 3; dir++)
			{
				int dirX = 0, dirY = 0;

				switch (i)
				{
				case BigCell::LT:
					if (dir % 2 == 0)
					{
						dirX = -1;
					}
					if (dir / 2 == 0)
					{
						dirY = -1;
					}
					break;
				case BigCell::RT:
					if (dir % 2 == 1)
					{
						dirX = +1;
					}
					if (dir / 2 == 0)
					{
						dirY = -1;
					}
					break;
				case BigCell::LB:
					if (dir % 2 == 0)
					{
						dirX = -1;
					}
					if (dir / 2 == 1)
					{
						dirY = +1;
					}
					break;
				case BigCell::RB:
					if (dir % 2 == 1)
					{
						dirX = +1;
					}
					if (dir / 2 == 1)
					{
						dirY = +1;
					}
					break;
				default:
					break;
				}

				if (x + dirX < 0 || y + dirY < 0 || x + dirX >= width || y + dirY >= height)
				{
					continue;
				}

				index = (y + dirY) * width + (x + dirX);
				isBigW = false;
				if (cell[index].colorFlag == bigOther)
				{
					isBigW = true;
				}
				else if (cell[index].colorFlag != other)
				{
					continue;
				}

				const int size = (width > height) ? width : height;
				for (int s = 2; s < size; s++)
				{
					if (x + (dirX * s) < 0 || y + (dirY * s) < 0 || x + (dirX * s) >= width || y + (dirY * s) >= height)
					{
						break;
					}

					index += dirY * width + dirX;
					if (index >= 0 && index < cell.size())
					{
						bool flag = false;
						for (auto j : skipColor)
						{
							if (cell[index].colorFlag == j)
							{
								flag = true;
								break;
							}
						}
						if (flag)
						{
							break;
						}

						if (cell[index].colorFlag == color)
						{
							if (isBigW || isBigB)
							{
								break;
							}

							index = y * width + x;
							BigChange(index, bigColor);

							for (int n = 1; n < s; n++)
							{
								index += dirY * width + dirX;
								if (cell[index].colorFlag >= ColorFlag::EMPTY && cell[index].colorFlag < ColorFlag::HOLE)
								{
									cell[index].colorFlag = color;
									count += 1;
								}
								else if (cell[index].colorFlag >= ColorFlag::BIG_E && cell[index].colorFlag <= ColorFlag::BIG_W)
								{
									index += dirY * width + dirX;
									n++;
									BigChange(index, bigColor);
									count += 4;
								}
							}
							break;
						}
						else if (cell[index].colorFlag == bigColor)
						{
							if (isBigW)
							{
								break;
							}

							if (isBigB)
							{
								index = y * width + x;
								BigChange(index, bigColor);

								for (int n = 1; n < s; n++)
								{
									index += dirY * width + dirX;
									if (cell[index].colorFlag >= ColorFlag::EMPTY && cell[index].colorFlag < ColorFlag::HOLE)
									{
										cell[index].colorFlag = color;
										count += 1;
									}
									else if (cell[index].colorFlag >= ColorFlag::BIG_E && cell[index].colorFlag <= ColorFlag::BIG_W)
									{
										index += dirY * width + dirX;
										n++;
										BigChange(index, bigColor);
										count += 4;
									}
								}
								break;
							}

							isBigB = !isBigB;
						}
						else if (cell[index].colorFlag == other)
						{
							if (isBigW)
							{
								break;
							}
						}
						else if (cell[index].colorFlag == bigOther)
						{
							isBigW = !isBigW;
						}
					}
				}
			}
		}

		if (count > 0)
		{
			count += 4;
		}
	}

	return count;
}

bool Othello::IsSkip(ColorFlag color)
{
	bool result = true;

	ColorFlag skipColor[] = {
		ColorFlag::EMPTY,
		ColorFlag::HOLE,
		ColorFlag::BIG_E
	};

	ColorFlag bigColor = ColorFlag::EMPTY; //自分の色
	ColorFlag other = ColorFlag::EMPTY; //相手の色
	ColorFlag bigOther = ColorFlag::EMPTY; //相手の色
	if (color == ColorFlag::BIG_B)
	{
		color = ColorFlag::BLACK;
	}
	else if (color == ColorFlag::BIG_W)
	{
		color = ColorFlag::WHITE;
	}
	if (color == ColorFlag::BLACK)
	{
		bigColor = ColorFlag::BIG_B;
		other = ColorFlag::WHITE;
		bigOther = ColorFlag::BIG_W;
	}
	else if (color == ColorFlag::WHITE)
	{
		bigColor = ColorFlag::BIG_W;
		other = ColorFlag::BLACK;
		bigOther = ColorFlag::BIG_B;
	}

	bool isBigB = false; //デカマスを通るかどうか(自分の色)
	bool isBigW = false; //デカマスを通るかどうか(相手の色)

	for (int i = 0; i < width * height; i++)
	{
		if (cell[i].colorFlag != ColorFlag::EMPTY && cell[i].colorFlag != ColorFlag::BIG_E)
		{
			continue;
		}

		int index = i;
		int x = i % width;
		int y = i / width;

		if (cell[i].colorFlag == ColorFlag::EMPTY)
		{
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
					isBigW = false;
					if (cell[index].colorFlag == bigOther)
					{
						isBigW = true;
					}
					else if (cell[index].colorFlag != other)
					{
						continue;
					}

					const int size = (width > height) ? width : height;
					for (int s = 2; s < size; s++)
					{
						if (x + (dirX * s) < 0 || y + (dirY * s) < 0 || x + (dirX * s) >= width || y + (dirY * s) >= height)
						{
							break;
						}

						index += dirY * width + dirX;
						if (index >= 0 && index < cell.size())
						{
							bool flag = false;
							for (auto j : skipColor)
							{
								if (cell[index].colorFlag == j)
								{
									flag = true;
									break;
								}
							}
							if (flag)
							{
								break;
							}

							if (cell[index].colorFlag == color)
							{
								if (isBigW || isBigB)
								{
									break;
								}

								result = false;
								break;
							}
							else if (cell[index].colorFlag == bigColor)
							{
								if (isBigW)
								{
									break;
								}

								if (isBigB)
								{
									result = false;
									break;
								}

								isBigB = !isBigB;
							}
							else if (cell[index].colorFlag == other)
							{
								if (isBigW)
								{
									break;
								}
							}
							else if (cell[index].colorFlag == bigOther)
							{
								isBigW = !isBigW;
							}
						}
					}

					if (result == false)
					{
						break;
					}
				}

				if (result == false)
				{
					break;
				}
			}
		}
		else if (cell[i].colorFlag == ColorFlag::BIG_E)
		{
			if (cell[i].GetBigCell() != BigCell::LT)
			{
				continue;
			}

			for (int j = 0; j < 4; j++)
			{
				index = i;
				if (j % 2 == 1)
				{
					index += 1;
				}
				if (j / 2 == 1)
				{
					index += width;
				}
				x = index % width;
				y = index / width;

				for (int dir = 0; dir < 3; dir++)
				{
					int dirX = 0, dirY = 0;

					switch (j)
					{
					case BigCell::LT:
						if (dir % 2 == 0)
						{
							dirX = -1;
						}
						if (dir / 2 == 0)
						{
							dirY = -1;
						}
						break;
					case BigCell::RT:
						if (dir % 2 == 1)
						{
							dirX = +1;
						}
						if (dir / 2 == 0)
						{
							dirY = -1;
						}
						break;
					case BigCell::LB:
						if (dir % 2 == 0)
						{
							dirX = -1;
						}
						if (dir / 2 == 1)
						{
							dirY = +1;
						}
						break;
					case BigCell::RB:
						if (dir % 2 == 1)
						{
							dirX = +1;
						}
						if (dir / 2 == 1)
						{
							dirY = +1;
						}
						break;
					default:
						break;
					}

					if (x + dirX < 0 || y + dirY < 0 || x + dirX >= width || y + dirY >= height)
					{
						continue;
					}

					index = (y + dirY) * width + (x + dirX);
					isBigW = false;
					if (cell[index].colorFlag == bigOther)
					{
						isBigW = true;
					}
					else if (cell[index].colorFlag != other)
					{
						continue;
					}

					const int size = (width > height) ? width : height;
					for (int s = 2; s < size; s++)
					{
						if (x + (dirX * s) < 0 || y + (dirY * s) < 0 || x + (dirX * s) >= width || y + (dirY * s) >= height)
						{
							break;
						}

						index += dirY * width + dirX;
						if (index >= 0 && index < cell.size())
						{
							bool flag = false;
							for (auto k : skipColor)
							{
								if (cell[index].colorFlag == k)
								{
									flag = true;
									break;
								}
							}
							if (flag)
							{
								break;
							}

							if (cell[index].colorFlag == color)
							{
								if (isBigW || isBigB)
								{
									break;
								}

								result = false;
								break;
							}
							else if (cell[index].colorFlag == bigColor)
							{
								if (isBigW)
								{
									break;
								}

								if (isBigB)
								{
									result = false;
									break;
								}

								isBigB = !isBigB;
							}
							else if (cell[index].colorFlag == other)
							{
								if (isBigW)
								{
									break;
								}
							}
							else if (cell[index].colorFlag == bigOther)
							{
								isBigW = !isBigW;
							}
						}
					}

					if (result == false)
					{
						break;
					}
				}

				if (result == false)
				{
					break;
				}
			}
		}

		if (result == false)
		{
			break;
		}
	}

	return result;
}

int Othello::BigChange(int index, ColorFlag color)
{
	if (index < 0 || index >= cell.size() ||
		cell[index].colorFlag <= ColorFlag::HOLE ||
		cell[index].GetBigCell() == BigCell::NONE ||
		color == ColorFlag::HOLE)
	{
		return -1;
	}

	int bigIndex[4] = {};
	switch (cell[index].GetBigCell())
	{
	case BigCell::LT:
	{
		bigIndex[0] = index;
		bigIndex[1] = index + 1;
		bigIndex[2] = index + width;
		bigIndex[3] = index + width + 1;
		break;
	}
	case BigCell::RT:
	{
		bigIndex[0] = index - 1;
		bigIndex[1] = index;
		bigIndex[2] = index + width - 1;
		bigIndex[3] = index + width;
		break;
	}
	case BigCell::LB:
	{
		bigIndex[0] = index - width;
		bigIndex[1] = index - width + 1;
		bigIndex[2] = index;
		bigIndex[3] = index + 1;
		break;
	}
	case BigCell::RB:
	{
		bigIndex[0] = index - width - 1;
		bigIndex[1] = index - width;
		bigIndex[2] = index - 1;
		bigIndex[3] = index;
		break;
	}
	default:
		break;
	}

	if (color == ColorFlag::BLACK)
	{
		color = ColorFlag::BIG_B;
	}
	if (color == ColorFlag::WHITE)
	{
		color = ColorFlag::BIG_W;
	}

	for (int i = 0; i < 4; i++)
	{
		cell[bigIndex[i]].colorFlag = color;
	}

	return 0;
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

	std::vector<std::array<int, 4>> bigCells;
	BigCell bigCellState = BigCell::NONE;
	for (int i = 0; i < width * height; i++)
	{
		bigCellState = BigCell::NONE;

		if (cellArray[i] == ColorFlag::BIG_E || cellArray[i] == ColorFlag::BIG_B || cellArray[i] == ColorFlag::BIG_W)
		{
			bool isHit = false;

			for (auto& itr : bigCells)
			{
				for (int j = 0; j < 4; j++)
				{
					if (itr[j] == i)
					{
						isHit = true;
						bigCellState = static_cast<BigCell>(j);
						break;
					}
				}

				if (isHit)
				{
					break;
				}
			}

			if (isHit == false)
			{
				bigCells.push_back({ i, i + 1,i + width, i + width + 1 });
				bigCellState = BigCell::LT;
			}
		}

		cell.push_back({});
		cell.back().Init(static_cast<ColorFlag>(cellArray[i]), bigCellState);
	}

	delete[] cellArray;
	ifs.close();
	return 0;
}

ColorFlag Othello::GetCell(int index) const
{
	if (index < 0 || index >= cell.size())
	{
		return ColorFlag::HOLE;
	}

	return cell[index].colorFlag;
}

ColorFlag Othello::GetStartColor() const
{
	if (startColor)
	{
		return ColorFlag::WHITE;
	}
	else
	{
		return ColorFlag::BLACK;
	}
}
