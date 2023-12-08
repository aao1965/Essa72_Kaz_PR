	/*_____________________________________________________________ */
/* iomodule.c - модуль базовых процедур ввода-вывода            */
/* для ПР 7084.32.01.400                                        */
/* 03.01.2003 __________________________________________________*/
/* Разработал: Воложин А.Г. ОАО "Пеленг"________________________*/
/*______________________________________________________________*/

#include <intrins.h>
#include <bios.h>
//
#define _B_VER_PR_AT    0x70 //
#define _B_VER_PR_MU    0x10 //
//
extern bit fl_mode_tkn_change;
extern bit fl_mode_dubl_tkn;
extern bit fl_mode_adj;
extern bit fl_mode_uvkv;
extern bit fl_start;
extern bit fl_mode_change_mode;
//extern bit  fl_mode_tas;
extern bit _B_GLOBAL_ERR_HARD;
extern bit _B_ERR_BOM;
extern bit _B_ERR_BV;
extern uchar count;
extern uchar i;
// обработка внешнего прерывания
extern   xdata uchar _RG_FLAG_INT;
extern   xdata uchar _RG_MASK_INT;
// запись в ЦАПы
extern   xdata uchar _RG_DAC_L;
extern   xdata uchar _RG_DAC_H;
//
extern   xdata uchar _RG_CTRL_BV;
extern   xdata uchar _RG_ERR;
extern   xdata uchar _RG_PU_GN_H;
extern   xdata uchar _RG_PU_GN_L;
extern   xdata uchar _RG_PU_VN_H;
extern   xdata uchar _RG_PU_VN_L;
extern   xdata uchar _RG_CTRL;
extern   xdata uchar _RG_CTRL_TAS_RX_START;
extern   xdata uchar ram_reg[15];
// электронные регулировок
extern   xdata uchar reg_eeprom[15];
extern   int _reg_value;
//
extern bit fl_ena_timer_eeprom;
extern bit fl_stop_timer_eeprom;
//
// обмен с БОМом
extern   uchar ptr_rx_bom;
extern   uchar ptr_tx_bom;
extern   uchar cnt_tx_bom;
extern   uchar cnt_rx_bom;
extern   uchar sum_rx_bom;
extern   uchar err_rx_bom;
extern   uchar buf_rx_bom[_C_SIZE_RX_BOM];
extern   uchar buf_tx_bom[_C_SIZE_TX_BOM];
extern   uchar cnt_err_rx_bom;
extern   uchar cnt_err_rx_bv;
extern   bit fl_busy_bom;
//
extern enum_sost_rx_bom sost_rx_bom;
extern uchar num_reg;
extern uchar num_dac;
// обмен с TAS
extern  bit  fl_new_tas_rx;
extern xdata uchar _RG_CTRL_TAS_TX;
extern xdata uchar _RG_CTRL_TAS_RX;
extern xdata uchar _RG_VER_X;
extern xdata uchar _RG_VER_MU;
extern xdata uchar tas_tx[31];
extern xdata uchar tas_rx[15];
//
// обмен с BV
extern bit  fl_new_bv;
extern bit  fl_ena_timer_bv_rx;
extern bit  fl_wait_bv_rx;
extern   xdata uchar bv_tx[15];
extern   xdata uchar bv_rx[15];
//
extern uchar count_tm1;
//uchar count_led=0;
unsigned short count_led=0;
uchar count_start=0;
uchar count_reset=0;

uchar temp;
uchar count_dubl=0;
//
// переменные для хранения наведения от ТАСа
extern uchar nav_tas_vn_h;
extern uchar nav_tas_vn_l;
extern uchar nav_tas_gn_h;
extern uchar nav_tas_gn_l;
//
/* время ожидания на получение информации от БВ  */
bit fl_ena_timer_bv=0;
bit fl_stop_timer_bv=0;
/************************************************/
/************* Начальная инициализация **********/
/************************************************/
  /*_____________________ Начальная инициализация __________________________*/
/* 1 - OKEY ,0 - ERROR */
void Begin_init(void)
{
 EA=0;                        /* запрет всех прерываний */
 //Delay_1ms(200);              /* задержка на всякий случай !!! */
 //
 //
  //
 /*  запись регулировок из EEPROM в ЦАПы */

 // B_EL
num_reg=0;
num_dac=0x0D;
Write_dac(num_dac,Read_eep(num_reg));
Delay_1ms(1);
 // B_AZ
num_reg=2;
num_dac=0x01;
Write_dac(num_dac,Read_eep(num_reg));
Delay_1ms(1);
// S_EL
num_reg=8;
num_dac=0x0C;
Write_dac(num_dac,Read_eep(num_reg));
Delay_1ms(1);
 //  A_AZ
num_reg=10;
num_dac=0x00;
Write_dac(num_dac,Read_eep(num_reg));
Delay_1ms(1);
     // UVEL
num_reg=12;
num_dac=0x0F;
Write_dac(num_dac,Read_eep(num_reg));
Delay_1ms(1);
 // Запись нулевых поправок
//
_reg_value=0;
// Попр ВН
num_dac=0x0E;
Write_dac(num_dac,_reg_value);
Delay_1ms(1);
// Попр ГН
num_dac=0x02;
Write_dac(num_dac,_reg_value);
Delay_1ms(1);
 // UVAZ
num_reg=14;
num_dac=0x03;
_reg_value=Read_eep(num_reg);
Write_dac(num_dac,_reg_value);
Delay_1ms(1);

//
 //
 /* Настройка таймеров  и внешних прерываний */
 /* TMR0 - 16 bit -timer на 1мс ,interrupt - enable */
 TMOD=0x11; // задает режим 16 bit
 TH1=_C_1000MKS >>8;
 TL1=(char)(_C_1000MKS);
 /* TMR1 - 16 bit -timer на 20мс ,interrupt - enable */
 TL0=(char)_C_20MS;
 TH0=_C_20MS >>8;
 //
 TF0=0; // флаг переполнения таймера.
 TF1=0;
 //
 TR0=1; // бит управления таймером 1-пуск 0-стоп
 TR1=1;
 //
 IE0=0; // флаг  внешнего прерывания
 IE1=1;
 //
 IT0=0; // бит управления типом прерывания срез/низкий ур.
 IT1=1;
 //
 /* Настройка UART */
 RCAP2L=_C_SPEED_UARTL;
 RCAP2H=_C_SPEED_UARTH;
 T2CON=0x34;
 T2MOD=0x00;
 SCON=0x50;
 TI=0;
 RI=0;
 IP=0x10;                       /* max priopitet UART-interrupt */
// IE=0x9F;
// запись маски внешних прерываний
_RG_MASK_INT|=_B_M_BV_RX|_B_M_TAS_TX|_B_M_TAS_RX|_B_M_ADJ|_B_M_UVKV|_B_M_TAS|_B_M_DUBL_TKN;

 IE=0x9F;

// задание количества байт в посылке в TAS
_RG_CTRL_TAS_TX=23*0x08;
tas_tx[0]=_C_START_TX_TAS;

// задание количества байт из TAS
_RG_CTRL_TAS_RX=0x42;
_RG_CTRL_TAS_RX_START=0xAA;
//
//  Версии прошивок
bv_tx[11]=_RG_VER_X;
bv_tx[12]=_B_VER_PR_AT;
bv_tx[13]=_B_VER_PR_MU;
//
 fl_mode_tkn_change=0;
}
//
//
/*________________________ Передача блока в BOM ____________________________*/
bit Write_bom()
{uchar sum=0,cnt;
 if (fl_busy_bom==0)
  {fl_busy_bom=1;
   ptr_tx_bom=0;
   cnt_tx_bom=_C_SIZE_TX_BOM-1;
   buf_tx_bom[0]=_C_START_CU_BOM;
   for(cnt=0;cnt<(_C_SIZE_TX_BOM-1);cnt++) {sum=sum^buf_tx_bom[cnt];}
   buf_tx_bom[_C_SIZE_TX_BOM-1]=sum;
   SBUF=buf_tx_bom[0];
   return(1);
  }
// else
//  {return(0);}
  return(0);
}

// Программная задержка на 1 ms
void Delay_1ms(unsigned char Par)
 { idata unsigned char i,j;
      #define c_count 229
    for (j=Par; j!=0; j--)
     for (i=0; i < c_count; i++)
     {_nop_();  _nop_(); _nop_();}
 }
/********************************/
/* Чтение из EEPROM *************/
/********************************/
int Read_eep(uchar n_r)
 { int _r_val;
  EECON=EECON | EEMEN_;
  _r_val=reg_eeprom[n_r+1]*256;
  _r_val=_r_val+reg_eeprom[n_r];
   EECON=EECON&(~EEMEN_);
  return(_r_val);
 }
/********************************/
/* Запись в ЕЕPROM  *************/
/********************************/
bit Write_eep(uchar n_r, int r_v)
 {
    EECON=EECON|EEMEN_|EEMWE_;
   reg_eeprom[n_r]=(char)r_v;
    while (EECON&RDY_);
   while ((EECON&RDY_)==0);
    reg_eeprom[n_r+1]=(r_v >>8);
     while (EECON&RDY_);
   while ((EECON&RDY_)==0);
  EECON&=~(EEMEN_ | EEMWE_);
   return(1);
 }
/********************************/
/* Запись в ЦАП  *************/
/********************************/
void Write_dac(uchar num, int value)
 {
    value=value+2048;
  _RG_DAC_L=num|((uchar)value)<<4;
  _RG_DAC_H=value>>4|((uchar)value)>>4;
  }
/******************************************/
/******************************************/
/********* СИСТЕМА ПРЕРЫВАНИЙ *************/
/******************************************/
/*____ ПРЕРЫВАНИЕ TMR0______*/
/* Медленный таймер - 20ms                                                         */
void T0_int(void) interrupt 1
{ TH0=(_C_20MS >> 8);
  TL0=(uchar)_C_20MS;
  _RG_CTRL^=_B_CLK_WDI; // частота на WDI

}
/*____ ПРЕРЫВАНИЕ TMR1______*/
/* Быстрый таймер: 1ms */
void T1_int(void) interrupt 3
{ TH1=(_C_1000MKS >>8);
  TL1=(char)(_C_1000MKS);
  // сброс через 15 мс после повисания
   ++count_reset;
  if (count_reset>15) {Rst_System();}
 }
/*____ ПРЕРЫВАНИЕ ПО ВНЕШНИМ СОБЫТИЯМ ____*/
void EX0_int(void) interrupt 0
{unsigned char self;
  self=_RG_FLAG_INT;
  // BV RX INTERRUPT
  if (self & _B_F_BV_RX)
  {_RG_CTRL_BV|=_B_RSTI_BV_RX;
   fl_new_bv=1;
   fl_ena_timer_bv_rx=0;
   fl_wait_bv_rx=0;
  }
// TAS_TX INTERRUPT
 if (self & _B_F_TAS_TX)
  {temp=_RG_CTRL_TAS_TX;
  }
// TAS_RX INTERRUPT
 if (self & _B_F_TAS_RX)
  {_RG_CTRL_TAS_RX|=_B_RSTI_TAS_RX;
   fl_new_tas_rx=1;
  }
// переключениу режима ADJ
 if (self & _B_F_ADJ)
  {_RG_CTRL^=_B_MODE_ADJ;
  // запись маски  прерывания 1- прием от БВ 0- нет приема
    fl_mode_adj=~fl_mode_adj;
  }
  // переключениу режима UVKV
 if (self & _B_F_UVKV)
  {_RG_CTRL^=_B_MODE_UVKV;
   fl_mode_uvkv=~fl_mode_uvkv;
   fl_mode_change_mode=1;
  }
  // переключениу режима TAS
 if (self & _B_F_TAS)
  {_RG_CTRL^=_B_MODE_TAS;
//   fl_mode_tas=~fl_mode_tas;
  }
    // переключение режима Дубль-ТКН
 if (self & _B_F_DUBL_TKN)
  {_RG_CTRL^=_B_MODE_DUBL_TKN;
   fl_mode_dubl_tkn=~fl_mode_dubl_tkn;
   fl_mode_tkn_change=1;
  }
}
/*____ ПРЕРЫВАНИЕ ПО ВНЕШНИМ СОБЫТИЯМ ____*/
void EX1_int(void) interrupt 2
{

  ++count_led;
  ++count_start;
  if (count_start==3)
        {fl_start=1;
        count_start=0;
        count_reset=0;}

  if  (fl_mode_adj)
      {

	  if (fl_mode_uvkv)
        {
         if (count_led>300) {_RG_CTRL^=_B_LED;count_led=0;}
        }
        else
        {
       if (count_led>150) {_RG_CTRL^=_B_LED;count_led=0;}
        }
      }
   else
      { //if (fl_mode_tas)
	  if (_RG_CTRL & _B_MODE_TAS)
        {
         if (count_led>400) {_RG_CTRL^=_B_LED;count_led=0;}
        }
         else   {if (count_led>50) {_RG_CTRL^=_B_LED;count_led=0;}}
      }// else
}

/*__________________ ПРЕРЫВАНИЕ UART __________________________________________*/
void UART_int(void) interrupt 4
{data uchar _SBUF;
 if (RI)
  {/*____________ Прием ______________*/
  RI=0;
   _SBUF=SBUF;
 switch (sost_rx_bom)  // анализ состяния обмена с БОМом
   {case s_endbom:
    {break;}
    case s_faultbom:
    {break;}
    case s_waitbom:
    {if (_SBUF==_C_START_BOM) // проверка старта приёма
      {ptr_rx_bom=0;
       cnt_rx_bom=_C_SIZE_RX_BOM;
       sum_rx_bom=_SBUF;
       sost_rx_bom=s_receiptbom;
      }
     break;
    }
    case s_receiptbom:
    {sum_rx_bom=sum_rx_bom ^ _SBUF;
     buf_rx_bom[ptr_rx_bom]=_SBUF;
     --cnt_rx_bom;
     ++ptr_rx_bom;
     if (cnt_rx_bom==0)
      {if (sum_rx_bom==0)
        {sost_rx_bom=s_endbom;}
       else
        {sost_rx_bom=s_faultbom;}
      }
     break;
    }
   }
  }
 else
  {/*_______________ Передача___________ */
    TI=0;
    if (cnt_tx_bom==0)
     {fl_busy_bom=0;
     }
    else
     {SBUF=buf_tx_bom[++ptr_tx_bom];
      cnt_tx_bom--;
     }
  }
}

/*_________________________ Анализ получения блока от BOM ______________________*/
/* если return(x): 0 - получены данные без ошибок */
/*                 1 - нет данных */
/*                 2 - начало приема блока */
/*                 3 - ошибка SUM */
/*                 4 - количество ошибок превысело допуск */
char Analiz_bom()
{ switch (sost_rx_bom)
  {case s_waitbom:       /* ожидание блока */
   {return(1);}

   case s_endbom:        /* блок принят правильно */
   {sost_rx_bom=s_waitbom;


    return(0);
   }

   case s_receiptbom:    /* начало приема блока */
   {return(2);}

   case s_faultbom:      /* принято с ошибкой */
   {if ((++cnt_err_rx_bom)>_C_CNT_FAULT)
     {_B_ERR_BOM=1;
      _B_GLOBAL_ERR_HARD=1;
      sost_rx_bom=s_waitbom;
      return(4);
     }
    else
     {sost_rx_bom=s_waitbom;
      return(3);
     }
    }

  } /* switch */
  return 1;
}
// *****************
// Системный сброс
// *****************
void Rst_System(void)
{
/*
 _RG_CTRL|=_B_H0_S1_WDI;
 EA=0;
 while (true);
*/
 count_reset = 0;
}
//
void Change_num(uchar Par, uchar *num_r,uchar *num_d)
 /* Перевод номера ЦАПа и Регулировки */
{ // анализ номера регулировки
  switch (Par&0x0F)
  {// B_EL
   case 1: {*num_r=0;*num_d=0x0D; break;}
   case 7: {*num_r=0;*num_d=0x0D; break;}
   case 9: {*num_r=0;*num_d=0x0D; break;}
   // B_AZ
   case 2: {*num_r=2;*num_d=0x01; break;}
   case 6: {*num_r=2;*num_d=0x01; break;}
   case 10: {*num_r=2;*num_d=0x01; break;}
   //BSEL
   case 14: {*num_r=4;*num_d=0x0D; break;}
   //BSAZ
   case 13: {*num_r=6;*num_d=0x01; break;}
   // S_EL
   case 11: {*num_r=8;*num_d=0x0C; break;}
   // A_AZ
   case 12: {*num_r=10;*num_d=0x00;break;}
   // D_EL
   case 3: {*num_r=12;*num_d=0x0F;break;}
   // D_AZ
   case 4: {*num_r=14;*num_d=0x03;break;}

   case 8: {*num_r=18;*num_d=0x01;break;}
   case 15: {*num_r=16;*num_d=0x0D;break;}
  }
  }
//
/* Перевод в дополнительный код */
int Tran_pr_dop(uchar vol1,vol2,maska,maska_sg)
{ int value;
  value=vol1+(vol2&maska)*256;
  if ((vol2&maska_sg)!=0) {value=-value;}
  return(value);
}



