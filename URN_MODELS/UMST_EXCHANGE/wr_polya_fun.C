#include<iostream>
#include<stdlib.h> 
#include"wr_polya.h"
#include"wr_polya_fun.h"
#include<vector>
#include<list>
#include<cctype>

using namespace std;

int choose_node(vector<NODE> &graph, int wri, int prom, int N, double plink, int &orario){

  int i,k,p;
  double wtot=0,sumw=0,r;
  
  for(i=0;i<N;i++){
    if(graph[i].tell_writer() == wri || graph[i].tell_writer() == prom || graph[i].tell_promoter() == wri ){
      wtot+=graph[i].tell_weight(); 
    }
    else {
      wtot+=plink*graph[i].tell_weight();
    }
  }
  
  r=(ran2(orario)*wtot);
  i=0;

  while(sumw < r){
     if(graph[i].tell_writer() == wri || graph[i].tell_writer() == prom || graph[i].tell_promoter() == wri ){
      sumw+=graph[i].tell_weight();
    }
    else {
      sumw+=plink*graph[i].tell_weight();
    }
    i++;
  }   
  return (i-1);

}


int add_nodes(vector<NODE> &graph,int N, int p, int &orario, int wr){
  int N_after,i,j;
  NODE helpnode;

  N_after=N+p;
  
  helpnode.initialize();
  helpnode.assign_writer(wr);
  for (i=N;i<N_after;i++){
    graph.push_back(helpnode);
  }  

  return N_after;
}

