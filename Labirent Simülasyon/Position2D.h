#ifndef Position2D_H
#define Position2D_H

struct Position2D {
	int x;
	int y;

	bool operator == (const Position2D& other) const
	{
		if (this->x == other.x && this->y == other.y)
			return true;
		else
			return false;
	}
};

#endif