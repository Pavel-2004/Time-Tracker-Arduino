#include <Keypad.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(5, 4, 3, 2, A4, A5);


char hours[99][4] = {};
bool timing[99] = {};
int lastMilli[99] = {};
int totalTime[99] = {};
char password[99][4] = {};







const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad
int LCDCol = 0;
int LCDRow = 0;

//{{{{start, end}, {start, end}, {start:end}, {start}}, code, hourIndex}, {{{start, end}, {start, end}, {start:end}, {start}}, code, hourIndex}, {{{start, end}, {start, end}, {start:end}, {start}}, code, hourIndex}}



Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );





void setup(){
   Serial.begin(115200);
   lcd.begin(16, 2);
   lcd.setCursor(LCDCol, LCDRow);
}


int hourCounter = 0;
int currentMode = 0;
int cursorPin = 0;
String currentPin = "";
bool found;
String message;
int displayCounter = 0;
String currentPinPass;
int counterPin = 0;
char key;
String compare;
char passwordReal;
int currentUserId;
void loop(){
  key = keypad.getKey();
  
  if (key){
    if(key == 'A'){
        lcd.clear();
        currentMode = 0;
        lcd.setCursor(0, 0);
        lcd.print("ENTER CODE");
      } else if(key == 'B'){
        lcd.clear();
        currentMode = 1;
        lcd.print("CREATE EMPLOYEE");
      } else if(key == 'C'){
        lcd.clear();
        currentMode = 2;
        lcd.print("FIND HOURS");
        if(!hourCounter){
          lcd.setCursor(0,1);
          lcd.print("NO EMPLOYEES");
        }
        
      } else {
        if (currentMode == 0){
          if(key == '*'){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ENTER CODE");
            cursorPin = 0;
            currentPin = "";
          } else if(key == '#') {
            lcd.clear();
            found = false;
            for(int i = 0; i < hourCounter; i = i + 1) {
              compare = String(hours[i][0]) + String(hours[i][1]) + String(hours[i][2]) + String(hours[i][3]);             
              if(compare == currentPin){
                found = true;
                Serial.println("here");
                if(timing[i]){
                  message = "LOGGED OUT";
                  totalTime[i] = totalTime[i] + (millis() - lastMilli[i]) / 1000;
                  timing[i] = false;
                  lcd.clear();
                  lcd.print("LOGGED OUT");
                  tone(12, 260);
                  delay(500);
                  noTone(12);
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("ENTER CODE");
                } else {
                  currestUserId = i;
                  currentMode = 4;
                  lcd.clear(0,0);
                  lcd.print("ENTER PASSCODE:");
                }
                break;
              }
            }

            if(!found){
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("INVALID");
              delay(500);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("ENTER CODE");
              cursorPin = 0;
              currentPin = "";
            }
          } else {
            currentPin = currentPin + key;
            lcd.setCursor(cursorPin, 1);
            lcd.print(key);
            cursorPin = cursorPin + 1;
            if(cursorPin == 5){
              cursorPin = 0;
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("ENTER CODE");
              currentPin = "";
            }
          }
        } else if(currentMode == 1) {
          if(key == '*'){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ADD EMPLOYEE");
            currentPin = "";
            cursorPin = 0;
          } else if(key == '#') {
            lcd.clear();
            if(cursorPin == 4){
              
              
              //char tempString[4] 

              
              hours[hourCounter][0] = currentPin.charAt(0);
              hours[hourCounter][1] = currentPin.charAt(1);
              hours[hourCounter][2] = currentPin.charAt(2);
              hours[hourCounter][3] = currentPin.charAt(3);
              
              timing[hourCounter] = false;
              lastMilli[hourCounter] = 0;
              totalTime[hourCounter] = 0;
              
              
              
              
              
              delay(500);
              /*
              lcd.setCursor(0, 0);
              lcd.print("SUCCESS");
              delay(1500);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("ADD EMPLOYEE");
              */
              //currentPin = "";
              cursorPin = 0;
              hourCounter = hourCounter + 1;
              currentMode = 3;
              lcd.setCursor(0, 0);
              lcd.print("CREATE PASSCODE");
              
            } else{
              lcd.setCursor(0,0);
              lcd.print("INVALID");
              delay(1500);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("ADD EMPLOYEE");
              currentPin = "";
              cursorPin = 0;
            }
          } else {
            currentPin = currentPin + key;
            lcd.setCursor(cursorPin, 1);
            lcd.print(key);
            cursorPin = cursorPin + 1;
            if(cursorPin == 5){
              cursorPin = 0;
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("ADD EMPLOYEE");
              currentPin = ""; 
            }
          }
          
        } else if(currentMode == 2) {
          if(hourCounter){
            if(key == '#'){
              displayCounter = displayCounter + 1;
              if(displayCounter == hourCounter){
                displayCounter = 0;
              }
            } else if(key == '*') {
              displayCounter = displayCounter - 1;
              if(displayCounter == -1){
                displayCounter = hourCounter - 1;
              }
            } else if(key == '0'){
              totalTime[displayCounter] = 0;
              timing[displayCounter] = false;
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(String(hours[displayCounter][0]) + String(hours[displayCounter][1]) + String(hours[displayCounter][2]) + String(hours[displayCounter][3])  + " - " + String(totalTime[displayCounter]));
              lcd.setCursor(0, 1);
              lcd.print("CLEARED");
              delay(500);
            }
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("FIND HOURS");
            lcd.setCursor(0, 1);
            lcd.print(String(hours[displayCounter][0]) + String(hours[displayCounter][1]) + String(hours[displayCounter][2]) + String(hours[displayCounter][3])  + " - " + String(totalTime[displayCounter]) + " SEC");
          } 
        } else if(currentMode == 3){
          if(key == '*'){
            counterPin = 0;
            currentPinPass = "";
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("CREATE PASSCODE");
          } else if(key == '#') {
            if(counterPin == 4){
            

              
              password[hourCounter][0] = currentPinPass.charAt(0);
              password[hourCounter][1] = currentPinPass.charAt(1);
              password[hourCounter][2] = currentPinPass.charAt(2);
              password[hourCounter][3] = currentPinPass.charAt(3);
              
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("PASSWORD SET");
              delay(500);
              lcd.clear();
              lcd.print("CREATE CODE");
              currentMode = 0;
            } else {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("INVALID");
              delay(500);
              lcd.clear();
              currentPinPass = "";
              currentPin = "";
              counterPin = 0;
            }
          } else {
            if(counterPin == 4){
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("CREATE PASSCODE");
              counterPin = 0;
              currentPinPass = "";
            } else {
              lcd.setCursor(counterPin, 1);
              lcd.print(key);

              counterPin = counterPin + 1;
              currentPinPass = currentPinPass + String(key);
            }
          }
        } else if(currentMode == 4){
          if(key == '*'){
            counterPin = 0;
            currentPinPass = "";
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("ENTER PASSCODE");
          } else if(key == '#'){
            if(counterPin == 4){
              if((String(password[currentUserId][0]) + String(password[currentUserId][1]) + String(password[currentUserId][2]) + String(password[currentUserId][3])) == currentPinPass){
                  timing[currentUserId] = true;
                  lastMilli[currentUserId] = millis();
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("PASSWORD CORRECT");
                  lcd.setCursor(0, 1);
                  lcd.print("LOGGING IN");
                  tone(12,260);
                  delay(500);
                  noTone(12);
                  currentMode = 0;
                  counterPin = 0;
                  currentPinPass = "";
                  
              } 
            }
          } else {
            if(counterPin == 4){
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("ENTER PASSCODE");
              counterPin = 0;
              currentPinPass = "";
            } else {
              lcd.setCursor(counterPin, 1);
              lcd.print(key);
              counterPin = counterPin + 1;
              currentPinPass = currentPinPass + String(key)
            }
          }
        }
      }
    
  }
}
