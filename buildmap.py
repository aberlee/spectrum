""" Assembles Tiled .tmx maps and .tsx tilesets into map data that is
    usable by Spectrum
"""

import xml.etree.ElementTree as et
import sys
import os.path

from PIL import Image

def fragment(root, width, height):
    ''' Generator to split the root tkimage into fragments of width * height
        Assumes row-major ordering.
    '''

    #Validate split
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

class Tileset(object):
    
    class Tile(object):
        
        def __init__(self):
            self.image = None
            self.overlay = False
            self.solid = False
            self.water = False
    
    def __init__(self, path):
        self._tiles = []
        tree = et.parse(path)
        root = tree.getroot()
        
        # Assemble tileset properties
        for tile in root.iter("tile"):
            built = Tileset.Tile()
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
        for index, image in enumerate(fragment(whole, self.width, self.height)):
            self._tiles[index].image = image
        return
    
    def tile(self, index):
        return self._tiles[index]

class Map(object):
    
    class Layer(object):
        
        def __init__(self, width, height):
            self._tiles = [[None for _ in range(width)] for _ in range(height)]
        
        def get(self, x, y):
            return self._tiles[y][x]
        
        def set(self, x, y, tile):
            self._tiles[y][x] = tile
    
    def __init__(self, path, tileset):
        self._tileset = tileset
        self._layers = []
        tree = et.parse(path)
        root = tree.getroot()
        
        for layer in root.iter("layer"):
            width = int(layer.get("width"))
            height = int(layer.get("height"))
            built = Map.Layer(width, height)
            for y, csv in enumerate(layer.find("data").text.split()):
                for x, dirty_index in enumerate(csv.split(",")[:-1]):
                    clean_index = int(dirty_index.strip())
                    if clean_index > 0:
                        tile = tileset.tile(clean_index-1)
                        built.set(x, y, tile)
            self._layers.append(built)
        self.width = int(root.get("width"))
        self.height = int(root.get("height"))
        return
    
    def compile(self, filename):
        ''' Render the map as a pair of images '''
        # Set up images
        tile_width = self._tileset.width
        tile_height = self._tileset.height
        bg = Image.new("RGBA", (self.width*tile_width, self.height*tile_height))
        fg = Image.new("RGBA", (self.width*tile_width, self.height*tile_height))
        
        # Assemble the composite images
        any_overlay = False
        for l, layer in enumerate(self._layers):
            for y, row in enumerate(layer._tiles):
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
            overlay_filename = base + "-overlay" + ext
            fg.save(overlay_filename)
        return
    
if __name__ == "__main__":
    tileset = Tileset(sys.argv[1])
    tilemap = Map(sys.argv[2], tileset)
    target = os.path.splitext(sys.argv[2])[0] + ".png"
    tilemap.compile(target)
    exit(0)
