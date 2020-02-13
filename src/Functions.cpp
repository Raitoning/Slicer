#include "Functions.hpp"

#include <iostream>

#define LOG(X) std::cout << X << std::endl;

void Arc(Printer &printer, double radian, double rayon, double x, double y, double& e, int approximations, double angle_depart)
{
    std::ofstream &file = printer.GetFile();

    double alpha = radian / static_cast<double>(approximations);

    double oldX;
    double xTarget = x+cos(angle_depart)*rayon;
    double yTarget = y+sin(angle_depart)*rayon;
    double oldY;

    file << "G0 E" << (e-0.5) << std::endl;
    file << "G0 X" << (x+rayon) << " Y" << y << std::endl;
    file << "G0 E" << e << std::endl;

    for (int i = 0; i <= approximations; i++)
    {
        oldX = xTarget;
        oldY = yTarget;

        xTarget = (cos(alpha * i) * rayon) + x;
        yTarget = (sin(alpha * i) * rayon) + y;

        e += printer.GetExtruderValue(std::sqrt((oldX-xTarget) * (oldX-xTarget) + (oldY-yTarget) * (oldY-yTarget)));
        file << "G1 X" << xTarget << " Y" << yTarget << " E" << e << " F600" << std::endl;
    }
}

void Filled_Square(Printer &printer, double x, double y, double taille, double& e, int nombre_aller_retour, bool orientation){
    std::ofstream &file = printer.GetFile();

    file << "G1 E" << (e-0.5) << std::endl;
    file << "G0 X" << x << " Y" << y << std::endl;
    file << "G1 E" << e << std::endl;

    e += printer.GetExtruderValue(taille);
    file << "G1 X" << x+taille << " E" << e << " F600" << std::endl;

    e += printer.GetExtruderValue(taille);
    file << "G1 Y" << y+taille << " E" << e << " F600" << std::endl;

    e += printer.GetExtruderValue(taille);
    file << "G1 X" << x << " E" << e << " F600" << std::endl;

    e += printer.GetExtruderValue(taille);
    file << "G1 Y" << (y+printer.GetNozzleDiameter()) << " E" << e << " F600" << std::endl;
    file << "G0 X" << (x+printer.GetNozzleDiameter()) << std::endl;

    double step = taille / static_cast<double>(nombre_aller_retour);
    double newE = printer.GetExtruderValue(taille-2*printer.GetNozzleDiameter());

    bool sens = true;

    for(int i=0; i<nombre_aller_retour-1; i++){
        e+=newE;
        if(orientation){
            file << "G0 X" << (x+step*(i+1)) << std::endl;

            if(sens)
                file << "G1 Y" << (y+taille-printer.GetNozzleDiameter()) << " E" << e << " F600" << std::endl;
            else
                file << "G1 Y" << (y+printer.GetNozzleDiameter()) << " E" << e << " F600" << std::endl;
        }
        else
        {
            file << "G0 Y" << (y+step*(i+1)) << std::endl;

            if(sens)
                file << "G1 X" << (x+taille-printer.GetNozzleDiameter()) << " E" << e << " F600" << std::endl;
            else
                file << "G1 X" << (x+printer.GetNozzleDiameter()) << " E" << e << " F600" << std::endl;
        }
        sens = !sens;
    }
}


double distance(double x1, double y1, double x2, double y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}