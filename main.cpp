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

sf::ConvexShape make_bound_rect();
sf::ConvexShape make_rect(double x, double y);
sf::ConvexShape make_shape();
void draw_line(sf::RenderWindow &window, Line &line);
void draw_point(sf::RenderWindow &window, sf::Vector2f &p);
void draw_visibility_polygon(sf::RenderWindow &window, std::vector<Line> &rays, std::vector<Line> &edges, std::map<double,sf::Vector2f> &inters, sf::Vector2f &light_source, sf::Texture* light_tex);

std::vector<sf::ConvexShape> shapes;
sf::Vector2f mouse;
sf::Vector2f size = sf::Vector2f(800,400);


int main()
{
    sf::RenderWindow window(sf::VideoMode(size.x,size.y), "2D Lighting Test");
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    shapes.push_back(make_bound_rect());
    for(int i = 0; i < 11; i++)
    {
        shapes.push_back(make_rect(20+i*70,100));
    }


    sf::Vector2f center(size.x/2,size.y/2);
    sf::Texture light;
	if(!light.loadFromFile("light.png"))
		std::cout<<"error loading image";
	sf::FloatRect bounds(0,0,size.x,size.y);
    double ray_length = sqrt(pow(size.x,2) + pow(size.y,2));
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        bool mouse_in_shape = false;

		std::vector<sf::Vector2f> light_sources;
        window.clear(sf::Color::Black);

        mouse = (sf::Vector2f)sf::Mouse::getPosition(window);
        for(int i = 0; i < 10; i++)
        {
        	light_sources.push_back(sf::Vector2f(mouse.x+i*50, mouse.y));
        }


        for(auto i : shapes)
        {
            window.draw(i);
        }
        for(int i = 1; i < shapes.size(); i++)
        {
        	if(shapes[i].getGlobalBounds().contains(mouse))
        	{
        		//mouse_in_shape = true;
        	}
        }
        //if(bounds.contains(mouse) && !mouse_in_shape)
        {
        	for(auto light_source : light_sources)
			{
				std::vector<Line> edges;
				std::vector<Line> rays;
				std::map<double, sf::Vector2f> inters;
				generate_lines(light_source, shapes, rays);
				get_shape_edges(shapes, edges);
				draw_visibility_polygon(window,rays,edges,inters,light_source, &light);

			}
        }
        for(int i = 0; i < 10; i++)
        {
        	draw_point(window, light_sources[i]);
        }

        window.display();
    }
}

sf::ConvexShape make_bound_rect()
{
	sf::ConvexShape rect;
	rect.setPointCount(4);
	rect.setPoint(0, sf::Vector2f(5,5));
	rect.setPoint(1, sf::Vector2f(size.x-5,5));
	rect.setPoint(2, sf::Vector2f(size.x-5, size.y-5));
	rect.setPoint(3, sf::Vector2f(5,size.y-5));

	rect.setOutlineColor(sf::Color::White);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(2);

	return rect;
}

sf::ConvexShape make_rect(double x, double y)
{
	sf::Vector2f rectsize = sf::Vector2f(50,10);
	sf::Vector2f pos = sf::Vector2f(x,y);
	sf::ConvexShape shape;
	shape.setPointCount(4);
	shape.setPoint(0, pos);
	shape.setPoint(1, sf::Vector2f(pos.x+rectsize.x, pos.y));
	shape.setPoint(2, sf::Vector2f(pos.x+rectsize.x,pos.y+rectsize.y));
	shape.setPoint(3, sf::Vector2f(pos.x,pos.y+rectsize.y));
	shape.setFillColor(sf::Color::Cyan);

	return shape;
}

void draw_line(sf::RenderWindow &window, Line &line_in)
{
	sf::VertexArray line(sf::LinesStrip, 2);
	line[0].position = line_in.p1;
	line[0].color = sf::Color::Red;
	line[1].position = line_in.p2;
	line[1].color = sf::Color(200,200,200);
	window.draw(line);
}


void draw_point(sf::RenderWindow &window, sf::Vector2f &p)
{
	sf::CircleShape dot;
	dot.setRadius(4);
	dot.setOutlineThickness(0);
	dot.setFillColor(sf::Color::White);
	dot.setPosition(p.x-4,p.y-4);
	window.draw(dot);
}

sf::ConvexShape make_shape()
{
	sf::ConvexShape shape;
	shape.setPointCount(3);
	shape.setPoint(0, sf::Vector2f(500, 50));
	shape.setPoint(1, sf::Vector2f(520, 70));
	shape.setPoint(2, sf::Vector2f(480, 50));
	shape.setFillColor(sf::Color::Cyan);
	return shape;
}


void draw_visibility_polygon(sf::RenderWindow &window, std::vector<Line> &rays, std::vector<Line> &edges, std::map<double,sf::Vector2f> &inters, sf::Vector2f &light_source, sf::Texture *light_tex)
{
    Line shortened_line;

    for(auto ray : rays)
    {
    	std::vector<sf::Vector2f> ray_inters;
    	for(auto side: edges)
    	{
        	sf::Vector2f* inter = get_intersection(ray, side);
        	if(inter != NULL)
        	{
        		ray_inters.push_back(*inter);
        	}
    	}
    	if(ray_inters.size()>0)
    	{
			sf::Vector2f closest = closest_point(light_source, ray_inters);
			shortened_line = Line(light_source, closest);
			//draw_line(window, shortened_line);
			//draw_point(window, closest);
			inters.insert(std::pair<double, sf::Vector2f>(get_angle(light_source, closest), closest));
    	}
    }
    if(inters.size() > 0 )
    	draw_triangles(window,light_source,inters, light_tex);
}
