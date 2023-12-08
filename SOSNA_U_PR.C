/*_____________________________________________________________ */
/* sosna_u_pr.c - Основной управляющий файл для ПР 7084.32.01.400  */
/* 03.01.2003 __________________________________________________*/
/* Разработал: Воложин А.Г. ОАО "Пеленг"________________________*/
/*______________________________________________________________*/
//
//
// Основной файл.
// Сюда внесены изменения по режиму выверки- строка 228 и 177
// добавил новый флаг fl_uvkv_gn
// Надо записать значение смещение по ГН в константу
// _C_UVKV_GN_PLUS  в файле BIOS.H
#include <intrins.h>
#include <bios.h>
//
extern enum_sost_syst    sost_sys; // состояния системы
//
extern bit fl_mode_tkn_change;
extern bit fl_mode_dubl_tkn;
//
extern bit fl_mode_adj;        // флаг режима регулировок
extern bit fl_mode_uvkv;       // флаг режима УВКВ
extern bit fl_start;           // флаг начала цикла
extern bit fl_mode_change_mode;// флаг смены режима
extern bit fl_new_bv;          // флаг получения посылки от БВ
extern bit fl_new_tas_rx;      // флаг получения посылки от ТАСа
extern bit fl_busy_bom;
extern bit  fl_mode_tas;
// Добавил для выверки по боковому срезу ствола
extern bit fl_uvkv_gn;
//
extern   uchar buf_rx_bom[_C_SIZE_RX_BOM];
extern   uchar buf_tx_bom[_C_SIZE_TX_BOM];
extern   int _reg_value;
extern   uchar sum_rx_bom;
extern   xdata uchar reg_eeprom[15];// буфер электр. рег.
extern   xdata uchar bv_rx[15];  // буфер приема от BV
extern   xdata uchar tas_rx[15]; // буфер приема от ТАСа
extern   xdata uchar tas_tx[31];
//
extern uchar num_dac;
extern uchar num_reg;
extern uchar com_bom;
extern   xdata uchar _RG_CTRL;
extern   xdata uchar _RG_DAC_L;
extern   xdata uchar _RG_DAC_H;
//
extern   xdata uchar _RG_PU_CC_GN_H;
extern   xdata uchar _RG_PU_CC_GN_L;
extern   xdata uchar _RG_PU_CC_VN_H;
extern   xdata uchar _RG_PU_CC_VN_L;
//
extern   xdata uchar _RG_PU_TAS_GN_H;
extern   xdata uchar _RG_PU_TAS_GN_L;
extern   xdata uchar _RG_PU_TAS_VN_H;
extern   xdata uchar _RG_PU_TAS_VN_L;
//
extern   xdata uchar bv_tx[15];
extern   xdata uchar bv_rx[15];
//
extern   xdata uchar _RG_CTRL_BV;
extern   xdata uchar _RG_ERR;
extern   xdata uchar _RG_PU_GN_H;
extern   xdata uchar _RG_PU_GN_L;
extern   xdata uchar _RG_PU_VN_H;
extern   xdata uchar _RG_PU_VN_L;
extern   xdata uchar _RG_CTRL;
extern   xdata uchar ram_reg[15];
extern   uchar count_tm1;
//
// переменные для хранения наведения от ТАСа
extern uchar nav_tas_vn_h;
extern uchar nav_tas_vn_l;
extern uchar nav_tas_gn_h;
extern uchar nav_tas_gn_l;
//
extern xdata uchar _RG_CTRL_TAS_TX;
//
int _reg_value_uvkv;
int _reg_value_bv;
//
int _reg_value_0_vn;
int _reg_value_0_gn;
//
int _reg_value_uvkv_vn;
int _reg_value_uvkv_gn;
//
int _popr_vn=0;
int _popr_gn=0;
//
int count_tas=0;
int ill=0;

bit fl_count=1;
//
//bit fl_bv_rx=0; // 0- первая посылка 1- вторая посылка
bit fl_bv_rx=1; // 0- первая посылка 1- вторая посылка
// переменные для хранения крена
// флаг для отработки вкл./выкл. режима Дубль-ТКН во время регулировок
bit fl_mode_change_tkn_reg=0;
uchar cren_l;
uchar cren_h;
//
uchar rg_err;
int _dop_value=0;
//
//
bit fl_gun=1;       // флаг режимов
/************************************************/
/************* Управляющий монитор **************/
/************************************************/
void RTOS(void)
{
   uchar	cont, vl,vh, gl,gh;
   switch (sost_sys)
 {
   case sys1:
   {
//------------------------------------------------
 if (fl_start) {         // пришел импульс старта
//*************************
//  запись поправок в ЦАПы
           num_dac=0x0E;
           Write_dac(num_dac,_popr_vn);
//
           num_dac=0x02;
           Write_dac(num_dac,_popr_gn);
//************************
// передача в БВ
//
// формирование посылки для передачи в БВ
           rg_err=_RG_ERR^0x40;
// Преобразование для ААО кода в цифру
if (rg_err & _B_OK)  // есть ГОТОВ ССЗ
//        { if (rg_err & _B_ERR4) { bv_tx[0]=4; } else {bv_tx[0]=7;}; }
        { if (rg_err & _B_ERR4) { cont = 4; } else {cont = 7;}; }
else // нет ГОТОВ ССЗ анализ почему
{
/*
  bv_tx[0]=0;
if (rg_err & _B_ERR1) { bv_tx[0]=1; };
if (rg_err & _B_ERR2) { bv_tx[0]=2; };
if (rg_err & _B_ERR3) { bv_tx[0]=3; };
if (rg_err & _B_ERR4) { bv_tx[0]=4; };
if (rg_err & _B_ERR5) { bv_tx[0]=5; };
if (rg_err & _B_ERR6) { bv_tx[0]=6; };
*/
	cont = 0;
	if (rg_err & _B_ERR1) { cont = 1; };
	if (rg_err & _B_ERR2) { cont = 2; };
	if (rg_err & _B_ERR3) { cont = 3; };
	if (rg_err & _B_ERR4) { cont = 4; };
	if (rg_err & _B_ERR5) { cont = 5; };
	if (rg_err & _B_ERR6) { cont = 6; };
};
//********************************************************
// Передача MSS condition в ТАС
//  tas_tx[14]=bv_tx[0];
	tas_tx[14] = cont;
//
//
// **********************************************************
// добаляю информацию о режиме регулировок и EDRD
/*
if (fl_mode_adj) {bv_tx[0]|=_C_ADJ;}
            else {bv_tx[0]&=~_C_ADJ;};
if (fl_mode_uvkv) {bv_tx[0]|=_C_UVKV;}
            else {bv_tx[0]&=~_C_UVKV;};
if (fl_mode_dubl_tkn) {bv_tx[0]|=_C_D_TKN;}
            else {bv_tx[0]&=~_C_D_TKN;};
if (fl_mode_tas) {bv_tx[0]|=_C_TAS;}
            else  {bv_tx[0]&=~_C_TAS;};
//
*/
	if (fl_mode_adj)
	{
		cont |= _C_ADJ;
	}
	else
	{
		cont &= ~_C_ADJ;
	}
	if (fl_mode_uvkv)
	{
		cont |= _C_UVKV;
	}
	else 
	{
		cont &= ~_C_UVKV;
	}
	if (fl_mode_dubl_tkn)
	{
		cont |= _C_D_TKN;
	}
	else 
	{
		cont &= ~_C_D_TKN;
	}
//	if (fl_mode_tas)
	if (_RG_CTRL & _B_MODE_TAS)
	{
		cont |= _C_TAS;
	}
	else
	{
		cont &= ~_C_TAS;
	}
	bv_tx[0] = cont;

//********************************************************

//   ********************* АНАЛИЗ РЕЖИМА ТАС *******************
//vl,vh, gl,gh

//if (fl_mode_tas)
if (_RG_CTRL & _B_MODE_TAS)
// режим автосопровождения в БВ передать наведение от ТАСа
	{
/*
     bv_tx[1]=nav_tas_vn_l;
     bv_tx[2]=nav_tas_vn_h;
     bv_tx[3]=nav_tas_gn_l;
     bv_tx[4]=nav_tas_gn_h;
*/
     vl = nav_tas_vn_l;
     vh = nav_tas_vn_h;
     gl = nav_tas_gn_l;
     gh = nav_tas_gn_h;
	}
   else //обычный режим в БВ наведение от МУ
	{
/*
	 bv_tx[1]=_RG_PU_VN_L;
     bv_tx[2]=_RG_PU_VN_H;
     bv_tx[3]=_RG_PU_GN_L;
     bv_tx[4]=_RG_PU_GN_H;
*/
     vl = _RG_PU_VN_L;
     vh = _RG_PU_VN_H;
     gl = _RG_PU_GN_L;
     gh = _RG_PU_GN_H;
	}
// *************************************************************
// перевод в дополнительный код наведения по ВН
//     _dop_value=Tran_pr_dop(bv_tx[1],bv_tx[2],0x0F,0x10);
     _dop_value=Tran_pr_dop(vl,vh,0x0F,0x10);
     bv_tx[1]=(char)_dop_value;
     bv_tx[2]=(_dop_value>>8);
// перевод в дополнительный код наведения по ГН
//     _dop_value=Tran_pr_dop(bv_tx[3],bv_tx[4],0x0F,0x10);
     _dop_value=Tran_pr_dop(gl,gh,0x0F,0x10);
     bv_tx[3]=(char)_dop_value;
     bv_tx[4]=(_dop_value>>8);
// ошибка по ВН
//     bv_tx[5]=ram_reg[3];
//     bv_tx[6]=ram_reg[2];
// перевод в доп код
//     _dop_value=Tran_pr_dop(bv_tx[5],bv_tx[6],0x03,0x04);
     _dop_value=Tran_pr_dop(ram_reg[3],ram_reg[2],0x03,0x04);
     bv_tx[5]=(char)_dop_value;
     bv_tx[6]=(_dop_value>>8);
// ошибка по ГН
//     bv_tx[7]=ram_reg[1];
//     bv_tx[8]=ram_reg[0];
// перевод в доп код
//     _dop_value=Tran_pr_dop(bv_tx[7],bv_tx[8],0x03,0x04);
     _dop_value=Tran_pr_dop(ram_reg[1],ram_reg[0],0x03,0x04);
     bv_tx[7]=(char)_dop_value;
     bv_tx[8]=(_dop_value>>8);
//
// Версии прошивки
// старт передачи блока в BV
     _RG_CTRL_BV|=_B_START_BV_TX;
//
     count_tm1=0;
     fl_start=0;
//

//-------------------------------
// передача в TAS
//
//
if (fl_gun)
{
//    наведение в ТАС---------------
           tas_tx[1]=_RG_PU_VN_L;
           tas_tx[2]=_RG_PU_VN_H;
           tas_tx[3]=_RG_PU_GN_L;
           tas_tx[4]=_RG_PU_GN_H;
}
// если нет, то остается наведение от ААО
//    ошибки МГ --------------------
           tas_tx[5]=ram_reg[7];
           tas_tx[6]=ram_reg[6];
           tas_tx[7]=ram_reg[5];
           tas_tx[8]=ram_reg[4];

//    угол крена -------------------
           tas_tx[9]=cren_l;
           tas_tx[10]=cren_h;

// старт передачи в ТАС
_RG_CTRL_TAS_TX|=_B_START_TAS_TX;

}
//-------------------------------------------
//*****************************************************************
if (fl_new_tas_rx) //  ***********************пришла посылка от ТАСа
// необходимо обработать посылку достать новую информацию
    {// первый байт - стартовая посылка
    // tas_rx[0] - D0<- признак успешного тестирования и готовности АСТ
     // достаем наведение по ВН
     nav_tas_vn_l=tas_rx[0];
     nav_tas_vn_h=tas_rx[1];
     // достаем наведение по ГН
     nav_tas_gn_l=tas_rx[2];
     nav_tas_gn_h=tas_rx[3];
     // Запись наведения в регистры
     //-------------------
     // запись наведения по ВН
      _RG_PU_TAS_VN_L=nav_tas_vn_l;
      _RG_PU_TAS_VN_H=nav_tas_vn_h;
     // запись наведения по ГН
      _RG_PU_TAS_GN_L=nav_tas_gn_l;
      _RG_PU_TAS_GN_H=nav_tas_gn_h;
     fl_new_tas_rx=0; // режим ожидания новой посылки
    }
//********************************************************************
//
//********************************************************************
//     анализ информации от БВ
//********************************************************************
if (fl_new_bv)// приняты данные от БВ
        {
        // вопрос какая посылка
        if (fl_bv_rx) //  вторая посылка
           {
        // проверка признака второй посылки
        if (bv_rx[0]==_C_BV_RX_S)
           {
           if (fl_mode_dubl_tkn)// если режим Дубль ТКН
                {
                  _popr_vn=0;  // поправки ноль
                  _popr_gn=0;
                }
        else //режима Дубль ТКН нет берем поправки
        {
          // запись поправки А
           _reg_value_bv=bv_rx[1]+bv_rx[2]*256;
           if (_reg_value_bv>_C_LIMIT_MAX){_reg_value_bv=_C_LIMIT_MAX;}
           if (_reg_value_bv<_C_LIMIT_MIN){_reg_value_bv=_C_LIMIT_MIN;}
          // поправка по ВН с минусом
           _popr_vn=-_reg_value_bv;
         // запись поправки В
           _reg_value_bv=bv_rx[3]+bv_rx[4]*256;
           if (_reg_value_bv>_C_LIMIT_MAX){_reg_value_bv=_C_LIMIT_MAX;}
           if (_reg_value_bv<_C_LIMIT_MIN){_reg_value_bv=_C_LIMIT_MIN;}
          _popr_gn=_reg_value_bv;
        }
         // получение пульта комадира по ВН
           _reg_value_bv=bv_rx[5]+bv_rx[6]*256;
           if (_reg_value_bv>4095 ) {_reg_value_bv=4095; }
           if (_reg_value_bv<-4095) {_reg_value_bv=-4095;}
           if (_reg_value_bv<0)
             {_reg_value_bv=((4096-(_reg_value_bv & 0x0FFF))|0x1000);}
// запись пульта командира по ВН в регистр
           _RG_PU_CC_VN_L=(char)_reg_value_bv;
           tas_tx[1]=(char)_reg_value_bv;
           tas_tx[2]=(_reg_value_bv>>8);
//           _RG_PU_CC_VN_H=tas_tx[2];
           _RG_PU_CC_VN_H = (uchar)(_reg_value_bv >> 8);
// получение пульта комадира по ГН
            _reg_value_bv=bv_rx[7]+bv_rx[8]*256;
           if (_reg_value_bv>4095 ) {_reg_value_bv=4095; }
           if (_reg_value_bv<-4095) {_reg_value_bv=-4095;}
           if (_reg_value_bv<0)
             {_reg_value_bv=((4096-(_reg_value_bv & 0x0FFF))|0x1000);}
             // запись пульта командира по ГН в регистр
           _RG_PU_CC_GN_L=(char)_reg_value_bv;
           tas_tx[3]=(char)_reg_value_bv;
           tas_tx[4]=(_reg_value_bv>>8);
//           _RG_PU_CC_GN_H= tas_tx[4];
           _RG_PU_CC_GN_H = (uchar)(_reg_value_bv >> 8);
  //------------------------------------------
  //     записываем угол крена для ТАСА
           cren_l=bv_rx[9];
           cren_h=bv_rx[10];
 //-------------------------------------------

				if (!fl_mode_uvkv)
					if (bv_rx[13] & (1 << 0))
					{
						Write_dac(0x0D,Read_eep(0));
						Write_dac(0x01,Read_eep(2));
					}
					else
					{
						Write_dac(0x0D,Read_eep(16));
						Write_dac(0x01,Read_eep(18));
					}
/*
				if (!fl_mode_uvkv)
					if (bv_rx[13] & (1 << 0))
					{
						Write_dac(0x0D,Read_eep(16));
						Write_dac(0x01,Read_eep(18));
					}
					else
					{
						Write_dac(0x0D,Read_eep(0));
						Write_dac(0x01,Read_eep(2));
					}
*/
             }
           fl_new_bv=0;
//           fl_bv_rx=0; // переход к ожиданию первой посылки
        }
       else // первая посылка
       {
        // проверка признака первой посылки
           if (bv_rx[0]==_C_BV_RX_F)
           {
 // обрабатываем полученную информацию
 // Обработка режима DC
if (!(bv_rx[1] & 64)) {_RG_CTRL&=(~_B_DC_BV);} else {_RG_CTRL|=_B_DC_BV;}
 //Определение режима
if (bv_rx[1]&_B_F_MODE) {fl_gun=0;} else {fl_gun=1;}
// записываем информацию для ТАСа
         tas_tx[11]=bv_rx[1];
         tas_tx[12]=bv_rx[2];
         tas_tx[13]=bv_rx[3];
// информация из КАТРИН
// БУ передает (ретранслирует) в ТАС то,что получил из БВ, а он из Катрин
         tas_tx[15]=bv_rx[4];
         tas_tx[16]=bv_rx[5];
         tas_tx[17]=bv_rx[6];
         tas_tx[18]=bv_rx[7];
         tas_tx[19]=bv_rx[8];
         tas_tx[20]=bv_rx[9];
         tas_tx[21]=bv_rx[10];
         tas_tx[22]=bv_rx[11];
//
         fl_new_bv=0;
         fl_bv_rx=1; // переход к ожиданию второй посылки
           }
       else   // это не первая посылка ждем другую
       {
       fl_new_bv=0;
//       fl_bv_rx=0;
       }
       }// end else
      }

//***********************************************************************
//
//-----------------------------------------------------
//                 Анализ режима УВКВ
//-----------------------------------------------------
//
       if (fl_mode_uvkv) // проверка режима UVKV
        // mode uvkv
        {  // вычисление значений регулировок для УВКВ
           _reg_value_uvkv=_reg_value_0_vn+_reg_value_uvkv_vn;
        if (_reg_value_uvkv>_C_LIMIT_MAX) {_reg_value_uvkv=_C_LIMIT_MAX;}
        if (_reg_value_uvkv<_C_LIMIT_MIN) {_reg_value_uvkv=_C_LIMIT_MIN;}
            Write_dac(0x0D,_reg_value_uvkv);
            _reg_value_uvkv=_reg_value_0_gn+_reg_value_uvkv_gn;
       if (_reg_value_uvkv>_C_LIMIT_MAX) {_reg_value_uvkv=_C_LIMIT_MAX;}
       if (_reg_value_uvkv<_C_LIMIT_MIN) {_reg_value_uvkv=_C_LIMIT_MIN;}
            Write_dac(0x01,_reg_value_uvkv);
         } // end if

        else  // если выключили УВКВ
        {
         if (fl_mode_change_mode)
         // not mode uvkv
                {fl_mode_change_mode=0;
                Write_dac(0x0D,Read_eep(0));
                Write_dac(0x01,Read_eep(2));
                }
        }
/*
// проверка режима Дубль ТКН
if ((fl_mode_dubl_tkn)&(~fl_mode_adj))
        // Дубль и было переключение
    {// записываем регулировки для Дубля
     Write_dac(0x0D,Read_eep(16));
     Write_dac(0x0C,Read_eep(18));
    }
//
if ((~fl_mode_dubl_tkn)&(~fl_mode_adj))
        // не Дубль и было переключение
    {// записываем регулировки для не Дубля
     Write_dac(0x0D,Read_eep(0));
     Write_dac(0x0C,Read_eep(8));
    }
*/
//-----------------------------------------------------------------------
//***********************************************************************
//*************************** РЕГУЛИРОВКИ *******************************
//***********************************************************************
//
//if (fl_mode_adj) // проверка режима регулировок
    // если режим то анализ
//    {
    switch (Analiz_bom())
    {case 1: break; // ещё нет данных
     case 3: break; // ошибка контрольной суммы
     case 4: {//sost_sys=sys4; // критическая ошибка
           break; }
     case 0: // данные от БОМА получены -> анализ
      {  Change_num(buf_rx_bom[0],&num_reg,&num_dac);

/*
        if (fl_mode_dubl_tkn)
        {
        switch (num_reg)
         { case 0: {num_reg=16; break;}
           case 8: {num_reg=18; break;}
         }
        }
*/

        // новая команда
        switch (buf_rx_bom[0]&0xF0)
 //------------------------------------------------------
        { case _C_COM_READ: // команда "чтение"
          { _reg_value=Read_eep(num_reg);
            buf_tx_bom[2]=(_reg_value>>8);
            buf_tx_bom[1]=(char)_reg_value;
   //
/*
                if (~fl_mode_dubl_tkn)
                        {
                          _reg_value_0_vn=Read_eep(0);
                          _reg_value_0_gn=Read_eep(2);
                 //
                          _reg_value_uvkv_vn=Read_eep(4);
                          _reg_value_uvkv_gn=Read_eep(6);
                        }
*/
//
// if ((~fl_mode_uvkv)&&(num_reg!=4)&&(num_reg!=6))
//  {Write_dac(num_dac,_reg_value);}
        // ответ БОМУ
           Write_bom();
         while (fl_busy_bom);
           break;};
//-------------------- конец команды ----------------
//
//---------------------------------------------------
case _C_COM_WRITE: // команда "запись"
          {
             if (Write_eep(num_reg,_reg_value))
             {
            buf_tx_bom[1]=_C_SAVE_AN_L;
            buf_tx_bom[2]=_C_SAVE_AN_H;
             }
               // ответ БОМУ
           Write_bom();
         while (fl_busy_bom);
            break;};
case _C_COM_MINUS: // команда "-"
            { if (fl_mode_tkn_change)
                  {_reg_value=Read_eep(num_reg);
                  fl_mode_tkn_change=0;}
              if (_reg_value>_C_LIMIT_MIN) {--_reg_value;}
              buf_tx_bom[2]=(_reg_value>>8);
              buf_tx_bom[1]=(char)_reg_value;
              switch (num_reg)
              {
              //
                case 0: {_reg_value_0_vn=_reg_value;break;}
                case 2: {_reg_value_0_gn=_reg_value;break;}
                //
                case 4: {_reg_value_uvkv_vn=_reg_value;break;}
                case 6: {_reg_value_uvkv_gn=_reg_value;break;}
              }
    if ((~fl_mode_uvkv)&&(num_reg!=4)&&(num_reg!=6))
  {Write_dac(num_dac,_reg_value);}
           // ответ БОМУ
           Write_bom();
         while (fl_busy_bom);
              break;
            };
case _C_COM_PLUS: // команда "+"
            {
             if (fl_mode_tkn_change)
                  {_reg_value=Read_eep(num_reg);
                  fl_mode_tkn_change=0;}
            if (_reg_value<_C_LIMIT_MAX) {++_reg_value;}
              buf_tx_bom[2]=(_reg_value>>8);
              buf_tx_bom[1]=(char)_reg_value;
              switch (num_reg)
              {
              //
                case 0: {_reg_value_0_vn=_reg_value;break;}
                case 2: {_reg_value_0_gn=_reg_value;break;}
                //
                case 4: {_reg_value_uvkv_vn=_reg_value;break;}
                case 6: {_reg_value_uvkv_gn=_reg_value;break;}
              }
  if ((~fl_mode_uvkv)&&(num_reg!=4)&&(num_reg!=6)) {Write_dac(num_dac,_reg_value);}
                // ответ БОМУ
           Write_bom();
         while (fl_busy_bom);
            break;
            };
           };
          break;
      }
    }// switch Analiz_bom
/*
    }
    else
       {
        // проверка режима Дубль ТКН
  if (fl_mode_tkn_change) // была смена режима
      { fl_mode_tkn_change=0;
        if (fl_mode_dubl_tkn)
            {// записываем регулировки для Дубля
                Write_dac(0x0D,Read_eep(16));
                Write_dac(0x0C,Read_eep(18));
            }
        else
                {// записываем регулировки для не Дубля
                Write_dac(0x0D,Read_eep(0));
                Write_dac(0x0C,Read_eep(8));
                }
      }
       }
*/
    break;
   }
   // режим УВКВ
  case sys6:
   {

    break;
   }
  } // switch sost_sys
 }

/************************************************/
/************* Основная программа ***************/
/************************************************/

main()
{
// Чтение из EEPROM значений регулировки 0
_reg_value_0_vn=Read_eep(0);
_reg_value_0_gn=Read_eep(2);
//  Чтение из EEPROM значений регулировки УВКВ
_reg_value_uvkv_vn=Read_eep(4);
_reg_value_uvkv_gn=Read_eep(6);
// начальная инициализация
Begin_init();
// управляющий монитор
while (true) RTOS(); /* бесконечный цикл */
}




