#pragma once
#ifndef OPTION_H
#define OPTION_H

#include "client/context.h"
#include "client/ui/uipage.h"

class Option {
    public:
        Option(std::string section, std::string label, std::string key, std::string value, char type = 0) :
            Section(section), Label(label), Key(key), Value(value), Type(type) {};
        
        std::string Section, Label, Key, Value;
        char Type;
};

#endif

