/********************************************************************************
 * Library EEG module ADS1299                                                   *
 *                                                                              *
 * file         ADS1299.h                                                       *
 * author       @YaroslavHordiienko                                             *
 * date         03.09.2020                                                      *
 *                                                                              *
 ********************************************************************************/
  #include <stdio.h>
  #include "main.h"
/********************************************************************************/

/*                   Зовнішні контакти керування периферією першого фронт-енду   */

  #define CS_ADS_SET()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)   // Активація ChipSelect
  #define CS_ADS_RESET()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)     // Деактивація ChipSelect

  #define RESET_ADS_OFF()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)      // Деактивація Reset
  #define RESET_ADS_ON()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)    // Активація Reset

  #define PWDN_ADS_OFF()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET)      // Деактивація PowerDown
  #define PWDN_ADS_ON()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET)    // Активація PowerDown

  #define START_ADS_SET()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)     // Аналог команти START
  #define START_ADS_RESET()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)   // Аналог команти STOP
/********************************************************************************/

/*                                  Адреси команд                               */

  #define _WAKEUP      0x02    // Вихід з режиму очіування
  #define _STANDBY     0x04    // Вхід в режим очікування
  #define _RESET       0x06    // Скидання мікросхеми фронт-енду
  #define _START       0x08    // Старт зчитування даних
  #define _STOP        0x0A    // Зупинка зчитування даних
  #define _RDATAC      0x10    // Ввімкнення режиму Read Data Continuous (вмикається автоматично після скидання)
  #define _SDATAC      0x11    // Зупинка режиму Read Data Continuous mode
  #define _RDATA       0x12    // Режим Read Data

  #define _RREG        0x20    // Перший додаток до OP-CODE для правильної комунікації з фронт-ендом під час читання регістрів
  #define _WREG        0x40    // Перший додаток до OP-CODE для правильної комунікації з фронт-ендом під час запису доних до регістрів
/********************************************************************************/

/*                                  Адреси регістрів фронт-енду                  */

  #define ID           0x00    //
  #define CONFIG1      0x01    //
  #define CONFIG2      0x02    //
  #define CONFIG3      0x03    //
  #define LOFF         0x04    //
  #define CH1SET       0x05    //
  #define CH2SET       0x06    //
  #define CH3SET       0x07    //
  #define CH4SET       0x08    //
  #define CH5SET       0x09    //
  #define CH6SET       0x0A    //
  #define CH7SET       0x0B    //
  #define CH8SET       0x0C    //
  #define BIAS_SENSP   0x0D    //
  #define BIAS_SENSN   0x0E    //
  #define LOFF_SENSP   0x0F    //
  #define LOFF_SENSN   0x10    //
  #define LOFF_FLIP    0x11    //
  #define LOFF_STATP   0x12    //
  #define LOFF_STATN   0x13    //
  #define GPIO         0x14    //
  #define MISC1        0x15    //
  #define MISC2        0x16    //
  #define CONFIG4      0x17    //
/********************************************************************************/

/*                                  Загальні функції                            */

  void    INIT_TIMER();                             // Ініціалізація таймеру тактування зовнішньої мікросхеми
  void    INIT_ADS();                               // Початкова ініціалізація мікросхеми
  void    SIMPLE_INIT();                            // Внутрішня ініціалізація та запис конфігурації в мікросхему
  void    WRITE_REG_ADS(uint8_t adr, uint8_t val);  // Запис данних до регістру мікросхеми. Вхідні параметри: адреса та значення.
  uint8_t READ_REG_ADS(uint8_t adr);                // Читання регістру мікросхеми. Вхідний параметр - адреса регістра. Вихіднинй - його значення.
  void    DATA_FORMATTING();
/********************************************************************************/

/*                                  Прямі команди в фронт-енд                   */

  void    WAKEUP_ADS();                             // Команда виходу мікросхеми з режиму очікування
  void    STANDBY_ADS();                            // Команда входу в режим очікування мікросхеми
  void    RESET_ADS();                              // Команда скидання мікросхеми
  void    START_ADS();                              // Команда початку роботи по передачі даних з першої мікросхеми
  void    STOP_ADS();                               // Команда зупинки роботи мікросхеми

/********************************************************************************/

/*                                  Команди режиу читання фронт-енду            */

  void    RDATAC_ADS();                             // Прийом даних в режимі ReadDataContinious 
  void    SDATAC_ADS();                             // Вимкнення режиму ReadDataContinious 
  void    RDATA_ADS();                              // Прийом даних в режимі ReadData 

/********************************************************************************/

/*                                  Архітектуро-залежні функції                 */

  void    DELAY(uint8_t x);                         // Блокуюча! затримка в міліСекундах
  uint8_t TRANSMITTING(uint8_t data);               // Передача одного байту даних по SPI
  void    UPDATE();                                 // Функція ініціювання прийому даних. Запуск за перериванням по Data Ready
/********************************************************************************/


  
 