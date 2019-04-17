// Yash Mistri

/*
 * generate lines from origin to the wall passing through each vertex 	[x]
 * shorten the line if it intersects with any line segments 			[x]
 * put all the endpoints in an array and sort them 						[x]
 * draw a triangle between each point, the next point, and the origin 	[ ]
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include "line.h"
#include "geometry.h"

sf::RectangleShape make_bound_rect(sf::RectangleShape &rect);
sf::RectangleShape make_rect(double x, double y);
sf::CircleShape make_dot(sf::CircleShape &dot);

std::vector<sf::RectangleShape> rects;
sf::Vector2f mouse;
sf::Vector2f size = sf::Vector2f(500,300);


int main()
{
    sf::RenderWindow window(sf::VideoMode(size.x,size.y), "2D Lighting Test");

    sf::RectangleShape bound_rect;
    sf::CircleShape dot;
    rects.push_back(make_bound_rect(bound_rect));
    rects.push_back(make_rect(100,100));
    make_dot(dot);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        std::vector<Line> lines;
        std::vector<sf::Vector2f> inters;
        std::vector<Line> rect_sides;
        std::map<double, sf::Vector2f> p;

        mouse = (sf::Vector2f)sf::Mouse::getPosition(window);

		generate_lines( mouse,rects,lines);
		get_rect_sides(rects, rect_sides);
		for(auto line : lines)
		{
			sf::VertexArray a(sf::LinesStrip, 2);
			a[0].position = line.p1;
			a[1].position = line.p2;
			window.draw(a);
		}
		shorten_lines(mouse, lines, rect_sides, inters, window);
		int i=0;
		for(auto inter : inters)
		{
			std::cout<<inter.x<<", "<<inter.y<<std::endl;
			sf::CircleShape dot;
			dot.setFillColor(sf::Color::Green);
			dot.setOrigin(inter);
			dot.setRadius(5);
			window.draw(dot);
		}
		p = sort_points(mouse, inters);

		draw_triangles(window, mouse, p);

        for(sf::RectangleShape i : rects)
        {
            window.draw(i);
        }


        window.display();
    }
}

sf::RectangleShape make_bound_rect(sf::RectangleShape &rect)
{
	rect.setSize(size);
	rect.setOutlineColor(sf::Color::White);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(2);
	rect.setPosition(0,0);

	return rect;
}

sf::RectangleShape make_rect(double x, double y)
{
	sf::Vector2f size = sf::Vector2f(50,50);
	sf::Vector2f pos = sf::Vector2f(x,y);
	sf::RectangleShape rect = sf::RectangleShape(size);
	rect.setPosition(pos);
	rect.setSize(size);
	rect.setFillColor(sf::Color::Green);

	return rect;
}

sf::CircleShape make_dot(sf::CircleShape &dot)
{
	dot.setRadius(3);
	dot.setOutlineColor(sf::Color::Green);
	dot.setFillColor(sf::Color::Green);

	return dot;
}

