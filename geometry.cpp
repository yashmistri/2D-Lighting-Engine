// Yash Mistri


#include "geometry.h"
#include "line.h"

#include <SFML/System/Vector2.hpp>  // sf::Vector2f
#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>

//returns a point that
sf::Vector2f* get_intersection(Line &l1, Line &l2)
{
	sf::Vector2f* inter = new sf::Vector2f;
	sf::Vector2f r = sf::Vector2f(l1.p2.x - l1.p1.x, l1.p2.y - l1.p1.y);
	sf::Vector2f s = sf::Vector2f(l2.p2.x - l2.p1.x, l2.p2.y - l2.p1.y);

	sf::Vector2f diff = sf::Vector2f(l2.p1.x - l1.p1.x, l2.p1.y - l1.p1.y);

	double numert = cross(diff, s);
	double denomt = cross(r,s);
	double numeru = cross(diff, r);
	double denomu = -denomt;

	if((numert != 0 && denomt == 0) || (denomt == 0 || denomu == 0))
		return NULL;
	double t = numert / denomt;
	double u = numeru / denomu;

	if(denomt != 0 && (t >= 0 && t <= 1) && (-u >= 0 && -u <= 1))
	{
		*inter =  sf::Vector2f(l1.p1.x + t*r.x, l1.p1.y + t * r.y);
		return inter;
	}
	return NULL;
}

double cross(sf::Vector2f &v1, sf::Vector2f &v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}
sf::Vector2f extend(sf::Vector2f &p1, sf::Vector2f &p2, double length)
{
	sf::Vector2f out;
	double hyp = Line::magnitude(p1, p2);
	double x = p2.x + (p2.x-p1.x)/hyp*length;
	double y = p2.y + (p2.y-p1.y)/hyp*length;

	out = sf::Vector2f(x, y);
	return out;
}

std::vector<sf::Vector2f> get_points(sf::ConvexShape &shape)
{
	std::vector<sf::Vector2f> points;
	for(int i = 0; i < shape.getPointCount(); i++)
	{
		points.push_back(shape.getPoint(i));
	}
	return points;
}

std::vector<Line*> get_lines(sf::ConvexShape &shape)
{
	std::vector<Line*> lines;
	int p_count = shape.getPointCount();
	for(int i = 1; i <= p_count; i++)
	{
		sf::Vector2f p1;
		sf::Vector2f p2;
		if(i == p_count)
		{
			p1 = shape.getPoint(0);
			p2 = shape.getPoint(p_count-1);
			Line* line = new Line(p1,p2);
			lines.push_back(line);
			break;
		}
		else
		{
			p1 = shape.getPoint(i);
			p2 = shape.getPoint(i-1);
			Line* line = new Line(p1,p2);
			lines.push_back(line);
		}
	}

	return lines;
}

double get_slope(sf::Vector2f p1, sf::Vector2f p2)
{
	return (p2.y - p1.y)/(p2.x - p1.y);
}

double get_angle(sf::Vector2f p1, sf::Vector2f p2)
{
	float a = atan2f(p2.y-p1.y, p2.x-p1.x);
	return (a > 0) ? a : 6.2832 + a;
}

void generate_lines(sf::Vector2f &origin, std::vector<sf::ConvexShape> &shapes, std::vector<Line> &lines)
{
	for (auto shape : shapes)
	{
		std::vector<sf::Vector2f> shape_points = get_points(shape);
		for(int i = 0; i < shape.getPointCount(); i++)
		{
			sf::Vector2f shape_point = shape_points[i];
			sf::Vector2f extended = extend(origin,shape_point, 1000.0);
			sf::Vector2f down(extended.x*cos(-.001)-extended.y*sin(-.001), extended.y*cos(-.001) + extended.x*sin(-.001));
			sf::Vector2f up(extended.x*cos(.001)-extended.y*sin(.001), extended.y*cos(.001) + extended.x*sin(.001));
			Line next(origin, extended);
			Line down_line(origin, down);//Lines that go to the corner but don't intersect it
			Line up_line(origin, up);

			lines.push_back(next);
			lines.push_back(down_line);
			lines.push_back(up_line);
		}
	}
}

void get_shape_edges(std::vector<sf::ConvexShape> &shapes, std::vector<Line> &lines)
{
	for (auto shape : shapes)
	{
		std::vector<Line*> segs = get_lines(shape);
		for(auto line : segs)
		{
			lines.push_back(*line);
		}
	}
}


sf::Vector2f closest_point(sf::Vector2f &origin, std::vector<sf::Vector2f> &points)
{
	for(auto p : points)
			{

				//std::cout <<p.x<<", "<< p.y<<std::endl;
			}
	int min = 0;

	for(int i = 1; i < points.size(); i++)
	{

		//std::cout <<points[i].x<<", "<< points[i].y<<std::endl;
		//std::cout<<"i: "<<i<<" "<<Line::magnitude(origin, points[i])<<std::endl;
		if(Line::magnitude(origin, points[min]) > Line::magnitude(origin, points[i]))
			min = i;
	}
	//std::cout<<std::endl;
	return points[min];

}


void draw_triangles(sf::RenderWindow &window, sf::Vector2f &mouse, std::map<double, sf::Vector2f> &points, sf::Texture* light)
{
	bool full_light = true;
	std::map<double, sf::Vector2f>::iterator it;
	for(it=points.begin(); it!= points.end(); it++)
	{
		sf::Vector2f p1;
		sf::Vector2f p2;
		bool break_loop;

		if(++it==points.end())
		{
			p1 = (--it)->second;
			it = points.begin();
			p2 = it->second;
			break_loop = true;
		}
		else
		{
			--it;
			p1 = it->second;
			p2 = (++it)->second;it--;
		}
		if(full_light)
		{
			sf::ConvexShape triangle;
			triangle.setPointCount(3);
			triangle.setPoint(0, mouse);
			triangle.setPoint(1, p1);
			triangle.setPoint(2, p2);
			triangle.setFillColor(sf::Color(200,200,100,60));
			//triangle.setTexture(light);
			//triangle.setTextureRect((sf::IntRect)triangle.getLocalBounds());
			window.draw(triangle);
		}
		else
		{
			sf::VertexArray line(sf::LineStrip, 2);
			line[0].position = p1;
			line[0].color = sf::Color::Green;
			line[1].position = p2;
			line[1].color = sf::Color::Green;
			window.draw(line);
		}
		if(break_loop)
			break;
	}
	std::cout<<std::endl;
}


