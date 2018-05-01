import os
import glob
import errno
import numpy as np
import matplotlib.pyplot as plt

def createdir(dir):
    if not os.path.exists(dir):
        try:
            os.makedirs(dir)
        except OSError as exc: # Guard against race condition
            if exc.errno != errno.EEXIST:
                raise

def csvtoarr(src):
    return np.nan_to_num(np.genfromtxt(src, delimiter=',', skip_header=0, skip_footer=0))

def genimage(src, outpath):
    createdir(outpath)
    filename = os.path.basename(src).replace('.bmp','').replace('csv','png')
    img = csvtoarr(src)
    cmap = 'jet' if "hough" in filename else 'gray'
    plt.imsave(fname=outpath+'/'+filename, arr=img, vmin=np.min(img), vmax=np.max(img), format='png', cmap=cmap)
    plt.imshow(img, cmap=cmap, vmin=np.min(img), vmax=np.max(img), interpolation='none')
    plt.title(filename)
    plt.colorbar()
    plt.show()
    return img

if __name__ == "__main__":
    csvs = glob.glob('examples/*.csv')
    for csv in csvs:
        genimage(csv, 'generated/')
