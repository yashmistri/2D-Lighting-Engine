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
void draw_line(sf::RenderWindow &window, Line &line);
void draw_point(sf::RenderWindow &window, sf::Vector2f &p);

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
    sf::Vector2f center(size.x/2,size.y/2);
    double ray_length = sqrt(pow(size.x,2) + pow(size.y,2));
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        Line line;
        sf::Vector2f extended_point;
        std::vector<Line> rect_sides;
        std::vector<Line> rays;
        Line shortened_line;
        window.clear();

        mouse = (sf::Vector2f)sf::Mouse::getPosition(window);

        int num_rays = 100;
        double angle_rad = 0.000001;
        double increment_rad = 6.2832/num_rays;
        for(int i = 0; i < num_rays; i++)
        {
        	sf::Vector2f end_point(mouse.x+ray_length*cos(angle_rad), mouse.y+ray_length*sin(angle_rad));
        	Line ray(mouse, end_point);
        	rays.push_back(ray);
        	angle_rad += increment_rad;

        }
        get_rect_sides(rects, rect_sides);


        for(sf::RectangleShape i : rects)
        {
            window.draw(i);
        }

        for(auto ray : rays)
        {
        	std::vector<sf::Vector2f> ray_inters;
        	for(auto side: rect_sides)
        	{
            	sf::Vector2f* inter = get_intersection(ray, side);
            	if(inter != NULL)
            		ray_inters.push_back(*inter);
        	}
			sf::Vector2f closest = closest_point(mouse, ray_inters);
			shortened_line = Line(mouse, closest);
			draw_line(window, shortened_line);
			draw_point(window, closest);

        }


        window.display();
    }
}

sf::RectangleShape make_bound_rect(sf::RectangleShape &rect)
{
	rect.setSize(size);
	rect.setOutlineColor(sf::Color::White);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(1);
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
	rect.setFillColor(sf::Color::Cyan);

	return rect;
}

sf::CircleShape make_dot(sf::CircleShape &dot)
{
	dot.setRadius(3);
	dot.setOutlineColor(sf::Color::Green);
	dot.setFillColor(sf::Color::Green);

	return dot;
}


void draw_line(sf::RenderWindow &window, Line &line_in)
{
	sf::VertexArray line(sf::LinesStrip, 2);
	line[0].position = line_in.p1;
	line[0].color = sf::Color::Red;
	line[1].position = line_in.p2;
	line[1].color = sf::Color::Red;
	window.draw(line);


}


void draw_point(sf::RenderWindow &window, sf::Vector2f &p)
{
	sf::CircleShape dot;
	dot.setRadius(4);
	dot.setOutlineThickness(0);
	dot.setFillColor(sf::Color::Blue);
	dot.setPosition(p.x-4,p.y-4);
	window.draw(dot);
}
