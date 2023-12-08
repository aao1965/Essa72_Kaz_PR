#include <AT89S8253.h>

#define TIMER_HIGH		0xE3
#define TIMER_LOW		0x5B

#define CONTROL			0
#define INT_MASK		1
#define INT_FLAGS		2

#define TAS_TX_CNTR		3
#define TAS_RX_CNTR		4

#define BU_BV_CNTR		5

#define DAC_LOW			6
#define DAC_HIGH		7

//#define TAS_RX_		13

#ifdef DEBUG
unsigned char code nl [] = {"\r\n"};
unsigned char code hw [] = {"Hello world!!!"};
#endif

xdata unsigned char fpga_cntr_reg [16]		_at_ 0x2100;
xdata unsigned char fpga_pu_reg [8]			_at_ 0x2200;

xdata unsigned char data_rx_tas [16]		_at_ 0x2020;
xdata unsigned char data_tx_tas [32]		_at_ 0x2000;

xdata unsigned char data_rx_bv [16]			_at_ 0x2060;
xdata unsigned char data_tx_bv [16]			_at_ 0x2040;

xdata unsigned char ram_reg [15]			_at_ 0x2080;    // Буфер от МУ
/*
xdata uchar _RG_PU_GN_H   _at_ 0x02109;
xdata uchar _RG_PU_GN_L   _at_ 0x0210A;
xdata uchar _RG_PU_VN_H   _at_ 0x0210B;
xdata uchar _RG_PU_VN_L   _at_ 0x0210C;
*/
int		dac_cnt;

unsigned char	cnt;
//unsigned char	cnt_bv;

#ifdef DEBUG
void send_info (unsigned char *str)
{
	while (*str)
	{
		do {} while (SPSR & WCOL);
		SPDR = *str;
		str++;
	}
}
#endif
/*
void isr_ex1 (void) interrupt IE1_VECTOR
{
	unsigned char buff;

	if (cnt == 133)
	{
		cnt = 0;
		buff = fpga_cntr_reg[CONTROL];
		buff ^= 0x80;
		fpga_cntr_reg[CONTROL] = buff;
	}
	else
	{
		cnt++;
	}
}
*/
void isr_ex0 (void) interrupt IE0_VECTOR
{
	unsigned char	flags, i, buff;

	flags = fpga_cntr_reg[INT_FLAGS];

	if (flags & (1 << 1))
	{
//		buff = fpga_pu_reg[1];
		fpga_cntr_reg[BU_BV_CNTR] |= (1 << 2);
		for (i = 0; i < 14; i++)
		{
//			if (data_rx_bv[i] != (i + buff))
			if (data_rx_bv[i] != (i + cnt))
			{
				break;
			}
		}
		if (i == 14)
		{
			buff = fpga_cntr_reg[CONTROL];
			buff ^= 0x80;
			fpga_cntr_reg[CONTROL] = buff;
		}
		cnt++;
	}
}

void isr_tc0 (void) interrupt TF0_VECTOR
{
	int				dac_buff;
	unsigned char	buff;//, i;

	TH0 = TIMER_HIGH;
	TL0 = TIMER_LOW;

	if (cnt == 166)
	{
		cnt = 0;
//	LED BLINK
		buff = fpga_cntr_reg[CONTROL];
		buff ^= 0x80;
		fpga_cntr_reg[CONTROL] = buff;
//	SEND TO TAS
		fpga_cntr_reg[TAS_TX_CNTR] = (23 << 3) | (1 << 0);
	}
	else
	{
		cnt++;
	}
//	LOAD TO DAC			
//	dac_buff = (dac_cnt << 4) | (0 or 3 << 2) | (0..3 << 0);
	dac_buff = (dac_cnt << 4) | (0 << 2) | (0 << 0);
	fpga_cntr_reg[DAC_LOW] = (unsigned char) dac_buff;
	dac_buff >>= 8;
	fpga_cntr_reg[DAC_HIGH] = (unsigned char) dac_buff;
//	dac_cnt++;
	dac_cnt += 8;
/*
	if (cnt_bv == 100)
	{
		cnt_bv = 0;
		data_tx_bv[0] = cnt;

		data_tx_bv[1] = ram_reg[0];
		data_tx_bv[2] = ram_reg[1];
*
		data_tx_bv[3] = ram_reg[2];
		data_tx_bv[4] = ram_reg[3];
*
//		for (i = 0; i < 14; i++)
//		{
//			data_tx_bv[i] = i + cnt;
//		}
		fpga_cntr_reg[BU_BV_CNTR] = 0x01;
//		fpga_pu_reg[1] = cnt;
		cnt++;
	}
	else
	{
		cnt_bv++;
	}

	SBUF = 0xAA;
*/
}

void main (void)
{
	int		i;
/*
	fpga_pu_reg[0] = 0;
	fpga_pu_reg[1] = 1;
	fpga_pu_reg[2] = 2;
	fpga_pu_reg[3] = 3;
	fpga_pu_reg[4] = 4;
	fpga_pu_reg[5] = 5;
	fpga_pu_reg[6] = 6;
	fpga_pu_reg[7] = 7;
*/
#ifdef DEBUG
	do {} while (P1_2 == 1);
#endif

	dac_cnt = 0;

	cnt = 0;
//	cnt_bv = 0;

//	TIMER 0
	TMOD = 0x01;
	TH0 = TIMER_HIGH;
	TL0 = TIMER_LOW;

	TR0 = 1;
	ET0 = 1;

//	INT0
//	IT0 = 0;
//	IE0 = 0;
//	EX0 = 1;

//	INT1
//	IT1 = 1;
//	IE1 = 0;
//	EX1 = 1;

//	T2
	RCAP2L = 0xFA;
	RCAP2H = 0xFF;
	T2CON = 0x34;
	T2MOD = 0x00;
	TI = 0;
	RI = 0;

//	UART
	SCON = 0x50;
//	ES = 1;

#ifdef DEBUG
//	SPI
	SPCR = 0x56;
	SPSR = 0x01;
//	SPI SS
	P1_4 = 0;

	send_info(hw);
	send_info(nl);

#else
	P1_4 = 1;
#endif

//	fpga_cntr_reg[INT_MASK] = (1 << 1);

//	if (fpga_cntr_reg[9] != 0)
//	if (fpga_cntr_reg[10] != 0x64)
//		do {} while (1);

	EA = 1;

	for (i = 1; i < 32; i++)
	{
		data_tx_tas[i] = i;
	}
	data_tx_tas[0] = 0xAA;

	do {} while (1);
}
