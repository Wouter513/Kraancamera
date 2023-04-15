import cv2
import numpy
from picamera.array import PiRGBArray
from picamera import PiCamera
import time

camera = PiCamera()
camera.resolution = (2592, 1944)
camera.framerate = 5
rawCapture = PiRGBArray(camera, size=(2592,1944))

time.sleep(0.1)

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    image = frame.array
    
    
    
    
    
    #display = cv2.resize(image, (1280,720))
    cv2.imshow("Frame", image)
    key = cv2.waitKey(1) & 0xFF
    rawCapture.truncate(0)
    if key == ord("q"):
        break
                    
    