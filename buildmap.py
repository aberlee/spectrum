""" Assembles Tiled .tmx maps and .tsx tilesets into map data that is
    usable by Spectrum
"""

# Standard library
import xml.etree.ElementTree as et
import sys
import os
import os.path
import traceback

# External libraries
from PIL import Image

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
                for x, dirty_index in enumerate(csv.split(",")[:-1]):
                    clean_index = int(dirty_index.strip())
                    if clean_index > 0:
                        tile = tileset.tile(clean_index-1)
                        built.set(x, y, tile)
            self._layers.append(built)

        # Store other data
        self.width = int(root.get("width"))
        self.height = int(root.get("height"))

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
            if overlay is None:
                base, ext = os.path.splitext(filename)
                overlay = base + "-overlay" + ext
            fg.save(overlay)
        return

###############################################################
## Main
###############################################################
if __name__ == "__main__":
    tileset_path = sys.argv[1] if len(sys.argv) > 1 else None
    tilemap_path = sys.argv[2] if len(sys.argv) > 2 else None
    output_path = sys.argv[3] if len(sys.argv) > 3 else None
    overlay_path = sys.argv[4] if len(sys.argv) > 4 else None

    # Deal with arguments
    if tileset_path is None or tilemap_path is None:
        print("Usage: %s <tileset> <map> [output?] [overlay?]" % (sys.argv[0],))
        exit(0)
    if output_path is None:
        output_path = os.path.splitext(tilemap_path)[0] + ".png"

    # Compile the map
    try:
        tileset = Tileset(tileset_path)
        tilemap = Map(tilemap_path, tileset)
        if not os.path.isdir(output_path):
            os.mkdir(os.path.dirname(output_path))
        if overlay_path and not os.path.isdir(overlay_path):
            os.mkdir(os.path.dirname(overlay_path))
        tilemap.compile(output_path, overlay_path)
    except:
        traceback.print_exc()
        exit(1)
    else:
        exit(0)
