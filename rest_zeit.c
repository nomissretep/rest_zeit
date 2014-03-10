//cc -o rest_zeit rest_zeit.c -ljabbah
//Autor: Simon Peters

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <jabbah.h>
#include <string.h>

int s_rest_std;
int s_rest_min;
int s_rest_sec;
int rest_min;
int rest_sec;
int hour=17;
int min=0;
int sek=0;
time_t t;
struct tm *ts;
char user[33];
char server[33];
char pass[33];
char resource[33];
char msg[500];

void parser(char* arg)
{
	int i=0;
	hour=0;
	while(arg[i]&&arg[i]!=':'){
		hour=hour*10+arg[i]-48;
		++i;
		}
	if(arg[i]==':')
		++i;
	while(arg[i]&&arg[i]!=':'){
		min=min*10+arg[i]-48;
		++i;
		}
	if(arg[i]==':')
		++i;
	while(arg[i]&&arg[i]!=':'){
		sek=sek*10+arg[i]-48;
		++i;
		}
}





void sent_msg(){
	int port=5222;
	int prio=1;
	//jabbah implentation
	jabbah_context_t *jct=jabbah_context_create(server, port, 0);

	if(jabbah_connect(jct, user,pass, resource,prio ))
		printf("not connected\n");
	else
		printf("connected\n");


	char jid[33];
	FILE* f=fopen("nerv_liste","r+");
	rewind(f);

	while(fgets(jid,30,f)){
		*(strchr(jid, (int) '\n'))='\0';
		if(strlen(jid))
			jabbah_message_send_chat(jct, jid, msg);
		}
	fclose(f);
	



	//jabbah_close(jct);
	jabbah_context_destroy(jct);
}

void time_send()
{
	char msgt[200];
	sprintf(msgt,"Aktuelle Zeit:\t%2i:%.2i:%.2i\n"
	"Gewuenschte Zeit:\t%2i:%.2i:%.2i\n"
	"Noch %i Stunden, %i Minuten und %i Sekunden verbleibend.\n"
	"In Minuten:\t%6i\n"
	"In Sekunden:\t%6i\n", ts->tm_hour,ts->tm_min,ts->tm_sec,hour,min,sek,s_rest_std,s_rest_min,s_rest_sec,rest_min,rest_sec);
	strcat(msg,msgt);
}

void time_to_console(){
    	printf("Aktuelle Zeit:\t %2i:%.2i:%.2i\n", ts->tm_hour,ts->tm_min,ts->tm_sec);
    	printf("Gewuenschte Zeit:%2i:%.2i:%.2i\n",hour,min,sek);
	printf("Noch %i Stunden, %i Minuten und %i Sekunden verbleibend.\n",s_rest_std,s_rest_min,s_rest_sec);
    	printf("Minuten:\t%6i\n",rest_min);
    	printf("Sekunden:\t%6i\n",rest_sec);
}


void copy_conf(char *a,char *b)
{
	//for(int i=3;b[i]&&b[i]!='\n';++i)
	//	a[i-3]=b[i];
	int i=3;
	while(b[i]&&b[i]!='\n'){
		a[i-3]=b[i];
		++i;
		}
}

int load_conf()
{	
	int res=0;
	FILE* f=fopen("rest_zeit_conf","r");
	rewind(f);
	char tmp[33];
	if(f)
	while(fgets(tmp,30,f)){
		if(tmp[1]=='u'){
			copy_conf(user,tmp);
			res|=1;
		}
		else if(tmp[1]=='p'){
			copy_conf(pass,tmp);
			res|=2;
		}
		else if(tmp[1]=='s'){
			copy_conf(server,tmp);
			res|=4;
		}
		else if(tmp[1]=='r'){	
			copy_conf(resource,tmp);
			res|=8;
		}
	}
	else{
		fclose(f);
		FILE* f=fopen("rest_zeit_conf","w");
	}
	fclose(f);
	return res;
}

void write_conf(int opt)
{
	FILE* f=fopen("rest_zeit_conf","r+");
	rewind(f);
	if(opt&1){
		fputs("-u ",f);	
		fputs(user,f);
		fputs("\n",f);
	}
	if(opt&2){
		fputs("-p ",f);	
		fputs(pass,f);
		fputs("\n",f);
	}
	if(opt&4){
		fputs("-s ",f);	
		fputs(server,f);
		fputs("\n",f);
	}
	if(opt&8){
		fputs("-r ",f);	
		fputs(resource,f);
		fputs("\n",f);
	}
	fclose(f);
}

void add_nerv(char* nerf)
{

	char use[33];
	FILE* f=fopen("nerv_liste","r+");
	rewind(f);
	int not_exist=1;
	char nerf_m[33];
	strcpy(nerf_m,nerf);
	strcat(nerf_m,"\n");
	while(fgets(use,30,f)&&(not_exist=strcmp(nerf_m,use)));
	
	if(not_exist){
		fputs(nerf,f);
		fputs("\n",f);
	}

	fclose(f);
}


void rm_nerv(char* nerf)
{
	char use[33];
	FILE* f=fopen("nerv_liste","r");
	FILE* fw=fopen("nerv_liste~","w");
	rewind(f);
	int len;
	char nerf_m[33];
	strcpy(nerf_m,nerf);
	strcat(nerf_m,"\n");
	while(fgets(use,30,f)){	
		if(strcmp(nerf_m,use)){
			fputs(use,fw);
			}
		}
	fclose(f);
	fclose(fw);
	rename( "nerv_liste~" , "nerv_liste" );

}


void rm_nerv2(char* nerf)
{
	char use[33];
	FILE* f=fopen("nerv_liste","r");
	FILE* fw=fopen("nerv_liste","r+");
	rewind(f);
	rewind(fw);
	int len;
	char nerf_m[33];
	strcpy(nerf_m,nerf);
	strcat(nerf_m,"\n");
	while(fgets(use,30,f)){	
		if(strcmp(nerf_m,use)){
			fputs(use,fw);
			}
		}
	fputc(EOF,fw);
	fclose(f);
	fclose(fw);
	rename( "nerv_liste~" , "nerv_liste" );

}



int messager(char** woerter ,int i){
	while(woerter[i]&&woerter[i][0]!='-'){
		strcat(msg,woerter[i]),i++;
		strcat(msg," ");
		}
	strcat(msg,"\n");
	return i;
}


int main( int argc,char *argv[])
  {
	int opt=load_conf();
	int help=0;
	int i=1;
	int jabber_b=0;
	int console_b=1;
	strcpy(msg,"");
	while(argv[i]!=0&&!help){
		if(0==strcmp(argv[i],"-t"))
			parser(argv[i+1]);
		else if(0==strcmp(argv[i],"-an"))
			add_nerv(argv[i+1]);
		else if(0==strcmp(argv[i],"-rn"))
			rm_nerv(argv[i+1]);
		else if(0==strcmp(argv[i],"-r")){
			strcpy(resource,argv[i+1]);
			opt|=8;
			}
		else if(0==strcmp(argv[i],"-s")){
			strcpy(server,argv[i+1]);
			opt|=4;
			}
		else if(0==strcmp(argv[i],"-p")){
			strcpy(pass,argv[i+1]);
			opt|=2;
			}
		else if(0==strcmp(argv[i],"-u")){
			strcpy(user,argv[i+1]);
			opt|=1;
			}
		else if(0==strcmp(argv[i],"-h"))
			help=1;
		else if(0==strcmp(argv[i],"-j")){
			jabber_b=1;
			i--;
			}
		else if(0==strcmp(argv[i],"-nc")){
			console_b=0;
			i--;
			}
		else if(0==strcmp(argv[i],"-m")){
			i=messager(argv,i+1)-2;		
}
			
		i=i+2;
	}
 	if(!help){
    	t = time(NULL);
    	ts = localtime(&t);
	int min_t=0,hour_t=0,smin=0,ssec=0;
	s_rest_sec=(sek>=ts->tm_sec)?sek-ts->tm_sec:((ssec=1),sek+60-ts->tm_sec);
	s_rest_min=((min_t=min-((ssec)?1:0))>=(ts->tm_min))?min_t-ts->tm_min:((smin=1),min_t+60-ts->tm_min);
	s_rest_std=((hour_t=hour-((smin)?1:0))>=ts->tm_hour)?hour_t-ts->tm_hour:hour_t+24-ts->tm_hour;
    	rest_min=(hour-(ts->tm_hour))*60+(min-ts->tm_min);
    	rest_sec=((hour-(ts->tm_hour))*60+(min-ts->tm_min))*60+(sek-ts->tm_sec);

   	if(console_b) 
		time_to_console();
	if(jabber_b){
		if(opt==15)
    			time_send();
		else{
			printf("Bitte die Fehlenden Optionen fuer den Jabber account angeben:\n");
			if(!(opt&1))
				printf("-u <user>\n");
			if(!(opt&2))
				printf("-p <password>\n");
			if(!(opt&4))
				printf("-s <server>\n");
			if(!(opt&8))
				printf("-r <resource>\n");
			}
	}
	if(strlen(msg))
		sent_msg();
	write_conf(opt);
	}
	else
		printf(" Hilfe fuer Restzeit messer\n"
			"	Anzeige Optionen\n"
			"		-nc			keine Consolen ausgabe\n"
			"		-j			mit Jabber aus gabe\n"
			"		-m  <Message>  		An Verteiler Nachricht verschicken\n"
			" 	Zeit Optionen \n"
			"		-t hh:mm:ss 		Uhrzeit an der man gehen will\n"
			" 	Verteiler Optionen \n"
			"		-an <jabber-id> 	hinzufuegen einer jabber-id\n"
			"		-rn <jaber-id> 		loeschen einer jabber-id\n"
			" 	Configurations Otionen\n"
			"		-u <user>		user angeben\n"
			"		-p <password>		passwort angeben\n"
			"		-s <server>		server angeben\n"
			"		-r <resource>		resource angeben\n"
			);
    	return 0;
  }
