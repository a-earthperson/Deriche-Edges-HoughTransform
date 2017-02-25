import sys
from PIL import Image
img = Image.open(sys.argv[1])
img.save('bitmaps/out.bmp')
