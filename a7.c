//compile command: gcc a7.c -lm -o a7 -std=c99

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

long long decimaltoBinary(int decimal){
	int dec = decimal;
	long long result = 0;
	long long i = 1;
	long long r;
	while(dec != 0)
	{
		r = dec % 2;
        dec = dec / 2;
        result = result + r * i;
        i = i * 10;
    }
    return result;
}

int binarytoDecimal(long long binary){
	int result = 0;
	long long bin = binary;
	int i = 0 ;
	int r;
	while(bin != 0)
	{
		r = bin % 10;
		bin = bin / 10;
		result = result + r * pow(2,i);
		i = i+1;
	}
	return result;
}

int main(void) {
	int physical[32762]; //physical memory
	int dpaddr; //decimal of physical address
	long long bpaddr; //binary of physical address in int format
    char paddress[16]; //binary of physical address
    int va; //decimal of virtual address
    char vaddress[16]; //binary of virtual address
    char * pagetable[16][2] = {{"010","1"},
    						{"001","1"}, 
    						{"110","1"},
    						{"000","1"},
    						{"100","1"}, 
    						{"011","1"},
    						{"000","0"},
    						{"000","0"}, 
    						{"000","0"},
    						{"101","1"},
    						{"000","0"}, 
    						{"111","1"},
    						{"000","0"},
    						{"000","0"}, 
    						{"000","0"},
    						{"000","0"}
							};
    char vpagenum[3]; //stores physcial pagen number from page table
    char voffset[12]; //virtual address offset

    //initialize physical memory data
    for (int i = 0; i < 32762; i++){
    	if (i <= 4095) {
    		physical[i] = 0;
    	} else if (i > 4095 && i <= 8191){
    		physical[i] = 1;
    	} else if (i > 8191 && i <= 12287){
    		physical[i] = 2;
    	} else if (i > 12287 && i <= 16383){
    		physical[i] = 3;
    	} else if (i > 16383 && i <= 20479){
    		physical[i] = 4;
    	} else if (i > 20479 && i <= 24575){
    		physical[i] = 5;
    	} else if (i > 24575 && i <= 28671){
    		physical[i] = 6;
    	} else { //if (i > 28671 && i <= 32767){
    		physical[i] = 7;
    	}
    }

    int vas[3] = {20, 4100, 8300}; //virtual address to test
    for (int n = 0; n < 3; n++)
    {
        va = vas[n];
        //convert va to binary, save as vaddress
        char temp[16];
        int count = 0;
        //int length;
        snprintf(temp, 16, "%ld", decimaltoBinary(va));
        int len = strlen(temp);
        if (strlen(temp) <= 16){
        	for (int z = 0; z < 16-len; z++){
        		vaddress[z] = '0';
        	}
        	for (int v = 16-len; v < 16; v++){
        		vaddress[v] = temp[count];
        		count++;
        	}
        } 
        printf("virtual address passed in: %s \n", vaddress);

        //save vpagenum from pagetable, first 4 digits, convert string to int
        char fdigit[4];
        for (int x = 0; x < 4; x++){ 
        	fdigit[x] = vaddress[x];
        }
        printf("First 4 digits of virtual address: %s\n", fdigit);

        //save voffset, last 12 digits
        count = 0;
        for (int y = 4; y < 16; y++){
            voffset[count] = vaddress[y];
            count++;
        }
        printf("Virtual address offset: %s\n", voffset);


        //the 4 digits convert to decimal for index to page table
        int index;
        index = binarytoDecimal(atoi(fdigit)); 
        strcpy(vpagenum,pagetable[index][0]); //ex. 101 , 001, 110, ....
        printf("physical page number from page table: %s\n\n", vpagenum);

        //check present bit, if 1 or 0 
        if (strcmp(pagetable[index][1],"1") == 0){
        	//print page hit
            printf("Page Hit\n");
        	//create physical address
        	//concatenate pagetable string with voffset, save as paddress
        	for (int w = 0; w < 3; w++){
        		paddress[w] = vpagenum[w];
        	}
        	count = 0;
            for (int u = 3; u < 16; u++){
            	paddress[u] = voffset[count];
            	count++;
            }
            printf("physical address is: %s\n", paddress);

            //convert physical address string to int, save as bpaddr
    		bpaddr = atoll(paddress);     

        	//convert physical address binary to decimal, save as dpaddr
    		dpaddr = binarytoDecimal(bpaddr);


        	//print out physical address, value in memory address
    		printf("Page frame in physical memory is: %d\n\n", physical[dpaddr]);
        
        //else print page fault
        } else {
            //print page fault
            printf("Page Fault\n");
            //create physical address
        	//concatenate pagetable string with voffset, save as paddress
        	for (int w = 0; w < 3; w++){
        		paddress[w] = vpagenum[w];
        	}
        	count = 0;
            for (int u = 3; u < 16; u++){
            	paddress[u] = voffset[count];
            	count++;
            }
            printf("physical address is: %s\n", paddress);
        }
        printf("---------------------------------------------------\n");

        //reset variables
        memset(voffset, 0, sizeof voffset);
        memset(vpagenum, 0, sizeof vpagenum);
        dpaddr = 0;
        bpaddr = 0;
        memset(paddress, 0, sizeof paddress);
        memset(vaddress, 0, sizeof vaddress);
    }
}
