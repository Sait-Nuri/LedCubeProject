/*
 * main.c
 */
#include "rs232.h"
#ifdef _WIN32
#include <windows.h>
#elif __linux
#include <unistd.h> // usleep
#endif  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rs232.h"
#define BAUDRATE 115200
#define DELAY_TIME_US 100000
#define RECONNECT_TIME 100000
#define BUFFER_SIZE 4096

int connectLedCuber();
int sendArrayLedCube(unsigned char m[8][8]);

int main(void) {
	unsigned char ledArray[8][8];
    unsigned char compactArr[8][8][1];
    unsigned char i, j, k;
	int portNo;

	srand(time(NULL));

	if (connectLedCuber() == -1){
		printf("Cannot open any port!\n");
		
		#ifdef _WIN32
		system("PAUSE");
		#endif 
	}

	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 8; ++j) {
			ledArray[i][j]= 0x00;
		}
	}

    for(;;){
		
		for(i=0; i<8; ++i){
			ledArray[7][i] = rand() % 255;
			ledArray[7][i] &= ~(rand() % 255);
			ledArray[7][i] &= ~(rand() % 255);
			ledArray[7][i] &= ~(rand() % 255);
		}

		/*Her satırı bir aşağı indir*/
		for (i = 0; i < 7 ; ++i) {
			for (j = 0; j < 8; ++j) {
				ledArray[i][j] = ledArray[i+1][j];
			}
		}
		
		for (i = 0; i < 8; ++i) {
			for (j = 0; j < 8; ++j) {
				if (SendByte(portNo,compactArr[i][j][0]) == 1){
					printf("SendByte Error!\n");
					
					#ifdef _WIN32
					system("PAUSE");
					#endif			
					
					return -1;
				}
			}
		}
		
		#ifdef _WIN32
			Sleep(10);
		#elif __linux
			usleep(10000);
		#endif  
		

		/* Compress algorithm */
		/*
		for (i = 0; i < 8; ++i) {
			for (j = 0; j < 8; ++j) {
				for (k = 0; k < 8; ++k) {
					if(arr[i][j][k]){
						compactArr[i][j][0] |= 1 << k;
					}
					else{
						compactArr[i][j][0] &= ~(1 << k);
					}
				}
			}
		}
		*/
		
	}

	CloseComport(portNo);

	printf("Program finished...\n");
	system("PAUSE");
    
    return 0;
}
int connectLedCuber(){
	char buffer[BUFFER_SIZE];
	int portNum;
	int openedFlag = 0;

	for(portNum = 0; portNum < 6 ; ++portNum){
		if (OpenComport(portNum, BAUDRATE) == 0){
			printf("Port ttyACM%d opened succesfully", portNum);
			return portNum;
		}
	}
	
	return -1;
}