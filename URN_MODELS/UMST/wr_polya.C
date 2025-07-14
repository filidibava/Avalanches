#include<iostream>
#include<fstream>
#include"wr_polya.h"
#include"wr_polya_fun.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


int main(int argc, char** argv){

  vector<NODE> graph;
  double reinforce,plink;
  int walker,N,new_nodes,N_in,wr;
  int i,orario,steps,maxsteps,wri,prom;
  char c,nomefile[100];
  int type;
  ofstream textfile;
  ofstream textfile2;

  while((c=getopt(argc, argv,"o:n:p:q:s:l:h:"))!=-1 || argc == 1) {
    switch (c) {
    case 'o':
      orario  = atoi(optarg);
      break;
    case 'n':
      N_in = atoi(optarg);
      break;
    case 'p':
      new_nodes = atoi(optarg);
      break;
    case 'q':
      reinforce = atof(optarg);
      break;
    case 's':
      maxsteps = atoi(optarg);
      break;
    case 'l':
      plink = atof(optarg);
      break;
    case 'h':
    default:
      fprintf(stderr, "%s [options]\n"
	      "\t-o\t\t <orario = seed realizzazione >\n"
	      "\t-n\t\t <N_in = initial number of nodes (fully connected) >\n"
	      "\t-p\t\t <new_nodes = number of nodes added when walking on a new node>\n"
	      "\t-q\t\t <reinforce = reinforcement >\n"
	      "\t-s\t\t <maxsteps = text length >\n"
	      "\t-l\t\t <plink = probability of connecting the new nodes to each of the old nodes>\n",argv[0]);
      exit(1);
    }
  }

  sprintf(nomefile,"wr_PUST_Nin_%d_p_%d_q_%f_l_%f_seed_%d.txt",N_in,new_nodes,reinforce,plink,orario);
  textfile.open(nomefile);

  N=N_in;

  new_nodes+=1;
  graph.resize(N_in);

  wr=0;
  for (i=0;i<N_in;i++){
    graph[i].initialize();
    if(!(i%(new_nodes))){wr++;}
    graph[i].assign_writer(wr);
  }
  
  walker=0;
  
  steps=1;

  while(steps<maxsteps){
    prom=graph[walker].tell_promoter();
    wri=graph[walker].tell_writer();
    walker=choose_node(graph,wri,prom,N,plink,orario);
    graph[walker].update_weight(reinforce);  
    if( graph[walker].tell_flag() == true){
      graph[walker].update_flag();
      wr++;
      graph[walker].assign_promoter(wr);
      N=add_nodes(graph,N, new_nodes,orario,wr); 
    }
   // textfile << walker << " " << graph[walker].tell_writer() << " " << steps << " " << N << endl;   
    textfile << walker << endl;   
    steps++;        
  }
}
