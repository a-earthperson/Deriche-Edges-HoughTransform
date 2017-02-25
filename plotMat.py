import matplotlib as mpl
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cbook as cbook
import scipy.misc

def show_plots(filename):

    hough_csv = np.genfromtxt('cmake-build-debug/hough_'+filename+'.csv', delimiter=',', skip_header=0, skip_footer=0)
    gray_csv  = np.genfromtxt('cmake-build-debug/grayscale_'+filename+'.csv', delimiter=',', skip_header=0, skip_footer=0)

    gray_img = scipy.misc.toimage(gray_csv, cmin=np.min(gray_csv), cmax=np.max(gray_csv))
    hough_img = scipy.misc.toimage(hough_csv, cmin=np.min(hough_csv), cmax=np.max(hough_csv))

    #plt.imshow('bitmaps/'+filename+'.bmp')
    #plt.show()

    plt.imshow(gray_img, cmap='gray', interpolation='none')
    plt.show()

    plt.imshow(hough_img, interpolation='none')
    plt.show()

#show_plots('rick_and_morty')


#how_plots('hexagon')
#show_plots('diamond')

show_plots('triangle')
#show_plots('image1')
#show_plots('image2')
#show_plots('image3')
#show_plots('diamond')
exit()
