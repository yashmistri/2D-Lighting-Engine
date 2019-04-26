// Yash Mistri


#include "geometry.h"
#include "line.h"

#include <SFML/System/Vector2.hpp>  // sf::Vector2f
#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>

//returns a list of all points on a rectangle rect that intersect with line
std::vector<sf::Vector2f> intersects_line_rect(Line &line, sf::RectangleShape &rect)
{
	sf::Vector2f point;
	std::vector<sf::Vector2f> points;
	std::vector<Line*> segs = get_lines(rect);
	for(int i = 0; i < (int)segs.size(); i++)
	{
		point = *get_intersection(line, *segs[i]);
		points[i] = point;
	}
	return points;
}

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

std::vector<sf::Vector2f> get_points(sf::RectangleShape &rect)
{
	std::vector<sf::Vector2f> points;
	sf::Vector2f origin = rect.getPosition();
	sf::Vector2f topRight(origin.x+rect.getSize().x, origin.y);
	sf::Vector2f bottomRight(origin.x+rect.getSize().x, origin.y+rect.getSize().y);
	sf::Vector2f bottomLeft(origin.x, origin.y+rect.getSize().y);
	points = {origin, topRight, bottomRight, bottomLeft};
	return points;
}

std::vector<Line*> get_lines(sf::RectangleShape &rect)
{
	std::vector<Line*> lines;
	sf::Vector2f origin = rect.getPosition();
	sf::Vector2f topRight(origin.x+rect.getSize().x, origin.y);
	sf::Vector2f bottomRight(origin.x+rect.getSize().x, origin.y+rect.getSize().y);
	sf::Vector2f bottomLeft(origin.x, origin.y+rect.getSize().y);

	lines.push_back(new Line(origin, topRight));
	lines.push_back(new Line(topRight, bottomRight));
	lines.push_back(new Line(bottomRight, bottomLeft));
	lines.push_back(new Line(bottomLeft, origin));

	return lines;
}

double get_slope(sf::Vector2f p1, sf::Vector2f p2)
{
	return (p2.y - p1.y)/(p2.x - p1.y);
}


void generate_lines(sf::Vector2f &origin, std::vector<sf::RectangleShape> &rects, std::vector<Line> &lines)
{
	for (auto rect : rects)
	{
		std::vector<sf::Vector2f> rect_points = get_points(rect);
		for(int i = 0; i < 4; i++)
		{
			sf::Vector2f rect_point = rect_points[i];
			sf::Vector2f extended = extend(origin,rect_point, 1000.0);
			sf::Vector2f down(extended.x + extended.x*cos(.00001), extended.y + extended.y*sin(.00001));
			sf::Vector2f up(extended.x - extended.x*cos(.00001), extended.y - extended.y*sin(.00001));
			Line next(origin, extended);
			Line down_line(origin, down);//Lines that go to the corner but don't intersect it
			Line up_line(origin, up);

			lines.push_back(next);
			lines.push_back(down_line);
			lines.push_back(up_line);
		}
	}
}

void get_rect_sides(std::vector<sf::RectangleShape> &rects, std::vector<Line> &lines)
{
	for (auto rect : rects)
	{
		std::vector<Line*> segs = get_lines(rect);
		for(auto line : segs)
		{
			lines.push_back(*line);
		}
	}
}

void shorten_lines(sf::Vector2f &origin, std::vector<Line> &lines, std::vector<Line> &rect_sides, std::vector<sf::Vector2f> &intersections, sf::RenderWindow &window)
{
	for(auto line : lines)
	{
		std::vector<sf::Vector2f> line_inters(4);
		for(auto side : rect_sides)
		{

			sf::Vector2f* inter = get_intersection(line, side);
			if(inter != NULL)
			{
				line_inters.push_back(*inter);

				//std::cout <<inter->x<<", "<< inter->y<<std::endl;
			}

		}
		for(auto p : line_inters)
		{
			//std::cout <<p.x<<", "<< p.y<<std::endl;
		}
		sf::Vector2f closest = closest_point(origin,line_inters);

		//std::cout <<closest.x<<", "<< closest.y<<std::endl;
		intersections.push_back(closest);
		sf::CircleShape dot;
		dot.setPosition(closest);
		window.draw(dot);
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

		std::cout <<points[i].x<<", "<< points[i].y<<std::endl;
		//std::cout<<"i: "<<i<<" "<<Line::magnitude(origin, points[i])<<std::endl;
		if(Line::magnitude(origin, points[min]) > Line::magnitude(origin, points[i]))
			min = i;
	}
	std::cout<<std::endl;
	return points[min];

}

// std::map automatically sorts elements by key
std::map<double, sf::Vector2f> sort_points(sf::Vector2f &origin, std::vector<sf::Vector2f> &points)
{
	std::map<double, sf::Vector2f> out;
	//std::cout<<points.size()<<std::endl;
	for(int i = 0; i < (int)points.size(); i++)
	{
		double slope = get_slope(points[i], origin);
		//std::cout<<slope<<std::endl;
		out.insert(std::pair<double, sf::Vector2f>(slope, points[i]));
	}
	for(auto p : points)
				{

					//std::cout <<p.x<<", "<< p.y<<std::endl;
				}
	return out;
}

void draw_triangles(sf::RenderWindow &window, sf::Vector2f &mouse, std::map<double, sf::Vector2f> &points)
{
	std::map<double, sf::Vector2f>::iterator it;
	for(it = points.begin(); it != points.end(); it++)
	{
		sf::Vector2f p1;
		sf::Vector2f p2;/*
		if(it++ == points.end())
		{
			it--;
			p1 = it->second;
			it = points.begin();
			p2 = it->second;
			break;
		}
		else*/
		{
			p1 = it->second;
			p2 = (++it)->second;
			it--;
		}
		sf::ConvexShape triangle;
		triangle.setPointCount(3);
		triangle.setPoint(0, mouse);
		triangle.setPoint(1, p1);
		triangle.setPoint(2, p2);
		triangle.setFillColor(sf::Color::White);
		window.draw(triangle);
	}
}


