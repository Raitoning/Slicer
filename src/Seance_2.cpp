#include "Printer.hpp"
#include "Functions.hpp"
#include "Seance_2.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>

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
    double layer = printer.GetLayerThickness();
    bool activate_fan = true;
    bool direction = true;
    double space = 5.0;
    double begin_x = 100.0 + curr_radius;
    double begin_y = 100.0 + curr_radius;

    //Variables pour les boucles
    double x, y;
    int signe = 1;
    double d;

    while(layer < init_radius){
        curr_radius = cos(layer/init_radius)*init_radius;
        Arc(printer, 2*M_PI, curr_radius, 100.0, 100.0, e, 16);

        //Lignes verticales droite -> gauche
        if(direction){
            x = begin_x;
            while(x > 100.0+curr_radius-printer.GetNozzleDiameter())
                x -= space;
            
            while(x > 100.0-curr_radius+printer.GetNozzleDiameter()){
                y = acos(x/curr_radius);//angle correspondant au cos (x). [0, PI]
                y = sin(y)*curr_radius;//bord haut du cercle
                y -= printer.GetNozzleDiameter()/2; //ne pas 'trop' recouvrir le cercle. /2 parce qu'il faut coller au cercle.
                y *= signe;//Commencer d'en haut ou d'en bas?

                file << "G0 X" << x << " Y" << y << "F600" << endl;

                d = sqrt((y*2)*(y*2));
                e += printer.GetExtruderValue(d);
                file << "G1 Y" << y << " E" << e << " F600" << endl;

                x -= space;
                signe *= -1;
            }
        }
        //Lignes horizontales haut -> bas
        else{
            y = begin_y;
            while(y > 100.0+curr_radius-printer.GetNozzleDiameter())
                y -= space;
            
            while(y > 100.0-curr_radius+printer.GetNozzleDiameter()){
                x = asin(y/curr_radius);//angle correspondant au sin (y). [-PI/2, PI/2]
                x = cos(x)*curr_radius;//bord haut du cercle
                x -= printer.GetNozzleDiameter()/2; //ne pas 'trop' recouvrir le cercle. /2 parce qu'il faut coller au cercle.
                x *= signe;//Commencer d'en haut ou d'en bas?

                file << "G0 X" << x << " Y" << y << "F600" << endl;

                d = sqrt((x*2)*(x*2));
                e += printer.GetExtruderValue(d);
                file << "G1 X" << x << " E" << e << " F600" << endl;

                y -= space;
                signe *= -1;
            }
        }
        direction = !direction;

        printer.SetLayerThickness(cos(layer/init_radius)*0.2 + 0.1);
        printer.NewLayer();
        layer += printer.GetLayerThickness();

        if(activate_fan){
            printer.ActivateFan();
            activate_fan = false;
        }
    }

    printer.CloseFile();
}

void Square_Filled_Sparse(){
    Printer printer;

    printer.NewFile("Square_Filled_Spare.gcode");

    ofstream &file = printer.GetFile();

    double e = 0.0;
    double height;
    double layer;
    double size = 10.0;
    double space = 0.4;
    bool direction;
    double begin_x = 100.0 - size/2 + printer.GetNozzleDiameter();
    double begin_y = 100.0 - size/2 + printer.GetNozzleDiameter();
    bool activate_fan = true;
    double haut = 100 + size/2 - printer.GetNozzleDiameter()/2;
    double bas = 100 - size/2 + printer.GetNozzleDiameter()/2;
    double gauche = 100 - size/2 + printer.GetNozzleDiameter()/2;
    double droite = 100 + size/2 - printer.GetNozzleDiameter()/2;

    double x,y;
    double tmp;

    for(layer = printer.GetLayerThickness(); layer < size; layer += printer.GetLayerThickness()){

        file << "G0 X" << (100.0+size/2) << " Y" << (100.0-size/2) << " F600" << endl;
        e += printer.GetExtruderValue(size);
        file << "G1 Y" << (100.0+size/2) << " E" << e << " F600" << endl;
        e += printer.GetExtruderValue(size);
        file << "G1 X" << (100.0-size/2) << " E" << e << " F600" << endl;
        e += printer.GetExtruderValue(size);
        file << "G1 Y" << (100.0-size/2) << " E" << e << " F600" << endl;
        e += printer.GetExtruderValue(size);
        file << "G1 X" << (100.0+size/2) << " E" << e << " F600" << endl;

        //Sens 45°
            direction = true;
            //Si il faut commencer un nouveau mur
            while(begin_x-space > gauche )
                begin_x -= space;
            
            //Commencer par le mur du haut, à gauche
            for(x = begin_x; x < droite; x += space){
                y = 200 - x;
                e += printer.GetExtruderValue(distance(x, gauche, haut, y));
                if(direction){
                    file << "G0 X" << x << " Y" << haut << " F600" << endl;
                    file << "G1 X" << gauche << " Y" << y << " E" << e << " F600" << endl;
                }
                else{
                    file << "G0 X" << gauche << " Y" << y << " F600" << endl;
                    file << "G0 X" << x << " Y" << haut << " E" << e << " F600" << endl;
                }
                
                direction = !direction;
            }

            //Commencer par le mur de droite
            //difference
            tmp = 100.0 + size/2 - x;
            y = 100.0 + size/2 - printer.GetNozzleDiameter() - tmp;
            for(; y > bas; y -= space){
                x = 200 - y;
                e += printer.GetExtruderValue(distance(x, droite, bas, y));
                if(direction){
                    file << "G0 X" << droite << " Y" << y << " F600" << endl;
                    file << "G0 X" << x << " Y" << bas << " E" << e << " F600" << endl;
                }
                else{
                    file << "G0 X" << x << " Y" << bas << " F600" << endl;
                    file << "G0 X" << droite << " Y" << y << " E" << e << " F600" << endl;
                }

                direction = !direction;
            }

        //Sens -45°
            direction = true;
            //Si il faut commencer un nouveau mur
            while(begin_x-space > gauche )
                begin_x -= space;
            
            //Commencer par le mur de droite
            for(x = begin_x; x < droite; x += space){
                y = x;
                e += printer.GetExtruderValue(distance(x, gauche, bas, y));
                if(direction){
                    file << "G0 X" << x << " Y" << bas << " F600" << endl;
                    file << "G1 X" << gauche << " Y" << y << " E" << e << " F600" << endl;
                }
                else{
                    file << "G0 X" << gauche << " Y" << y << " F600" << endl;
                    file << "G0 X" << x << " Y" << bas << " E" << e << " F600" << endl;
                }

                direction = !direction;
            }

            //Commencer par le mur du haut
            //difference
            tmp = 100.0 + size/2 - x;
            y = 100.0 - size/2 + printer.GetNozzleDiameter() + tmp;
            for(; y < haut; y += space){
                y = x;
                e += printer.GetExtruderValue(distance(x, droite, haut, y));
                if(direction){
                    file << "G0 X" << droite << " Y" << y << " F600" << endl;
                    file << "G1 X" << x << " Y" << haut << " E" << e << " F600" << endl;
                }
                else{
                    file << "G0 X" << x << " Y" << haut << " F600" << endl;
                    file << "G0 X" << droite << " Y" << y << " E" << e << " F600" << endl;
                }

                direction = !direction;
            }

        //Sens 0°
            direction = true;
            //Si il faut commencer un nouveau mur
            fin = 100.0 - size/2 + printer.GetNozzleDiameter();
            while(begin_y-space > fin )
                begin_y -= space;

            
            fin = 100.0 + size/2 - printer.GetNozzleDiameter();
            x = (100.0+size/2- (printer.GetNozzleDiameter()/2));
            file << "G0 X" << x << " F600" << endl;
            for(y = begin_y; y < fin; y += space){
                file << "G0 Y" << y << " F600" << endl;
                if (direction){
                    x = (100.0-size/2)+(printer.GetNozzleDiameter()/2);
                }else{
                    x = (100.0+size/2)-(printer.GetNozzleDiameter()/2);
                }
                e += printer.GetExtruderValue(size - printer.GetNozzleDiameter());
                file << "G1 X" << x << " E" << e << " F600" << endl;
                direction = !direction;
            }

        printer.NewLayer();

        if(activate_fan){
            printer.ActivateFan();
            activate_fan = false;
        }
    }

    printer.CloseFile();
}