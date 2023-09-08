#include "Cell.h"

void Cell::Init(ColorFlag colorFlag, BigCell bigCell)
{
	this->colorFlag = colorFlag;
	this->init = colorFlag;
	this->bigCell = bigCell;
}
