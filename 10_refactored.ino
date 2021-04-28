/* A program to use a 3 degree of freedom robot arm + 1 servo for the gripper
 *  It can simply move the robot anywhere within the range but it can also save
 *  multiple positions in a path. I use an SD card shield to save the paths.
 *  It could probably be done with only arduino, but I wanted to be able to save
 *  and edit the files externally, and eventually be able to select a path from 
 *  multiple saved paths. The program was created with the Eezybot MK2 in mind.
 *  
 *  General labels: LR for LEFT-RIGHT, FB for FRONT-BACK, UD for UP-DOWN, GR for GRIPPER
 */

#include <Bounce2.h>
#include <Servo.h>
#include <SPI.h>
#include <SD.h> 

// Potentiometers
const PROGMEM uint8_t LR_POT = A0, FB_POT = A1, UD_POT = A2; 

// Declaring variables for potentiometer readings, setting centerpoint for joystick
uint8_t val0 = 90, val1 = (77+180)/2, val2 = (35+130)/2, val3 = 135;

// Buttons
const PROGMEM uint8_t GRIPPER = 12, RECORD = 7, PLAY = 6, ERASE = 4;

// Instantiate buttons objects from bounce2 class
Bounce2::Button Gripper = Bounce2::Button(); Bounce2::Button Record = Bounce2::Button();
Bounce2::Button Play = Bounce2::Button(); Bounce2::Button Erase = Bounce2::Button();

// Servos
const PROGMEM uint8_t Orange = 3, Blue = 5, Yellow = 9, Green = 2;

// Servo's state
uint8_t gripperState, recordState, playState, eraseState = LOW;

// Creating servo instances (objects) from servo class
Servo LR, FB, UD, GR;

// CS pin for SD Card Module, may vary between shields (is usually pin 4)
const PROGMEM uint8_t chipSelect = 10;

// Declaring miscalleanous variables
uint8_t i, j;
String t = "\t";

void setup() {
  
  // Set servos origin (before Serial to avoid laggy behavior)
  writeServo(val0, LR, 15), (val1, FB, 15), (val2, UD, 15), (val3, GR, 15);
  
  // Start Serial and initiate SD card
  Serial.begin(9600), initiateSDCard();
  
  // Initialize the pushbuttons pin as pullup inputs
  Gripper.attach(GRIPPER, INPUT_PULLUP), Record.attach(RECORD, INPUT_PULLUP);
  Play.attach(PLAY, INPUT_PULLUP), Erase.attach(ERASE, INPUT_PULLUP);
  
  // DEBOUNCE (INTERVAL) TIME IN MILLISECONDS
  Gripper.interval(15), Record.interval(15), Play.interval(15),  Erase.interval(15);
  
  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  Gripper.setPressedState(LOW), Record.setPressedState(LOW), Play.setPressedState(LOW);
  Erase.setPressedState(LOW);
  
  // Assign servos to pins 
  LR.attach(Orange), FB.attach(Blue), UD.attach(Yellow), GR.attach(Green);
  
}

void loop() {
  
 // Buttons must be updated every loop
  Gripper.update(), Record.update(), Play.update(), Erase.update();

  if (Record.pressed()){
    record();
  }
  if (Play.pressed()){
    play();
  }
  if (Erase.pressed()){
    erase();  
  }
  
 // Read potentiometer, send angle to servo if within safe margins
  val0 = joyStick(0, 180, 20, LR_POT, val0, 0);
    writeServo(val0, LR, 15);
  
  val1 = joyStick(77, 180, 20, FB_POT, val1, 0);
    writeServo(val1, FB, 15);
  
  val2 = readPot(UD_POT, 35, 130, val2);
    writeServo(val2, UD, 15);
  
 // Read gripper button state and open/close the claws accordingly
  if (Gripper.pressed()){
  gripperState = !gripperState;
     if (gripperState == LOW){
      val3 = 135;
      }
    else if (gripperState == HIGH){
      val3 = 50;
    }
    writeServo(val3, GR, 15);
  }
      
//  String t = "\t";
//  Serial.println(val0+t+val1+t+val2+t+val3);
  
}

// Declaring functions:

// Function to initiate SD Card

    void initiateSDCard(){
      // Open serial communications and wait for port to open:
      while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
      }
    
      Serial.print(F("Initializing SD card..."));
    
      if (!SD.begin(chipSelect)) {
        Serial.println(F("initialization failed!"));
        while (1);
      }
      Serial.println(F("initialization done."));
    }


// Function to read potentiometer value and map it within servo's range

    int readPot(int POT, int minimum, int maximum, int VAL){
      VAL = analogRead(POT);
      VAL = map(VAL,0,1023, minimum, maximum);
      return VAL;
    }


// Function to send value to servo

    void writeServo(int VAL, Servo &SERVO, int wait){
      SERVO.write(VAL);
      delay(wait);
    }

// Function to use joystick in an incremental way (within range):

    int joyStick(int minimum, int maximum, int k,int POT, int val, int pot){
      pot = map(analogRead(POT), 0, 1023, 0, 180);
      if (pot > 91 || pot < 89){
        val += (pot-90)/k;
      }
      else if (pot < 91 && pot > 89){}
      val = constrain(val , minimum, maximum);
      return val;
    }


// Function to record values sent to servo in an array (path)

    void record(){
      // Declare an array to store values:
      int dataArray[4] = {val0,val1,val2,val3};
    
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      File dataFile = SD.open("servopos.txt", FILE_WRITE);
      // if the file is available, write to it:
        if (dataFile) {
          // Print the values to their respective index
          for (int i =0; i < 4; i++){
          dataFile.println(dataArray[i]);
          }
          dataFile.close();
          // print to the serial port too:
          String t = "\t";
          Serial.println(dataArray[0]+t+dataArray[1]+t+dataArray[2]+t+dataArray[3]);
          
        }
        // if the file isn't open, pop up an error:
        else {
          Serial.println(F("error opening servopos.txt"));
        }
    }

// Function to play back recorded path from record() function.

  void play(){
    // Open the file. Note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("servopos.txt");
     
    // If the file is available, read it unless play button is pressed again
    if (dataFile) {
      while (dataFile.available()) {
       // Append buffer to respective value in the array
       int values[4] = {val0,val1,val2,val3};
          for (j = 0; j < 4; j++){
            values[j] = dataFile.readStringUntil('\n').toInt();
          }
          // Send array to servos
            writeServo(values[0], LR, 15); writeServo(values[1], FB, 15);
            writeServo(values[2], UD, 15); writeServo(values[3], GR, 15);
          // Serial print and delay to let robot reach position
        Serial.println(values[0]+t+values[1]+t+values[2]+t+values[3]);
        delay(1000); 
      }
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println(F("error opening servopos.txt"));
      }
    }

// Function to reset the path (actually deletes the whole file)

    void erase(){
        // delete the file:
      Serial.println(F("Removing servopos.txt..."));
      SD.remove("servopos.txt");
    
      if (SD.exists("servopos.txt")) {
        Serial.println(F("servopos.txt exists."));
      } else {
        Serial.println(F("servopos.txt doesn't exist."));
      }
    }
