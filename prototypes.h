#pragma once
#include "Task.h"

// events
#define MENU_ADD_TASK 0
#define AddTaskWndAddBtn 2001
#define AddTaskWndCancel 2002
#define TaskContainerRemoveTask 3001
#define TaskContainerEditTask 3002

// constants
#define TaskHeight 190
#define MarginBtwTasks 20
#define PixelsByScroll 300

// prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TaskProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AddTaskProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegisterTaskClass();
ATOM RegisterAddTaskClass();
void AddMenu(HWND);
int max(int, int);
int min(int, int);
double ToPer(double, double);
double ToPix(double, double);
double CountTaskArea();
void CreateAddTaskWnd(HWND);
void CreateTaskContainer(Task&);

// variables
TCHAR WinName[] = _T("MainFrame");
TCHAR TaskClassName[] = _T("TaskWindow");
TCHAR AddTaskClassName[] = _T("AddTaskClass");
HINSTANCE hInst;
HMENU MainMenu;
std::vector<Task> tasks;
std::vector<HWND> tasksContainers;
double MainWndVScrolled = 0;
int sx, sy, fullY, taskArea;
HWND AddTaskWnd, AddTaskTitleEdit, AddTaskDescriptionEdit, MainWnd;
