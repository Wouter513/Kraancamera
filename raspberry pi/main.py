# import the opencv library
import numpy as np
#from numpy.core import multiarray
import cv2
import random
import time

# define a video capture objectpython
image = np.ones((4388,3292,3), np.uint8) * 0

pt1 = (1564, 4388)
pt2 = (1728, 4388)
pt3 = (1728, 0)

triangle_cnt = np.array( [pt1, pt2, pt3] )
cv2.drawContours(image, [triangle_cnt], 0, (255,255,255), -1)

i = 0
randy = 3308
randx = 0
camera_image = image[randy:randy+1080, randx:randx+1920]
cv2.imshow("vertical", camera_image)

while(True):   
    if cv2.waitKey(1) & 0xFF == ord('q'):
        i = i + 1
        print(str(randx) + " " +str(randy)+ " " + str(xvertical) + " " + str(yvalue))
        if i > 100:
            break
        else:
            randx = random.randrange(0, 1372)
            randy = random.randrange(0, 3308)

            camera_image = image[randy:randy+1080, randx:randx+1920]
    
    gray = cv2.cvtColor(camera_image, cv2.COLOR_RGB2GRAY)
    gray = cv2.bitwise_not(gray)
    bw = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 3, -2)
    #start_time = time.time()
    #vertical = np.copy(bw)
    #rows = vertical.shape[0]
    #verticalsize = rows // 1
    #verticalStructure = cv2.getStructuringElement(cv2.MORPH_RECT, (1, verticalsize))
    #vertical = cv2.erode(vertical, verticalStructure)
    #vertical = cv2.dilate(vertical, verticalStructure)
    #print("Process finished --- %s seconds ---" % (time.time() - start_time))
    #bw2 = cv2.cvtColor(bw, cv2.)
    #start_time = time.time()
    vertical = np.mean(bw, axis=0)
    #print("Process finished --- %s seconds ---" % (time.time() - start_time))
    #print(vertical) 
    #vertical = cv2.resize(vertical,[1920,1])
    #vertical2 = cv2.resize(vertical,[1920,1080])
   
    p = 0
    for p in range(0, len(vertical)):
        if vertical[p] > 1: 
            xvertical = p
            #print(vertical[p])
            #print(xvertical)
            #break

    triangle = bw[0:1080, xvertical-165:xvertical]
    

    counter = 0
    p = 0
    #q = len(triangle) - 1
    #while p < len(triangle):
    #    if q < 
    
    triangle_width = len(triangle[0])
    
    for p in range(0,len(triangle)):                #kan herschreven worden als while loop waar q niet reset, als 1 niet bezet is boven is deze ook beneden vrij en worden er enorm veel dingen te veel gecheckt
        start_time = time.time()
        q = 0
        for q in range(0, triangle_width):
            
            r = triangle_width-q-1
            if triangle[p][r] > 240:
                counter = counter + q
                #print(q)
                break
        print(p)
        print("Process finished --- %s seconds ---" % (time.time() - start_time))
    average = counter/len(triangle)
    yvalue = int((average/164*4388)-(1080/2))
    
    
    
    cv2.imshow("vertical", triangle)
    

# After the loop release the cap object
#vid.release()
# Destroy all the windows
cv2.destroyAllWindows()
#cv2.imwrite('C:/Users/Esmee/Documents/arduinocamera/image.png', image)