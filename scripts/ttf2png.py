from sys import argv
from PIL import Image, ImageFont, ImageDraw

# supported char's : 33 - 126

size = int(argv[2])
data = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

font = ImageFont.truetype(argv[1], size)
img = Image.new("RGBA", (size*10, size*10), (255, 255, 255))
draw = ImageDraw.Draw(img)


for i in range(10):
    for j in range(10):
        if 10*i+j <= len(data)-1:
            draw.text((j*size, i*size),
                      data[10*i+j], (0, 0, 0), font=font)

draw = ImageDraw.Draw(img)

data = img.load()
w, h = img.size

for y in range(h):
    for x in range(w):
        if data[x, y] == (255, 255, 255, 255):
            data[x, y] = (255, 255, 255, 0)

img.save(argv[1][:-3]+"png")
