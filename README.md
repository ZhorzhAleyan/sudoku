# Sudoku Game

A complete desktop Sudoku game built with C++17 and SFML, featuring difficulty levels, hints, timer, and note mode.

## Features

- **Difficulty Levels**: Easy, Medium, Hard, and Expert
- **Sudoku Solver**: Backtracking algorithm ensures unique solutions
- **Hint System**: Get help with up to 3 hints per game
- **Timer**: Track your solving time with pause/resume functionality
- **Note Mode**: Toggle pencil marks to track candidate numbers
- **Modern UI**: Clean, minimalistic interface with smooth colors
- **Error Detection**: Visual feedback for incorrect placements

## Requirements

### Dependencies

- **C++17** or higher compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake** 3.12 or higher
- **SFML** 2.5 or higher (tested with SFML 3.x)

### Installing SFML

#### macOS (using Homebrew)

```bash
brew install sfml
```

#### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install libsfml-dev
```

#### Linux (Fedora)

```bash
sudo dnf install SFML-devel
```

#### Windows

1. Download SFML from https://www.sfml-dev.org/download.php
2. Extract to a location (e.g., `C:\SFML`)
3. Set environment variable `SFML_ROOT` to the SFML directory
4. Or use vcpkg: `vcpkg install sfml`

## Building the Project

### Using CMake (Recommended)

1. **Create build directory:**

   ```bash
   mkdir build
   cd build
   ```

2. **Configure the project:**

   ```bash
   cmake ..
   ```

   On Windows with SFML in a custom location:

   ```bash
   cmake .. -DSFML_ROOT=C:/SFML
   ```

3. **Build the project:**

   ```bash
   cmake --build .
   ```

   Or use your system's build tool:

   - Linux/macOS: `make`
   - Windows: Open the generated `.sln` file in Visual Studio

### Manual Compilation

If you prefer to compile manually:

**On macOS (with Homebrew SFML):**

```bash
g++ -std=c++17 -O2 -I/opt/homebrew/opt/sfml/include src/*.cpp -o sudoku -L/opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system
```

**On Linux:**

```bash
g++ -std=c++17 -O2 src/*.cpp -o sudoku -lsfml-graphics -lsfml-window -lsfml-system
```

**Note:** Adjust the include (`-I`) and library (`-L`) paths based on your SFML installation location.

## Running the Game

### From Build Directory

After building, run the executable:

```bash
./SudokuGame
```

On Windows:

```bash
SudokuGame.exe
```

**Important**: The game expects the `assets` folder to be in the same directory as the executable. CMake automatically copies it to the build directory.

### Font Setup

The game requires a font file. You need to:

1. Download a font (e.g., Roboto from https://fonts.google.com/specimen/Roboto)
2. Place `Roboto-Regular.ttf` in `assets/fonts/` directory
3. Or modify `UI.cpp` to use a system font or another font path

If the font is not found, the game will still run but text may not render properly.

## Controls

### Mouse

- **Left Click**: Select a cell
- **Click Buttons**: Interact with UI buttons

### Keyboard

- **1-9** or **Numpad 1-9**: Enter a number in the selected cell
- **Backspace/Delete**: Clear the selected cell
- **N**: Toggle note mode
- **ESC**: Pause/resume game or close menus

## Game Rules

1. Fill the 9×9 grid so that each row, column, and 3×3 box contains digits 1-9 exactly once
2. Numbers in gray are given clues (cannot be changed)
3. Numbers in blue are your entries
4. Red numbers indicate errors
5. Use note mode to track possible values

## Project Structure

```
sudoku/
├── src/
│   ├── main.cpp           # Entry point
│   ├── Cell.h/cpp         # Cell representation
│   ├── Board.h/cpp        # 9×9 board logic
│   ├── SudokuSolver.h/cpp # Backtracking solver
│   ├── SudokuGenerator.h/cpp # Puzzle generator
│   ├── Game.h/cpp         # Game state management
│   └── UI.h/cpp           # SFML rendering and input
├── assets/
│   └── fonts/             # Font files
├── CMakeLists.txt         # Build configuration
└── README.md              # This file
```

## Code Quality

- **No global variables**: All state is encapsulated in classes
- **Modern C++**: Uses C++17 features (smart pointers, structured bindings, etc.)
- **Clean architecture**: Separation of concerns with logical modules
- **Memory safe**: Uses RAII and avoids raw pointers where possible
- **FPS-independent**: Rendering loop is decoupled from game logic

## Troubleshooting

### Font Not Loading

- Ensure `assets/fonts/Roboto-Regular.ttf` exists
- Check file permissions
- Try using an absolute path in `UI.cpp`

### SFML Not Found

- Verify SFML is installed: `pkg-config --modversion sfml-all`
- On macOS, ensure Homebrew paths are in your environment
- On Windows, set `SFML_ROOT` environment variable

### Build Errors

- Ensure C++17 support: `g++ --version` or `clang++ --version`
- Update CMake: `cmake --version`
- Clean build: Delete `build/` and reconfigure

## License

This project is provided as-is for educational purposes.

## Contributing

Feel free to fork and modify this project. Suggestions for improvements are welcome!
