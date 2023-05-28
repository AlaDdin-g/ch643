/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch643_adc.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/04/06
 * Description        : This file provides all the ADC firmware functions.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "ch643_adc.h"
#include "ch643_rcc.h"

/* ADC DISCNUM mask */
#define CTLR1_DISCNUM_Reset              ((uint32_t)0xFFFF1FFF)

/* ADC DISCEN mask */
#define CTLR1_DISCEN_Set                 ((uint32_t)0x00000800)
#define CTLR1_DISCEN_Reset               ((uint32_t)0xFFFFF7FF)

/* ADC JAUTO mask */
#define CTLR1_JAUTO_Set                  ((uint32_t)0x00000400)
#define CTLR1_JAUTO_Reset                ((uint32_t)0xFFFFFBFF)

/* ADC JDISCEN mask */
#define CTLR1_JDISCEN_Set                ((uint32_t)0x00001000)
#define CTLR1_JDISCEN_Reset              ((uint32_t)0xFFFFEFFF)

/* ADC AWDCH mask */
#define CTLR1_AWDCH_Reset                ((uint32_t)0xFFFFFFE0)

/* ADC Analog watchdog enable mode mask */
#define CTLR1_AWDMode_Reset              ((uint32_t)0xFF3FFDFF)

/* CTLR1 register Mask */
#define CTLR1_CLEAR_Mask                 ((uint32_t)0xE0F0FEFF)

/* ADC ADON mask */
#define CTLR2_ADON_Set                   ((uint32_t)0x00000001)
#define CTLR2_ADON_Reset                 ((uint32_t)0xFFFFFFFE)

/* ADC DMA mask */
#define CTLR2_DMA_Set                    ((uint32_t)0x00000100)
#define CTLR2_DMA_Reset                  ((uint32_t)0xFFFFFEFF)

/* ADC RSTCAL mask */
#define CTLR2_RSTCAL_Set                 ((uint32_t)0x00000008)

/* ADC CAL mask */
#define CTLR2_CAL_Set                    ((uint32_t)0x00000004)

/* ADC SWSTART mask */
#define CTLR2_SWSTART_Set                ((uint32_t)0x00400000)

/* ADC EXTTRIG mask */
#define CTLR2_EXTTRIG_Set                ((uint32_t)0x00100000)
#define CTLR2_EXTTRIG_Reset              ((uint32_t)0xFFEFFFFF)

/* ADC Software start mask */
#define CTLR2_EXTTRIG_SWSTART_Set        ((uint32_t)0x00500000)
#define CTLR2_EXTTRIG_SWSTART_Reset      ((uint32_t)0xFFAFFFFF)

/* ADC JEXTSEL mask */
#define CTLR2_JEXTSEL_Reset              ((uint32_t)0xFFFF8FFF)

/* ADC JEXTTRIG mask */
#define CTLR2_JEXTTRIG_Set               ((uint32_t)0x00008000)
#define CTLR2_JEXTTRIG_Reset             ((uint32_t)0xFFFF7FFF)

/* ADC JSWSTART mask */
#define CTLR2_JSWSTART_Set               ((uint32_t)0x00200000)

/* ADC injected software start mask */
#define CTLR2_JEXTTRIG_JSWSTART_Set      ((uint32_t)0x00208000)
#define CTLR2_JEXTTRIG_JSWSTART_Reset    ((uint32_t)0xFFDF7FFF)

/* ADC TSPD mask */
#define CTLR2_TSVREFE_Set                ((uint32_t)0x00800000)
#define CTLR2_TSVREFE_Reset              ((uint32_t)0xFF7FFFFF)

/* CTLR2 register Mask */
#define CTLR2_CLEAR_Mask                 ((uint32_t)0xFFF1F7FD)

/* ADC SQx mask */
#define RSQR3_SQ_Set                     ((uint32_t)0x0000001F)
#define RSQR2_SQ_Set                     ((uint32_t)0x0000001F)
#define RSQR1_SQ_Set                     ((uint32_t)0x0000001F)

/* RSQR1 register Mask */
#define RSQR1_CLEAR_Mask                 ((uint32_t)0xFF0FFFFF)

/* ADC JSQx mask */
#define ISQR_JSQ_Set                     ((uint32_t)0x0000001F)

/* ADC JL mask */
#define ISQR_JL_Set                      ((uint32_t)0x00300000)
#define ISQR_JL_Reset                    ((uint32_t)0xFFCFFFFF)

/* ADC SMPx mask */
#define SAMPTR1_SMP_Set                  ((uint32_t)0x00000007)
#define SAMPTR2_SMP_Set                  ((uint32_t)0x00000007)

/* ADC IDATARx registers offset */
#define IDATAR_Offset                    ((uint8_t)0x28)

/* ADC1 RDATAR register base address */
#define RDATAR_ADDRESS                   ((uint32_t)0x4001244C)

/* ADC CLK */
#define CTLR3_CLK_Mask                   ((uint32_t)0xFFFFFE00)

/*********************************************************************
 * @fn      ADC_DeInit
 *
 * @brief   Deinitializes the ADCx peripheral registers to their default
 *        reset values.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *
 * @return  none
 */
void ADC_DeInit(ADC_TypeDef *ADCx)
{
    if(ADCx == ADC1)
    {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
    }
}

/*********************************************************************
 * @fn      ADC_Init
 *
 * @brief   Initializes the ADCx peripheral according to the specified
 *        parameters in the ADC_InitStruct.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_InitStruct - pointer to an ADC_InitTypeDef structure that
 *        contains the configuration information for the specified ADC
 *        peripheral.
 *
 * @return  none
 */
void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct)
{
    uint32_t tmpreg1 = 0;
    uint8_t  tmpreg2 = 0;

    tmpreg1 = ADCx->CTLR1;
    tmpreg1 &= CTLR1_CLEAR_Mask;
    tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_Mode | (uint32_t)ADC_InitStruct->ADC_OutputBuffer |
                          (uint32_t)ADC_InitStruct->ADC_Pga | ((uint32_t)ADC_InitStruct->ADC_ScanConvMode << 8));
    ADCx->CTLR1 = tmpreg1;

    tmpreg1 = ADCx->CTLR2;
    tmpreg1 &= CTLR2_CLEAR_Mask;
    tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_DataAlign | ADC_InitStruct->ADC_ExternalTrigConv |
                          ((uint32_t)ADC_InitStruct->ADC_ContinuousConvMode << 1));
    ADCx->CTLR2 = tmpreg1;

    tmpreg1 = ADCx->RSQR1;
    tmpreg1 &= RSQR1_CLEAR_Mask;
    tmpreg2 |= (uint8_t)(ADC_InitStruct->ADC_NbrOfChannel - (uint8_t)1);
    tmpreg1 |= (uint32_t)tmpreg2 << 20;
    ADCx->RSQR1 = tmpreg1;
}

/*********************************************************************
 * @fn      ADC_StructInit
 *
 * @brief   Fills each ADC_InitStruct member with its default value.
 *
 * @param   ADC_InitStruct - pointer to an ADC_InitTypeDef structure that
 *        contains the configuration information for the specified ADC
 *        peripheral.
 *
 * @return  none
 */
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct)
{
    ADC_InitStruct->ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct->ADC_ScanConvMode = DISABLE;
    ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct->ADC_NbrOfChannel = 1;
}

/*********************************************************************
 * @fn      ADC_Cmd
 *
 * @brief   Enables or disables the specified ADC peripheral.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void ADC_Cmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR2 |= CTLR2_ADON_Set;
    }
    else
    {
        ADCx->CTLR2 &= CTLR2_ADON_Reset;
    }
}

/*********************************************************************
 * @fn      ADC_DMACmd
 *
 * @brief   Enables or disables the specified ADC DMA request.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void ADC_DMACmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR2 |= CTLR2_DMA_Set;
    }
    else
    {
        ADCx->CTLR2 &= CTLR2_DMA_Reset;
    }
}

/*********************************************************************
 * @fn      ADC_ITConfig
 *
 * @brief   Enables or disables the specified ADC interrupts.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_IT - specifies the ADC interrupt sources to be enabled or disabled.
 *            ADC_IT_EOC - End of conversion interrupt mask.
 *            ADC_IT_AWD - Analog watchdog interrupt mask.
 *            ADC_IT_JEOC - End of injected conversion interrupt mask.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void ADC_ITConfig(ADC_TypeDef *ADCx, uint16_t ADC_IT, FunctionalState NewState)
{
    uint8_t itmask = 0;

    itmask = (uint8_t)ADC_IT;

    if(NewState != DISABLE)
    {
        ADCx->CTLR1 |= itmask;
    }
    else
    {
        ADCx->CTLR1 &= (~(uint32_t)itmask);
    }
}

/*********************************************************************
 * @fn      ADC_SoftwareStartConvCmd
 *
 * @brief   Enables or disables the selected ADC software start conversion.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  None
 */
void ADC_SoftwareStartConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR2 |= CTLR2_EXTTRIG_SWSTART_Set;
    }
    else
    {
        ADCx->CTLR2 &= CTLR2_EXTTRIG_SWSTART_Reset;
    }
}

/*********************************************************************
 * @fn      ADC_GetSoftwareStartConvStatus
 *
 * @brief   Gets the selected ADC Software start conversion Status.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *
 * @return  FlagStatus - SET or RESET.
 */

FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef *ADCx)
{
    FlagStatus bitstatus = RESET;

    if((ADCx->CTLR2 & CTLR2_SWSTART_Set) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*********************************************************************
 * @fn      ADC_DiscModeChannelCountConfig
 *
 * @brief   Configures the discontinuous mode for the selected ADC regular
 *        group channel.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          Number - specifies the discontinuous mode regular channel
 *            count value(1-8).
 *
 * @return  None
 */
void ADC_DiscModeChannelCountConfig(ADC_TypeDef *ADCx, uint8_t Number)
{
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;

    tmpreg1 = ADCx->CTLR1;
    tmpreg1 &= CTLR1_DISCNUM_Reset;
    tmpreg2 = Number - 1;
    tmpreg1 |= tmpreg2 << 13;
    ADCx->CTLR1 = tmpreg1;
}

/*********************************************************************
 * @fn      ADC_DiscModeCmd
 *
 * @brief   Enables or disables the discontinuous mode on regular group
 *        channel for the specified ADC.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  None
 */
void ADC_DiscModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR1 |= CTLR1_DISCEN_Set;
    }
    else
    {
        ADCx->CTLR1 &= CTLR1_DISCEN_Reset;
    }
}

/*********************************************************************
 * @fn      ADC_RegularChannelConfig
 *
 * @brief   Configures for the selected ADC regular channel its corresponding
 *        rank in the sequencer and its sample time.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_Channel - the ADC channel to configure.
 *            ADC_Channel_0 - ADC Channel0 selected.
 *            ADC_Channel_1 - ADC Channel1 selected.
 *            ADC_Channel_2 - ADC Channel2 selected.
 *            ADC_Channel_3 - ADC Channel3 selected.
 *            ADC_Channel_4 - ADC Channel4 selected.
 *            ADC_Channel_5 - ADC Channel5 selected.
 *            ADC_Channel_6 - ADC Channel6 selected.
 *            ADC_Channel_7 - ADC Channel7 selected.
 *            ADC_Channel_8 - ADC Channel8 selected.
 *            ADC_Channel_9 - ADC Channel9 selected.
 *            ADC_Channel_10 - ADC Channel10 selected.
 *            ADC_Channel_11 - ADC Channel11 selected.
 *            ADC_Channel_12 - ADC Channel12 selected.
 *            ADC_Channel_13 - ADC Channel13 selected.
 *            ADC_Channel_14 - ADC Channel14 selected.
 *            ADC_Channel_15 - ADC Channel15 selected.
 *          Rank - The rank in the regular group sequencer.
 *            This parameter must be between 1 to 16.
 *          ADC_SampleTime - The sample time value to be set for the selected channel.
 *            ADC_SampleTime_4Cycles - Sample time equal to 4 cycles.
 *            ADC_SampleTime_5Cycles - Sample time equal to 5 cycles.
 *            ADC_SampleTime_6Cycles - Sample time equal to 6 cycles.
 *            ADC_SampleTime_7Cycles - Sample time equal to 7 cycles.
 *            ADC_SampleTime_8Cycles - Sample time equal to 8 cycles.
 *            ADC_SampleTime_9Cycles - Sample time equal to 9 cycles.
 *            ADC_SampleTime_10Cycles - Sample time equal to 10 cycles.
 *            ADC_SampleTime_11Cycles - Sample time equal to 11 cycles.
 *
 * @return  None
 */
void ADC_RegularChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
    uint32_t tmpreg1 = 0, tmpreg2 = 0;

    if(ADC_Channel > ADC_Channel_9)
    {
        tmpreg1 = ADCx->SAMPTR1;
        tmpreg2 = SAMPTR1_SMP_Set << (3 * (ADC_Channel - 10));
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
        tmpreg1 |= tmpreg2;
        ADCx->SAMPTR1 = tmpreg1;
    }
    else
    {
        tmpreg1 = ADCx->SAMPTR2;
        tmpreg2 = SAMPTR2_SMP_Set << (3 * ADC_Channel);
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
        tmpreg1 |= tmpreg2;
        ADCx->SAMPTR2 = tmpreg1;
    }

    if(Rank < 7)
    {
        tmpreg1 = ADCx->RSQR3;
        tmpreg2 = RSQR3_SQ_Set << (5 * (Rank - 1));
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
        tmpreg1 |= tmpreg2;
        ADCx->RSQR3 = tmpreg1;
    }
    else if(Rank < 13)
    {
        tmpreg1 = ADCx->RSQR2;
        tmpreg2 = RSQR2_SQ_Set << (5 * (Rank - 7));
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
        tmpreg1 |= tmpreg2;
        ADCx->RSQR2 = tmpreg1;
    }
    else
    {
        tmpreg1 = ADCx->RSQR1;
        tmpreg2 = RSQR1_SQ_Set << (5 * (Rank - 13));
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
        tmpreg1 |= tmpreg2;
        ADCx->RSQR1 = tmpreg1;
    }
}

/*********************************************************************
 * @fn      ADC_ExternalTrigConvCmd
 *
 * @brief   Enables or disables the ADCx conversion through external trigger.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  None
 */
void ADC_ExternalTrigConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR2 |= CTLR2_EXTTRIG_Set;
    }
    else
    {
        ADCx->CTLR2 &= CTLR2_EXTTRIG_Reset;
    }
}

/*********************************************************************
 * @fn      ADC_GetConversionValue
 *
 * @brief   Returns the last ADCx conversion result data for regular channel.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *
 * @return  ADCx->RDATAR - The Data conversion value.
 */
uint16_t ADC_GetConversionValue(ADC_TypeDef *ADCx)
{
    return (uint16_t)ADCx->RDATAR;
}

/*********************************************************************
 * @fn      ADC_GetDualModeConversionValue
 *
 * @brief   Returns the last ADC1 conversion result data in dual mode.
 *
 * @return  RDATAR_ADDRESS - The Data conversion value.
 */
uint32_t ADC_GetDualModeConversionValue(void)
{
    return (*(__IO uint32_t *)RDATAR_ADDRESS);
}

/*********************************************************************
 * @fn      ADC_AutoInjectedConvCmd
 *
 * @brief   Enables or disables the selected ADC automatic injected group
 *        conversion after regular one.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  None
 */
void ADC_AutoInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR1 |= CTLR1_JAUTO_Set;
    }
    else
    {
        ADCx->CTLR1 &= CTLR1_JAUTO_Reset;
    }
}

/*********************************************************************
 * @fn      ADC_InjectedDiscModeCmd
 *
 * @brief   Enables or disables the discontinuous mode for injected group
 *        channel for the specified ADC.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  None
 */
void ADC_InjectedDiscModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR1 |= CTLR1_JDISCEN_Set;
    }
    else
    {
        ADCx->CTLR1 &= CTLR1_JDISCEN_Reset;
    }
}

/*********************************************************************
 * @fn      ADC_ExternalTrigInjectedConvConfig
 *
 * @brief   Configures the ADCx external trigger for injected channels conversion.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_ExternalTrigInjecConv - specifies the ADC trigger to start
 *        injected conversion.
 *            ADC_ExternalTrigInjecConv_T1_CC3 - Timer1 TRGO event selected.
 *            ADC_ExternalTrigInjecConv_T1_CC4 - Timer1 capture compare4 selected.
 *            ADC_ExternalTrigInjecConv_T2_CC3 - Timer2 capture compare3 selected.
 *            ADC_ExternalTrigInjecConv_T2_CC4 - Timer2 capture compare4 selected.
 *            ADC_ExternalTrigInjecConv_T2_CC2 - Timer2 capture compare2 selected.
 *            ADC_ExternalTrigInjecConv_T3_CC2 - Timer3 capture compare2 selected.
 *            ADC_ExternalTrigInjecConv_ADC_ETRGREG - ADC ETRGREG selected.
 *            ADC_ExternalTrigInjecConv_None: Injected conversion started
 *        by software and not by external trigger.
 *
 * @return  None
 */
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef *ADCx, uint32_t ADC_ExternalTrigInjecConv)
{
    uint32_t tmpreg = 0;

    tmpreg = ADCx->CTLR2;
    tmpreg &= CTLR2_JEXTSEL_Reset;
    tmpreg |= ADC_ExternalTrigInjecConv;
    ADCx->CTLR2 = tmpreg;
}

/*********************************************************************
 * @fn      ADC_ExternalTrigInjectedConvCmd
 *
 * @brief   Enables or disables the ADCx injected channels conversion through
 *        external trigger.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  None
 */
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR2 |= CTLR2_JEXTTRIG_Set;
    }
    else
    {
        ADCx->CTLR2 &= CTLR2_JEXTTRIG_Reset;
    }
}

/*********************************************************************
 * @fn      ADC_SoftwareStartInjectedConvCmd
 *
 * @brief   Enables or disables the selected ADC start of the injected
 *        channels conversion.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  None
 */
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR2 |= CTLR2_JEXTTRIG_JSWSTART_Set;
    }
    else
    {
        ADCx->CTLR2 &= CTLR2_JEXTTRIG_JSWSTART_Reset;
    }
}

/*********************************************************************
 * @fn      ADC_GetSoftwareStartInjectedConvCmdStatus
 *
 * @brief   Gets the selected ADC Software start injected conversion Status.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *
 * @return  FlagStatus: SET or RESET.
 */
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef *ADCx)
{
    FlagStatus bitstatus = RESET;

    if((ADCx->CTLR2 & CTLR2_JSWSTART_Set) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*********************************************************************
 * @fn      ADC_InjectedChannelConfig
 *
 * @brief   Configures for the selected ADC injected channel its corresponding
 *        rank in the sequencer and its sample time.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_Channel - the ADC channel to configure.
 *            ADC_Channel_0 - ADC Channel0 selected.
 *            ADC_Channel_1 - ADC Channel1 selected.
 *            ADC_Channel_2 - ADC Channel2 selected.
 *            ADC_Channel_3 - ADC Channel3 selected.
 *            ADC_Channel_4 - ADC Channel4 selected.
 *            ADC_Channel_5 - ADC Channel5 selected.
 *            ADC_Channel_6 - ADC Channel6 selected.
 *            ADC_Channel_7 - ADC Channel7 selected.
 *            ADC_Channel_8 - ADC Channel8 selected.
 *            ADC_Channel_9 - ADC Channel9 selected.
 *            ADC_Channel_10 - ADC Channel10 selected.
 *            ADC_Channel_11 - ADC Channel11 selected.
 *            ADC_Channel_12 - ADC Channel12 selected.
 *            ADC_Channel_13 - ADC Channel13 selected.
 *            ADC_Channel_14 - ADC Channel14 selected.
 *            ADC_Channel_15 - ADC Channel15 selected.
 *          Rank - The rank in the regular group sequencer.
 *            This parameter must be between 1 to 4.
 *          ADC_SampleTime - The sample time value to be set for the selected channel.
 *            ADC_SampleTime_4Cycles - Sample time equal to 4 cycles.
 *            ADC_SampleTime_5Cycles - Sample time equal to 5 cycles.
 *            ADC_SampleTime_6Cycles - Sample time equal to 6 cycles.
 *            ADC_SampleTime_7Cycles - Sample time equal to 7 cycles.
 *            ADC_SampleTime_8Cycles - Sample time equal to 8 cycles.
 *            ADC_SampleTime_9Cycles - Sample time equal to 9 cycles.
 *            ADC_SampleTime_10Cycles - Sample time equal to 10 cycles.
 *            ADC_SampleTime_11Cycles - Sample time equal to 11 cycles.
 *
 * @return  None
 */
void ADC_InjectedChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
    uint32_t tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0;

    if(ADC_Channel > ADC_Channel_9)
    {
        tmpreg1 = ADCx->SAMPTR1;
        tmpreg2 = SAMPTR1_SMP_Set << (3 * (ADC_Channel - 10));
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
        tmpreg1 |= tmpreg2;
        ADCx->SAMPTR1 = tmpreg1;
    }
    else
    {
        tmpreg1 = ADCx->SAMPTR2;
        tmpreg2 = SAMPTR2_SMP_Set << (3 * ADC_Channel);
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
        tmpreg1 |= tmpreg2;
        ADCx->SAMPTR2 = tmpreg1;
    }

    tmpreg1 = ADCx->ISQR;
    tmpreg3 = (tmpreg1 & ISQR_JL_Set) >> 20;
    tmpreg2 = ISQR_JSQ_Set << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
    tmpreg1 &= ~tmpreg2;
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
    tmpreg1 |= tmpreg2;
    ADCx->ISQR = tmpreg1;
}

/*********************************************************************
 * @fn      ADC_InjectedSequencerLengthConfig
 *
 * @brief   Configures the sequencer length for injected channels.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          Length - The sequencer length.
 *            This parameter must be a number between 1 to 4.
 *
 * @return  None
 */
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef *ADCx, uint8_t Length)
{
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;

    tmpreg1 = ADCx->ISQR;
    tmpreg1 &= ISQR_JL_Reset;
    tmpreg2 = Length - 1;
    tmpreg1 |= tmpreg2 << 20;
    ADCx->ISQR = tmpreg1;
}

/*********************************************************************
 * @fn      ADC_SetInjectedOffset
 *
 * @brief   Set the injected channels conversion value offset.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_InjectedChannel: the ADC injected channel to set its offset.
 *            ADC_InjectedChannel_1 - Injected Channel1 selected.
 *            ADC_InjectedChannel_2 - Injected Channel2 selected.
 *            ADC_InjectedChannel_3 - Injected Channel3 selected.
 *            ADC_InjectedChannel_4 - Injected Channel4 selected.
 *          Offset - the offset value for the selected ADC injected channel.
 *            This parameter must be a 12bit value.
 *
 * @return  None
 */
void ADC_SetInjectedOffset(ADC_TypeDef *ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset)
{
    __IO uint32_t tmp = 0;

    tmp = (uint32_t)ADCx;
    tmp += ADC_InjectedChannel;

    *(__IO uint32_t *)tmp = (uint32_t)Offset;
}

/*********************************************************************
 * @fn      ADC_GetInjectedConversionValue
 *
 * @brief   Returns the ADC injected channel conversion result.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_InjectedChannel - the ADC injected channel to set its offset.
 *            ADC_InjectedChannel_1 - Injected Channel1 selected.
 *            ADC_InjectedChannel_2 - Injected Channel2 selected.
 *            ADC_InjectedChannel_3 - Injected Channel3 selected.
 *            ADC_InjectedChannel_4 - Injected Channel4 selected.
 *
 * @return  tmp - The Data conversion value.
 */
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef *ADCx, uint8_t ADC_InjectedChannel)
{
    __IO uint32_t tmp = 0;

    tmp = (uint32_t)ADCx;
    tmp += ADC_InjectedChannel + IDATAR_Offset;

    return (uint16_t)(*(__IO uint32_t *)tmp);
}

/*********************************************************************
 * @fn      ADC_AnalogWatchdogCmd
 *
 * @brief   Enables or disables the analog watchdog on single/all regular
 *        or injected channels.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_AnalogWatchdog - the ADC analog watchdog configuration.
 *            ADC_AnalogWatchdog_SingleRegEnable - Analog watchdog on a
 *        single regular channel.
 *            ADC_AnalogWatchdog_SingleInjecEnable - Analog watchdog on a
 *        single injected channel.
 *            ADC_AnalogWatchdog_SingleRegOrInjecEnable - Analog watchdog
 *        on a single regular or injected channel.
 *            ADC_AnalogWatchdog_AllRegEnable - Analog watchdog on  all
 *        regular channel.
 *            ADC_AnalogWatchdog_AllInjecEnable - Analog watchdog on  all
 *        injected channel.
 *            ADC_AnalogWatchdog_AllRegAllInjecEnable - Analog watchdog on
 *        all regular and injected channels.
 *            ADC_AnalogWatchdog_None - No channel guarded by the analog
 *        watchdog.
 *
 * @return  none
 */
void ADC_AnalogWatchdogCmd(ADC_TypeDef *ADCx, uint32_t ADC_AnalogWatchdog)
{
    uint32_t tmpreg = 0;

    tmpreg = ADCx->CTLR1;
    tmpreg &= CTLR1_AWDMode_Reset;
    tmpreg |= ADC_AnalogWatchdog;
    ADCx->CTLR1 = tmpreg;
}

/*********************************************************************
 * @fn      ADC_AnalogWatchdogThresholdsConfig
 *
 * @brief   Configures the high and low thresholds of the analog watchdog.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          HighThreshold - the ADC analog watchdog High threshold value.
 *            This parameter must be a 12bit value.
 *          LowThreshold - the ADC analog watchdog Low threshold value.
 *            This parameter must be a 12bit value.
 *
 * @return  none
 */
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef *ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold)
{
    ADCx->WDHTR = HighThreshold;
    ADCx->WDLTR = LowThreshold;
}

/*********************************************************************
 * @fn      ADC_AnalogWatchdog1ThresholdsConfig
 *
 * @brief   Configures the high and low thresholds of the analog watchdog1.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          HighThreshold - the ADC analog watchdog1 High threshold value.
 *            This parameter must be a 12bit value.
 *          LowThreshold - the ADC analog watchdog1 Low threshold value.
 *            This parameter must be a 12bit value.
 *
 * @return  none
 */
void ADC_AnalogWatchdog1ThresholdsConfig(ADC_TypeDef *ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold)
{
    ADCx->WDTR1 = (uint32_t)HighThreshold<<16;
    ADCx->WDTR1 |= (uint32_t)LowThreshold;
}

/*********************************************************************
 * @fn      ADC_AnalogWatchdog2ThresholdsConfig
 *
 * @brief   Configures the high and low thresholds of the analog watchdog2.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          HighThreshold - the ADC analog watchdog2 High threshold value.
 *            This parameter must be a 12bit value.
 *          LowThreshold - the ADC analog watchdog2 Low threshold value.
 *            This parameter must be a 12bit value.
 *
 * @return  none
 */
void ADC_AnalogWatchdog2ThresholdsConfig(ADC_TypeDef *ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold)
{
    ADCx->WDTR2 = (uint32_t)HighThreshold<<16;
    ADCx->WDTR2 |= (uint32_t)LowThreshold;
}

/*********************************************************************
 * @fn      ADC_AnalogWatchdog3ThresholdsConfig
 *
 * @brief   Configures the high and low thresholds of the analog watchdog3.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          HighThreshold - the ADC analog watchdog3 High threshold value.
 *            This parameter must be a 12bit value.
 *          LowThreshold - the ADC analog watchdog3 Low threshold value.
 *            This parameter must be a 12bit value.
 *
 * @return  none
 */
void ADC_AnalogWatchdog3ThresholdsConfig(ADC_TypeDef *ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold)
{
    ADCx->WDTR3 = (uint32_t)HighThreshold<<16;
    ADCx->WDTR3 |= (uint32_t)LowThreshold;
}

/*********************************************************************
 * @fn      ADC_AnalogWatchdogSingleChannelConfig
 *
 * @brief   Configures the analog watchdog guarded single channel.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_Channel - the ADC channel to configure.
 *            ADC_Channel_0 - ADC Channel0 selected.
 *            ADC_Channel_1 - ADC Channel1 selected.
 *            ADC_Channel_2 - ADC Channel2 selected.
 *            ADC_Channel_3 - ADC Channel3 selected.
 *            ADC_Channel_4 - ADC Channel4 selected.
 *            ADC_Channel_5 - ADC Channel5 selected.
 *            ADC_Channel_6 - ADC Channel6 selected.
 *            ADC_Channel_7 - ADC Channel7 selected.
 *            ADC_Channel_8 - ADC Channel8 selected.
 *            ADC_Channel_9 - ADC Channel9 selected.
 *            ADC_Channel_10 - ADC Channel10 selected.
 *            ADC_Channel_11 - ADC Channel11 selected.
 *            ADC_Channel_12 - ADC Channel12 selected.
 *            ADC_Channel_13 - ADC Channel13 selected.
 *            ADC_Channel_14 - ADC Channel14 selected.
 *            ADC_Channel_15 - ADC Channel15 selected.
 *
 * @return  None
 */
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel)
{
    uint32_t tmpreg = 0;

    tmpreg = ADCx->CTLR1;
    tmpreg &= CTLR1_AWDCH_Reset;
    tmpreg |= ADC_Channel;
    ADCx->CTLR1 = tmpreg;
}

/*********************************************************************
 * @fn      ADC_GetFlagStatus
 *
 * @brief   Checks whether the specified ADC flag is set or not.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_FLAG - specifies the flag to check.
 *            ADC_FLAG_AWD - Analog watchdog flag.
 *            ADC_FLAG_EOC - End of conversion flag.
 *            ADC_FLAG_JEOC - End of injected group conversion flag.
 *            ADC_FLAG_JSTRT - Start of injected group conversion flag.
 *            ADC_FLAG_STRT - Start of regular group conversion flag.
 *
 * @return  FlagStatus: SET or RESET.
 */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, uint8_t ADC_FLAG)
{
    FlagStatus bitstatus = RESET;

    if((ADCx->STATR & ADC_FLAG) != (uint8_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*********************************************************************
 * @fn      ADC_ClearFlag
 *
 * @brief   Clears the ADCx's pending flags.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_FLAG - specifies the flag to clear.
 *            ADC_FLAG_AWD - Analog watchdog flag.
 *            ADC_FLAG_EOC - End of conversion flag.
 *            ADC_FLAG_JEOC - End of injected group conversion flag.
 *            ADC_FLAG_JSTRT - Start of injected group conversion flag.
 *            ADC_FLAG_STRT - Start of regular group conversion flag.
 *
 * @return  none
 */
void ADC_ClearFlag(ADC_TypeDef *ADCx, uint8_t ADC_FLAG)
{
    ADCx->STATR = ~(uint32_t)ADC_FLAG;
}

/*********************************************************************
 * @fn      ADC_GetITStatus
 *
 * @brief   Checks whether the specified ADC interrupt has occurred or not.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_IT - specifies the ADC interrupt source to check.
 *            ADC_IT_EOC - End of conversion interrupt mask.
 *            ADC_IT_AWD - Analog watchdog interrupt mask.
 *            ADC_IT_JEOC - End of injected conversion interrupt mask.
 *
 * @return  FlagStatus: SET or RESET.
 */
ITStatus ADC_GetITStatus(ADC_TypeDef *ADCx, uint16_t ADC_IT)
{
    ITStatus bitstatus = RESET;
    uint32_t itmask = 0, enablestatus = 0;

    itmask = ADC_IT >> 8;
    enablestatus = (ADCx->CTLR1 & (uint8_t)ADC_IT);

    if(((ADCx->STATR & itmask) != (uint32_t)RESET) && enablestatus)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*********************************************************************
 * @fn      ADC_ClearITPendingBit
 *
 * @brief   Clears the ADCx's interrupt pending bits.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_IT - specifies the ADC interrupt pending bit to clear.
 *            ADC_IT_EOC - End of conversion interrupt mask.
 *            ADC_IT_AWD - Analog watchdog interrupt mask.
 *            ADC_IT_JEOC - End of injected conversion interrupt mask.
 *
 * @return  none
 */
void ADC_ClearITPendingBit(ADC_TypeDef *ADCx, uint16_t ADC_IT)
{
    uint8_t itmask = 0;

    itmask = (uint8_t)(ADC_IT >> 8);
    ADCx->STATR = ~(uint32_t)itmask;
}

/*********************************************************************
 * @fn      ADC_AnalogWatchdogResetCmd
 *
 * @brief   Enables or disables the analog watchdog reset
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_AnalogWatchdog_x -  Analog watchdog X.
 *            ADC_AnalogWatchdog_0_RST_EN.
 *            ADC_AnalogWatchdog_1_RST_EN.
 *            ADC_AnalogWatchdog_2_RST_EN.
 *            ADC_AnalogWatchdog_3_RST_EN.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void ADC_AnalogWatchdogResetCmd(ADC_TypeDef *ADCx, uint32_t ADC_AnalogWatchdog_x, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
      ADCx->CTLR3 |= ADC_AnalogWatchdog_x;
  }
  else
  {
      ADCx->CTLR3 &= ~ADC_AnalogWatchdog_x;
  }
}

/*********************************************************************
 * @fn      ADC_AnalogWatchdogScanCmd
 *
 * @brief   Enable ADC clock duty cycle adjustment.
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void ADC_AnalogWatchdogScanCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
      ADCx->CTLR3 |= (1<<9);
    }
    else
    {
      ADCx->CTLR3 &= ~(1<<9);
    }
}

/*********************************************************************
 * @fn      ADC_CLKConfig
 *
 * @brief   Configures the PADC clock.
 *          Note - ADC_CLK_Div_x > H_Level_Cycles_x
 *
 * @param   ADCx - where x can be 1 to select the ADC peripheral.
 *          ADC_CLK_Div_x - defines the ADC clock divider.
 *            ADC_CLK_Div4 - ADC clock = SYSCLK/4
 *            ADC_CLK_Div5 - ADC clock = SYSCLK/5
 *            ADC_CLK_Div6 - ADC clock = SYSCLK/6
 *            ADC_CLK_Div7 - ADC clock = SYSCLK/7
 *            ADC_CLK_Div8 - ADC clock = SYSCLK/8
 *            ADC_CLK_Div9 - ADC clock = SYSCLK/9
 *            ADC_CLK_Div10 - ADC clock = SYSCLK/10
 *            ADC_CLK_Div11 - ADC clock = SYSCLK/11
 *            ADC_CLK_Div12 - ADC clock = SYSCLK/12
 *            ADC_CLK_Div13 - ADC clock = SYSCLK/13
 *            ADC_CLK_Div14 - ADC clock = SYSCLK/14
 *            ADC_CLK_Div15 - ADC clock = SYSCLK/15
 *            ADC_CLK_Div16 - ADC clock = SYSCLK/16
 * @return  none
 */
void ADC_CLKConfig(ADC_TypeDef *ADCx, uint32_t ADC_CLK_Div_x)
{
  uint32_t tmpreg = 0;

  tmpreg = ADCx->CTLR3;

  tmpreg &= CTLR3_CLK_Mask;
  tmpreg |= ADC_CLK_Div_x;
  ADCx->CTLR3 = tmpreg;
}

