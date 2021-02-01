#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

struct node1{
	int data;
	struct node1 *next;
	int gene_position;
};
typedef struct node1 Gene; //Gene struct

struct node2{
	struct node1 *gene_head;
	struct node2 *next;
	int chromosome_position;int decimal_value;int match;int order;
	bool xover;
};
typedef struct node2 Chromosome; //Chromosome struct


char* lineScan(FILE *fptr,int n){
	//gives n'th line in the file
	
	rewind(fptr); //go to the begining of file
	char *line = NULL;
    	size_t len = 0;
    	ssize_t line_len;	
	int i=1;
	while ((line_len = getline(&line, &len, fptr)) != -1){
		if(i==n){
			return line;
			break;
		}
		else{
			i++;
		}
	}
}


void binaryToDecimal(Chromosome **population_head,int POP_SIZE,int PROB_SIZE){
	//gives binary to decimal value in a linked list 

	Chromosome *curr=(Chromosome *)malloc(sizeof(Chromosome));
	curr=*population_head;
	while(curr!=NULL){
		Gene *temp=(Gene *)malloc(sizeof(Gene));
		temp=curr->gene_head;
		int sum=0;
		int power=PROB_SIZE-1;
		while(temp!=NULL){
			if(temp->data==1){
				sum=sum + pow(2,power);	
			}
			power--;
			temp=temp->next;
		}
		curr->decimal_value=sum;
		curr=curr->next;
	}
}

void sort(Chromosome **population_head,int POP_SIZE){
	//sort the linked list 
	//bubble sort
	
	Chromosome *current=(Chromosome *)malloc(sizeof(Chromosome));
	Chromosome *next=(Chromosome *)malloc(sizeof(Chromosome));

	for(current=*population_head;current->next!=NULL;current=current->next){
		for(next=current->next;next!=NULL;next=next->next){
			if(current->decimal_value > next->decimal_value){
				Chromosome *temp=(Chromosome *)malloc(sizeof(Chromosome));
				temp->decimal_value=current->decimal_value;
				temp->match=current->match;
				temp->xover=current->xover;
				Gene *gene_head=(Gene *)malloc(sizeof(Gene));
				gene_head=current->gene_head;
				temp->gene_head=gene_head;
				
				current->decimal_value=next->decimal_value;
				current->match=next->match;
				current->xover=next->xover;
				current->gene_head=next->gene_head;
				
				next->decimal_value=temp->decimal_value;
				next->gene_head=temp->gene_head;
				next->match=temp->match;
				next->xover=temp->xover;
			}
		}
	}
}


void print(Chromosome **population_head,Chromosome *best,int generation, int POP_SIZE){
	//print out everything
	
	printf("GENERATION: %d\n",generation);
	Chromosome *curr=(Chromosome *)malloc(sizeof(Chromosome));
	curr=*population_head;
	while(curr!=NULL){
		Gene *temp=(Gene *)malloc(sizeof(Gene));
		temp=curr->gene_head;
		while(temp!=NULL){
			printf("%d ",temp->data);
			temp=temp->next;
		}
		printf(" -> %d\n",curr->decimal_value);
		curr=curr->next;
	}
	
	//find best and print
	curr=*population_head;
	if(best->decimal_value==-1){
		best->decimal_value=curr->decimal_value;
		best->gene_head=curr->gene_head;
	}

	if(best->decimal_value>curr->decimal_value){
		best->decimal_value=curr->decimal_value;
		best->gene_head=curr->gene_head;
	}
	Gene *tempe=(Gene *)malloc(sizeof(Gene));
	tempe=best->gene_head;
	printf("Best chromosome found so far: ");
	while(tempe!=NULL){
		printf("%d ",tempe->data);
		tempe=tempe->next;
	}
	printf(" -> %d\n",best->decimal_value);			
}


void createPopulation(FILE *f_pop,Chromosome **population_head,int POP_SIZE){
	//keep population info and connect with population_head

	//reading file part
	char *line = NULL;
    	size_t len = 0;
    	ssize_t line_len;
   
    	int i=1,j=1;
	while ((line_len = getline(&line, &len, f_pop)) != -1){
		char *token = strtok(line,":");
		Gene *gn_head = (Gene *)malloc(sizeof(Gene));
		gn_head=NULL;
		while(token!=NULL ){
			Gene *gn = (Gene *)malloc(sizeof(Gene));
			gn->data=atoi(token);
			gn->gene_position = j;
			gn->next=NULL;
			if(j==1){
				gn_head = gn;
			}
			else{
				Gene *current = (Gene *)malloc(sizeof(Gene));
				current = gn_head;
				while(current->next!=NULL){
					current = current->next;
				}
				current->next=gn;
			}
			j++;
			token = strtok(NULL,":");
		}
		j=1;
		Chromosome *chrsm = (Chromosome *)malloc(sizeof(Chromosome));
		chrsm->chromosome_position=i;
		chrsm->order=i;
		chrsm->gene_head = gn_head;
		chrsm->next=NULL;
		chrsm->xover=false;
		if(i==1){
			*population_head=chrsm;	
		}
		else{
			Chromosome *current1 = (Chromosome *)malloc(sizeof(Chromosome));
			current1 = *population_head;
			while(current1->next!=NULL){
				current1=current1->next;
			}
			current1->next=chrsm;
		}
		i++;	
	}	
}
	


void selection(FILE *f_slc,Chromosome **population_head,int generation,int POP_SIZE){
	//declarations
	int *line_head=(int *)malloc(1*sizeof(int));

	//line scanning
	char *line=lineScan(f_slc,generation);

	//execution selection
	//declarations
	int line_pointer=0,count=0;
	int *chromosome1,*chromosome2;
	chromosome1 = (int *)malloc((POP_SIZE/2)*sizeof(int));
	chromosome2 = (int *)malloc((POP_SIZE/2)*sizeof(int));

	while(count!=(POP_SIZE/2)){
		int i=0;
		char *num1=(char *)malloc(10*sizeof(char));		
		while(line[line_pointer]!=':'){
			num1[i]=line[line_pointer];
			i++;line_pointer++;
		}
		chromosome1[count]=atoi(num1);
		
		i=0;
		char *num2=(char *)malloc(10*sizeof(char));
		line_pointer=line_pointer+1;

		while(line[line_pointer]!=' ' && line[line_pointer]!='\r' && line[line_pointer]!='\n' && line[line_pointer]!='\0' && line[line_pointer]!='\t'){
			num2[i]=line[line_pointer];
			i++;line_pointer++;	
		}
		chromosome2[count]=atoi(num2);
		count++;
	}
				
	int i=0;
	Chromosome *temp=(Chromosome *)malloc(sizeof(Chromosome));
	while(i!=(POP_SIZE/2)){
		int counter=1;
		temp=*population_head;
		while(temp!=NULL){
			if(counter==chromosome1[i]){
				temp->match = chromosome2[i];
				counter++;
				temp=temp->next;
			}
			else if(counter==chromosome2[i]){
				temp->match = chromosome1[i];
				counter++;
				temp=temp->next;
			}
			else{
				counter++;
				temp=temp->next;
			}
		}
		i++;
	}
}


void xover(FILE *f_xov,Chromosome **population_head,int generation){	
	//line scanning
	char *line = lineScan(f_xov,generation);

	//execution xover
	//declaration
	char *token;int start,finish;
	
	//execution
	token = strtok(line,":");
	while(token!=NULL){
		start=atoi(token);
		token = strtok(NULL," ");
		finish=atoi(token);
		break;
	}
	Chromosome *ch1=(Chromosome *)malloc(sizeof(Chromosome));
	ch1=*population_head;
	Chromosome *ch2=(Chromosome *)malloc(sizeof(Chromosome));
	
	while(ch1!=NULL){
		ch1->xover=true;
		int counter=1;
		ch2=*population_head;

		while(counter!=ch1->match){
			ch2=ch2->next;
			counter++;
		}

		if(ch2->xover==true){
			ch1=ch1->next;
		}
		else{
			Gene *gn1=(Gene *)malloc(sizeof(Gene ));
			gn1=ch1->gene_head;
			Gene *gn2=(Gene *)malloc(sizeof(Gene ));
			gn2=ch2->gene_head;
			int count=start;
			while(count!=finish+1){
				if(gn1->gene_position==count){
					//xover operation
					int x = gn1->data;
					gn1->data=gn2->data;
					gn2->data=x;
					count++;
				}
				gn1=gn1->next;
				gn2=gn2->next;	
			}
			gn1=ch1->gene_head;
			gn2=ch2->gene_head;
			ch1=ch1->next;
		}
	}
	Chromosome *curr=(Chromosome *)malloc(sizeof(Chromosome));
	curr=*population_head;
	while(curr!=NULL){
		curr->xover=false;
		curr=curr->next;	
	}
}


void mutate(FILE *f_mut,Chromosome **population_head,int generation){
	//line scanning
	char *line = lineScan(f_mut,generation);
	
	int x = atoi(line);

	//execution mutate 
	Chromosome *current_chr=(Chromosome *)malloc(sizeof(Chromosome));
	current_chr=*population_head;
		
	while(current_chr!=NULL){
		Gene *current_gene = (Gene *)malloc(sizeof(Gene));
		current_gene = current_chr->gene_head;

		int counter=1;
		if(x-1==0){
			if(current_gene->data==1){
				current_gene->data=0;
			}
			else if(current_gene->data==0){
				current_gene->data=1;
			}
			
		}
		else{
			while(counter!=x-1){
				current_gene=current_gene->next;
				counter++;	
			}
			//mutate operation
			if(current_gene->next->data==1){
				current_gene->next->data=0;
			}
			else if(current_gene->next->data==0){
				current_gene->next->data=1;
			}
		}
		current_chr=current_chr->next;
	}
}



void main(int argc, char *argv[]){
	//printf("successfull\n");
	int PROB_SIZE=atoi(argv[1]);	//Gene number
	int POP_SIZE=atoi(argv[2]);	//Chromosome number
	int MAX_GENE=atoi(argv[3]);	//Generation number	 
	
	FILE *f_pop = fopen("population","r");
	FILE *f_slc = fopen("selection","r");
	FILE *f_xov = fopen("xover","r");
	FILE *f_mut = fopen("mutate","r");
	
	Chromosome *population_head=(Chromosome *)malloc(sizeof(Chromosome)); 	//head of everything
	
	Chromosome best; 	//best chromosome
	best.decimal_value=-1;

	if(f_pop==NULL || f_slc==NULL || f_xov==NULL || f_mut==NULL){
		printf("file(s) not found\n");
		exit(88);
	}
	
	//initialize 
	createPopulation(f_pop,&population_head,POP_SIZE);
	
	//GENERATION :0
	binaryToDecimal(&population_head,POP_SIZE,PROB_SIZE);
	sort(&population_head,POP_SIZE);
	print(&population_head,&best,0,POP_SIZE);
	
	//Execute operations in a loop 
	int generation_count=1;
	while(generation_count!=MAX_GENE+1){
		selection(f_slc,&population_head,generation_count,POP_SIZE);
		xover(f_xov,&population_head,generation_count);
		mutate(f_mut,&population_head,generation_count);
		binaryToDecimal(&population_head,POP_SIZE,PROB_SIZE);
		sort(&population_head,POP_SIZE);
		print(&population_head,&best,generation_count,POP_SIZE);
		generation_count++;
	}
	//printf("end of program\n");
}
