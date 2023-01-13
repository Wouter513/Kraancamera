import cv2 as cv
import math
from numpy import array
import glob
import time

#alles werkt met rad
#en mm
dictionary = cv.aruco.Dictionary_get(cv.aruco.DICT_4X4_50)
parameters = cv.aruco.DetectorParameters_create()

#lens distortion
#mtx = array([[655.3220204, 0., 681.09479455],
#             [  0., 635.54889483, 462.26196468],
#             [  0., 0., 1.]])
mtx = array([[690.05922287, 0., 647.5],
             [  0., 669.23796669, 485.5],
             [  0., 0., 1.]])

roi = (0, 0, 1295, 982)

dist = array([[-0.28475065, 0.07639245, 0.00168269, -0.00070382, -0.00897706]])
newcameramtx = array([[319.19474375, 0, 647.5],[  0, 309.56363484, 485.5],[0, 0, 1]])




pixtoradmultiplier = 61*math.pi/90/1296


def pixtorad(pixel):
    rad = pixel*pixtoradmultiplier
    return rad

def calculateheight(x1,x2):
    if abs(x2)>abs(x1):
        c2 = pixtorad(x2-x1)
        c1 = pixtorad(x1)
    else:
        c2 = pixtorad(x1-x2)
        c1 = pixtorad(x2) 
    c3 = (math.pi/2)-c1-c2
    #sinc3 = math.sin(c3)
    height = math.cos(c1)*(43*math.sin(c3)/math.sin(c2))
    return height

def calculatecamerapos(pixelcount,height=290):
    angle = pixtorad(pixelcount)
    distance = math.tan(angle)*height
    return distance


images = glob.glob('Esmee\KraanCamera\\fotos kraan\\aruco y as test\\*.jpg')
for file in images:
    print(file)
    start = time.time()
    img = cv.imread(file)
    imgundist = cv.undistort(img, mtx, dist, None, newcameramtx)
    x, y, w, h = roi
    hight, width, color = imgundist.shape
    centerx = width/2
    centery = hight/2
    
    gray = cv.cvtColor(imgundist, cv.COLOR_RGB2GRAY)
    (corners, ids, rejectedImgPoints) = cv.aruco.detectMarkers(gray, dictionary, parameters=parameters)
    if len(corners) > 0:
        for (markerCorner, markerID) in zip(corners, ids):
            cornersaruco = markerCorner.reshape((4, 2))
            (topLeft, topRight, bottomRight, bottomLeft) = cornersaruco
            topRight = (int(topRight[0])-centerx, int(topRight[1])-centery)
            bottomRight = (int(bottomRight[0])-centerx, int(bottomRight[1])-centery)
            bottomLeft = (int(bottomLeft[0])-centerx, int(bottomLeft[1])-centery)
            topLeft = (int(topLeft[0])-centerx, int(topLeft[1])-centery)
            print(markerID)
            print(calculateheight(topLeft[0],topRight[0]))
            print(calculateheight(bottomLeft[0],bottomRight[0]))
        img_marker = cv.aruco.drawDetectedMarkers(imgundist.copy(), corners, ids)
        cv.imshow('frame', img_marker)
        #print("this took: ", time.time()-start, " ms")
        

    cv.waitKey(0)