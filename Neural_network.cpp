#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

#define g 6  //simplify the picture
#define rate 2
struct tra {
	int a[8][8];
	int value;
};

unsigned int seed = 0;
struct node {
	double w[8][8];
	node() {
		for(int i =  0; i < 8; i++) {
			for(int a = 0; a < 8; a++) {
				srand(seed++);
				w[i][a] = (rand()%10)/10-0.5;
			}
		}
	}
};

struct node1 {
	double w[10];
	node1() {
		for(int i =  0; i < 10; i++) {
				w[i] = 0.0;
		}
	}
};

struct node2 {
	double w[10];
	node2() {
		for(int i =  0; i < 10; i++) {
			srand(seed++);	
			w[i] = (rand()%10)/10-0.5;
		}
	}
};
vector<tra> tradata;
vector<tra> testdata;

node hNN[10][10];
node1 NN1[10];
node2 NN[10];

void update1(int i, double e, double y) {
	for(int a = 0; a < 10; a++) {
		NN[i].w[a] = NN[i].w[a] + 0.1*e*y*(1-y)*NN1[i].w[a];
	}
}

void update2(int i, double e, tra temp) {
	for(int a = 0; a < 10; a++) {
	  for(int c = 0; c < 8; c++) {
		for(int n = 0; n < 8; n++) {
		 hNN[i][a].w[n][c] =hNN[i][a].w[n][c] + 0.1*e*NN[i].w[a]*NN1[i].w[a]*(1-NN1[i].w[a])*temp.a[n][c];
		}
	  }
    }
}
void single_train(int i, tra temp) {
	double d;
    double e;
    double y;
    
	if(i == temp.value) {
		d = 1;
	} else {
		d = 0;
	}
	
	for(int a = 0; a < 10;a++) {
	  for(int c = 0; c < 8; c++) {
		for(int n = 0; n < 8; n++) {
			NN1[i].w[a] = NN1[i].w[a] + temp.a[n][c]*hNN[i][a].w[n][c];
		}
	  }
	  NN1[i].w[a] = 1/(1+exp(0-NN1[i].w[a]));
    }
	
	for(int a = 0; a < 10; a++) {
		y = y + NN1[i].w[a]*NN[i].w[a];
	}
	y = 1/(1+exp(0-y));
	e = d - y;
	update1(i,e,y);
	update2(i,e,temp);
}

void train(tra temp) {
	for(int i = 0; i < 10; i++) {
		single_train(i,temp);
	}
}

bool inputtra() {
	ifstream in("digitstra.txt");
	if (!in.is_open()) {
		cout << "Error opening file";
		return 0;
	}
	
	string tdata;
	string get = "";
	
	tra temp;
	while(getline(in,tdata)) {
	int index = 0;
	for (int i = 0; i < 8; i++) {
		for (int c = 0; c < 8; c++) {
			int t = 0;  //store number
			while(index < tdata.length() && tdata[index] != ',') {
				t = t*10 + (int)(tdata[index] - '0');
				index++;
			}
			if(t >= g) {
			  temp.a[i][c] = 1;
		    } else {
		      temp.a[i][c] = 0;
		    }
			index++;
		}
	}
	temp.value = (int)(tdata[index] - '0');
	tradata.push_back(temp);
	}
	return 1;
}

bool inputtest() {
	ifstream in("digitstest.txt");
	if (!in.is_open()) {
		cout << "Error opening file";
		return 0;
	}
	
	string tdata;
	string get = "";
	
	tra temp;
	while(getline(in,tdata)) {
	int index = 0;
	for (int i = 0; i < 8; i++) {
		for (int c = 0; c < 8; c++) {
			int t = 0;  //store number
			while(index < tdata.length() && tdata[index] != ',') {
				t = t*10 + (int)(tdata[index] - '0');
				index++;
			}
			if(t >= g) {
			  temp.a[i][c] = 1;
		    } else {
		      temp.a[i][c] = 0;
		    }
			index++;
		}
	}
	temp.value = (int)(tdata[index] - '0');
	testdata.push_back(temp);
	}
	return 1;
}

double get(int i, tra temp) {
	double d;
    double e;
    double y;
    
	if(i == temp.value) {
		d = 1;
	} else {
		d = 0;
	}
	
	for(int a = 0; a < 10;a++) {
	  for(int c = 0; c < 8; c++) {
		for(int n = 0; n < 8; n++) {
			NN1[i].w[a] = NN1[i].w[a] + temp.a[n][c]*hNN[i][a].w[n][c];
		}
	  }
	  NN1[i].w[a] = 1/(1+exp(0-NN1[i].w[a]));
    }
	
	for(int a = 0; a < 10; a++) {
		y = y + NN1[i].w[a]*NN[i].w[a];
	}
	y = 1/(1+exp(0-y));
  return y;
}
bool check(tra temp) {
	int n = 0;
	double v[10];
	for(int i = 0; i < 10; i++) {
		v[i] = get(i,temp);
	}
	double a = v[0];
	for(int i = 1; i < 10; i++) {
		if(v[i] > a) {
			a = v[i];
			n = i;
		}
	}
	if(n == temp.value) {
		return true;
	} else {
		return false;
	}
}


int main() {
	seed = 0;
	inputtra();
	for(int c = 0; c < 170;c++) {
	    for(int i = 0; i < tradata.size(); i++) {
			train(tradata[i]);
	    }
    }
	inputtest();
	double r = 0;
	double f = 0;
	for(int i = 0; i < testdata.size(); i++) {
		if(check(testdata[i])) {
			r++;
		} else {
			f++;
		}
	}
	cout<<"Size_of_trainning_set:"<<tradata.size()<<endl;
	cout<<"Size_of_testing_set:"<<testdata.size()<<endl;
	cout<<"Accuracy:"<<(r/(r+f))*100<<"%"<<endl;
}
