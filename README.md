# 🎮 GameBoy Console Simulation (C++ & SFML)

*A multi-game simulation project built using C++ and SFML in Visual Studio.*

---

## 📚 Table of Contents

- [Introduction](#introduction)
- [Games Included](#games-included)
- [Features](#features)
- [Screenshots](#screenshots)
- [Installation](#installation)
- [Usage](#usage)
- [Known Issues](#known-issues)
- [Contribution](#contribution)
- [Author](#author)
- [License](#license)

---

## 📘 Introduction

**GameBoy Console Simulation** is a creative desktop application that mimics the behavior of a handheld gaming console. It features a **graphical main menu**, **game selector**, and three fully playable classic games: **Hangman**, **Snake**, and **Wordle**.

Developed using **C++** and the **Simple and Fast Multimedia Library (SFML)**, this project focuses on interactive gameplay, keyboard controls, and retro-inspired game mechanics. The UI is built using SFML's graphics module, and everything runs in real time.

---

## 🎮 Games Included

### 🟢 Wordle
- A 5-letter guessing game.
- Green = correct letter and position.
- Yellow = correct letter, wrong position.
- Red = letter not in the word.
- 5 attempts to guess the word.

### 🔤 Hangman
- Choose from 3 categories:  
  `1 → Animals` | `2 → Fruits` | `3 → Countries`
- Guess letters before the man is fully drawn.
- Fun word pool and simple visuals.

### 🐍 Snake
- Classic snake mechanics.
- Eat fruits to grow longer.
- Avoid colliding with yourself or the wall.
- Real-time movement with keyboard input.

---

## ✨ Features

- 🎨 SFML-powered graphical interface
- 🎹 Keyboard navigation using arrow keys + Enter
- 📜 Menu system with Play, Instructions, and Exit
- 📦 Organized C++ files with `.cpp` and `.h`
- 🔠 Category selection using number keys in Hangman
- ✅ Designed and compiled in **Visual Studio**
- 🔁 Replay functionality for each game
- 💡 Minimal dependencies (SFML only)

---

## 📸 Screenshots

![Screenshot 2025-06-11 200836](https://github.com/user-attachments/assets/842af1f2-b67f-47b3-9e60-e71f477a7c64)
![Screenshot 2025-06-11 200932](https://github.com/user-attachments/assets/37adfb14-1e36-4fa4-8f3d-dd918d537341)
![Screenshot 2025-06-11 201007](https://github.com/user-attachments/assets/c1ed907b-dffd-4d6c-bb8e-3565b22568a0)
![Screenshot 2025-06-11 201101](https://github.com/user-attachments/assets/0bdc0cce-4ad5-466e-86a8-67e02a2e58ee)
![Screenshot 2025-06-11 201228](https://github.com/user-attachments/assets/01370ac8-c5eb-48de-8ca1-38bcde6e401a)

---

## ⚙️ Installation

### 📥 Requirements

- Visual Studio 2019 or newer
- SFML (Simple and Fast Multimedia Library)

### 🔧 Setting up SFML with Visual Studio

1. Download SFML from: [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)
2. Extract it to a location on your PC.
3. In Visual Studio:
   - Go to **Project → Properties → VC++ Directories**
     - Add SFML `include/` to **Include Directories**
     - Add SFML `lib/` to **Library Directories**
   - Go to **Linker → Input → Additional Dependencies**
     - Add:
       - `sfml-graphics-d.lib`
       - `sfml-window-d.lib`
       - `sfml-system-d.lib`
   - Copy `.dll` files from `SFML/bin/` to your project's `Debug` folder.

---

## 🚀 Usage

### 🔁 Build & Run

1. Clone or download this repository.
2. Open the `.sln` file in Visual Studio.
3. Build the solution.
4. Press **F5** or click **Start Debugging** to run.

### 🎮 Controls

- `↑ ↓` → Navigate menu or game list
- `Enter` → Select menu/game
- `1 / 2 / 3` → Choose Hangman category
- `W A S D` or Arrow Keys → Snake movement
- `Alphabet Keys` → Word input

---

---

## 🐞 Known Issues

- Word validation is basic (no dictionary check)
- Game resolution is fixed (may not scale well)
- No music or sound effects yet

---

## 🤝 Contribution

You're welcome to contribute!

1. Fork this repository
2. Clone it:  
   `git clone https://github.com/yourusername/gameboy-console.git`
3. Create a new branch:  
   `git checkout -b feature/your-feature`
4. Make your changes
5. Commit and push:  
   `git commit -m "Add new feature"`  
   `git push origin feature/your-feature`
6. Open a Pull Request

---

## 👨‍💻 Author

**Usman Bari**  
📧 usmanbari2005@gmail.com  
🎓 BSCS — NUCES FAST Islamabad (Batch 2023–2027)

---

## 🪪 License

This project is licensed under the [MIT License](LICENSE).

---

> _"Built from scratch using C++ and SFML — No built-in libraries, just logic, graphics, and a love for old-school games."_

