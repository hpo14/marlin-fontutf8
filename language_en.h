#ifndef LANGUAGE_EN_H
#define LANGUAGE_EN_H 1

#ifndef MACHINE_NAME
#define MACHINE_NAME _UxGT("3D Printer")
#endif

#ifndef WELCOME_MSG
  #define WELCOME_MSG                         MACHINE_NAME _UxGT(" ready.")
#endif
#ifndef MSG_SD_INSERTED
  #define MSG_SD_INSERTED                     _UxGT("Card inserted")
#endif
#ifndef MSG_SD_REMOVED
  #define MSG_SD_REMOVED                      _UxGT("Card removed")
#endif
#ifndef MSG_LCD_ENDSTOPS
  #define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#endif
#ifndef MSG_MAIN
  #define MSG_MAIN                            _UxGT("Main")
#endif
#ifndef MSG_AUTOSTART
  #define MSG_AUTOSTART                       _UxGT("Autostart")
#endif

#endif // LANGUAGE_EN_H
