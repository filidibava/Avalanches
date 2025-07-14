#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include"wr_polya.h"
#include<vector>
#include<list>
#include<cctype>
#include<sstream>


void NODE::update_flag(){
  first=false;
}

void NODE::update_weight(double q){
  weight=weight+q;
}


void NODE::assign_writer(int i){
  writer=i;
}


void NODE::assign_promoter(int i){
  promoter=i;
}

int NODE::tell_promoter(){
  return promoter;
}

double NODE::tell_weight(){
  return weight;
}

int NODE::tell_writer(){
  return writer;
}

bool NODE::tell_flag(){
  return first;
}


void NODE::initialize(){
  weight=1.;
  first=true;
  promoter=-1;
}


