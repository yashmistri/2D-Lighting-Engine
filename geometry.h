// Yash Mistri

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "line.h"

#include <SFML/System/Vector2.hpp>  // sf::Vector2f
#include <SFML/Graphics.hpp>   // sf::RectangleShape
#include <vector>

sf::Vector2f* get_intersection(Line &l1, Line &l2);
double cross(sf::Vector2f &v1, sf::Vector2f &v2);
sf::Vector2f extend(sf::Vector2f &p1, sf::Vector2f &p2, double length);
std::vector<sf::Vector2f> get_points(sf::RectangleShape &rect);
std::vector<Line*> get_lines(sf::RectangleShape &rect);
double get_slope(sf::Vector2f p1, sf::Vector2f p2);
double get_angle(sf::Vector2f p1, sf::Vector2f p2);
void generate_lines(sf::Vector2f &origin, std::vector<sf::ConvexShape> &shapes, std::vector<Line> &lines);
void get_shape_edges(std::vector<sf::ConvexShape> &shapes, std::vector<Line> &edges);
sf::Vector2f closest_point(sf::Vector2f &origin, std::vector<sf::Vector2f> &points);
void draw_triangles(sf::RenderWindow &window, sf::Vector2f &mouse, std::map<double, sf::Vector2f> &points, sf::Texture* light);

#endif
