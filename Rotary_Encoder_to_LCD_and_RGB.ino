//source: https://yopiardinal.wordpress.com/2021/02/23/
//        counter-menggunakan-ky-040-dengan-arduino-uno-dan-lcd-1602-i2c-module/
//Modified and Expanded by Aldi Farhan Fahrosa
//IG: @dialectricals

//--------------------------------------------------------------------------------
#include <Wire.h> //library i2c
#include <LiquidCrystal_I2C.h> //library LCD16x02

LiquidCrystal_I2C lcd (0x27, 16, 2);

#define A 4 //CLK
#define B 3 //DT
#define SW 5 //SW
//--------------------------------------------------------------------------------
unsigned int counter = 0;
int currentStateA; //Deklarasi kondisi awal A (CLK)
int lastStateA; //Deklarasi kondisi akhir A (CLK)
int buttonState; //Info Kondisi terkini dari button
unsigned long lastButtonPress = 0; //info kondisi button sebelumnya 

String currentDir =""; 
String LED =""; 

int leds [] = {7, 8, 9, 10}; //array led rgby

int ledStatus [5] = {0};
int timer = 1000;

//--------------------------------------------------------------------------------
void setup ()
{
  //set rotary encoder
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(SW, INPUT_PULLUP);
  updateCounter();

  for (int i = 0; i < 4; i++)
  {
    pinMode(leds[i], OUTPUT);
  }
  counter = 0;
    
  //baca status awal CLK (A)
  lastStateA = digitalRead(A);

  //lcd.init();
  //inisialisi LCD
  Serial.begin(9600);

  //Loading screen
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("LED Control...");
  lcd.setCursor(0, 1);
  lcd.print("ROTARY ENCODER");
  
}
//--------------------------------------------------------------------------------
void loop()
{
  currentStateA = digitalRead(A); //baca status saat ini A (CLK)

  // Jika status CLK terakhir dan saat ini berbeda, maka pulse terjadi
  // Bereaksi hanya untuk 1 perubahan status untuk menghindari penghitungan ganda                                      
  if(currentStateA != lastStateA && currentStateA == 1)   
  {
    if (digitalRead(B) != currentStateA) 
    // Encoder memutar searah jarum jam sehingga terjadi penambahan
    {
      counter ++;
      currentDir = "CW";
    }
    else
    {
       // Jika status DT berbeda dari status CLK maka
       // encoder akan memutar berlawanan arah jarum jam menjadi pengurangan
      counter--;
      currentDir = "CCW";
    }
    updateCounter();
    Serial.print("ARAH: "); 
    Serial.print(currentDir); 
    Serial.print(" | NILAI: "); 
    Serial.println(counter);

  }
  //mengambil status A (CLK) terakhir
  lastStateA = currentStateA;

  //baca status button (SW)
  buttonState = digitalRead(SW);

  // Jika mendeteksi sinyal LOW, tombol ditekan
  if(buttonState == LOW)
  {
    /*jika 50ms telah berlalu sejak pulsa LOW terakhir, itu berarti bahwa
     tombol telah ditekan, dilepas dan ditekan lagi*/
    if(millis() - lastButtonPress > 50)
    {
      eksekusi();
      updateCounter();
    }
    lastButtonPress = millis();
  }
  delay(1);
} 

//--------------------------------------------------------------------------------
void updateCounter()
{
  switch (counter)
  {
    case 0:
      counter = 1;
      break;
    case 1:
      printLcd();
      lcd.print("| RED >");
      break;
    case 2:
      printLcd();
      lcd.print("| <GREEN>");
      break;
    case 3:
      printLcd();
      lcd.print("| <BLUE>");
      break; 
    case 4:
      printLcd();
      lcd.print("| <YELLOW");
      break; 
    case 5:
      counter = 4;
      break;
  }
}
//--------------------------------------------------------------------------------
void printLcd()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LED Switcher");
  lcd.setCursor(0, 1);
  lcd.print(currentDir);
  lcd.setCursor(5, 1);
}
//--------------------------------------------------------------------------------
void eksekusi()
{
  switch(counter)
  {
    case 1:
      aksi1();
      break;
    case 2:
      aksi2();
      break;
    case 3:
      aksi3();
      break;
    case 4:
      aksi4();
      break;
  }
}
//--------------------------------------------------------------------------------
void aksi1()
{
  if(ledStatus[counter] != 0)
  {
    ledStatus[counter] = 0;
    lcd.clear();
    lcd.print("> LED RED OFF...");
    digitalWrite(leds[counter - 1], ledStatus[counter]);
    delay(timer);
  } 
  else
  {
    ledStatus[counter] = 1;
    lcd.clear();
    lcd.print("> LED RED ON...");
    digitalWrite(leds[counter - 1], ledStatus[counter]);
    delay(timer);
  }
}

void aksi2()
{
  if(ledStatus[counter] != 0)
  {
    ledStatus[counter] = 0;
    lcd.clear();
    lcd.print("> LED GREEN OFF...");
    digitalWrite(leds[counter - 1], ledStatus[counter]);

    delay(timer);
  } 
  else
  {
    ledStatus[counter] = 1;
    lcd.clear();
    lcd.print("> LED GREEN ON...");
    digitalWrite(leds[counter - 1], ledStatus[counter]);
    delay(timer);
  }
}

void aksi3()
{
  if(ledStatus[counter] != 0)
  {
    ledStatus[counter] = 0;
    lcd.clear();
    lcd.print("> LED BLUE OFF...");
    digitalWrite(leds[counter - 1], ledStatus[counter]);
    delay(timer);
  } 
  else
  {
    ledStatus[counter] = 1;
    lcd.clear();
    lcd.print("> LED BLUE ON...");
    digitalWrite(leds[counter - 1], ledStatus[counter]);
    delay(timer);
  }
}

void aksi4()
{
  if(ledStatus[counter] != 0)
  {
    ledStatus[counter] = 0;
    lcd.clear();
    lcd.print("> LED YELLOW OFF...");
    digitalWrite(leds[counter - 1], ledStatus[counter]);
    delay(timer);
  } 
  else
  {
    ledStatus[counter] = 1;
    lcd.clear();
    lcd.print("> LED YELLOW ON...");
    digitalWrite(leds[counter - 1], ledStatus[counter]);
    delay(timer);
  }
}