// Yash Mistri

#ifndef LINE_H
#define LINE_H

#include <SFML/System/Vector2.hpp>

class Line
{
public:
	sf::Vector2f p1;
	sf::Vector2f p2;

	Line();
	Line(sf::Vector2f &point1, sf::Vector2f &point2);
    static double magnitude(sf::Vector2f &v1, sf::Vector2f &v2);
};

#endif
