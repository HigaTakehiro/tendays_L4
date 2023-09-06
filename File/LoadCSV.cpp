#include "LoadCSV.h"
#include <Windows.h>

namespace File
{
int LoadMapChip(const std::string& filePath, int* mapArray, int mapSize, bool isNumberStop, int LoadStopNumber)
{
	if (filePath.empty())
	{
		return -1;
	}

	std::ifstream ifs(filePath);
	std::string string;
	int index = 0;

	if (ifs.fail())
	{
		OutputDebugStringA("ファイルが開けません。\n");
		return -1;
	}

	for (int i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	bool isMinus = false;

	while (getline(ifs, string))
	{
		bool end = false;

		for (auto s : string)
		{
			if (s == ',' || s == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (s == '-')
			{
				isMinus = true;
			}
			else if (s >= '0' && s <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= s - '0';
				}
				else
				{
					mapArray[index] += s - '0';
				}
			}

			if ((index >= mapSize) || ((isNumberStop) && mapArray[index] == LoadStopNumber))
			{
				// ヒットしたら、読み込みを強制的に終了する。
				end = true;
				break;
			}
		}
		if (end)
		{
			break;
		}
	}

	ifs.close();
	return 0;
}

int LoadMapChip(std::ifstream& ifs, int* mapArray, int mapSize, bool isNumberStop, int LoadStopNumber)
{
	for (int i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	std::string string;
	int index = 0;

	bool isMinus = false;

	while (getline(ifs, string))
	{
		bool end = false;

		for (auto s : string)
		{
			if (s == ',' || s == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (s == '-')
			{
				isMinus = true;
			}
			else if (s >= '0' && s <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= s - '0';
				}
				else
				{
					mapArray[index] += s - '0';
				}
			}

			if ((index >= mapSize) || ((isNumberStop) && mapArray[index] == LoadStopNumber))
			{
				// ヒットしたら、読み込みを強制的に終了する。
				end = true;
				break;
			}
		}

		index++;
		if (index >= mapSize)
		{
			end = true;
		}

		if (end)
		{
			break;
		}
	}

	return 0;
}
} //File
