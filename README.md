# EezyBotMK2
Arduino Code for the EezyBot MK2 with memory for path

Hi folks! First and foremost, thank you for considering my code to run your EezyBot MK2 or any servo driven 3 DOF robot arm! Credits to Carlo Franciscone aka daGHIZmo on thingiverse for the 3D printable model! 

  Now to wire this thing you need either a 10K joystick, a 10k potentiometer and 3 push-buttons, or 4  10K potentiometers and 3 buttons. I did the memory functions using an SD shield, but will probably work out a solution without one eventually, if people manifest interest for it. 
  
  The buttons use the INPUT_PULLUP of the arduino digitalPins. This means one wire goes to the ground and the other to the digital input of your choice. 
    After some experimentation, I would suggest building it with 4 potentiometers instead of a joystick! Saves a lot of hassles and will be easier to program for movements!
    
   You need a rather good power source. I ordered an 0-30V 0-10A but 5A would've been waaay okay. It almost never goes above 1A, except in little bursts. So a 3-5A power supply would be sufficient. It needs to run at around 5V. The official MG996r (cheap amazon ones) datasheet says that they run from 4.8V to 7.2V at 2.5A. 
   
   There are soft limits in the code. This is to be set experimentally! Set the limits from 0 to 180 first and then find where the servo starts to have a hard time and take note of the value. Then adjust limits accordingly in the function calls. 
   
   If you dare use the joyStick function instead of the readPot, the k value affects the response of the arm. In the first place I thought it would be nice to have a fast arm and using this method it CAN be very fast, but after fun had been had, it is pointless and less precise to record paths! The precision of the little joystick is actually the problem, compared to a more direct input standard potentiometer. 
    
  This code is only a start! I would like to add a function and a button to loop a recorded path (right now it runs it only once) This shouldn't be too bad. Eventually I'd like to set some real cartesian coordonates to manipulate it, but it looks rather complicated and I am not able to do it at the moment. 
   
   Feel free to comment, use, modify, swear or curse this code! :) 

![20210428_124715](https://user-images.githubusercontent.com/81876951/116444084-5c8c8700-a822-11eb-8304-6fdb869e4cab.jpg)
