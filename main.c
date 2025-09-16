#include <windows.h>
#include <string.h>
#include <stdio.h>

void simulateCtrlL() {
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('L', 0, 0, 0);
    keybd_event('L', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
}

void simulateTyping(const char *text, int intervalMs) {
    size_t len = strlen(text);
    for (size_t i = 0; i < len; i++) {
        INPUT ip = {0};
        ip.type = INPUT_KEYBOARD;
        ip.ki.wVk = VkKeyScan(text[i]) & 0xFF;
        ip.ki.dwFlags = 0;
        SendInput(1, &ip, sizeof(INPUT));
        
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
        
        Sleep(intervalMs);
    }
}

int main() {
    ShellExecute(0, "open", "Spotify.exe", 0, 0, SW_SHOWDEFAULT);
    Sleep(20000);

    char name[100];
    printf("Enter the song name: ");
    
    fgets(name, sizeof(name), stdin);
    
    size_t len = strlen(name);
    if (len > 0 && name[len - 1] == '\n') {
        name[len - 1] = '\0';
    }

    simulateCtrlL();
    Sleep(5000);
    simulateTyping(name, 100);

    return 0;
}