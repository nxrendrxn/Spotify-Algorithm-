import os
import pyautogui
import time

os.system("Spotify")
time.sleep(10)
pyautogui.hotkey('ctrl','l')
pyautogui.write("Timeless",interval=0.1)

for key in ['enter','tab','tab','enter','enter']:
    time.sleep(0.5)
    pyautogui.press(key)

pyautogui.hotkey('ctrl','l')
pyautogui.write("Is there someone else?",interval=0.1)

for key in ['enter','tab','tab','enter','right','right','right','enter','down','down','down','enter']:
    time.sleep(0.5)
    pyautogui.press(key)

pyautogui.hotkey('ctrl','l')
pyautogui.write("Blinding lights",interval=0.1)

for key in ['enter','tab','tab','enter','right','right','right','enter','down','down','down','enter']:
    time.sleep(0.5)
    pyautogui.press(key)
