#include "Printer.hpp"
#include "Functions.hpp"
#include "Seance_1.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

void Square()
{
    Printer printer;
    printer.NewFile("square.gcode");

    ofstream &file = printer.GetFile();

    //Retenir la valeur d'extrusion du filament
    double e = 0.0;

    printer.ActivateFan();

    e += printer.GetExtruderValue(10);
    file << "G1 X110 E" << e << " F600" << endl;

    e += printer.GetExtruderValue(10);
    file << "G1 Y110 E" << e << " F600" << endl;

    e += printer.GetExtruderValue(10);
    file << "G1 X100 E" << e << " F600" << endl;

    e += printer.GetExtruderValue(10);
    file << "G1 Y100 E" << e << " F600" << endl;

    printer.CloseFile();
}

void Square_Wall()
{
    Printer printer;
    printer.NewFile("square_wall.gcode");

    ofstream &file = printer.GetFile();

    //Retenir la valeur d'extrusion du filament
    double e = 0.0;

    printer.ActivateFan();

    e += printer.GetExtruderValue(10);
    file << "G1 X110 E" << e << " F600" << endl;

    e += printer.GetExtruderValue(10);
    file << "G1 Y110 E" << e << " F600" << endl;

    e += printer.GetExtruderValue(10);
    file << "G1 X100 E" << e << " F600" << endl;

    e += printer.GetExtruderValue(10);
    file << "G1 Y100.4 E" << e << " F600" << endl;

    double layer = printer.GetLayerThickness();

    for (int i = 0; i < 25; i++)
    {
        e += printer.GetExtruderValue(10);
        file << "G1 X110 E" << e << " F600" << endl;

        e += printer.GetExtruderValue(10);
        file << "G1 Y110 E" << e << " F600" << endl;

        e += printer.GetExtruderValue(10);
        file << "G1 X100 E" << e << " F600" << endl;

        e += printer.GetExtruderValue(10);
        file << "G1 Y100.4 E" << e << " F600" << endl;

        layer += printer.GetLayerThickness();
        file << "G1 E" << (e-0.5) << endl;
        file << "G0 Y100 X100 Z" << layer << " F600" << endl;
        file << "G1 E" << e << endl;
    }

    printer.CloseFile();
}

void Square_Wall_Filled()
{
    Printer printer;
    printer.NewFile("square_wall_filled.gcode");

    //Retenir la valeur d'extrusion du filament
    double e = 0.0;

    printer.ActivateFan();

    bool orientation = true;;

    double layer = printer.GetLayerThickness();
    for(; layer < 10; layer+=printer.GetLayerThickness()){
        Filled_Square(printer, 100.0, 100.0, 10, e, 4, orientation);
        printer.NewLayer();
        orientation = !orientation;
    }

    printer.CloseFile();
}

void Circle()
{
    Printer printer;
    printer.NewFile("circle.gcode");

    double e = 0.0;

    Arc(printer, 2*M_PI, 4.0, 100.0, 100.0, e, 16);

    printer.CloseFile();
}

void Cylinder()
{
    Printer printer;
    printer.NewFile("cylinder.gcode");

    double e = 0.0;
    double layer = printer.GetLayerThickness();

    Arc(printer, 2*M_PI, 4.0, 100.0, 100.0, e, 16);

    printer.ActivateFan();

    for(; layer < 10; layer+=printer.GetLayerThickness()){
        printer.NewLayer();
        Arc(printer, 2*M_PI, 4.0, 100.0, 100.0, e, 16);
    }

    printer.CloseFile();
}


void Cylinder_Filled()
{
    Printer printer;
    printer.NewFile("cylinder_filled.gcode");
    ofstream& file = printer.GetFile();

    double e = 0.0;
    double layer = printer.GetLayerThickness();
    bool sens = true;
    bool fan = true;
    double rayon;
    double epaisseur = 0.4;

    for(; layer < 10; layer+=printer.GetLayerThickness()){
        //file << "G1 E" << e << endl;
        file << "G0 Z" << layer << endl;
        if(sens){
            rayon = 4.0-epaisseur;
            Arc(printer, 2*M_PI, 4.0, 100.0, 100.0, e, 16);
            for(; rayon > epaisseur; rayon -= epaisseur)
                Arc(printer, 2*M_PI, rayon, 100.0, 100.0, e, 16);
        }
        else{
            rayon = epaisseur;
            for(; rayon < (4.0-epaisseur); rayon += epaisseur)
                Arc(printer, 2*M_PI, rayon, 100.0, 100.0, e, 16);
            Arc(printer, 2*M_PI, 4.0, 100.0, 100.0, e, 16);
        }
        if(fan){
            printer.ActivateFan();
            fan = false;
        }
        sens = !sens;
        //file << "G0 Z" << (layer+1) << endl;
        //file << "G1 E" << (e-0.5) << endl;
        //file << "G4 P1000" << endl;
    }

    printer.CloseFile();
}