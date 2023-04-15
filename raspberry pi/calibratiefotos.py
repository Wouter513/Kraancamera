from time import sleep
from picamera import PiCamera

camera = PiCamera()
camera.resolution = (1296,972)
camera.start_preview()
camera.preview.alpha = 128

sleep(2)
i = 49
while (True):
    camera.capture('/home/kraancamera/WindowsShare/Esmee/Documents/arduinocamera/raspberry pi branch 21-12-2022/Esmee/KraanCamera/threadtest/calibratie{}.jpg'.format(i))
    i = i + 1
camera.stop_preview()
#for filename in camera.capture_continuous("/home/kraancamera/Documents/KraanCamera/aruco/arucofotos{counter:05d}.jpg"):
#    print(filename)

#sleep(30)
#camera.stop_preview()
#'/home/kraancamera/WindowsShare/Esmee/Documents/arduinocamera/raspberry pi branch 21-12-2022/Esmee/KraanCamera/rechte lijnen test/lijntest{counter:05d}.jpg'