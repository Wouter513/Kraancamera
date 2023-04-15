import cv2 as cv
import math
from numpy import array
import glob
import time
from picamera.array import PiRGBArray
from picamera import PiCamera


#alles werkt met rad
#en mm
dictionary = cv.aruco.Dictionary_get(cv.aruco.DICT_4X4_50)
parameters = cv.aruco.DetectorParameters_create()

#lens distortion
#mtx = array([[655.3220204, 0., 681.09479455],
#             [  0., 635.54889483, 462.26196468],
#             [  0., 0., 1.]])
#mtx = array([[690.05922287, 0., 647.5],
#             [  0., 669.23796669, 485.5],
#             [  0., 0., 1.]])
mtx = array([[635.48795981, 0., 697.06268869],
             [0., 634.44232829, 486.74944264],
             [0., 0., 1.]])

#roi = (0, 0, 1295, 982)
roi = (383, 301, 492, 369)

dist = array([[-0.28475065, 0.07639245, 0.00168269, -0.00070382, -0.00897706]])

newcameramtx = array([[319.19474375, 0, 647.5],[0, 309.56363484, 485.5],[0, 0, 1]])

def calculatecamerapos(pixelcount,height=290):
    angle = pixtorad(pixelcount)
    distance = math.tan(angle)*height
    return distance



camera = PiCamera()
camera.resolution = (1296,976)
camera.framerate = 5
time.sleep(1)
rawCapture = PiRGBArray(camera, size=(1296,976))
time.sleep(1)
#images = glob.glob('/home/kraancamera/Documents/KraanCamera/aruco/*.jpg')
#print(images)
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    #for file in images:
    #print("test")
    #    print(file)
    #    start = time.time()
    #    img = cv.imread(file)
    #    img = cv.rotate(img, cv.ROTATE_180)
    img = frame.array
    imgundist = cv.undistort(img, mtx, dist, None, newcameramtx)
    x, y, w, h = roi
    hight, width, color = imgundist.shape
    #hight, width, color = img.shape
    centerx = width/2
    centery = hight/2
    gray = cv.cvtColor(imgundist, cv.COLOR_RGB2GRAY)
    #gray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)
    (corners, ids, rejectedImgPoints) = cv.aruco.detectMarkers(gray, dictionary, parameters=parameters)
    
    if len(corners) > 0:
        for (markerCorner, markerID) in zip(corners, ids):
            cornersaruco = markerCorner.reshape((4, 2))
            (topLeft, topRight, bottomRight, bottomLeft) = cornersaruco
            topRight = (centerx-int(topRight[0]), int(topRight[1])-centery)
            bottomRight = (centerx-int(bottomRight[0]), int(bottomRight[1])-centery)
            bottomLeft = (centerx-int(bottomLeft[0]), int(bottomLeft[1])-centery)
            topLeft = (centerx-int(topLeft[0]), int(topLeft[1])-centery)

            if(markerID == 5):
                #bottomLeft = bottomLeft[0]/46*43, bottomLeft[1]/46*43
                print(bottomLeft)
            img_marker = cv.aruco.drawDetectedMarkers(imgundist.copy(), corners, ids)
        #img_marker = cv.aruco.drawDetectedMarkers(img.copy(), corners, ids)
        cv.imshow('frame', img_marker)
        #print("this took: ", time.time()-start, " ms")
    rawCapture.truncate(0)
    if cv.waitKey(5) & 0xFF == ord('q'):
        break
cv.destroyAllWindows()
