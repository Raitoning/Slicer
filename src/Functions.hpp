#pragma once

#include <fstream>
#include <cmath>

#include "Printer.hpp"

void Arc(Printer &printer, double radian, double rayon, double x, double y, double& e, int approximations, double angle_depart=0.0);

void Filled_Square(Printer &printer, double x, double y, double taille, double& e, int nombre_aller_retour, bool orientation);

double distance(double x1, double y1, double x2, double y2);