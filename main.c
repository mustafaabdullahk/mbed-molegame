#include "stm32f4xx.h"                  // Device header
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

int skor,sayi=0,ceza,zaman=1000;
void TIM6_DAC_IRQHandler()
{
		TIM6->SR &= ~TIM_SR_UIF;
    sayi=TIM6->CNT;
	  sayi=rand()%3;
	if(ceza<=skor){
	switch(sayi)	
	{ 
		case 0:
		GPIOD->ODR &=~(GPIO_ODR_OD15 |GPIO_ODR_OD14 |GPIO_ODR_OD13);
		GPIOD->ODR|=GPIO_ODR_OD13;
		ceza++;
		break;
		case 1:
		GPIOD->ODR &=~(GPIO_ODR_OD15 |GPIO_ODR_OD14 |GPIO_ODR_OD13);
		GPIOD->ODR|=GPIO_ODR_OD14;
		ceza++;
		break;
		case 2:
		GPIOD->ODR &=~(GPIO_ODR_OD15 |GPIO_ODR_OD14 |GPIO_ODR_OD13);
		GPIOD->ODR|=GPIO_ODR_OD15;
		ceza++;
		break;			
	}
}
		else
		{
			TIM6->CR1 &= ~TIM_CR1_CEN;
			GPIOD->ODR=skor<<12;
		}

}
void EXTI0_IRQHandler()
{
	EXTI->PR |= EXTI_PR_PR0;
	TIM6->CR1 |= TIM_CR1_CEN;
	sayi=TIM6->CNT;
	sayi=rand()%3;
	skor=0;
	ceza=0;
}
void EXTI1_IRQHandler()
{
	EXTI->PR |= EXTI_PR_PR1;
	if(GPIOD->ODR & GPIO_ODR_OD13)
	{
		skor++;
		zaman*=0.8;
	}
	else
	{
		GPIOD->ODR=skor<<12;
		TIM6->CR1 &= ~TIM_CR1_CEN;
	}
	for(int i=0;i<500000;i++);
	GPIOD->ODR &=~(GPIO_ODR_OD13 );
	
	TIM6->PSC &= ~TIM_PSC_PSC;
	TIM6->PSC |= (49999U & TIM_PSC_PSC);
	TIM6->ARR &= ~TIM_PSC_PSC;
	TIM6->ARR |= zaman & TIM_PSC_PSC;
}
void EXTI2_IRQHandler()
{
	EXTI->PR |= EXTI_PR_PR2;
	if(GPIOD->ODR&GPIO_ODR_OD14)
	{
		skor++;
		zaman*=0.8;
	}
	else
	{
		GPIOD->ODR=skor<<12;
		TIM6->CR1 &= ~TIM_CR1_CEN;
	}
	for(int i=0;i<500000;i++);
	GPIOD->ODR &=~(GPIO_ODR_OD14);
	
	TIM6->PSC &= ~TIM_PSC_PSC;
	TIM6->PSC |= (49999U & TIM_PSC_PSC);
	TIM6->ARR &= ~TIM_PSC_PSC;
	TIM6->ARR |= zaman & TIM_PSC_PSC;
	
	TIM6->CR1 |= TIM_CR1_CEN;
}
void EXTI3_IRQHandler()
{
	EXTI->PR |= EXTI_PR_PR3;
	if(GPIOD->ODR&GPIO_ODR_OD15)
	{
		skor++;
		zaman*=0.8;
	}
	else
	{
		GPIOD->ODR=skor<<12;
		TIM6->CR1 &= ~TIM_CR1_CEN;
	}
	for(int i=0;i<500000;i++);
	GPIOD->ODR &=~(GPIO_ODR_OD15);
	
	TIM6->PSC &= ~TIM_PSC_PSC;
	TIM6->PSC |= (49999U & TIM_PSC_PSC);
	TIM6->ARR &= ~TIM_PSC_PSC;
	TIM6->ARR |= zaman & TIM_PSC_PSC;	
}

int main(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN| RCC_AHB1ENR_GPIOCEN;
	GPIOD->MODER |= GPIO_MODER_MODER12_0 |GPIO_MODER_MODER13_0 |GPIO_MODER_MODER14_0 |GPIO_MODER_MODER15_0 ;
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
		
	EXTI->IMR |= EXTI_IMR_IM0|EXTI_IMR_IM1|EXTI_IMR_IM2|EXTI_IMR_IM3;
	EXTI->RTSR |= EXTI_RTSR_TR0|EXTI_RTSR_TR1|EXTI_RTSR_TR2|EXTI_RTSR_TR3;
		
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	//SYSCFG->EXTICR[1] |= SYSCFG_EXTICR1_EXTI1_PA;
	//SYSCFG->EXTICR[2] |= SYSCFG_EXTICR1_EXTI2_PA;
	//SYSCFG->EXTICR[3] |= SYSCFG_EXTICR1_EXTI3_PA;
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_1|GPIO_PUPDR_PUPD2_1|GPIO_PUPDR_PUPD3_1;
	
	TIM6->PSC &= ~TIM_PSC_PSC;
	TIM6->PSC |= (49999U & TIM_PSC_PSC);
	
	TIM6->ARR &= ~TIM_PSC_PSC;
	TIM6->ARR |= zaman & TIM_PSC_PSC;
	
	TIM6->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	
	while(1);			
}
