
# Pass a set of images, convert to bytearray and store in 2D array.
# Convert pixel values to target BITDEPTH. (Necessary for LCD display.)
# Write output as ascii data to C source files, for use in compilation of compression programs generating Pico binaries.


import sys
import numpy as np
import matplotlib.pyplot as plt
#from icecream import ic


RGB_BITDEPTHS = [5,6,5]         # Target: RGB565
BITDEPTH = sum(RGB_BITDEPTHS)   # Bits per pixel of output bytearray.
BG = [0,0,0]                    # Black    
OUTFILE = "ImageDataRaw"
PRINTWIDTH = 16/2


def main():
    frames = len(sys.argv)-1
    
    if (frames<1):
        print("err: No image argument")
        return

    # Dimensions of first image
    img = plt.imread(sys.argv[1])
    w = img.shape[1]
    h = img.shape[0]
    
    # Check that all images have the same size.
    for i in range(2,frames+1):
        img = plt.imread(sys.argv[i])
        if (w != img.shape[1] or h != img.shape[0]):
            print("err: Supplied images do not all have the same size.")
            return
    
    imgtitle = "const unsigned char frames"
    
    with open(OUTFILE+".h","w") as fo:
        fo.write("#ifndef _{}_H_\n".format(OUTFILE.upper()))
        fo.write("#define _{}_H_\n\n".format(OUTFILE.upper()))
        fo.write("#define FRAME_WIDTH {}\n".format(w))
        fo.write("#define FRAME_HEIGHT {}\n".format(h))
        fo.write("#define FRAME_BITDEPTH {}\n".format(BITDEPTH)) 
        fo.write("extern "+imgtitle+"["+str(frames)+"]["+str(2*w*h)+"];\n")
        fo.write("#endif\n")
    
    with open(OUTFILE+".c","w") as fo:
        fo.write("#include \""+OUTFILE+".h\"\n")
        fo.write("\n")
        
        fo.write(imgtitle+"["+str(frames)+"]["+str(2*w*h)+"] = {\n")
        
        print("* Converting {} images to raw byte array ascii data.".format(frames))
        
        for f in range(1, frames+1):            
            print("{}".format(sys.argv[f]))
            img = plt.imread(sys.argv[f])
            
            fo.write("{\n")
            for y in range(h):
                for x in range(w):                
                    # If transparent
                    if len(img[y][x])>3 and img[y][x][3]==0:
                        p = np.array(BG)
                    else:
                        p = img[y][x][:3]
                    
                    #ic(p)
                    rgb = np.round(p * (2**np.array(RGB_BITDEPTHS)-1))
                    #ic(rgb)
                    
                    # Convert to binary, and then hex
                    b = '{0:05b}'.format(int(rgb[0]))+'{0:06b}'.format(int(rgb[1]))+'{0:05b}'.format(int(rgb[2]))
                    hexbyt = hex(int(b,2)).upper()
                    # Pad out with zeros if necessary
                    if len(hexbyt)<6:
                        r = 6-len(hexbyt)
                        hexbyt = "0X"+"0"*r+hexbyt[-(4-r):]
                
                    fo.write("0X{},0X{},".format(hexbyt[4:],hexbyt[2:4]))
                    if (y*w+x+1)%PRINTWIDTH==0:
                        fo.write("\n")
            if f<frames:
                fo.write("},")
            else:
                fo.write("}")
        
        fo.write("};\n")
            
    print("* Data written to {}.[ch]".format(OUTFILE))
    
    return
main()

