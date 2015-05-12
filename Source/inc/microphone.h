/**
 * @file         microphone.h
 * @version      1.0
 * @date         2015
 * @author       Christoph Lauer
 * @compiler     armcc
 * @copyright    Christoph Lauer engineering
 */
 
#ifndef __MICROPHONE_H
#define __MICROPHONE_H

#include "stm32f4xx.h"
#include "main.h"

void WaveRecorderBeginSampling (void);
uint32_t WaveRecorderInit(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
/* Start the audio recording */
uint8_t WaveRecorderStart(uint16_t* pbuf, uint32_t size);
/* This function handles AUDIO_REC_SPI global interrupt request. */
void AUDIO_REC_SPI_IRQHANDLER(void);
/* Initializes the GPIO for wave recorder. */
static void WaveRecorder_GPIO_Init(void);
/* Initializes the SPI peripheral. */
static void WaveRecorder_SPI_Init(uint32_t Freq);
static void WaveRecorder_NVIC_Init(void);

#endif
