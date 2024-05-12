#include <LiquidCrystal.h>

//INPUTS
#define FirstButton 13
#define SecondButton 12
#define ThirdButton 11

//OUTPUTS
#define RedLight 10
#define GreenLight 9
#define Sound 8

#define drinksNum 6

bool arrowDownButtonPrevState;
bool arrowUpButtonPrevState;

byte upChar[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

byte downChar[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100
};

struct drink {
  char name[12];
  bool acceptsSugar;
};

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

drink drinks[drinksNum];

int selectedDrinkIndex = -1;
int currentShownDrinkIndex = 3;

void initDrinks(){
  char drinksNames[drinksNum][20] = {
   "Coffee",
   "Tea",
   "Latte",
   "Cappuccino",
   "Espresso",
   "Water"
  };
  
  bool drinksSugarVal[drinksNum] = {
   true,
   true,
   true,
   true,
   true,
   false
  };
  for(int i = 0 ; i < drinksNum ; i++){
    struct drink d;
    strcpy(d.name, drinksNames[i]);
    d.acceptsSugar = drinksSugarVal[i];
    drinks[i] = d;
  }
}

void setup()
{
  lcd.begin(16, 2);
  lcd.createChar(0, downChar);
  lcd.createChar(1, upChar);
  
  pinMode(FirstButton, INPUT_PULLUP);
  pinMode(SecondButton, INPUT);
  pinMode(ThirdButton, INPUT_PULLUP);
  
  pinMode(RedLight, OUTPUT);
  pinMode(GreenLight, OUTPUT);
  pinMode(Sound, OUTPUT);
  
  arrowDownButtonPrevState = digitalRead(FirstButton);
  arrowUpButtonPrevState = digitalRead(ThirdButton);
  
  initDrinks();
}

void loop()
{
  if(selectedDrinkIndex == -1){
    chooseDrinkState();
    readArrowsButtons();
  } else {
    digitalWrite(GreenLight, LOW);
    digitalWrite(RedLight, HIGH);
    askForSugarState();
    prepareDrinkState();
    digitalWrite(RedLight, LOW);
  }
}

void chooseDrinkState(){
  digitalWrite(GreenLight, HIGH);
  lcd.setCursor(0, 0);
  lcd.print(" Kahwe  Machine ");
  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  lcd.setCursor(15, 1);
  lcd.write((byte)1);
  int len = strlen(drinks[currentShownDrinkIndex].name);
  lcd.setCursor((14 - len) / 2 + 1, 1);
  lcd.print(drinks[currentShownDrinkIndex].name);
}

void readArrowsButtons(){
  if(digitalRead(SecondButton)){
    selectedDrinkIndex = currentShownDrinkIndex;
    tone(Sound, 2000, 100);
    return;
  }
  
  bool arrowDownButtonCurrentState = digitalRead(FirstButton);
  bool arrowUpButtonCurrentState = digitalRead(ThirdButton);
    
 if (arrowDownButtonCurrentState == LOW 
     && arrowDownButtonPrevState == HIGH)
  {
   currentShownDrinkIndex--;
   if(currentShownDrinkIndex < 0) 
     currentShownDrinkIndex = drinksNum - 1;
   tone(Sound, 2000, 100);
   lcd.clear();
  }
  
  if (arrowUpButtonCurrentState == LOW 
     && arrowUpButtonPrevState == HIGH)
  {
   currentShownDrinkIndex++;
   if(currentShownDrinkIndex >= drinksNum) 
     currentShownDrinkIndex = 0;
   tone(Sound, 2000, 100);
   lcd.clear();
  }

  arrowDownButtonPrevState = digitalRead(FirstButton);
  arrowUpButtonPrevState = digitalRead(ThirdButton);
}

void askForSugarState(){
  if(!drinks[selectedDrinkIndex].acceptsSugar){
    return;
  }
  lcd.clear();
  int len = strlen(drinks[selectedDrinkIndex].name);
  lcd.setCursor((16 - len) / 2 + 1, 0);
  lcd.print(drinks[selectedDrinkIndex].name);
  
  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  lcd.setCursor(15, 1);
  lcd.write((byte)1);
  
  len = strlen("Needs Sugar?");
  lcd.setCursor((14 - len) / 2 + 1, 1);
  lcd.print("Needs Sugar?");
  while(!digitalRead(FirstButton) && !digitalRead(ThirdButton));
  tone(Sound, 2000, 100);
}

void prepareDrinkState(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Preparing drink");
  int i = 0;
  while(i <= 16){
    lcd.setCursor(i++, 1);
    lcd.print(".");
    tone(Sound, 2000, 500);
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done !!");
  lcd.setCursor(0, 1);
  lcd.print("Welcome any time");
  tone(Sound, 2000);
  delay(3000);
  noTone(Sound);
  lcd.clear();
  selectedDrinkIndex = -1;
  currentShownDrinkIndex = 0;
}