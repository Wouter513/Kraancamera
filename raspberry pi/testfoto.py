from time import sleep
from picamera import PiCamera

camera = PiCamera()
camera.resolution = (1920,1080)
camera.start_preview()
camera.preview.alpha = 128

sleep(2)
camera.capture('test.jpg')
camera.stop_preview()