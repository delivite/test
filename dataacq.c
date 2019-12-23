#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <errno.h> 
#include <termios.h> 
#include <unistd.h> 
struct patient   //Storing data of patient 
{
	int id; char name[30]; 
	int heartrate; 
	int heartrates; 
	int resprate; 
	int resprates; 
}; 
void newrecord();   //Function to add new information 
void display();     //Function to display patient information 
void displayAll();  //Function to display all patient records 
void modify();      //Function to update existing records 
void del();         //Function to delete existing records 
void search();      //Function to search the record in the store 
//Data structure example as reference is seen in bibliography [17] 
char mygetch(); //Linux doesnt have getch ,mygetch is created 
char fname[]={"mydb.dat"}; //name of the file 
int main() 
{ 
	int ch; 
	while(1) //To ask all the time what option you would like to have 
	{ 
		system("clear"); 
		printf("=======Patient Management System for Anxiety Factor======\n\n"); 
		printf("1. New Record \n\n"); 
		printf("2. Modify \n\n"); 
		printf("3. Delete \n\n"); 
		printf("4. Search\n\n"); 
		printf("5. Display \n\n"); 
		printf("6. Display All \n\n"); 
		printf("0. Exit\n\n"); 
		printf("=========================================================\n\n"); 
		printf("What would you like to do?\n\n"); 
		printf("Please enter your choice: "); 
		scanf("%d",&ch); 
		switch(ch) 
		{ 
			case 1: newrecord(); 
			break; 
			case 2: modify(); 
			break; 
			case 3: del(); 
			break;
			case 4: search(); 
			break; 
			case 5: display(); 
			break; 
			case 6: displayAll(); 
			break; 
			case 0: exit(0); 
		}
		mygetch(); 
	} 
return 0; 
} 

void newrecord() //To add new information 
{ 
        int j=0; int jj=0; int hr; int hrs; int rr; int rrs; 
	int mo; int mos; int day; int days; int pos; int poss; 
	int sumhr=0; int sumhrs=0; int sumrr=0; int sumrrs=0; 
	int avhr; int avhrs; int avrr; int avrrs; int num; 
	printf("\nTo get normal values of the patient please press 1: "); 
	scanf("%d",&num); 
	if(num==1) 
	{ 
		int fd; 
		const char *device = "/dev/rfcomm0"; 
		speed_t baud = B9600; //Baud rate 
		fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY ); //Openning Serial Port 
	//Reference for opening serial port is in Bibliography [18] 
	struct termios settings; 
	tcgetattr(fd, &settings); 
	cfsetispeed(&settings, baud); // baud rate 
	cfsetospeed(&settings, baud); 
	settings.c_cflag &= ~PARENB; // no parity 
	settings.c_cflag &= ~CSTOPB; // 1 stop bit 
	settings.c_cflag &= ~CSIZE; 
	settings.c_cflag |= CS8 | CLOCAL; // 8 bits 
	settings.c_lflag = ICANON; // canonical mode 
	settings.c_oflag &= ~OPOST; // raw output 
	tcsetattr(fd, TCSANOW, &settings); // apply the settings 
	tcflush(fd, TCOFLUSH); //Reference for the settings is in Bibliography [19] 
	if(fd==-1) 
	{ 
		printf("Failed to open port\n"); 
	} 
	else 
	{ 
		int kbhit() //Function to hit keyboard so that program will stop 
		{ 
			struct timeval tv;

			fd_set fds; 
			tv.tv_sec=0; 
			tv.tv_usec=0; 
			FD_ZERO(&fds); 
			FD_SET(STDIN_FILENO,&fds); 
			select(STDIN_FILENO+1,&fds,NULL,NULL,&tv); 
			return FD_ISSET(STDIN_FILENO,&fds); 
		} 
	char bufA[1]; //to recieve acknowlegment 
	char bufM[55]; //to recieve data packet 
	FILE *fp; 
	fp = fopen("NormalHR.txt","w"); //creating a txt file to store the HR 
	FILE *fpr; 
	fpr = fopen("NormalRR.txt","w"); //creating a txt file to store the RR 
	printf("\n Month | Day | HR | RR | Posture\n");

	while(!kbhit()) 
		{ 
			write(fd,"\x02\x14\x01\x01\x5E\x03",6);//general data packet request 
			fcntl(fd, F_SETFL, 0); 
			int n = read(fd, bufA, 1);//reading the values byte by byte 
			if(bufA[0]==2) { n = read(fd, bufA, 1);//Reading the message ID 
			if(bufA[0]==32) { n = read(fd, bufA, 1);//Reading the payload 
			if(bufA[0]==53) { n = read(fd, bufM, 55); //reading the values from the buffer 
			mo = bufM[3]; 
			day = bufM[4]; 
			hr = bufM[9]; 
			rr = bufM[11]/10; 
			pos = bufM[15]; 
			if( mo==bufM[3] && day==bufM[4] && hr!=0 && rr!=0) //To get rid of interraptions and 0 values 
				{ 
					j++; 
					printf("%d | %d | %d | %d | %d\n",mo,day,hr,rr,pos); 
					fprintf(fp,"%d\r\n",hr); 
					fprintf(fpr,"%d\r\n",rr);
                                        fflush(fp);
                                        fflush(fpr); 
					sumhr=sumhr+hr; //Summation of printed Heart Rates 
					sumrr=sumrr+rr; //Summation of printed Respiratory Rates

//fclose(fp); //close fp file
  //      fclose(fpr); //close fpr file

 	} } } } }// end of while loop
	fclose(fp); //close fp file
        fclose(fpr); //close fpr file
        close(fd); //close serial port
        avhr=sumhr/j; //Average value of Normal HR
        avrr=sumrr/j; //Avarage value of Normal RR
        printf("Summation HR is: %d Average HR is: %d\n",sumhr,avhr);
        printf("Summation RR is: %d Average RR is: %d\n",sumrr,avrr);
        }
  
  
	sleep(3); //to get ready for second measurements 
//Same procedure is done with above for getting values. This time for Experimental HR and RR 
	int num1; 
	printf("\nWhen you are ready for the experiment please press 1: "); 
	scanf("%d",&num1); 
	if(num1==1) 
	{ 
		int fdd; 
		const char *device = "/dev/rfcomm1"; 
		speed_t baud = B9600; //Baud rate 
		fdd = open(device, O_RDWR | O_NOCTTY | O_NDELAY ); 
	struct termios settings; 
	tcgetattr(fdd, &settings); 
	cfsetispeed(&settings, baud); // baud rate 
	cfsetospeed(&settings, baud); 
	settings.c_cflag &= ~PARENB; // no parity 
	settings.c_cflag &= ~CSTOPB; // 1 stop bit 
	settings.c_cflag &= ~CSIZE; 
	settings.c_cflag |= CS8 | CLOCAL; // 8 bits 
	settings.c_lflag = ICANON; // canonical mode 
	settings.c_oflag &= ~OPOST; // raw output 
	tcsetattr(fdd, TCSANOW, &settings); // apply the settings 
	tcflush(fdd, TCOFLUSH); 
	if(fdd == -1) 
	{ 
		printf( "failed to open port\n" ); 
	} else 
	{ 
		int kbhitt() 
			{ 
			struct timeval tv; 
			fd_set fdss; 
			tv.tv_sec=0; 
			tv.tv_usec=0;//without this it directly blocks 
			FD_ZERO(&fdss); 
			FD_SET(STDIN_FILENO,&fdss); 
			select(STDIN_FILENO+1,&fdss,NULL,NULL,&tv); 
	return FD_ISSET(STDIN_FILENO,&fdss); 
} 
//Reference for the kbhit function is in Bibliography [20] 
char bufAA[1]; //to recieve acknowlegment 
char bufMM[55]; //to recieve data packet 
FILE *fpp; 
FILE *fppr; 
fpp = fopen("ScaredHR.txt","w");//creating a txt file to store values 
fppr= fopen("ScaredRR.txt","w");//creating a txt file to store values 
printf("\n Number | Month | Day | HR | RR | Posture\n"); 
while(!kbhitt())

{ 
	write(fdd,"\x02\x14\x01\x01\x5E\x03",6);//General DataPacket request 
	fcntl(fdd, F_SETFL, 0); 
	int nn = read(fdd, bufAA, 1);//reading acknowledgment from buffer 
if(bufAA[0]==2) 
{ nn = read(fdd, bufAA, 1);//reading acknowledgment from buffer 
if(bufAA[0]==32) 
{ nn=read(fdd, bufAA, 1); 
if(bufAA[0]==53) { nn = read(fdd, bufMM, 55);//reading acknowledgment from buffer 
mos=bufMM[3]; 
days=bufMM[4]; 
hrs=bufMM[9]; 
rrs=bufMM[11]/10; 
poss=bufMM[15]; 
if( mos==bufMM[3] && days==bufMM[4] && hrs!=0 && rrs!=0) 
{ 
jj++; 
printf("%d |%d | %d | %d | %d |%d\n",jj,mos,days,hrs,rrs,poss ); 
fprintf(fpp,"%d " "%d\n",jj,hrs); 
fprintf(fppr,"%d " "%d\n",jj,rrs); 
sumhrs=sumhrs+hrs; sumrrs=sumrrs+rrs; 
} } } } } //end while loop 
fclose(fpp); 
fclose(fppr); 
close(fdd); 
avhrs=sumhrs/jj; 
avrrs=sumrrs/jj; 
printf("Summation HRS is: %d Average HRS is: %d\n",sumhrs,avhrs); 
printf("Summation RRS is: %d Average RRS is: %d\n",sumrrs,avrrs); 
} 
FILE *fpointer;//Actual file where the information is stored 
struct patient p1; //p1 is the variable name 
fpointer=fopen(fname,"ab");//open in append mode that do in binary 
printf("\nEnter ID:"); //To store p1's id 
scanf("%d",&p1.id); 
printf("\nEnter name:"); //To store p1's name 
scanf("%s",p1.name); //To store avarage values 
p1.heartrate=avhr; 
p1.heartrates=avhrs; 
p1.resprate=avrr;
p1.resprates=avrrs; 
fwrite(&p1,sizeof(p1),1,fpointer); //To transfer p1's details into file 
fclose(fpointer); 
sleep(2); 
printf("\nPlease press Enter to go to Menu\n"); 
}} } 

void modify() 
{ 
FILE *fpointer,*fp1;//fp1 is temporary file 
struct patient p,p1; 
int id,found=0,count=0;
fpointer=fopen(fname,"rb"); 
fp1=fopen("temp.dat","wb"); 
printf("\nEnter the Patient ID you want to Modify:"); 
scanf("%d",&id); 
while(1) 
{ 
fread(&p,sizeof(p),1,fpointer); 
if(feof(fpointer)) 
{ break; } 
if(p.id==id)//id which is read(p.id) and id that user said(id) matches 
{ found=1; printf("\nEnter Patient ID:"); //Asking for new value 
scanf("%d",&p.id); 
fflush(stdin); 
printf("\nEnter Patient Name:"); 
scanf("%s",p.name); 
fwrite(&p,sizeof(p),1,fp1);//writing in a temporary file(fp1) 
} 
else { fwrite(&p,sizeof(p),1,fp1);//writing the exist information 
} } 
fclose(fpointer); 
fclose(fp1); 
if(found==0) //if you don't have the records that you asked 
{ printf("Sorry No Record Found\n\n"); } 
else { fpointer=fopen(fname,"wb"); 
fp1=fopen("temp.dat","rb"); 
while(1) 
{ fread(&p,sizeof(p),1,fp1); //reading the values from temp. file 
if(feof(fp1)) //until end of the text 
{ break;
} 
fwrite(&p,sizeof(p),1,fpointer);//writing the values to the actual file 
} } 
fclose(fpointer); 
fclose(fp1); 
} 

void del() { 
FILE *fpointer,*fp1; 
struct patient p,p1; 
int id,found=0,count=0; 
fpointer=fopen(fname,"rb"); 
fp1=fopen("temp.dat","wb"); 
printf("\nEnter the Patient ID you want to Delete:"); 
scanf("%d",&id); 
while(1) { 
fread(&p,sizeof(p),1,fpointer); 
if(feof(fpointer)) 
{ break; } 
if(p.id==id) 
{
int a;
 printf("If you really want to delete this record, please press 1:"); 
scanf("%d",&a); 
if(a==1) 
{ found=1; //it will just delete 
} } 
else 
{ fwrite(&p,sizeof(p),1,fp1);//simply writes the old values
 } } 
fclose(fpointer); 
fclose(fp1); 
if(found==0) 
{ printf("Sorry No Record Found or Deleted\n\n"); 
} 
else { fpointer=fopen(fname,"wb"); 
fp1=fopen("temp.dat","rb"); 
while(1) 
{ 
fread(&p,sizeof(p),1,fp1); 
if(feof(fp1)) { break; 
} 
fwrite(&p,sizeof(p),1,fpointer); 
} } 
fclose(fpointer);
fclose(fp1); 
} 
void display() 
{ FILE *fpointer; 
struct patient p; 
int id,found=0; 
fpointer=fopen(fname,"rb"); 
printf("\nEnter the Patient ID:"); 
scanf("%d",&id); 
while(1) 
{ 
fread(&p,sizeof(p),1,fpointer); 
if(feof(fpointer)) 
{ break; } 
if(p.id==id) //verifying the id,if id matches then information will come 
{ found=1; printf("\n======================================================\n\n"); 
printf("\t\t Patient Details of %d\n\n",p.id); 
printf("\n======================================================\n\n"); 
printf("Name\t\tNormalHR\tExperimentHR\tNormalRR\tExperimentRR\n\n"); 
printf("%s\t\t",p.name); printf("%d\t\t",p.heartrate); 
printf("%d\t\t",p.heartrates); 
printf("%d\t\t",p.resprate); 
printf("%d\n\n",p.resprates); 
printf("\n======================================================\n\n"); 
} } 
if(found==0) 
{ printf("\nSorry No Record Found"); 
} 
fclose(fpointer); 
} 

void search() 
{ 
FILE *fpointer; 
struct patient p; 
int found=0; 
char name[30]; 
fpointer=fopen(fname,"rb"); 
printf("\nEnter the Patient Name:"); 
scanf("%s",&name); 
while(1) { 
fread(&p,sizeof(p),1,fpointer); 
if(feof(fpointer)) 
{ break; } 
if(strcmp(name,p.name)==0) //To compare the strings, strcmp is used 
{
printf("\n======================================================\n\n");
 printf("\t\t Patient Details of %d\n\n",p.id); 
printf("\n======================================================\n\n"); 
printf("Name\t\tNormalHR\tExperimentHR\tNormalRR\tExperimentRR\n\n"); 
printf("%s\t\t",p.name); 
printf("%d\t\t",p.heartrate); 
printf("%d\t\t",p.heartrates); 
printf("%d\t\t",p.resprate); 
printf("%d\n\n",p.resprates); 
printf("\n======================================================\n\n"); 
} } 
if(found==0) 
{ printf("\nSorry No Record Found"); } 
fclose(fpointer); 
} 

void displayAll() 
{ 
FILE *fpointer; 
struct patient p; 
fpointer=fopen(fname,"rb"); //To open the file in read binary mode 
printf("\n========================================================\n\n"); 
printf("\t\t All Patient Details\n\n"); 
printf("\n========================================================\n\n"); 
printf("ID\tName\t\tNormalHR\tExperimentHR\tNormalRR\tExperimentRR\n\n"); 
while(1) //To read the file until finding end of the file or not 
{ fread(&p,sizeof(p),1,fpointer);//To read first record and store into p 
if(feof(fpointer)) 
{ break; //when it founds end of the file simply break 
} //Display the values 
printf("%d\t",p.id); 
printf("%s\t\t",p.name); 
printf("%d\t\t",p.heartrate); 
printf("%d\t\t",p.heartrates); 
printf("%d\t\t",p.resprate); 
printf("%d\n\n",p.resprates); 
} 
printf("\n=======================================================\n\n"); 
fclose(fpointer); 
} 
char mygetch() 
{ 
char val; 
char rel; 
scanf("%c",&val); 
scanf("%c",&rel); 
return (val); 
}


