#ifndef _matrix_
#define _matrix_
class serverops{
	int sfd,id;
	char buf[1024];
	public:
	~serverops();
	serverops(int,const char*);
	void help(const char *command = "\0");
	void market();
	void inf (int user=0);
	void newplant();
	void sell(int ,int );
	void buy(int ,int );
	void end();
	char *get();
	int my_id();
	
	int turn();
	int active_players();
	int raw_price();
	int money(int);
	int supply();
	int production();
	int production_price();
	
};
#endif
