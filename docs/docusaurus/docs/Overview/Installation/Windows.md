
# Windows Installation

### How to Install the Game on Windows

This document explains how to install the game on Windows using a wizard installer and the provided `build.bat` script.

#### Prerequisites:
1. **CMake**: Ensure you have CMake installed on your system. You can download it from [CMake's official website](https://cmake.org/download/).
2. **VCPKG**: This project uses VCPKG as a package manager for C++. Make sure you have it installed. You can find the instructions [here](https://github.com/microsoft/vcpkg).
3. **Wizard Installer**: The game installation is packaged with a wizard installer.

#### Steps to Install:

1. **Download the Installer**: 
   - First, download the installer from the provided source.

2. **Run the Installer**:
   - Execute the wizard installer by double-clicking on the `.exe` file.
   - Follow the on-screen instructions to complete the installation process.

3. **Set up VCPKG**:
   - If you haven't installed VCPKG, set it up following the instructions from the official VCPKG GitHub repository.
   - After setting up VCPKG, ensure the environment variable `VCPKG_ROOT` is correctly set. This variable should point to the root of your VCPKG installation.

4. **Build the Game Using `build.bat`**:
   - Open a command prompt.
   - Navigate to the game’s root directory.
   - Run the build script:
     - For a **Release build**, run:
       ```bash
       build.bat
       ```
     - For a **Debug build**, run:
       ```bash
       build.bat -d
       ```

   This script will:
   - Set the output directory to `build\` within the game's directory.
   - Use the `vcpkg` toolchain for dependency management.
   - Configure the build for either `Debug` or `Release` mode.
   - Run `CMake` to generate and build the project.

5. **Running the Game**:
   - Once the build is complete, navigate to the `build\` directory.
   - You will find the executable for the game. Simply double-click the executable to start the game.

That’s it! You’ve successfully installed and built the game on your Windows machine.