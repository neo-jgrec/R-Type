# Directory Structure

The R-Type project follows a structured organization to maintain clarity and separation of concerns. Here's an overview of the main directories and their purposes:

## Root Directory

- `src/`: Contains all source code for the project
- `docs/`: Documentation files, including this developer guide
- `assets/`: Game assets such as images, sounds, and fonts
- `CMakeLists.txt`: Root CMake configuration file
- `LICENSE`: MIT License file
- `.github/`: GitHub-specific files, including workflows

## Source Code (`src/`)

- `client/`: Client-side game code
  - `src/`: Client source files
    - `Game/`: Game logic and components
      - `Game.*`: Game logic
      - `Scenes.*`: Game scenes and transitions
      - `EntityFactory.*`: Factory for creating game entities
      - `Systems.*`: Game systems for updating entities
      - `Map.*`: Game map and tile initialization
    - `events/`: Event handling and management
    - `main.cpp`: Entry point for the client application

## Documentation (`docs/`)

- `docusaurus/`: Docusaurus configuration and content
  - `docs/`: Markdown files for documentation
    - `Client/`: Client-specific documentation

## Build and Configuration

- `CMakeLists.txt`: CMake configuration files (root and in subdirectories)
- `.gitignore`: Git ignore file

## Assets

- `assets/`: Game assets
  - `Fonts/`: Font files
  - `Background/`: Background images
  - `Obstacles/`: Obstacle sprites
  - `music/`: Game music files

This structure allows for easy navigation and maintenance of the project, separating client, server, and editor code while sharing common components and utilities.
