#include "Printer.hpp"
#include <cmath>
#include <iomanip>

using namespace std;

Printer::Printer(double t, double nw, double ft)
{
    this->m_LayerThickness = t;

    this->m_NozzleDiameter = nw;
    this->m_FilamentThickness = ft;
    this->m_FilamentThicknessSquare = ft * ft;
    this->m_FilamentSurface = M_PI * this->m_FilamentThicknessSquare;
}

double Printer::GetExtruderValue(double distance)
{
    double volume = this->m_NozzleDiameter * this->m_LayerThickness * distance;
    double res = volume / (this->m_FilamentSurface / 4.0);

    return res;
}

void Printer::ActivateFan(bool state){
    if(state)
        file << "M106 S255;\tActiver le ventilateur" << endl;
    else
        file << "M107;\tDesactiver le ventilateur" << endl;
}

void Printer::ResetExtruderValue(){
    file << "G92 E0; reset E -> extruder value" << endl;
}

void Printer::NewLayer(){
    file << "G91; coordonnees relatives" << endl;
    file << "G0 Z" << m_LayerThickness << endl;
    file << "G90; coordonnees absolues" << endl;
}

void Printer::NewFile(string filename){
    this->file.open(filename);
    file << std::fixed << std::setprecision(6);

    file << "M104 S210; extruder a 210" << endl;
    file << "M140 S60; bed a 60" << endl;

    file << "G21; millimeters" << endl;
    file << "G90; absolute positioning" << endl;
    file << "G28; auto home" << endl;

    file << "M109 S210; attendre que l'extruder soit a 210" << endl;
    file << "M190 S60; attendre que le bed soit a 60" << endl;

    ResetExtruderValue();
    file << "G0 Z" << this->m_LayerThickness << " Y0.5 F3600; Se positionner en Z" << endl;
    file << "M106 S255; Activer le ventilateur" << endl;
    file << "G1 X100 E15 F1200; Se centrer en X & Purger le fil" << endl;
    file << "M107;" << endl;
    file << "G1 E14.5; Retracter legerement le fil" << endl;
    file << "G0 Y100 F3600; Se centrer en Y" << endl;
    file << "G1 E15; remet le fil au bon endroit" << endl;
    ResetExtruderValue();
    file << ";HEADER;" << endl << endl << endl;
}

void Printer::CloseFile(){
    file << endl << endl << ";FOOTER;" << endl;
    ResetExtruderValue();
    file << "G1 E-5; Eviter de laisser un fil a la fin" << endl;
    file << "G91; coordonnees relative, pour monter simplement" << endl;
    file << "G0 Z5 F1200; Monter pour eviter d'endommager la structure" << endl;
    file << "G90; absolute positioning" << endl;
    file << "G0 Y200 F3600; Permet de 'tirer la langue' avec le plateau" << endl;
    file << "G0 X0 F3600; Retour en 0 pour le X" << endl;
    file << "M104 S0; Extruder a 0 degre" << endl;
    file << "M140 S0; Bed a 0 degree" << endl;
    file << "M107; desactiver le ventilateur" << endl;

    this->file.close();
}

ofstream& Printer::GetFile(){ return this->file; }

double Printer::GetFilamentThickness() { return this->m_FilamentThickness; }
double Printer::GetLayerThickness() { return this->m_LayerThickness; }
double Printer::GetNozzleDiameter() { return this->m_NozzleDiameter; }

void Printer::SetFilamentThickness(double value) { this->m_FilamentThickness = value; }
void Printer::SetLayerThickness(double value) { this->m_LayerThickness = value; }
void Printer::SetNozzleDiameter(double value) { this->m_NozzleDiameter = value; }
