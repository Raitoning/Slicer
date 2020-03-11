#include "Printer.hpp"
#include "Functions.hpp"
#include "Seance_2.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>

#define LOG(X) std::cout << X << std::endl

using namespace std;

void Hemisphere_Filled()
{
    Printer printer;
    printer.NewFile("Hemisphere_Filled.gcode");

    ofstream &file = printer.GetFile();

    printer.SetLayerThickness(0.3);

    double e = 0.0;
    double init_radius = 10.0;
    double curr_radius;
    double next_radius;
    double layer = printer.GetLayerThickness();
    double next_layer;
    bool activate_fan = true;
    bool direction = true;
    double space = 2.0;
    double begin_x = 100.0 + init_radius;
    double begin_y = 100.0 + init_radius;

    //Variables pour les boucles
    double x, y;
    int signe = 1;
    double d;

    next_radius = cos(asin(layer / init_radius)) * init_radius;

    while (layer < init_radius)
    {
        curr_radius = next_radius;

        //Calcul du prochain rayon
        next_layer = layer+cos(layer / init_radius) * 0.2 + 0.1;
        //cout << "\tRadius: " << curr_radius;
        if( (next_layer / init_radius) < 1.0){
            next_radius = cos(asin(next_layer / init_radius)) * init_radius;
        }
        //Asin ne fonctionne pas pour une entrée >=1. On considère le prochain rayon à 0.
        else{
            next_radius = 0;
        }
        //cout << "\tNext_radius: " << next_radius << "\tLayerHeight: " << printer.GetLayerThickness() << endl;

        Arc(printer, 2 * M_PI, curr_radius, 100.0, 100.0, e, 32);

        //On fait des contours tant que ce contour sera visible de l'extérieur
        while(curr_radius > next_radius){
            curr_radius -= printer.GetNozzleDiameter();
            Arc(printer, 2 * M_PI, curr_radius, 100.0, 100.0, e, 32);
        }

        //Lignes verticales droite -> gauche
        if (direction)
        {
            x = begin_x;
            while (x > 100.0 + curr_radius - printer.GetNozzleDiameter())
                x -= space;

            while (x > 100.0 - curr_radius + printer.GetNozzleDiameter())
            {
                y = acos((x - 100) / curr_radius);    //angle correspondant au cos (x). [0, PI]
                y = sin(y) * curr_radius;             //bord haut du cercle
                y -= printer.GetNozzleDiameter() / 2; //ne pas 'trop' recouvrir le cercle. /2 parce qu'il faut coller au cercle.
                y *= signe;                           //Commencer d'en haut ou d'en bas?

                file << "G0 X" << x << " Y" << (100 + y) << " F600" << endl;

                d = sqrt((y * 2) * (y * 2));
                e += printer.GetExtruderValue(d);
                file << "G1 Y" << (100 - y) << " E" << e << " F600" << endl;

                x -= space;
                signe *= -1;
            }
        }
        //Lignes horizontales haut -> bas
        else
        {
            y = begin_y;
            while (y > 100.0 + curr_radius - printer.GetNozzleDiameter())
                y -= space;

            while (y > 100.0 - curr_radius + printer.GetNozzleDiameter())
            {
                x = asin((y - 100) / curr_radius);    //angle correspondant au sin (y). [-PI/2, PI/2]
                x = cos(x) * curr_radius;             //bord haut du cercle
                x -= printer.GetNozzleDiameter() / 2; //ne pas 'trop' recouvrir le cercle. /2 parce qu'il faut coller au cercle.
                x *= signe;                           //Commencer d'en haut ou d'en bas?
                file << "G0 X" << (100 + x) << " Y" << y << "F600" << endl;

                d = sqrt((x * 2) * (x * 2));
                e += printer.GetExtruderValue(d);
                file << "G1 X" << (100 - x) << " E" << e << " F600" << endl;

                y -= space;
                signe *= -1;
            }
        }
        direction = !direction;

        printer.SetLayerThickness(cos(layer / init_radius) * 0.2 + 0.1);
        printer.NewLayer();
        layer += printer.GetLayerThickness();

        if (activate_fan)
        {
            printer.ActivateFan();
            activate_fan = false;
        }
    }

    printer.CloseFile();
}

void Cube_Filled_Sparse()
{
    Printer printer;

    printer.NewFile("Cube_Filled_Sparse.gcode");

    ofstream &file = printer.GetFile();

    double e = 0.0;
    double layer;
    double size = 24.0;
    double height = size;
    double space = 4;
    double d_space = space * sqrt(2);
    bool direction;
    double begin_x = 100.0 - size / 2 + printer.GetNozzleDiameter();
    double begin_y = 100.0 - size / 2 + printer.GetNozzleDiameter();
    bool activate_fan = true;
    double haut = 100 + size / 2 - printer.GetNozzleDiameter() / 2;
    double bas = 100 - size / 2 + printer.GetNozzleDiameter() / 2;
    double gauche = 100 - size / 2 + printer.GetNozzleDiameter() / 2;
    double droite = 100 + size / 2 - printer.GetNozzleDiameter() / 2;

    double x, y;
    double tmp;

    for (layer = printer.GetLayerThickness(); layer < height; layer += printer.GetLayerThickness())
    {

        //Sens 45°
            direction = true;
            //Si il faut commencer un nouveau mur
            while (begin_x - d_space > gauche)
                begin_x -= d_space;

            //Commencer par le mur du haut, à gauche
            for (x = begin_x; x < droite; x += d_space)
            {
                y = 200 - x;
                e += printer.GetExtruderValue(distance(x, haut, gauche, y));
                if (direction)
                {
                    file << "G1 E" << (e-1) << endl;
                    file << "G0 X" << x << " Y" << haut <<  " F2000" << endl;
                    file << "G1 E" << e << endl;
                    file << "G1 X" << gauche << " Y" << y << " E" << e << " F2000" << endl;
                }
                else
                {
                    file << "G1 E" << (e-1) << endl;
                    file << "G0 X" << gauche << " Y" << y << " F2000" << endl;
                    file << "G1 E" << e << endl;
                    file << "G0 X" << x << " Y" << haut << " E" << e << " F2000" << endl;
                }

                direction = !direction;
            }

            //Commencer par le mur de droite
            //difference
            tmp = (100.0 + size / 2 - x);
            y = 100.0 + size / 2 - printer.GetNozzleDiameter() + tmp;
            for (; y  > bas; y -= d_space)
            {
                x = 200 - y;
                e += printer.GetExtruderValue(distance(droite, y, x, bas));
                if (direction)
                {
                    file << "G1 E" << (e-1) << endl;
                    file << "G0 X" << droite << " Y" << y << " F2000" << endl;
                    file << "G1 E" << e << endl;
                    file << "G1 X" << x << " Y" << bas << " E" << e << " F2000" << endl;
                }
                else
                {
                    file << "G1 E" << (e-1) << endl;
                    file << "G0 X" << x << " Y" << bas << " F2000" << endl;
                    file << "G1 E" << e << endl;
                    file << "G1 X" << droite << " Y" << y << " E" << e << " F2000" << endl;
                }

                direction = !direction;
            }

        //Sens -45°
            direction = true;

            //Commencer par le mur de droite
            for (x = begin_x; x < droite; x += d_space)
            {
                y = x;
                e += printer.GetExtruderValue(distance(x, bas, gauche, y));
                if (direction)
                {
                    file << "G1 E" << (e-1) << endl;
                    file << "G0 X" << x << " Y" << bas << " F2000" << endl;
                    file << "G1 E" << e << endl;
                    file << "G1 X" << gauche << " Y" << y << " E" << e << " F2000" << endl;
                }
                else
                {
                    file << "G1 E" << (e-1) << endl;
                    file << "G0 X" << gauche << " Y" << y << " F2000" << endl;
                    file << "G1 E" << e << endl;
                    file << "G1 X" << x << " Y" << bas << " E" << e << " F2000" << endl;
                }

                direction = !direction;
            }

            //Commencer par le mur du haut
            //difference
            tmp = (100.0 + size / 2 - x);
            y = 100.0 - size / 2 + printer.GetNozzleDiameter() - tmp;
            for (; y  < haut; y += d_space)
            {
            
                x = y;
                e += printer.GetExtruderValue(distance(droite, y, x, haut));
                if (direction)
                {
                    file << "G1 E" << (e-1) << endl;
                    file << "G0 X" << droite << " Y" << y << " F2000" << endl;
                    file << "G1 E" << e << endl;
                    file << "G1 X" << x << " Y" << haut << " E" << e << " F2000" << endl;
                }
                else
                {
                    file << "G1 E" << (e-1) << endl;
                    file << "G0 X" << x << " Y" << haut << " F2000" << endl;
                    file << "G1 E" << e << endl;
                    file << "G1 X" << droite << " Y" << y << " E" << e << " F2000" << endl;
                }

                direction = !direction;
            }

        //Sens 0°
            direction = true;
            //Si il faut commencer un nouveau mur
            while (begin_y - space > bas)
                begin_y -= space;

            x = (100.0 + size / 2 - (printer.GetNozzleDiameter() / 2));

            file << "G1 E" << (e-1) << endl;
            file << "G0 X" << x << " F2000" << endl;
            file << "G1 E" << e << endl;

            for (y = begin_y; y  < haut; y += space)
            {
                file << "G1 E" << (e-1) << endl;
                file << "G0 Y" << y << " F2000" << endl;
                file << "G1 E" << e << endl;

                if (direction)
                    x = gauche;
                else
                    x = droite;

                e += printer.GetExtruderValue(size - printer.GetNozzleDiameter());
                file << "G1 X" << x << " E" << e << " F2000" << endl;
                direction = !direction;
            }

        printer.NewLayer();
        begin_x += printer.GetNozzleDiameter() / 2 * sqrt(2);
        begin_y += printer.GetNozzleDiameter() / 2;

        if (activate_fan)
        {
            printer.ActivateFan();
            activate_fan = false;
        }
    }

    printer.CloseFile();
}
