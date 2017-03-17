import glob
import scipy.misc
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.cbook as cbook

def get_files(dirs='outputs/'):
    grayfiles = glob.glob(dirs+'grayscale/*.csv')
    houghfiles = glob.glob(dirs+'hough/*.csv')
    return zip(grayfiles, houghfiles)

def plot_images(gray, hough):
    gray_csv    = np.genfromtxt(gray,  delimiter=',', skip_header=0, skip_footer=0)
    hough_csv   = np.genfromtxt(hough, delimiter=',', skip_header=0, skip_footer=0)
    gray_image  = scipy.misc.toimage(gray_csv, cmin=np.min(gray_csv), cmax=np.max(gray_csv))
    hough_image = scipy.misc.toimage(hough_csv, cmin=np.min(hough_csv), cmax=np.max(hough_csv))

    plt.imshow(gray_image, cmap='gray', interpolation='none')
    plt.show()

    plt.imshow(hough_image, interpolation='none')
    plt.show()

def show_plots():
    filetuples = get_files()
    for gray_file, hough_file in filetuples:
        plot_images(gray_file, hough_file)


if __name__ == "__main__":
    show_plots()
