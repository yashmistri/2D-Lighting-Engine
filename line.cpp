// Yash Mistri

#include "line.h"
#include <math.h>
#include <SFML/System/Vector2.hpp>

Line::Line(sf::Vector2f &point1, sf::Vector2f &point2)
{
	p1 = point1;
	p2 = point2;
}

Line::Line()
{

}

double Line::magnitude(sf::Vector2f &v1, sf::Vector2f &v2)
{
	return sqrt(pow(v2.y - v1.y,2) + pow(v2.x-v1.x,2));
}
