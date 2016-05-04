#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "serverops.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;



serverops::serverops(int fd , const char *name){
	struct hostent *phe;
	
	struct sockaddr_in sin;
	int port, n;
	
	try {
		cout << name << endl;
		if (!(phe = gethostbyname(name))) throw "Wrong name";
		if ((sfd = socket(PF_INET, SOCK_STREAM,0))<0) throw "Socket error: wrong adress";
		//cout<<phe->h_addr_list[0]<<" name"<<endl;
		//cout<< inet_ntoa(*((struct in_addr *)phe->h_addr))<<endl;
		//cout<<strlen(phe->h_addr)<<endl;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(fd);
		//sin.sin_addr.s_addr = INADDR_ANY;
		memcpy(&sin.sin_addr,phe->h_addr_list[0],sizeof(sin.sin_addr));
		
		if (connect(sfd,(struct sockaddr*) &sin,sizeof(sin)) <0) throw "Socket error";
		char *mes = new char[200];
		strcpy(mes,get());
		int i = 0;
		while (!(isdigit(mes[i]))) i++;
		char *n = new char[10];
		int j=0;
		while (i!=strlen(mes)){
			n[j] = mes[i];
			j++;
			i++;
		}
		id = atoi(n);
		cout<<id<<endl;
		cout<<mes;
		cout<<get();
		
	}
	catch(const char*s){
		cout<<"error"<<endl;
		cout<<s<<endl;
	}
}

serverops::~serverops(){
//	close(sfd);
}
char *serverops::get(){
	int c = 'r',r=0,i;
	char *res;
	read(sfd,&c,1);
	for (i=0; (i<200) && (c!='\0'); i++){
		buf[i] = c;
		read(sfd,&c,1);
	}
	if (c == '\0'){
		buf[i] = '\0';
		res = new char[i+1];
		strcpy(res,buf);
	}
	if (i==200)
		throw "wrong message received";
	//char buf[1024];
	//int r = read(sfd,buf,1024);
	//cout<<buf<<endl;
	//for (int i =0; i< r; i++)
		//cout<<buf[i];
	//cout<<"number"<<r<<endl;
	return res;
}

void serverops::help(const char* command ){
	char *mes = new char[42];
	strcpy(mes,"help ");
	//cout<<strcmp(command,"\0")<<endl;
	//if (strcmp(command,"\0")) 
	//	strcat(mes,command);
	strcat(mes,"\n\0");
	//cout<<mes;
	write(sfd, mes, strlen(mes));
	//cout<<"finish"<<endl;
	//cout<<get();
	if (strcmp(command, "\0") == 0){
		for (int j = 0 ; j < 10 ; j ++)
			cout<<get();
	}
	else
		cout<<get();
					
}

void serverops::market(){
	char *mes = new char[42];
	
	strcpy(mes,"market\n");
	write(sfd, mes, strlen(mes));
	//cout<<get();
	//cout<<get();
}
void serverops::inf (int user){
	char *mes = new char[42];
	char *u = new char[10];
	strcpy(mes,"inf ");
	
//	write(sfd, mes, strlen(mes));
	if (user !=0){
		sprintf(u,"%d",user);
		strcat(mes,u);}
		//write(sfd,&user,sizeof(user));
	strcat(mes,"\n");
	cout<<mes<<endl;
	
	write(sfd,mes,strlen(mes));
	//for (int j = 0 ; j < 1 ; j ++)
		//cout<<get();
}
void serverops::newplant(){
	char *mes = new char[42];
	
	strcpy(mes,"market\n");
	write(sfd, mes, strlen(mes));
	cout<<get();
//	cout<<get();	
	
}
void serverops::sell(int number,int price){
	char *mes = new char[42];
	
	strcpy(mes,"sell ");
	
	write(sfd, mes, strlen(mes));
	
	write(sfd,&number,sizeof(number));
	write(sfd,&price,sizeof(price));
	
	write(sfd,"\n",2);
}
void serverops::buy(int number,int price){
	char *mes = new char[42];
	
	strcpy(mes,"buy ");
	
	//write(sfd, mes, strlen(mes));
	
	write(sfd,&number,sizeof(number));
	write(sfd,&price,sizeof(price));
	
	write(sfd,"\n",2);
}
void serverops::end(){
	char *mes = new char[42];
	
	strcpy(mes,"end\n");
	write(sfd, mes, strlen(mes));
	for (int j=0; j<1; j++)
		cout<<get();
}
int serverops::active_players(){
	char *mes= new char[200];
	market();
	strcpy(mes,get());
	//cout<<"mes   "<<mes;
	
	int i=0,j=0;
	while (mes[i]!='.') 
			i++;
	
	while (!isdigit(mes[i])) i++;//cos' no one knows wtf is in that message 
	char *n = new char[10];
	while (mes[i] != ' ') {
		n[j] = mes[i];
		
		j++; i++;
	}
	n[j] = '\0';
	int pl = atoi(n);
	//cout<<pl;
	return pl;
}
int serverops::my_id(){
	return id;
}

int serverops::raw_price(){
	char *mes= new char[200];
	market();
	strcpy(mes,get());
	cout<<mes<<endl;
	int i=0,j=0;
	while (mes[i]!='$') 
			i++;
	i++;
	char *n = new char[10];
	while (isdigit(mes[i])){
		 n[j] = mes[i];
		 i++;
		 j++;
	}
	n[j] = '\0';
	int pr = atoi(n);
return pr;
}
int serverops::money(int user){
	char *mes= new char[200];
	inf(user);
	int i=0,j=0;
	strcpy(mes,get());
	cout<<mes<<endl;
	for (int k = 0; k<4; k++){
	while (!isdigit(mes[i])) i++;
	while (isdigit(mes[i])) i++;
}
	char *n = new char[10];
	while (!isdigit(mes[i])) i++;
	while (isdigit(mes[i])) {
		n[j] = mes[i];
		i++; j++;
	}
	n[j] = '\0';
	int l = atoi(n);
	return l;
}
int serverops::supply(){
	char *mes= new char[200];
	market();
	int i=0,j=0;
	strcpy(mes,get());
	cout<<mes<<endl;
	while (mes[i] != ':') i++;
	i+=2;
	char *n = new char[10];
	
	while (isdigit(mes[i])){
		n[j] = mes[i];
		j++; i++;
	}
	n[j] = '\0';
	return atoi(n);
}
int serverops::production(){
	char *mes= new char[200];
	market();
	int i=0,j=0;
	strcpy(mes,get());
	cout<<mes<<endl;
	while (mes[i] != ';') i++;
	i+=2;
	char *n = new char[10];
	
	while (isdigit(mes[i])){
		n[j] = mes[i];
		j++; i++;
	}
	n[j] = '\0';
	return atoi(n);
}

int serverops:: production_price(){
	char *mes= new char[200];
	market();
	int i=0,j=0;
	strcpy(mes,get());
	cout<<mes<<endl;
	while (mes[i] != '$') i++;
	i++;
	while (mes[i] != '$') i++;
	i++;
	char *n = new char[10];
	
	
	while (isdigit(mes[i])){
		n[j] = mes[i];
		j++; i++;
	}
	n[j] = '\0';
	return atoi(n);
}
	
