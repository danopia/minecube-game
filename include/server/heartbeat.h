#pragma once
#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <fstream>

class Heartbeat {
    public:
        bool Beat();
        
    protected:
        std::string ReadKey();
        void SaveKey(std::string key);
};

#endif
