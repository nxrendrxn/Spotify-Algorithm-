#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void trim(char* str) {
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
    while (*str && isspace(*str)) {
        memmove(str, str + 1, strlen(str));
    }
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

typedef struct Song {
    char title[50];
    struct Song* next;
} Song;

typedef struct Artist {
    char name[50];
    Song* songs;
    struct Artist* next;
} Artist;

typedef struct Emotion {
    char name[50];
    Artist* artists;
    struct Emotion* next;
} Emotion;

typedef struct Genre {
    char name[50];
    Emotion* emotions;
    struct Genre* next;
} Genre;

Genre* createGenre(const char* name) {
    Genre* newGenre = (Genre*)malloc(sizeof(Genre));
    strcpy(newGenre->name, name);
    newGenre->emotions = NULL;
    newGenre->next = NULL;
    return newGenre;
}

Emotion* createEmotion(const char* name) {
    Emotion* newEmotion = (Emotion*)malloc(sizeof(Emotion));
    strcpy(newEmotion->name, name);
    newEmotion->artists = NULL;
    newEmotion->next = NULL;
    return newEmotion;
}

Artist* createArtist(const char* name) {
    Artist* newArtist = (Artist*)malloc(sizeof(Artist));
    strcpy(newArtist->name, name);
    newArtist->songs = NULL;
    newArtist->next = NULL;
    return newArtist;
}

Song* createSong(const char* title) {
    Song* newSong = (Song*)malloc(sizeof(Song));
    strcpy(newSong->title, title);
    newSong->next = NULL;
    return newSong;
}

Genre* parseFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    Genre* genres = NULL;
    Genre* currentGenre = NULL;
    Emotion* currentEmotion = NULL;
    Artist* currentArtist = NULL;

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        trim(line);

        if (strncmp(line, "Genre: ", 7) == 0) {
            Genre* newGenre = createGenre(line + 7);
            trim(newGenre->name);
            if (!genres) {
                genres = newGenre;
            } else {
                currentGenre->next = newGenre;
            }
            currentGenre = newGenre;
            currentEmotion = NULL;
        } else if (strncmp(line, "Emotion: ", 9) == 0) {
            Emotion* newEmotion = createEmotion(line + 9);
            trim(newEmotion->name);
            if (!currentGenre->emotions) {
                currentGenre->emotions = newEmotion;
            } else {
                currentEmotion->next = newEmotion;
            }
            currentEmotion = newEmotion;
            currentArtist = NULL;
        } else if (strncmp(line, "Artist: ", 8) == 0) {
            Artist* newArtist = createArtist(line + 8);
            trim(newArtist->name);
            if (!currentEmotion->artists) {
                currentEmotion->artists = newArtist;
            } else {
                currentArtist->next = newArtist;
            }
            currentArtist = newArtist;
        } else if (strncmp(line, "Song: ", 6) == 0) {
            Song* newSong = createSong(line + 6);
            trim(newSong->title);
            if (!currentArtist->songs) {
                currentArtist->songs = newSong;
            } else {
                Song* temp = currentArtist->songs;
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = newSong;
            }
        }
    }

    fclose(file);
    return genres;
}

void queue(const char* songName, const char* artistName);

void searchSongs(Genre* genres, const char* genre, const char* emotion, const char* artist) {
    char lowerGenre[100], lowerEmotion[100], lowerArtist[100];
    strcpy(lowerGenre, genre);
    strcpy(lowerEmotion, emotion);
    strcpy(lowerArtist, artist);
    toLowerCase(lowerGenre);
    toLowerCase(lowerEmotion);
    toLowerCase(lowerArtist);

    printf("Searching for Genre: '%s', Emotion: '%s', Artist: '%s'\n", lowerGenre, lowerEmotion, lowerArtist);

    Genre* g = genres;
    while (g) {
        char genreName[100];
        strcpy(genreName, g->name);
        toLowerCase(genreName);

        printf("Comparing with Genre: '%s'\n", genreName);

        if (strcmp(genreName, lowerGenre) == 0) {
            Emotion* e = g->emotions;
            while (e) {
                char emotionName[100];
                strcpy(emotionName, e->name);
                toLowerCase(emotionName);

                if (strcmp(emotionName, lowerEmotion) == 0) {
                    Artist* a = e->artists;
                    while (a) {
                        char artistName[100];
                        strcpy(artistName, a->name);
                        toLowerCase(artistName);

                        if (strcmp(artistName, lowerArtist) == 0) {
                            printf("Songs by %s in %s (%s):\n", artist, genre, emotion);
                            Song* s = a->songs;
                            while (s) {
                                printf("  - %s\n", s->title);
                                queue(s->title,artist);
                                s = s->next;
                            }
                            return;
                        }
                        a = a->next;
                    }
                    printf("Artist '%s' not found under emotion '%s'.\n", artist, emotion);
                    return;
                }
                e = e->next;
            }
            printf("Emotion '%s' not found under genre '%s'.\n", emotion, genre);
            return;
        }
        g = g->next;
    }
    printf("Genre '%s' not found.\n", genre);
}


void freeMemory(Genre* genres) {
    while (genres) {
        Genre* g = genres;
        genres = genres->next;

        while (g->emotions) {
            Emotion* e = g->emotions;
            g->emotions = e->next;

            while (e->artists) {
                Artist* a = e->artists;
                e->artists = a->next;

                while (a->songs) {
                    Song* s = a->songs;
                    a->songs = s->next;
                    free(s);
                }
                free(a);
            }
            free(e);
        }
        free(g);
    }
}

void simulateCtrlL() {
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('L', 0, 0, 0);        
    keybd_event('L', 0, KEYEVENTF_KEYUP, 0); 
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); 
}

void simulateCtrlQ(){
    keybd_event(VK_CONTROL,0,0,0);
    keybd_event('Q',0,0,0);
    keybd_event('Q',0,KEYEVENTF_KEYUP,0);
    keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
}

void simulateTab(){
    keybd_event(VK_TAB,0,0,0);
    keybd_event(VK_TAB,0,KEYEVENTF_KEYUP,0);
}

void simulateEnter(){
    keybd_event(VK_RETURN,0,0,0);
    keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
}

void simulateDown(){
    keybd_event(VK_DOWN,0,0,0);
    keybd_event(VK_DOWN,0,KEYEVENTF_KEYUP,0);
}

void simulateTyping(const char *name, int intervalMs) {
    size_t len = strlen(name);
    for (size_t i = 0; i < len; i++) {
        INPUT inputs[4] = {0};
        int inputCount = 0;
        SHORT vkCode = VkKeyScan(name[i]);
        if (vkCode == -1) {
            continue;
        }

        BYTE virtualKey = vkCode & 0xFF;
        BYTE shiftState = (vkCode >> 8) & 0xFF;
        if (shiftState & 1) {
            inputs[inputCount].type = INPUT_KEYBOARD;
            inputs[inputCount].ki.wVk = VK_SHIFT;
            inputs[inputCount].ki.dwFlags = 0;
            inputCount++;
        }
        inputs[inputCount].type = INPUT_KEYBOARD;
        inputs[inputCount].ki.wVk = virtualKey;
        inputs[inputCount].ki.dwFlags = 0;
        inputCount++;
        inputs[inputCount].type = INPUT_KEYBOARD;
        inputs[inputCount].ki.wVk = virtualKey;
        inputs[inputCount].ki.dwFlags = KEYEVENTF_KEYUP;
        inputCount++;
        if (shiftState & 1) {
            inputs[inputCount].type = INPUT_KEYBOARD;
            inputs[inputCount].ki.wVk = VK_SHIFT;
            inputs[inputCount].ki.dwFlags = KEYEVENTF_KEYUP;
            inputCount++;
        }
        SendInput(inputCount, inputs, sizeof(INPUT));
        Sleep(intervalMs);
    }
}

void play(){
    simulateEnter();
    Sleep(5000);
    simulateTab();
    Sleep(2000);
    simulateTab();
    Sleep(2000);
    simulateEnter();
    Sleep(2000);
    simulateEnter();
}

void queue(const char* songName,const char* artistName){
    simulateCtrlL();
    Sleep(3000);
    simulateTyping(songName,100);
    simulateTyping(" ",100);
    simulateTyping(artistName,100);
    simulateEnter();
    Sleep(3000);
    simulateTab();
    Sleep(2000);
    simulateTab();
    Sleep(2000);
    simulateEnter();
    Sleep(3000);
    simulateCtrlQ();
    Sleep(2000);
    simulateDown();
    Sleep(2000);
    simulateDown();
    Sleep(2000);
    simulateDown();
    Sleep(2000);
    simulateEnter();
}


int main() {
    Genre* genres = parseFile("music_tree.txt");
    if (!genres) {
        return 1;
    }
    char name[20];
    printf("Enter the name of the song:");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    char genre[100], emotion[100], artist[100];
    printf("Enter genre: ");
    fgets(genre, sizeof(genre), stdin);
    genre[strcspn(genre, "\n")] = 0;
    trim(genre);

    printf("Enter emotion: ");
    fgets(emotion, sizeof(emotion), stdin);
    emotion[strcspn(emotion, "\n")] = 0;
    trim(emotion);

    printf("Enter artist: ");
    fgets(artist, sizeof(artist), stdin);
    artist[strcspn(artist, "\n")] = 0;
    trim(artist);
    ShellExecute(0, "open", "Spotify.exe", 0, 0, SW_SHOWDEFAULT);
    Sleep(5000); 
    simulateCtrlL();
    Sleep(2000);
    simulateTyping(name, 100);
    play();

    searchSongs(genres, genre, emotion, artist);

    freeMemory(genres);
    return 0;
}

