#pragma once
#include <fstream>
#include <ostream>
#include <string>

class Printer
{
private:
    double m_FilamentThicknessSquare;
    double m_FilamentSurface;
    double m_LayerThickness;
    double m_NozzleDiameter;
    double m_FilamentThickness;
    std::ofstream file;

public:
    Printer(double t = 0.2, double nw = 0.4, double ft = 1.75);

    void NewFile(std::string filename);
    void CloseFile();
    std::ofstream& GetFile();

    double GetExtruderValue(double distance);
    void ActivateFan(bool state=true);
    void ResetExtruderValue();
    void NewLayer();

    double GetFilamentThickness();
    double GetLayerThickness();
    double GetNozzleDiameter();

    void SetFilamentThickness(double value);
    void SetLayerThickness(double value);
    void SetNozzleDiameter(double value);
};
