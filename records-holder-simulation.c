#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE 5000

/*

example for new record: newCitizen Christos, Tryfonopoulos, 27/06/1978, 271-1102030, trifon@uop.gr, 147.27.32.15:9535, birthCert, 79927-39871-3

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
    printf("Efarmogi e3upiretisis pelatwn e-government kolokotrwnitsiou.\n");
    printf("Welcome.\n");
    do{
      printf("Parakalw dwste entoli:\n");
      dwseEntoli();
    }while(strcmp(subString1a,"exit")!=0);
    printf("Kalh sunexeia kai prosoxi stin PAPADIA(milaei polu)!\n");
    free_q();
}

//sunartisi pou elegxei ton arithmo sunallagis
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

//sunartisi gia ton elegxo egkurotitas twn stoixeiwn pou eisagoume
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
		printf("Lathos mikos imerominias (%s)\n",subString[2]);
		return 0;
	}
	//printf("imerominia (%s)\n",subString[2]);
	if (!isdigit(subString[2][0]) || !isdigit(subString[2][1]) || subString[2][2] != '/' ||
		!isdigit(subString[2][3]) || !isdigit(subString[2][4]) || subString[2][5] != '/' ||
		!isdigit(subString[2][6]) || !isdigit(subString[2][7]) ||
		!isdigit(subString[2][8]) || !isdigit(subString[2][9]) )
	{
		printf("Lathos morfi imerominias\n");
		return 0;
	}

	i = atoi(subString[2]);
	if (i < 1 || i > 31){
		printf("Lathos mera\n");
		return 0;
	}

	i = atoi(subString[2] + 3);
	if (i < 1 || i > 12){
		printf("Lathos minas\n");
		return 0;
	}

	i = atoi(subString[2] + 6);
	if (i < 1900 || i > 1995){
		printf("Lathos etos\n");
		return 0;
	}

	/* tel */
	if (strlen(subString[3]) != 11){
		printf("Lathos mikos tilefonou\n");
		return 0;
	}

	pavla = digits = 0;
	for (i = 0 ; i < 11 ; i++){
		if (subString[3][i] == '-'){
			pavla++;
			if (i != 3 && i != 4){
				printf("Lathos thesi pavlas sto tilefono\n");
				return 0;
			}
		}
		else if (!isdigit(subString[3][i])){
			printf("Lathos xaraktiras %c sto tilefono\n", subString[3][i]);
			return 0;
		}
		else digits++;
	}
	if (digits != 10 || pavla != 1){
		printf("Lathos morfi tilefonou\n");
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
			printf("Lathos xaraktiras %c sto email\n", subString[4][i]);
			return 0;
		}
	}
	i = strlen(subString[4]);

	if (isdigit(subString[4][0])){
		printf("O protos xaraktiras sto email den borei na einai psifio\n");
		return 0;
	}
	else if (papaki != 1 || teleies == 0 || digits < 4){
		printf("Lathos morfi email\n");
		return 0;
	}
	else if (!isalpha(subString[4][i-1]) || !isalpha(subString[4][i-2]) || subString[4][i-3] != '.'){
		printf("Lathos kataliksi email\n");
		return 0;
	}

	/* ip */
	digits = teleies = k = 0;
	for ( i = 0 ; i < strlen(subString[5]) ; i++){
		if (subString[5][i] == '.')teleies++;
		else if (subString[5][i] == ':') {
			k++;
			if (teleies != 3){
				printf("Lathos morfi ip (sindiasmos '.' kai ':'\n");
				return 0;
			}
		}
		else if (!isdigit(subString[5][i])) {

			printf("Lathos xaraktiras %c sto ip\n", subString[5][i]);
			return 0;
		}
		else digits++;
	}

	if (teleies != 3){
		printf("Lathos plithos pedion sto ip\n");
		return 0;
	}
	if (k != 1){
		printf("Lathos plithos pedion sto ip (epitrepetai mia fora to ':')\n");
		return 0;
	}
	strcpy(a, subString[5]);

	token = strtok(a, ".");
	for (j = 0 ; j < 4 ; j++){
		i = atoi(token);
		if ( i < 0 || i > 256){
			printf("Lathos timi ip %d\n", i);
			return 0;
		}
		token = strtok(NULL, ".:");
	}

	if ( i < 0 || i > 65535){
			printf("Lathos timi port %d\n", i);
			return 0;
	}

	/* cert */
	if (strcmp(subString[6], "familyCert") != 0 &&
		strcmp(subString[6], "birthCert") != 0 &&
		strcmp(subString[6], "debtCert") != 0 &&
		strcmp(subString[6], "residenceCert") != 0 &&
		strcmp(subString[6], "electionCert") != 0 ){
			printf("To pistopoiitiko einai lathos\n");
			return 0;
		}

	/* arithmos sunallagis */
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

//apodesmefsi mnimis gia tin ouritsa mas me tin entoli exit
void free_q(){
	citizen* q;
	while (head != NULL){
		q = head;
		head = q->next;
		free(q);
	}
}

void backup(){
	char* filename = strtok(NULL, "\n");
	FILE *f = NULL;
	citizen* temp = head;
	if (head == NULL){
		printf("Adeia oura!\n");
		return;
	}
	if (filename == NULL){
		printf("Prepei na dosete onoma arxeiou!\n");
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
	printf("ta dedomena apothikeftikan sto arxeio %s\n", filename);
}


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
// sunarthsh pou elegxei poia entoli ehei eisagei o xristis kai gemizei tin oura mas
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
        printf("Dwsate tin entoli eksuphretisis tou prwtou politi.\n");
        Dequeue();
        x=2;
        return;
      }else if(strcmp(subString1a,"removeCitizen")==0){
        printf("Dwsate tin entoli apoxwrisis politi apo tin oura.\n");
        x=3;
        return;
      }else if(strcmp(subString1a,"print")==0){
        printQueue();
      }else if(strcmp(subString1a,"printRev")==0){
        printf("Dwsate tin entoli ektupwsis tis ouras anamonis anapoda!.\n");
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
        	printf("Prepei na dosete onoma arxeiou\n");
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

//sunartisi emfanisis tis ouras
void printQueue(){
  citizen* temp = head;
  char *mode = strtok(NULL, " \n");
  if (mode == NULL){
  	printf("Prepei na dosete '*' h' pistopoihtiko\n");
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


//sunartisi gia na vgazei ton prwto politi pou perimenei stin oura
void Dequeue() {
	citizen* temp = head;
	if(head == NULL) {
		printf("H oura einai adeia.\n");
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

//anadromiki sunartisi antimetathesis tis ouras
void reverse(citizen *temp) {
	if (temp == NULL) return;

	reverse(temp->next);
	if (strcmp(subString1a, "*") == 0 || strcmp(subString1a, temp->dikaiologitiko) == 0)
	 printf("%50s %50s %02d/%02d/%d %s %50s %16s:%05d %13s %s\n",temp->onoma, temp->epitheto, temp->birthday.day, temp->birthday.month,
      	temp->birthday.year, temp->thlefwno, temp->mail, temp->terminal.IP, temp->terminal.port, temp->dikaiologitiko ,temp->arithmosSunallagis);
}
