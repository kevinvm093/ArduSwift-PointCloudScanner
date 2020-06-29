#include "KLCD.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

namespace KLCD
{
   void Configure()
   {
      lcd.init();
      lcd.clear();
      lcd.backlight();
      lcd.setCursor(3, 0);
      lcd.print("Initalizing");
   }
   void DisplayStatus(State &state)
   {
      switch (state)
      {
      case State::NotReady:
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Device not ready...");
         lcd.setCursor(0, 1);
         lcd.print("Press button to");
         lcd.setCursor(0, 2);
         lcd.print("Configure.");
         break;

      case State::Ready:
         lcd.clear();
         lcd.setCursor(3, 0);
         lcd.print("Device Ready!");
         lcd.setCursor(0, 1);
         lcd.print("Push button to scan.");
         lcd.setCursor(0, 2);
         break;

      case State::Calibrate:
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Move MPU to config.");
         lcd.setCursor(0, 1);
         lcd.print("Point LIDAR UP ^");
         lcd.setCursor(0, 2);
         lcd.print("Press button when done");
         ;
         break;

      case State::Done:
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Scan Complete");
         break;

      case State::Scan:
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Scanning room...");
         lcd.setCursor(0, 1);
         lcd.print("Status bar:");
         lcd.setCursor(0, 2);
         lcd.print("[");
         lcd.setCursor(20, 2);
         lcd.print("]");
         break;
      }
   }
} // namespace KLCD
