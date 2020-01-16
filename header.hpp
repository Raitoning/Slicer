#pragma once
#include <iostream>

std::ostream new_header(){
    std::ostream res;

    res << "G21; millimiters" << std::endl;
    res << "G90; absolute positioning" << std::endl;
    res << "G28; auto home" << std::endl;

    res << "M104 S210; extruder a 210" << std::endl;
    res << "M140 S60; bed a 60" << std::endl;

    res << "M190 S210; attendre que l'extruder soit a 210" << std::endl;
    res << "M109 S60; attendre que le bed soit a 60" << std::endl;

    res << "G92 E0; reset E" << std::endl;
    
}