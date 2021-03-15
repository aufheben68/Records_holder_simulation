#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE 5000

/*

example for new record: newCitizen Manwlis, Antwnopoulos, 27/06/1978, 271-1102030, manwlis@uop.com, 147.27.32.15:9535, birthCert, 79927-39871-3
*/

typedef struct{
  int year;
  int month;
  int day;
}date;

typedef struct{
  char IP[16];
  int port;
}netData;


typedef struct ctzn{
  char *onoma;
  char *epitheto;
  date birthday;
  char *thlefwno;
  char *mail;
  netData terminal;
	char *dikaiologitiko;
  char *arithmosSunallagis;
	struct ctzn *next;
}citizen;


void dwseEntoli();
void enQueue(char** subString);
void printQueue();
void Dequeue();
void reverse(citizen *temp);
void free_q();
void restore();

char entoli[MAX_LINE];
char p[MAX_LINE];
char* subString[MAX_LINE];
char* subString1a;
int x,numero,i=0;
struct ctzn *head;
struct ctzn *tail;

int main(int argc, char *argv[]){
	  head = tail = NULL;
    srand(time(NULL));
    printf("Records holder simulation.\n");
    printf("Welcome.\n");
    do{
      printf("Please give an operation to be executed:\n");
      dwseEntoli();
    }while(strcmp(subString1a,"exit")!=0);
    printf("Program exiting!\n");
    free_q();
}

//function for checking the transaction code
char *neos_arithmos_sinallagis(){
	char *s = strdup("xXxXx-XxXxX-q");
	int sum = 0, k;

	for (i = 0 ; i < 13 ; i++){
		switch (s[i]){
			case 'x':
				k = rand() % 10;
				s[i] = k + '0';
				sum += k;
				break;
			case 'X':
				k = rand() % 10;
				s[i] = k + '0';
				k *= 2;
				if (k > 9) k -= 9;
				sum += k;
				break;
			case 'q':
				k = (sum * 9) % 10;
				s[i] = k + '0';
		}
	}
	return s;
}

//function for checking if all input is valid
int checks(){
	int j, k, pavla, digits, papaki, teleies, sum;
	char a[60] = {0};
	char *token = NULL;

	/* name surname*/
	for (j = 0  ; j < 2 ; j++){
		k = 0;
		for (i = 0 ; i < strlen(subString[j]) ; i++){
			if (isalpha(subString[j][i])){
				a[k++] = subString[j][i];
			}
		}
		a[k] = '\0';
		subString[j] = strdup(a);
	}

	/* date */
	if (strlen(subString[2]) != 10){
		printf("Wrong length of date (%s)\n",subString[2]);
		return 0;
	}
	//printf("imerominia (%s)\n",subString[2]);
	if (!isdigit(subString[2][0]) || !isdigit(subString[2][1]) || subString[2][2] != '/' ||
		!isdigit(subString[2][3]) || !isdigit(subString[2][4]) || subString[2][5] != '/' ||
		!isdigit(subString[2][6]) || !isdigit(subString[2][7]) ||
		!isdigit(subString[2][8]) || !isdigit(subString[2][9]) )
	{
		printf("Wrong format of date\n");
		return 0;
	}

	i = atoi(subString[2]);
	if (i < 1 || i > 31){
		printf("Wrong day\n");
		return 0;
	}

	i = atoi(subString[2] + 3);
	if (i < 1 || i > 12){
		printf("Wrong month\n");
		return 0;
	}

	i = atoi(subString[2] + 6);
	if (i < 1900 || i > 1995){
		printf("Wrong year\n");
		return 0;
	}

	/* tel */
	if (strlen(subString[3]) != 11){
		printf("Wrong length of phone number.\n");
		return 0;
	}

	pavla = digits = 0;
	for (i = 0 ; i < 11 ; i++){
		if (subString[3][i] == '-'){
			pavla++;
			if (i != 3 && i != 4){
				printf("You have inserted '-' in a wrong position of the cellphone number.\n");
				return 0;
			}
		}
		else if (!isdigit(subString[3][i])){
			printf("Wrong character %c on the cellphone number.\n", subString[3][i]);
			return 0;
		}
		else digits++;
	}
	if (digits != 10 || pavla != 1){
		printf("Wrong format of cellphone number.\n");
		return 0;
	}

	/* email */
	//printf("email (%s)\n",subString[4]);
	digits = papaki = teleies = 0;
	for (i = 0 ; i < strlen(subString[4]) ; i++){
		if (subString[4][i] == '@') papaki++;
		else if (subString[4][i] == '.') teleies++;
		else if (isalnum(subString[4][i])) digits++;
		else {
			printf("Wrong character %c on email input.\n", subString[4][i]);
			return 0;
		}
	}
	i = strlen(subString[4]);

	if (isdigit(subString[4][0])){
		printf("First character on an email cant be a digit.\n");
		return 0;
	}
	else if (papaki != 1 || teleies == 0 || digits < 4){
		printf("Wrong format of email.\n");
		return 0;
	}
	else if (!isalpha(subString[4][i-1]) || !isalpha(subString[4][i-2]) || subString[4][i-3] != '.'){
		printf("Wrong postfix of email.\n");
		return 0;
	}

	/* ip */
	digits = teleies = k = 0;
	for ( i = 0 ; i < strlen(subString[5]) ; i++){
		if (subString[5][i] == '.')teleies++;
		else if (subString[5][i] == ':') {
			k++;
			if (teleies != 3){
				printf("Wrong format of ip (please use acombination of '.' and ':'\n");
				return 0;
			}
		}
		else if (!isdigit(subString[5][i])) {

			printf("Wrong character %c on ip input.\n", subString[5][i]);
			return 0;
		}
		else digits++;
	}

	if (teleies != 3){
		printf("Wrong number of inputs in ip field.\n");
		return 0;
	}
	if (k != 1){
		printf("Wrong number of inputs in ip field (':' is required only once)\n");
		return 0;
	}
	strcpy(a, subString[5]);

	token = strtok(a, ".");
	for (j = 0 ; j < 4 ; j++){
		i = atoi(token);
		if ( i < 0 || i > 256){
			printf("Wrong ip value %d\n", i);
			return 0;
		}
		token = strtok(NULL, ".:");
	}

	if ( i < 0 || i > 65535){
			printf("Wrong port value %d\n", i);
			return 0;
	}

	/* cert */
	if (strcmp(subString[6], "familyCert") != 0 &&
		strcmp(subString[6], "birthCert") != 0 &&
		strcmp(subString[6], "debtCert") != 0 &&
		strcmp(subString[6], "residenceCert") != 0 &&
		strcmp(subString[6], "electionCert") != 0 ){
			printf("Certification is wrong.\n");
			return 0;
		}

	/* transaction code */
	if (strlen(subString[7]) != 13){
		//printf("(%s) %d\n", subString[7], strlen(subString[7]));
		subString[7] = neos_arithmos_sinallagis();
		return 1;
	}
	sum  = 0;
	for ( i = 0 ; i < 13 ; i++){
		if (!isdigit(subString[7][i])){
			if (subString[7][i] != '-'){
				subString[7] = neos_arithmos_sinallagis();
				return 1;
			}
			else if (i != 5 && i != 11){
				subString[7] = neos_arithmos_sinallagis();
				return 1;
			}
		}
		else if (i == 1 || i == 3 || i == 6 || i == 8 || i == 10){

			k = (subString[7][i] - '0') * 2;
			if (k > 9) k-= 9;
			sum += k;
			//printf("k=%d sum=%d\n", k , sum);
		}
		else {
			k = (subString[7][i] - '0');
			sum += k;
			//printf("k=%d sum=%d\n", k , sum);
		}
	}
	if (sum % 10 > 0){
		subString[7] = neos_arithmos_sinallagis();
	}
	return 1;
}

//free momey of the queue we used
void free_q(){
	citizen* q;
	while (head != NULL){
		q = head;
		head = q->next;
		free(q);
	}
}

//function for saving a file of the records
void backup(){
	char* filename = strtok(NULL, "\n");
	FILE *f = NULL;
	citizen* temp = head;
	if (head == NULL){
		printf("Empty queue!\n");
		return;
	}
	if (filename == NULL){
		printf("You have to give a file name!\n");
		return;
	}

	f = fopen(filename, "w");
	if (f == NULL){
		printf("fopen error\n");
		return;
	}

	while (temp != NULL){
		fprintf(f, "%50s %50s %02d/%02d/%d %s %50s %16s:%05d %13s %s\n",temp->onoma, temp->epitheto, temp->birthday.day, temp->birthday.month,
      	temp->birthday.year, temp->thlefwno, temp->mail, temp->terminal.IP, temp->terminal.port, temp->dikaiologitiko ,temp->arithmosSunallagis);

  		temp = temp->next;
	}
	fclose(f);
	printf("Data succesfully saved on file %s !\n", filename);
}

//function for obtaining records from a file
void restore(){
	char delimit[]=" ,\n";
	FILE *f = fopen(subString1a, "r");
	if (f == NULL){
		printf("fopen error\n");
		return;
  }
	while (fgets(entoli, sizeof entoli, f) != NULL){
		i = 0;
        subString[i]=strtok(entoli," ");
        while (subString[i] != NULL){
	        subString[++i]=strtok(NULL,delimit);
	    }
        if (checks() == 0) return;

        enQueue(subString);
	}

	fclose(f);
}
// function that checks which operation the user has demanded and feels in the queue with the inserted data
void dwseEntoli(){
    char delimit[]=" ,\n";
      if(fgets(entoli, sizeof entoli, stdin)){
        strcpy(p,entoli);
        subString1a = strtok(entoli,delimit);
    }
      if(strcmp(subString1a,"newCitizen")==0){
        strtok(p," ");
        i = 0;
        subString[i]=strtok(NULL," ");
        while (subString[i] != NULL){
	        subString[++i]=strtok(NULL,delimit);
	      }
        if (checks() == 0) return;
        enQueue(subString);
      }else if(strcmp(subString1a,"serveCitizen")==0){
        printf("You have chosen to server the first customer.\n");
        Dequeue();
        x=2;
        return;
      }else if(strcmp(subString1a,"removeCitizen")==0){
        printf("You have chosen for a citizen to leave the queue.\n");
        x=3;
        return;
      }else if(strcmp(subString1a,"print")==0){
        printQueue();
      }else if(strcmp(subString1a,"printRev")==0){
        printf("You have chosen for the queue to be printed backwards!\n");
        subString1a = strtok(NULL, " \n");
        reverse(head);
        return;
      }
      else if(strcmp(subString1a,"backup")==0){
        backup();
      }
      else if(strcmp(subString1a,"restore")==0){
        subString1a = strtok(NULL," \n");
        if (subString1a == NULL){
        	printf("You have to give a file name!\n");
        	return;
        }
        if (head != NULL){
        	free_q();
        	head = tail = NULL;
        }
        restore();
      }

  }

  // To Enqueue an integer
  void enQueue(char* subString[MAX_LINE]) {
  	char *token = NULL;
    citizen* temp = (citizen*)malloc(sizeof(citizen));
    temp->onoma=strdup(subString[0]);
	  temp->epitheto=strdup(subString[1]);
	  temp->thlefwno  = strdup(subString[3]);
	  temp->mail  = strdup(subString[4]);
	  temp->dikaiologitiko  = strdup(subString[6]);
	  temp->arithmosSunallagis  = strdup(subString[7]);

  	token = strtok(subString[5], ":");
  	strcpy(temp->terminal.IP, token);

  	token = strtok(NULL, " ");
  	temp->terminal.port = atoi(token);

  	token = strtok(subString[2], "/");
  	temp->birthday.day = atoi(token);

  	token = strtok(NULL, "/");
  	temp->birthday.month = atoi(token);

  	token = strtok(NULL, "/");
  	temp->birthday.year = atoi(token);
  	temp->next = NULL;
  	if(head == NULL && tail == NULL){
  		head = tail = temp;
  		return;
  	}
  	tail->next = temp;
  	tail = temp;
  }

//function to print the queue
void printQueue(){
  citizen* temp = head;
  char *mode = strtok(NULL, " \n");
  if (mode == NULL){
  	printf("You have to type '*' or a give a certificate number.\n");
  	return;
  }
	while(temp != NULL) {
		if (strcmp(mode, "*") == 0 || strcmp(mode, temp->dikaiologitiko) == 0)
      printf("%50s %50s %02d/%02d/%d %s %50s %16s:%05d %13s %s\n",temp->onoma, temp->epitheto, temp->birthday.day, temp->birthday.month,
      	temp->birthday.year, temp->thlefwno, temp->mail, temp->terminal.IP, temp->terminal.port, temp->dikaiologitiko ,temp->arithmosSunallagis);

  		temp = temp->next;
	}
	printf("\n");

}


//function to dequeue the first cell
void Dequeue() {
	citizen* temp = head;
	if(head == NULL) {
		printf("Queue is empty.\n");
		return;
	}
	if(head == tail) {
		head = tail = NULL;
	}
	else {
		head = head->next;
	}
	free(temp);
}

//recursive function to reverse the queue
void reverse(citizen *temp) {
	if (temp == NULL) return;

	reverse(temp->next);
	if (strcmp(subString1a, "*") == 0 || strcmp(subString1a, temp->dikaiologitiko) == 0)
	 printf("%50s %50s %02d/%02d/%d %s %50s %16s:%05d %13s %s\n",temp->onoma, temp->epitheto, temp->birthday.day, temp->birthday.month,
      	temp->birthday.year, temp->thlefwno, temp->mail, temp->terminal.IP, temp->terminal.port, temp->dikaiologitiko ,temp->arithmosSunallagis);
}
