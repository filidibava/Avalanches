#include<iostream>
#include<fstream>
#include<vector>
#include<ctime>
#include<string>
#include<queue>
#include<assert.h>
#include<sstream>
#include<list>
#include"math.h"
#include"rangen.h"



using namespace std;


class NODE {
 protected:

 double weight ;
 bool first;
 int writer;
 int promoter;

 public:
 void update_flag();
 void update_weight(double q);
 void add_neigh(int index);
 void initialize();
 void assign_writer(int i);
 int tell_writer();
 void assign_promoter(int i);
 int tell_promoter();
 double  tell_weight();
 bool  tell_flag();
};
