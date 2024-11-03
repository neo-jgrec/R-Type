# Command Line Interface

The command line interface (CLI) is a tool that allows you to interact with a game made by the game engine.
All the code related to the CLI is located in the `Shell.hpp` inside the `GameEngine` folder.

## Features

The CLI start a new thread that will listen to the user input and execute the commands.

The Game Engine exposes a two commands of the CLI:

- `addCommand` - Add a new command to the CLI.
- `out` - Print to the engine instead of the standard output.

## Usage

The CLI is run as start of the game, and it will be listening to the user input until the game is closed and the CLI exits.

:::warning
The CLI not sutdown automatically when the game is closed, you need to manually close the CLI.
:::

- To add a new command to the CLI, you need to call the `addCommand` method from the `GameEngine` class.
- To print use the `out` method from the `GameEngine` class instead of the standard output.

Example of adding a new command:
```cpp
gameEngine.addCommand("echo", "Print the message", [this](const std::string &args) {
    if (args.empty())
        return "Usage: echo <message>";
    return args;
});
```

## Default Commands

- `exit` - Close the game. **Note:** Not displayed in the help message.
- `help` - Show the help message.
- `log` - Show the log message.
- `save` - Create a backup of the log message.
