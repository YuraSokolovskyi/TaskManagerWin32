#include "Task.h"
#include "string"

Task::Task(std::string _title, std::string _description) {
    title = std::move(_title);
    description = std::move(_description);
}

std::string Task::getTitle() {
    return title;
}

std::string Task::getDescription() {
    return description;
}

void Task::setTitle(std::string _title) {
    title = std::move(_title);
}

void Task::setDescription(std::string _description) {
    description = std::move(_description);
}

void Task::setDescription(char *_description) {
    description = _description;
}

void Task::setTitle(char *_title) {
    title = _title;
}

HWND Task::getHwnd() {
    return wnd;
}

void Task::setHwnd(HWND _wnd) {
    wnd = _wnd;
}
