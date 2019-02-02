//
// Created by Viktor Hundahl Strate on 01/02/2019.
//


#pragma once

#include <vector>
#include <string>
#include <functional>

namespace CLI {

    struct MenuItem {
        std::string title;
        void (*onSelect)();
    };

    struct Menu {
        std::string menuTitle;
        std::vector<MenuItem> items;

        void showMenu();
    };

    void start();
};

