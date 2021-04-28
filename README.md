# EezyBotMK2
Arduino Code for the EezyBot MK2 with memory for path

Hi folks! First and foremost, thank you for considering my code to run your EezyBot MK2 or any servo driven 3 DOF robot arm! Credits to Carlo Franciscone aka daGHIZmo on thingiverse for the 3D printable model! 

  Now to wire this thing you need either a 10K joystick, a 10k potentiometer and 3 push-buttons, or 4  10K potentiometers and 3 buttons. I did the memory functions using an SD shield, but will probably work out a solution without one eventually. 
  
  The buttons use the INPUT_PULLUP of the arduino digitalPins. This means one wire goes to the ground and the other to the digital input of your choice. 
    After some experimentation, I would suggest building it with 4 potentiometers instead of a joystick! Saves a lot of hassles and will be easier to program for movements!
    
  This code is only a start! I would like to add a function and a button to loop a recorded path (right now it runs it only once) This shouldn't be too bad. Eventually I'd like to set some real cartesian coordonates to manipulate it, but it looks rather complicated and I am not able to do it at the moment. 
   
   Feel free to comment, use, modify, swear or curse this code! :) 

![20210428_124715](https://user-images.githubusercontent.com/81876951/116444084-5c8c8700-a822-11eb-8304-6fdb869e4cab.jpg)
