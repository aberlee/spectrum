""" Assembles Tiled .tmx maps and .tsx tilesets into map data that is
    usable by Spectrum
"""

# Standard library
import xml.etree.ElementTree as et
import sys
import os
import os.path
import struct
import traceback

# External libraries
from PIL import Image

###############################################################
TILE_SOLID = 0x1
TILE_WATER = 0x2
TILE_EVENT = 0x4

###############################################################
## Tile
###############################################################
class Tile(object):
    """ Contains the properties of one tile.
    """

    __slots__ = [
        "image",    # Image: Tile image
        "overlay",  # bool: Tile is rendered above player
        "solid",    # bool: Tile cannot be moved through
        "water",    # bool: Tile is water
    ]

    def __init__(self):
        """ Initializes a tile with no data inside.
        """
        self.image = None
        self.overlay = False
        self.solid = False
        self.water = False

###############################################################
## Tileset
###############################################################
class Tileset(object):
    """ Contains information from a .tsx tileset encoded by Tiled.
        The tileset needs to have three properties defined per tile.

        1.  overlay (bool): The tile is rendered above the player.
        2.  solid (bool):   The tile cannot be moved through.
        3.  water (bool):   The tile is water, and cannot be moved through.
    """

    __slots__ = [
        "_tiles",   # list: Tile data (protected)
        "width",    # int: Tile width in px
        "height",   # int: Tile height in px
    ]

    def __init__(self, path):
        """ Creates a Tileset by unpacking a .tmx file provided as
            the path.

            path - string: File path to open.
        """
        self._tiles = []
        tree = et.parse(path)
        root = tree.getroot()

        # Assemble tileset properties
        for tile in root.iter("tile"):
            built = Tile()
            for prop in tile.iter("property"):
                if prop.get("name") == "overlay":
                    built.overlay = prop.get("value") == "true"
                elif prop.get("name") == "solid":
                    built.solid = prop.get("value") == "true"
                elif prop.get("name") == "water":
                    built.water = prop.get("value") == "true"
            self._tiles.append(built)

        # Assemble images
        image = root.find("image")
        self.width = int(root.get("tilewidth"))
        self.height = int(root.get("tileheight"))
        image_path = os.path.join(os.path.dirname(path), image.get("source"))
        whole = Image.open(image_path)
        for index, image in enumerate(self.fragment(whole, self.width, self.height)):
            self._tiles[index].image = image
        return

    def tile(self, index):
        """ Gets a tile with the given index.

            index - int: Tile ID

            Returns a Tile object.
        """
        return self._tiles[index]

    @staticmethod
    def fragment(root, width, height):
        """ Generator to split the root tkimage into fragments of width * height
            Assumes row-major ordering.

            root - Image: Image to fragment
            width - int: Fragment width
            height - int: Fragment height

            Yields a series of Image objects.
        """
        if (root.width % width) or (root.height % height):
            raise ValueError("Invalid image dimensions: cannot split evenly")

        #Generate tiles
        y = 0
        while y < root.height:
            x = 0
            while x < root.width:
                yield root.crop((x, y, x+width, y+height))
                x += width
            y += height
        return

###############################################################
## Layer
###############################################################
class Layer(object):
    """ Stores all tiles that occur on one layer of a map.
    """

    __slots__ = [
        "tiles",   # list: Tile data
    ]

    def __init__(self, width, height):
        """ Create an empty layer with the given dimensions.

            width - int: Layer width in tiles
            height - int: Layer height in tiles
        """
        self.tiles = [[None for _ in range(width)] for _ in range(height)]

    def get(self, x, y):
        """ Get the tile at the coordinate.

            x - int: Coordinate
            y - int: Coordinate

            Returns a Tile object or None.
        """
        return self.tiles[y][x]

    def set(self, x, y, tile):
        """ Set the tile at the coordinate.

            x - int: Coordinate
            y - int: Coordinate
            tile - Tile or None: New tile to add
        """
        self.tiles[y][x] = tile

###############################################################
## Map
###############################################################
class Map(object):
    """ Contains information from a .tmx map encoded by Tiled.
        The map must be encoded in CSV format.
    """

    __slots__ = [
        "_tileset", # Tileset: Map tileset (protected)
        "_layers",  # list of Layer: Map layers in ascending order (protected)
        "_events",  # Layer: Events on the map
        "width",    # int: Map width in tiles
        "height",   # int: Map height in tiles
    ]

    def __init__(self, path, tileset):
        """ Loads a map from a .tmx file.
        """
        self._tileset = tileset
        self._layers = []
        tree = et.parse(path)
        root = tree.getroot()

        # Unpack layers
        for layer in root.iter("layer"):
            width = int(layer.get("width"))
            height = int(layer.get("height"))
            built = Layer(width, height)
            for y, csv in enumerate(layer.find("data").text.split()):
                for x, dirty_index in enumerate(csv.split(",")):
                    if not dirty_index:
                        continue
                    clean_index = int(dirty_index.strip())
                    if clean_index > 0:
                        tile = tileset.tile(clean_index-1)
                        built.set(x, y, tile)
            self._layers.append(built)

        # Store other data
        self.width = int(root.get("width"))
        self.height = int(root.get("height"))
        
        # Unpack events
        self._events = Layer(self.width, self.height)
        for event in root.iter("object"):
            x = int(event.get("x")) // self.width
            y = int(event.get("y")) // self.height
            id = int(event.get("id"))
            self._events.set(x, y, id)
        return
    
    def _pack(self, x, y):
        """ Packs the tile properties at x,y into a struct of 2 bytes.
            The first byte is the struct flags, and the second is the
            event ID.
        """
        # Get tile properties
        flags = 0x0
        for layer in self._layers:
            tile = layer.get(x, y)
            if tile and tile.water:
                flags |= TILE_WATER
            if tile and tile.solid:
                flags |= TILE_SOLID

        # Get event ID
        event = self._events.get(x, y)
        if event:
            flags |= TILE_EVENT
        else:
            event = 0
        
        # Yield packed data
        return struct.pack("BB", flags, event)

    def compile(self, filename, overlay=None):
        """ Render the map as a pair of images.
        """
        tile_width = self._tileset.width
        tile_height = self._tileset.height
        bg = Image.new("RGBA", (self.width*tile_width, self.height*tile_height))
        fg = Image.new("RGBA", (self.width*tile_width, self.height*tile_height))

        # Assemble the composite images
        any_overlay = False
        for l, layer in enumerate(self._layers):
            for y, row in enumerate(layer.tiles):
                for x, tile in enumerate(row):
                    if tile is not None:
                        if tile.overlay:
                            fg.alpha_composite(tile.image, (x*tile_width, y*tile_height))
                            any_overlay = True
                        else:
                            bg.alpha_composite(tile.image, (x*tile_width, y*tile_height))

        # Save the images
        bg.save(filename)
        if any_overlay:
            base, ext = os.path.splitext(filename)
            overlay = base + "-overlay" + ext
            fg.save(overlay)
        
        # Save the events
        sensor = bytearray()
        for y in range(self.height):
            for x in range(self.width):
                sensor.extend(self._pack(x, y))
        base, ext = os.path.splitext(filename)
        events = base + ".dat"
        with open(events, "wb") as file:
            file.write(sensor)
        return

###############################################################
## Main
###############################################################
if __name__ == "__main__":
    tileset_path = sys.argv[1] if len(sys.argv) > 1 else None
    tilemap_path = sys.argv[2] if len(sys.argv) > 2 else None
    output_path = sys.argv[3] if len(sys.argv) > 3 else None

    # Deal with arguments
    if tileset_path is None or tilemap_path is None:
        print("Usage: %s <tileset> <map> [output?]" % (sys.argv[0],))
        exit(0)
    if output_path is None:
        output_path = os.path.splitext(tilemap_path)[0] + ".png"

    # Compile the map
    try:
        tileset = Tileset(tileset_path)
        tilemap = Map(tilemap_path, tileset)
        if not os.path.isdir(os.path.dirname(output_path)):
            os.mkdir(os.path.dirname(output_path))
        tilemap.compile(output_path)
    except:
        traceback.print_exc()
        exit(1)
    else:
        exit(0)
