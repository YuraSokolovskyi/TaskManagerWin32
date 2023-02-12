#pragma once
#include "string"
#include "windows.h"

class Task{
private:
    std::string title;
    std::string description;
    HWND wnd;
public:
    Task(){};
    Task(std::string, std::string);
    std::string getTitle();
    std::string getDescription();
    HWND getHwnd();
    void setTitle(std::string);
    void setDescription(std::string);
    void setTitle(char[]);
    void setDescription(char[]);
    void setHwnd(HWND);
};