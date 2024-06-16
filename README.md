# Project Title: My OS

![Project Screenshot](https://github.com/ShahbazGhafil/My-Operating-System-Project/blob/main/img.png "Project Screenshot")

## Table of Contents
* [General Info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)
* [Usage](#usage)
* [Credits](#credits)
* [License](#license)

## General Info
This project is a collection of six distinct applications:

1. **Digital Clock**: A simple digital clock implemented using GTK.
2. **File Manager**: A basic file manager with functionalities like create, copy, move, delete, get info, and rename files, implemented using GTK.
3. **Minesweeper Game**: A classic Minesweeper game implemented using SDL2.
4. **Notepad**: A basic text editor implemented using GTK.
5. **Music Player**: A simple music player implemented using SDL2.
6. **Calculator**: A basic calculator implemented using GTK.

## Technologies
The project is created with:
* GTK+ 3.0
* SDL2
* SDL2_image
* SDL2_mixer (for the Music Player)

## Setup
To run this project, follow these steps:

1. **Clone the repository:**
    ```bash
    git clone https://github.com/yourusername/yourproject.git
    cd yourproject
    ```

2. **Install dependencies:**
    - For GTK (Digital Clock, File Manager, Notepad, Calculator):
      ```bash
      sudo apt-get install libgtk-3-dev
      ```
    - For SDL (Minesweeper Game, Music Player):
      ```bash
      sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev
      ```

3. **Compile the code:**
    - Digital Clock:
      ```bash
      gcc clock.c -o clock `pkg-config --cflags --libs gtk+-3.0`
      ```
    - File Manager:
      ```bash
      gcc file_manager.c -o file_manager `pkg-config --cflags --libs gtk+-3.0 gio-2.0`
      ```
    - Minesweeper Game:
      ```bash
      gcc minigames.c -o minesweeper -lSDL2 -lSDL2_image
      ```
    - Notepad:
      ```bash
      gcc notepad.c -o notepad `pkg-config --cflags --libs gtk+-3.0`
      ```
    - Music Player:
      ```bash
      gcc musicplayer.c -o musicplayer -lSDL2 -lSDL2_mixer
      ```
    - Calculator:
      ```bash
      gcc calculator.c -o calculator `pkg-config --cflags --libs gtk+-3.0`
      ```
      - mainGUI.c:
      ```bash
      gcc mainGUI.c -o mainGUI `pkg-config --cflags --libs gtk+-3.0`
      ```

## Usage
1. **Run the Digital Clock:**
    ```bash
    ./clock
    ```

2. **Run the File Manager:**
    ```bash
    ./file_manager
    ```

3. **Run the Minesweeper Game:**
    ```bash
    ./minesweeper
    ```

4. **Run the Notepad:**
    ```bash
    ./notepad
    ```

5. **Run the Music Player:**
    ```bash
    ./musicplayer
    ```

6. **Run the Calculator:**
    ```bash
    ./calculator
    ```
7. **Run mainGUI:**
    ```bash
    ./mainGUI
    ```


## Acknowledgement 
- I would like to express my sincere gratitude to **[Numan Shafi](https://www.linkedin.com/in/numan-shafi786/)**, Ph.D. Scholar, for his invaluable guidance, support, and encouragement throughout this project.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

