---
sidebar_position: 2
---

# Map File Format

The R-Type Map Editor uses a JSON-based file format to store map data. This document describes the structure and fields of the map file format.

## File Structure

The map file is a JSON object with the following main sections:

- Map properties
- Tileset information
- Tile placements

## Example

Here's an example of a minimal map file:

```json
{
    "cellSize": 24,
    "editorVersion": "0.1",
    "height": 20,
    "name": "Default map",
    "tileSets": [
        {
            "filePath": "oak_woods_tileset.png",
            "tileCount": 315,
            "tileHeight": 24,
            "tileWidth": 24
        }
    ],
    "tiles": [
        {
            "tileIndex": 1,
            "x": 0,
            "y": 0
        }
    ],
    "background": {
        "height": 24,
        "path": "assets/Background/image.png"
    },
    "width": 100
}
```

## Field Descriptions

### Map Properties

- `cellSize`: The size of each cell in pixels (integer)
- `editorVersion`: The version of the editor used to create the map (string)
- `height`: The height of the map in cells (integer)
- `width`: The width of the map in cells (integer)
- `name`: The name of the map (string)

### Tileset Information

The `tileSets` field is an array of objects, each describing a tileset used in the map:

- `filePath`: The file path to the tileset image (string)
- `tileCount`: The total number of tiles in the tileset (integer)
- `tileHeight`: The height of each tile in pixels (integer)
- `tileWidth`: The width of each tile in pixels (integer)

### Tile Placements

The `tiles` field is an array of objects, each representing a tile placed on the map:

- `tileIndex`: The index of the tile in its tileset (integer)
- `x`: The x-coordinate of the tile placement (integer)
- `y`: The y-coordinate of the tile placement (integer)

### Background

- `height`: The height of the background image in pixels (integer)
- `path`: The file path to the background image (string)

## Notes

- Coordinates (x, y) are in cell units, not pixels.
- The `tileIndex` refers to the index of the tile within its tileset, starting from 0.
- Multiple tilesets can be used in a single map.
- The `editorVersion` field can be used to handle backwards compatibility if the file format changes in future versions.
