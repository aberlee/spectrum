""" Assembles Tiled .tmx maps and .tsx tilesets into map data that is
    usable by Spectrum
"""

import xml.etree.ElementTree as et
import sys
import tkinter
import os.path

tk = tkinter.Tk()
tk.wm_withdraw()

def crop(tkimage, xo=0, yo=0, xf=None, yf=None):
    ''' Return a cropped portion of this image from xo, yo
        to xf, yf. If endpoints are not specified, default to
        the width and height.
    '''
    if xf is None:
        xf = tkimage.width()
    if yf is None:
        yf = tkimage.height()
    
    # Copy pixels over
    cls = type(tkimage)
    ret = cls()
    tkimage.tk.call(ret.name, "copy", tkimage.name, "-from", xo, yo, xf, yf)
    return ret

def fragment(root, width, height):
    ''' Generator to split the root tkimage into fragments of width * height
        Assumes row-major ordering.
    '''

    #Validate split
    if (root.width() % width) or (root.height() % height):
        raise ValueError("Invalid image dimensions: cannot split evenly")
    
    #Generate tiles
    y = 0
    while y < root.height():
        x = 0
        while x < root.width():
            yield crop(root, x, y, x+width, y+height)
            x += width
        y += height
    return

def composite(tkimage, other, *, x=0, y=0, discard=False):
    ''' Combine the other image into this image '''
    rule = "set" if discard else "overlay"
    tkimage.tk.call(tkimage.name, "copy", other.name, "-to", x, y, "-compositingrule", rule)

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
        whole = tkinter.PhotoImage(master=tk, file=image_path)
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
        bg = tkinter.PhotoImage(
            master = tk,
            width  = self.width*tile_width,
            height = self.height*tile_height,
            format = "png",
        )
        fg = tkinter.PhotoImage(
            master = tk,
            width  = self.width*tile_width,
            height = self.height*tile_height,
            format = "png",
        )
        
        # Assemble the composite images
        any_overlay = False
        for l, layer in enumerate(self._layers):
            print("Compiling layer %d of %d..." % (l, len(self._layers)))
            for y, row in enumerate(layer._tiles):
                print("Progress: %d%%" % (y*100/self.height))
                for x, tile in enumerate(row):
                    if tile is not None:
                        if tile.overlay:
                            composite(fg, tile.image, x=x*tile_width, y=y*tile_height)
                            any_overlay = True
                        else:
                            composite(bg, tile.image, x=x*tile_width, y=y*tile_height)
        
        # Save the images
        bg.write(filename=filename, format="png")
        if any_overlay:
            base, ext = os.path.splitext(filename)
            overlay_filename = base + "-overlay" + ext
            fg.write(filename=overlay_filename, format="png")
        return
    
if __name__ == "__main__":
    tileset = Tileset(sys.argv[1])
    tilemap = Map(sys.argv[2], tileset)
    target = os.path.splitext(sys.argv[2])[0] + ".png"
    tilemap.compile(target)
    exit(0)
