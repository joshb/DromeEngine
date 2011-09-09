#include <DromeMath/Rect2i.h>

using namespace std;

namespace DromeMath {

Rect2i::Rect2i(const Vector2i &min_param, const Vector2i &max_param)
{
	min = min_param;
	max = max_param;
}

int
Rect2i::getWidth() const
{
	return max.x - min.x;
}

int
Rect2i::getHeight() const
{
	return max.y - min.y;
}

Vector2i
Rect2i::getDimensions() const
{
	return max - min;
}

string
Rect2i::toString() const
{
	return string("min = (") + min.toString() + "), max = (" + max.toString() + ")";
}

} // namespace DromeMath
