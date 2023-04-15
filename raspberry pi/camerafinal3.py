import cv2 as cv
import math
from numpy import array
import glob
import time
from picamera.array import PiRGBArray
from picamera import PiCamera
import threading
import serial 


ser = serial.Serial('/dev/ttyACM0', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
posrequest = 0

time.sleep(2)
def get_input():
    while(True):
        data = input()
        datasplit = data.split()
        
        intdata = [int(numeric_string) for numeric_string in datasplit]
        eindarray = [254, 2, int(intdata[0]%256), int(intdata[0]/256),int(intdata[1]%256), int(intdata[1]/256),int(50%256), int(50/256), 8, 255]
        i = 2
        while(i < 7):
            if(eindarray[i] >253):
                eindarray[i] = 253
            i = i + 1
        bytedata = bytearray(eindarray)
        #while(posrequest == 0):
        #    time.sleep(0.1)
        print(bytedata)
        ser.write(bytedata)

input_thread = threading.Thread(target=get_input)
input_thread.start()

cornerlookup = array([[[143, 400], [189, 400], [189, 354], [143, 354]],
                [[143, 335], [189, 335], [189, 289], [143, 289]],
                [[143, 270], [189, 270], [189, 224], [143, 224]],
                [[143, 185], [189, 185], [189, 139], [143, 139]],
                [[143, 116], [189, 116], [189, 70],  [143, 70]],
                [[143, 48],  [189, 48],  [189, 2],   [143, 2]],
                [[333, 400], [378, 400], [378, 354], [333, 354]],
                [[333, 335], [378, 335], [378, 289], [333, 289]],
                [[333, 270], [378, 270], [378, 224], [333, 224]],
                [[333, 185], [378, 185], [378, 139], [333, 139]],
                [[333, 116], [378, 116], [378, 70],  [333, 70]],
                [[333, 48],  [378, 48],  [378, 2],   [333, 2]]])

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
    posrequest = 1;
    img = frame.array
    posrequest = 0;
    imgundist = cv.undistort(img, mtx, dist, None, newcameramtx)
    x, y, w, h = roi
    hight, width, color = imgundist.shape
    #hight, width, color = img.shape
    centerx = width/2
    centery = hight/2
    gray = cv.cvtColor(imgundist, cv.COLOR_RGB2GRAY)
    #gray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)
    (corners, ids, rejectedImgPoints) = cv.aruco.detectMarkers(gray, dictionary, parameters=parameters)
    squares = 0
    totalx = 0
    totaly = 0
    if len(corners) > 0:
        for (markerCorner, markerID) in zip(corners, ids):
            cornersaruco = markerCorner.reshape((4, 2))
            (topLeft, topRight, bottomRight, bottomLeft) = cornersaruco
            topRight = (centerx-int(topRight[0]), int(topRight[1])-centery)
            bottomRight = (centerx-int(bottomRight[0]), int(bottomRight[1])-centery)
            bottomLeft = (centerx-int(bottomLeft[0]), int(bottomLeft[1])-centery)
            topLeft = (centerx-int(topLeft[0]), int(topLeft[1])-centery)
            if(markerID < 12):
                squares = squares + 1
                camerapositiony4 = cornerlookup[markerID, 0, 0]+topLeft[0] + cornerlookup[markerID, 1, 0]+ topRight[0] + cornerlookup[markerID, 2, 0]+bottomRight[0] + cornerlookup[markerID, 3, 0]+bottomLeft[0]
                camerapositionx4 = cornerlookup[markerID, 0, 1]+topLeft[1] + cornerlookup[markerID, 1, 1]+ topRight[1] + cornerlookup[markerID, 2, 1]+bottomRight[1] + cornerlookup[markerID, 3, 1]+bottomLeft[1]
                totaly = totaly + camerapositiony4
                totalx = totalx + camerapositionx4
        if squares > 0:
            coordsy = totaly/(squares*4)
            coordsx = totalx/(squares*4)
            print(coordsx)
            print(coordsy)
        
            posarray = [254, 1, int(coordsx%256), int(coordsx/256),int(coordsy%256), int(coordsy/256),int(50%256), int(50/256), 8, 255]
            n = 2
            while(n < 7):
                if(posarray[n] >253):
                    posarray[n] = 253
                n = n + 1
            posbytedata = bytearray(posarray)
            ser.write(posbytedata)
            print(posbytedata)
            
        img_marker = cv.aruco.drawDetectedMarkers(imgundist.copy(), corners, ids)
        #img_marker = cv.aruco.drawDetectedMarkers(img.copy(), corners, ids)
        cv.imshow('frame', img_marker)
        #print("this took: ", time.time()-start, " ms")
    rawCapture.truncate(0)
    if cv.waitKey(5) & 0xFF == ord('q'):
        break
cv.destroyAllWindows()


