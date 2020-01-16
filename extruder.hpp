#pragma once
#include <math.h>

struct extruder{
    private:
        static double filament_thickness_square;
        static double filament_surface;
    static double layer_thickness;
    static double nozzle_diameter;
    static double filament_thickness;
        

    extruder(double t=0.2, double nw = 0.4, double ft = 1.75);

    static double value(double distance);
};

static extruder static_block;

extruder::extruder(double t, double nw, double ft){
    extruder::layer_thickness = t;
    extruder::nozzle_diameter = nw;
    extruder::filament_thickness = ft;
    extruder::filament_thickness_square = ft*ft;
    extruder::filament_surface = M_PI * extruder::filament_thickness_square;
}

double extruder::value(double distance){
    double volume = extruder::nozzle_diameter * extruder::layer_thickness * distance;

    double res = volume / extruder::filament_surface;
    return res;
}