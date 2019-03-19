########################################################################
#launch before starting
#sudo rfcomm bind /dev/rfcomm0 20:15:11:23:10:89 1

import Tkinter as tk
import serial
from PIL import Image, ImageDraw, ImageTk
from pyswip import Prolog


ser = serial.Serial('/dev/rfcomm0', 9600)

prolog = Prolog()
prolog.consult("kbrobot.pl")
prolog.assertz("safetemp(29)")
prolog.assertz("criticaltemp(38)")

#im = Image.open("map.png")
#draw.line((0, 0, 30, 30), fill=128)
#draw.point((200,150),fill=255)

ser.write("start-mapping")

im = Image.new("RGB", (400,250))
draw = ImageDraw.Draw(im)
draw.rectangle((170, 220, 230, 250), fill=(25,25,255,0))

dist = 0
headdeg = 0
dirdeg = 0
temperature = 24

            
root = tk.Tk() 
root.title("display image")   
photo=ImageTk.PhotoImage(im)  
cv = tk.Canvas()  
cv.pack(side='top', fill='both', expand='yes')  
cv.create_image(10, 10, image=photo, anchor='nw')
root.update()

counter = 0

while(1):
    valore = ser.readline()
    val1, val2 = valore.split("=")

    if(val1 == "dist"):
        dist = int(val2)
    elif(val1 == "headdeg"):
        val2 = int(45 - int(val2));
        headdeg = val2
    elif(val1 == "dirdeg"):
        dirdeg = int(val2)
    elif(val1 == "temp"):
        temperature = val2

        prolog.retractall("temp(_)")
        prolog.assertz("temp(" + temperature + ")")

        contt = 0
        for soln in prolog.query("saveme(X,Y)"):

            if contt == 1:
                break
            
            if soln["Y"] == 2:
                # qui invio al robot il comando per ruotare e andare via
                # ....
                print ("I'm escaping...")
                ser.write("stop-mapping")
                ser.write("ruota-dx");
                ser.write("ruota-dx");
                ser.write("start-mapping")
            elif soln["Y"] == 1:
                # questa e' un'altra reazione...
                print ("Too high! I'm dead")
                ser.write("stop-mapping")
            contt += 1

        

    print (val2)

    im = Image.new("RGB", (400,250))
    draw = ImageDraw.Draw(im)
    draw.rectangle((170, 220, 230, 250), fill=(25,25,255,0))    
    draw.rectangle((headdeg + 170, 200-dist, headdeg + 230, 220-dist), fill=(255,255,255,0))

    cv.destroy()
    photo=ImageTk.PhotoImage(im)  
    cv = tk.Canvas()  
    cv.pack(side='top', fill='both', expand='yes')  
    cv.create_image(10, 10, image=photo, anchor='nw')    
    root.update()

    counter += 1
