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

/*                   ������ �������� ��������� �������� ������� �����-����   */

  #define CS_ADS_SET()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)   // ��������� ChipSelect
  #define CS_ADS_RESET()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)     // ����������� ChipSelect

  #define RESET_ADS_OFF()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)      // ����������� Reset
  #define RESET_ADS_ON()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)    // ��������� Reset

  #define PWDN_ADS_OFF()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET)      // ����������� PowerDown
  #define PWDN_ADS_ON()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET)    // ��������� PowerDown

  #define START_ADS_SET()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)     // ������ ������� START
  #define START_ADS_RESET()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)   // ������ ������� STOP
/********************************************************************************/

/*                                  ������ ������                               */

  #define _WAKEUP      0x02    // ����� � ������ ���������
  #define _STANDBY     0x04    // ���� � ����� ����������
  #define _RESET       0x06    // �������� ��������� �����-����
  #define _START       0x08    // ����� ���������� �����
  #define _STOP        0x0A    // ������� ���������� �����
  #define _RDATAC      0x10    // ��������� ������ Read Data Continuous (��������� ����������� ���� ��������)
  #define _SDATAC      0x11    // ������� ������ Read Data Continuous mode
  #define _RDATA       0x12    // ����� Read Data

  #define _RREG        0x20    // ������ ������� �� OP-CODE ��� ��������� ���������� � �����-����� �� ��� ������� �������
  #define _WREG        0x40    // ������ ������� �� OP-CODE ��� ��������� ���������� � �����-����� �� ��� ������ ����� �� �������
/********************************************************************************/

/*                                  ������ ������� �����-����                  */

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

/*                                  ������� �������                            */

  void    INIT_TIMER();                             // ����������� ������� ���������� �������� ���������
  void    INIT_ADS();                               // ��������� ����������� ���������
  void    SIMPLE_INIT();                            // �������� ����������� �� ����� ������������ � ���������
  void    WRITE_REG_ADS(uint8_t adr, uint8_t val);  // ����� ������ �� ������� ���������. ����� ���������: ������ �� ��������.
  uint8_t READ_REG_ADS(uint8_t adr);                // ������� ������� ���������. ������� �������� - ������ �������. ��������� - ���� ��������.
  void    DATA_FORMATTING();
/********************************************************************************/

/*                                  ���� ������� � �����-���                   */

  void    WAKEUP_ADS();                             // ������� ������ ��������� � ������ ����������
  void    STANDBY_ADS();                            // ������� ����� � ����� ���������� ���������
  void    RESET_ADS();                              // ������� �������� ���������
  void    START_ADS();                              // ������� ������� ������ �� �������� ����� � ����� ���������
  void    STOP_ADS();                               // ������� ������� ������ ���������

/********************************************************************************/

/*                                  ������� ����� ������� �����-����            */

  void    RDATAC_ADS();                             // ������ ����� � ����� ReadDataContinious 
  void    SDATAC_ADS();                             // ��������� ������ ReadDataContinious 
  void    RDATA_ADS();                              // ������ ����� � ����� ReadData 

/********************************************************************************/

/*                                  �����������-������ �������                 */

  void    DELAY(uint8_t x);                         // ��������! �������� � ����������
  uint8_t TRANSMITTING(uint8_t data);               // �������� ������ ����� ����� � ������ �����-��� �� SPI
  void    UPDATE();                                 // ������� ���������� ������� ����� � ������� �����-����. ������ �� ������������ �� Data Ready
/********************************************************************************/


  
 