#include "windows.h"
#include "string"
#include "vector"
#include "tchar.h"
#include "prototypes.h"
#include "iostream"

int APIENTRY _tWinMain(HINSTANCE This, // Дескриптор текущего приложения
                       HINSTANCE Prev, // В современных системах всегда 0
                       LPTSTR cmd, // Командная строка
                       int mode){ // Режим отображения окна
    HWND hWnd; // Дескриптор главного окна программы
    MSG msg; // Структура для хранения сообщений
    WNDCLASS wc; // Класс окна

    // Определение окна
    wc.hInstance = This;
    wc.lpszClassName = WinName; // Имя класса окна
    wc.lpfnWndProc = WndProc; // Функция окна
    wc.style = CS_HREDRAW | CS_VREDRAW; // Стиль окна, обезпечивают перерисовку окна при изменении размера
    wc.hIcon - LoadIcon(NULL, IDI_APPLICATION); // Стандартная иконка
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Стандартный курсор
    wc.lpszMenuName = NULL; // Нет меню
    wc.cbClsExtra = 0; // Нет дополнительных данных класса
    wc.cbWndExtra = 0; // Нет дополнительнных данных окна
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Заполнение окна белым цветом

    if (!RegisterClass(&wc)) return 0; // Регестрация класса окна, если неудачно - выходим из программы

    // Создание окна
    hWnd = CreateWindow(
            WinName, // Имя класса окна
            _T("Title of the program"), // Загаловок окна
            WS_OVERLAPPEDWINDOW | WS_VSCROLL, // Стиль окна
            CW_USEDEFAULT, // х координаты левого верхнего угла
            CW_USEDEFAULT, // у координаты левого верхнего угла
            CW_USEDEFAULT, // Ширина
            CW_USEDEFAULT, // Высота
            HWND_DESKTOP, // Дескриптор родительского окна
            NULL, // Нет меню
            This, // Дескриптор приложения
            NULL // Дополнительной информации нет
    );
    ShowWindow(hWnd, mode); // Показать окно

    hInst = This;
    MainWnd = hWnd;

    while(GetMessage(&msg, NULL, 0, 0)){ // Цикл обработки сообщений
        TranslateMessage(&msg); // Функция трансляции кодов нажатой клавиши
        DispatchMessage(&msg); // Посылает сообщение функции WndProc()
    }

    return 0; // Завершение функции
}

// функция обработки событий
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    HWND tempWnd;
    HDC hdc;
    PAINTSTRUCT ps;
    int i, tempInt;
    double tempDouble;
    RECT tempRect;
    TCHAR buffer[10];

    switch (message) { // Обработчик событий
        case WM_CREATE:
            RegisterTaskClass();
            RegisterAddTaskClass();
            AddMenu(hWnd);
            break;
        case WM_DESTROY: // При закрытии программы
            PostQuitMessage(0); // Завершение программы
            break;
        case WM_SIZE:
            sx = LOWORD(lParam);
            sy = HIWORD(lParam);
            // std::cout << "size: " << sx << " " << sy << std::endl;
            fullY = CountTaskArea();
            if (fullY < sy) fullY = sy;
            // set taskContainers
            for (i = 0; i < tasks.size(); i++){
                std::cout << "Debug: " << i << "  " << TaskHeight * i + MarginBtwTasks * i << std::endl;
                SetWindowPos(tasksContainers[i], tasksContainers[i], 0, TaskHeight * i + MarginBtwTasks * i,
                             sx, TaskHeight, SWP_NOZORDER);
            }
            // set scroll
            if (taskArea <= sy){
                MainWndVScrolled = 0;
                tempInt = 0;
            }else tempInt = taskArea - sy;
            SetScrollRange(hWnd, SB_VERT, 0, tempInt, FALSE);
            SetScrollPos(hWnd, SB_VERT, ToPix(MainWndVScrolled, taskArea), TRUE);
            if (taskArea < sy){
                MainWndVScrolled = 0;
            }else if (tempDouble > 100 - ToPer(sy, taskArea)){
                MainWndVScrolled = tempDouble > 100 - ToPer(sy, taskArea);
                ScrollWindow(hWnd, 0, -ToPix(MainWndVScrolled, taskArea), NULL, NULL);
            }else{
                ScrollWindow(hWnd, 0, -ToPix(MainWndVScrolled, taskArea), NULL, NULL);
            }
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case MENU_ADD_TASK:
                    CreateAddTaskWnd(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_VSCROLL:
            switch (LOWORD(wParam)) {
                case SB_LINEDOWN:
                    tempDouble = ToPer(ToPix(MainWndVScrolled, taskArea) + PixelsByScroll, taskArea);
                    if (tempDouble > 100 - ToPer(sy, taskArea)){
                        tempDouble = 100 - ToPer(sy, taskArea);
                    }
                    std::cout << "DOWN: " << tempDouble << "  " << fullY << std::endl;
                    ScrollWindow(hWnd, 0, -1 * (ToPix(tempDouble, taskArea) - ToPix(MainWndVScrolled, taskArea)), NULL, NULL);
                    MainWndVScrolled = tempDouble;
                    SetScrollPos(hWnd, SB_VERT, ToPix(MainWndVScrolled, taskArea), TRUE);
                    break;
                case SB_LINEUP:
                    tempDouble = ToPer(ToPix(MainWndVScrolled, taskArea) - PixelsByScroll, taskArea);
                    if (tempDouble < 0) tempDouble = 0;
                    std::cout << "UP: " << tempDouble << "  " << fullY << std::endl;
                    ScrollWindow(hWnd, 0, ToPix(MainWndVScrolled, taskArea) - ToPix(tempDouble, taskArea), NULL, NULL);
                    MainWndVScrolled = tempDouble;
                    SetScrollPos(hWnd, SB_VERT, ToPix(MainWndVScrolled, taskArea), TRUE);
                    break;
                case SB_PAGEDOWN:
                    tempDouble = ToPer(ToPix(MainWndVScrolled, taskArea) + PixelsByScroll, taskArea);
                    if (tempDouble > 100 - ToPer(sy, taskArea)){
                        tempDouble = 100 - ToPer(sy, taskArea);
                    }
                    std::cout << "DOWN: " << tempDouble << "  " << fullY << std::endl;
                    ScrollWindow(hWnd, 0, -1 * (ToPix(tempDouble, taskArea) - ToPix(MainWndVScrolled, taskArea)), NULL, NULL);
                    MainWndVScrolled = tempDouble;
                    SetScrollPos(hWnd, SB_VERT, ToPix(MainWndVScrolled, taskArea), TRUE);
                    break;
                case SB_PAGEUP:
                    tempDouble = ToPer(ToPix(MainWndVScrolled, taskArea) - PixelsByScroll, taskArea);
                    if (tempDouble < 0) tempDouble = 0;
                    std::cout << "UP: " << tempDouble << "  " << fullY << std::endl;
                    ScrollWindow(hWnd, 0, ToPix(MainWndVScrolled, taskArea) - ToPix(tempDouble, taskArea), NULL, NULL);
                    MainWndVScrolled = tempDouble;
                    SetScrollPos(hWnd, SB_VERT, ToPix(MainWndVScrolled, taskArea), TRUE);
                    break;
                case SB_THUMBPOSITION:
                    tempDouble = ToPer(HIWORD(wParam), taskArea);
                    std::cout << "THUMB " << tempDouble << std::endl;
                    if (tempDouble > MainWndVScrolled){
                        ScrollWindow(hWnd, 0, -1 * (ToPix(tempDouble, taskArea) - ToPix(MainWndVScrolled, taskArea)), NULL, NULL);
                    }else{
                        ScrollWindow(hWnd, 0, ToPix(MainWndVScrolled, taskArea) - ToPix(tempDouble, taskArea), NULL, NULL);
                    }
                    MainWndVScrolled = tempDouble;
                    SetScrollPos(hWnd, SB_VERT, ToPix(MainWndVScrolled, taskArea), TRUE);
                    break;
            }
            break;
        default: // Обработка событий по умолчанию
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void AddMenu(HWND hWnd) {
    MainMenu = CreateMenu();
    AppendMenu(MainMenu, MF_STRING, MENU_ADD_TASK, _T("New task"));
    SetMenu(hWnd, MainMenu);
}

LRESULT TaskProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int i;
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case TaskContainerRemoveTask:
                    for (i = 0; tasks[i].getHwnd() != hWnd && i < tasks.size(); i++);
                    tasks.erase(tasks.begin() + i);
                    DeleteObject(tasks[i].getHwnd());
//                    InvalidateRect(MainWnd, NULL, TRUE);
//                    UpdateWindow(MainWnd);
                    for (i = 0; i < tasksContainers.size() && tasksContainers[i] != hWnd; i++);
                    DestroyWindow(tasksContainers[i]);
                    tasksContainers.erase(tasksContainers.begin() + i);
                    SendMessage(MainWnd, WM_SIZE, 0, sy << 16 | sx);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

ATOM RegisterTaskClass() {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = TaskProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszClassName = TaskClassName;
    return RegisterClassEx(&wc);
}

int max(int x, int y) {
    if (x > y) return x;
    return y;
}

int min(int x, int y) {
    if (x < y) return x;
    return y;
}

double ToPer(double p, double full) {
    return (p * 100) / full;
}

double ToPix(double p, double full) {
    return (p * full) / 100;
}

double CountTaskArea() {
    return tasks.size() * TaskHeight + (tasks.size()) * MarginBtwTasks;
}

LRESULT AddTaskProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    int len = 0;
    HWND tempWnd;
    LPSTR buffer;
    std::string title, description;
    Task *newTask;
    switch (message) {
        case WM_CTLCOLORSTATIC:
            hdc = (HDC)wParam;
            SetBkColor(hdc, RGB(255, 255, 255));
            return (LRESULT)GetStockObject(HOLLOW_BRUSH);
        case WM_CTLCOLOREDIT:
            hdc = (HDC)wParam;
            SetBkColor(hdc, RGB(200, 200, 200));
            return (LRESULT) GetStockObject(HOLLOW_BRUSH);
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case AddTaskWndAddBtn:
                    len = GetWindowTextLength(AddTaskTitleEdit);
                    buffer = (PSTR) VirtualAlloc((LPVOID)NULL, (DWORD)(len + 1), MEM_COMMIT, PAGE_READWRITE);
                    GetWindowText(AddTaskTitleEdit, buffer, len + 1);
                    title = buffer;

                    len = GetWindowTextLength(AddTaskDescriptionEdit);
                    buffer = (PSTR) VirtualAlloc((LPVOID)NULL, (DWORD)(len + 1), MEM_COMMIT, PAGE_READWRITE);
                    GetWindowText(AddTaskDescriptionEdit, buffer, len + 1);
                    description = buffer;
                    newTask = new Task(title, description);
                    tasks.push_back(*newTask);

                    CreateTaskContainer(*newTask);

                    DestroyWindow(AddTaskWnd);
                    break;
                case AddTaskWndCancel:
                    DestroyWindow(AddTaskWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_DESTROY:
            CloseWindow(AddTaskWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

ATOM RegisterAddTaskClass() {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = AddTaskProc;
    wc.hInstance = hInst;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = AddTaskClassName;
    wc.lpszMenuName = NULL;
    return RegisterClassEx(&wc);
}

void CreateAddTaskWnd(HWND hWnd) {
    AddTaskWnd = CreateWindow(AddTaskClassName, NULL, WS_CHILD | WS_VISIBLE | WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION,
                           CW_USEDEFAULT, CW_USEDEFAULT, 400, 500, hWnd, NULL, hInst, NULL);
    CreateWindow("static", "Title", WS_VISIBLE | WS_CHILD, 10, 10, 200, 20, AddTaskWnd, NULL, NULL, NULL);
    AddTaskTitleEdit = CreateWindow("edit", "task title", WS_VISIBLE | WS_CHILD, 10, 40, 200, 20, AddTaskWnd, NULL, NULL, NULL);
    CreateWindow("static", "Description", WS_VISIBLE | WS_CHILD, 10, 70, 200, 20, AddTaskWnd, NULL, NULL, NULL);
    AddTaskDescriptionEdit = CreateWindow("edit", "task description", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 10, 100, 200, 60, AddTaskWnd, NULL, NULL, NULL);

    CreateWindow("button", "Add task", WS_VISIBLE | WS_CHILD, 10, 170, 60, 30, AddTaskWnd, (HMENU)AddTaskWndAddBtn, NULL, NULL);
    CreateWindow("button", "Cancel", WS_VISIBLE | WS_CHILD, 100, 170, 60, 30, AddTaskWnd, (HMENU)AddTaskWndCancel, NULL, NULL);
}

void CreateTaskContainer(Task& task) {
    HWND tempWnd = CreateWindow(TaskClassName, NULL, WS_CHILD | WS_VISIBLE,
                           0, // x pos
                           0, // y pos
                           sx, // width
                           TaskHeight, // height
                           MainWnd, NULL, hInst, NULL);

    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
    SetClassLongPtr(tempWnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);

    CreateWindow("static", "Title: ", WS_VISIBLE | WS_CHILD, 10, 5, 60, 20, tempWnd, NULL, NULL, NULL);
    CreateWindow("static", tasks[tasks.size() - 1].getTitle().c_str(), WS_VISIBLE | WS_CHILD, 75, 5, 300, 20, tempWnd, NULL, NULL, NULL);
    CreateWindow("static", "Description: ", WS_VISIBLE | WS_CHILD, 10, 70, 100, 20, tempWnd, NULL, NULL, NULL);
    CreateWindow("static", tasks[tasks.size() - 1].getDescription().c_str(), WS_VISIBLE | WS_CHILD, 10, 100, (sx * 80) / 100, 80, tempWnd, NULL, NULL, NULL);

    CreateWindow("button", "Remove", WS_VISIBLE | WS_CHILD, sx - ToPix(10, sx), 10, min(ToPix(5, sx), 60), 40, tempWnd, (HMENU)TaskContainerRemoveTask, NULL, NULL);
    CreateWindow("button", "Edit", WS_VISIBLE | WS_CHILD, sx - ToPix(10, sx), 70, min(ToPix(5, sx), 60), 40, tempWnd, (HMENU)TaskContainerEditTask, NULL, NULL);


    task.setHwnd(tempWnd);
    tasksContainers.push_back(tempWnd);
    InvalidateRect(MainWnd, NULL, 1);

    fullY = CountTaskArea();
    if (fullY < sy) fullY = sy;
    taskArea = CountTaskArea();
    SendMessage(MainWnd, WM_SIZE, 0, sy << 16 | sx);
}
