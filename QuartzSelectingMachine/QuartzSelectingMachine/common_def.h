#ifndef __COMMON_DEF_H
#define __COMMON_DEF_H

#define COMMAND_SET_SW_AND_ATC		0x00000001
#define COMMAND_SET_DDS_PARAM			0x00000002

typedef struct _SW_AND_ATC_STRUCT
{
	unsigned int atc_val;
	unsigned char sw1A_val;
	unsigned char sw1B_val;
	unsigned char sw2A_val;
	unsigned char sw2B_val;
	unsigned char sw3A_val;
	unsigned char sw3B_val;
	unsigned char sw5A_val;
	unsigned char sw5B_val;
}sw_and_atc_struct;

typedef struct _DDS_STRUCT
{
	double OutputFreq;
}dds_struct;

struct DAT_STRUCT_1
{
	unsigned int cmd_type;
	union
	{
		sw_and_atc_struct m_sw_and_stc;
		dds_struct m_dds_param;
	}cmd_data;
};

struct DAT_STRUCT_2
{
	unsigned int ADC_Value[3];
};

#ifdef STM32F4XX

typedef struct DAT_STRUCT_1 Recv_Data_Struct, *pRecv_Data_Struct;
typedef struct DAT_STRUCT_2 Send_Data_Struct, *pSend_Data_Struct;

#else

typedef struct DAT_STRUCT_1 Send_Data_Struct, *pSend_Data_Struct;
typedef struct DAT_STRUCT_2 Recv_Data_Struct, *pRecv_Data_Struct;

#endif /* STM32F4XX */

#endif /* __COMMON_DEF_H */



