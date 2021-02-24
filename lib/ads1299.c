/********************************************************************************
 * Library EEG module ADS1299                                                   *
 *                                                                              *
 * file         ADS1299.c                                                       *
 * author       @YaroslavHordiienko                                             *
 * date         03.09.2020                                                      *
 *                                                                              *
 ********************************************************************************/
  #include "main.h"
  #include "ads1299.h"

  extern SPI_HandleTypeDef hspi2;
  extern TIM_HandleTypeDef htim1;

/********************************************************************************/

/*                    ����������� ������� ���������� �����-����               */

  void INIT_TIMER() {

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);                                     // 2.048MHz
  }

/********************************************************************************/

/*                         ������� ����������� �����-����                     */

  void INIT_ADS() {

    CS_ADS_RESET();
    RESET_ADS_ON();
    DELAY(5);
    RESET_ADS_OFF();
    DELAY(5);
    PWDN_ADS_OFF();
    DELAY(5);
  }

  void SIMPLE_INIT(){
    
    WAKEUP_ADS();
    DELAY(20);

    WRITE_REG_ADS(CONFIG1, 0xB4);
    DELAY(20);

    WRITE_REG_ADS(CONFIG2, 0xC6);
    DELAY(20);

    WRITE_REG_ADS(CH1SET, 0x00);
    DELAY(20);
    WRITE_REG_ADS(CH2SET, 0x00);
    DELAY(20);
    WRITE_REG_ADS(CH3SET, 0x00);
    DELAY(20);
    WRITE_REG_ADS(CH4SET, 0x00);
    DELAY(20);
    WRITE_REG_ADS(CH5SET, 0x00);
    DELAY(20);
    WRITE_REG_ADS(CH6SET, 0x00);
    DELAY(20);
    WRITE_REG_ADS(CH7SET, 0x00);
    DELAY(20);
    WRITE_REG_ADS(CH8SET, 0x00);
    DELAY(20);

    WRITE_REG_ADS(CONFIG3, 0xE8);
    DELAY(20);

    WRITE_REG_ADS(BIAS_SENSP, 0xFF);
    DELAY(20);
    WRITE_REG_ADS(BIAS_SENSN, 0xFF);
    DELAY(20);

    START_ADS_SET();
    START_ADS();
  }

/********************************************************************************/

/*                          ������� ����������� �������                      */

  void WRITE_REG_ADS(uint8_t adr, uint8_t val){

    uint8_t opcode = adr + _WREG;
  
    CS_ADS_SET();
    TRANSMITTING(opcode);
    opcode = 0x00;
    TRANSMITTING(opcode);
    opcode = val;
    TRANSMITTING(opcode);
    CS_ADS_RESET();
  }

  uint8_t READ_REG_ADS(uint8_t adr){

    uint8_t opcode = 0;
    uint8_t ret = 0;

    CS_ADS_SET();
    TRANSMITTING(_SDATAC);
    opcode = adr + _RREG;
    TRANSMITTING(opcode);
    opcode = 0x00;
    TRANSMITTING(opcode);
    ret = TRANSMITTING(0x00);
    TRANSMITTING(_RDATAC);
    CS_ADS_RESET();
    return ret;
  }

/********************************************************************************/

/*                              ������� �������� ������                         */

  void WAKEUP_ADS() {

    CS_ADS_SET();
    TRANSMITTING(_WAKEUP);
    CS_ADS_RESET();
    DELAY(50);
  }

  void STANDBY_ADS() {

    CS_ADS_SET();
    TRANSMITTING(_STANDBY);
    CS_ADS_RESET();
  }

  void RESET_ADS() {

    CS_ADS_SET();
    TRANSMITTING(_RESET);
    DELAY(100);             // �������� (18.0*tCLK) (�������, ��. 35)
    CS_ADS_RESET();
  }

  void START_ADS() {

    CS_ADS_SET();
    TRANSMITTING(_START);
    CS_ADS_RESET();
  }

  void STOP_ADS() {

    CS_ADS_SET();
    TRANSMITTING(_STOP);
    CS_ADS_RESET();
 }
/********************************************************************************/

/*                      ������� ���� ������ ������� �����                      */

  void RDATAC_ADS() {

    CS_ADS_SET();
    TRANSMITTING(_RDATAC);
    CS_ADS_RESET();
  }

  void SDATAC_ADS() {

    CS_ADS_SET();
    TRANSMITTING(_SDATAC);
    CS_ADS_RESET();
  }

  void RDATA_ADS() {

	TRANSMITTING(_RDATA);
  }

/********************************************************************************/

/*                          ������������-������ �������                        */

  void DELAY(uint8_t x) {

    HAL_Delay(x);
  }


  uint8_t TRANSMITTING(uint8_t data) {

    uint8_t opcode1 = data;
    uint8_t opcode2 = 0;
    HAL_SPI_TransmitReceive(&hspi2, &opcode1, &opcode2, sizeof(opcode2), 0x100);
    return opcode2;
  }


  uint8_t RECEIVE() {

    uint8_t ret = 0;
    HAL_SPI_Receive(&hspi2, &ret, sizeof(ret), 100);
    return ret;
  }

    
/********************************************************************************/
/*                      ���������� ������� ����� � �����-����                  */

  void UPDATE(){

    CS_ADS_SET();
    for(int i=0; i<27; i++){
      output[i] = 0;
    }

    HAL_SPI_Receive_DMA(&hspi2, output, 27);
  }

/********************************************************************************/ 
   void DATA_FORMATTING() {
    CS_ADS_RESET();                     // �������� ChipSelect ������� �����-����
    int x = 0;                          // ����� ��� �������� ������ �� ��� ������� ������
    uint32_t dataPacket = 0;            // ����� ��� ���������� ������
    uint32_t buff_ads[9];				// ������ ����� ������ ����� � ���������
  	__NVIC_DisableIRQ(EXTI15_10_IRQn);  // �������� ����������� DataReady �� ��� ������� ������ � ����� � ������

// *********************************************************************************************** //
//                                                                                                 //
//                           ���������� ������ ������� ����� � ������ ������� ����                 //
//                                                                                                 //
// *********************************************************************************************** //
                                                                                                   
    for (int i = 0; i<9; i++){       // ���� �������� 27 ���� � ����������� ������ 9*3            
      for(int j = x; j<x+3; j++){    // ���� ��'�������� ����� ���� � ���� �����                  
        dataPacket = (dataPacket<<8)| output[j];                                                  
      }                                                                                            
                                                                                                   
      x=x+3;	                     // ���������� ������ (3 ����� ����� �� ������ �����)          
                                                                                                   
// *********************** ������������ ����� � ���������� ����� � ������� ********************** //
                                                                                                   
      if(dataPacket & 0x00800000){                                                                 
      dataPacket |= 0xFF000000;                                                                    
      }                                                                                            
                                                                                                   
// *********************************************************************************************** //
                                                                                                   
      buff_ads[i] = dataPacket;      // ����� ����� � ������� �������� ������                    
      dataPacket = 0;                // �������� ��������� ������                                
    }                                                                                              
                                                                                                        
// *********************************************************************************************** //
       __NVIC_EnableIRQ(EXTI15_10_IRQn);   // ����� �� ����������� DataReady    
  }
  