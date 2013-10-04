#include <stdlib.h>
#include <string.h>
#include<malloc.h>
#define DEBUG 0

typedef struct text_t {
	int nodeCount;
	struct text_t   *left;
	struct text_t  *right;
	int height;
}text_t;


/*
 creates an empty text, whose length is 0.
*/
text_t * create_text(){
	text_t *tmp = (text_t *)malloc(sizeof(text_t));
	tmp->left = NULL;
	tmp->nodeCount = 0;
	tmp->height = 0;
	return (tmp);
}

text_t * getNode(){
	text_t *tmp = (text_t *)malloc(sizeof(text_t));
	tmp->left = NULL;
	tmp->right = NULL;
	tmp->nodeCount = 0;
	tmp->height = 0;
	return (tmp);
}

/*
 returns the number of lines of the current text.
*/
int length_text( text_t *txt){
	if(txt->left == NULL){
		return 0;
	}
	else{
		return (txt->nodeCount);
	}
}



/*
gets the line of number index, if such a line exists, and
returns NULL else.
*/
char * get_line( text_t *txt, int index){
	text_t *tmp;
	if(txt->left == NULL){
		return NULL;
	}

	else if(txt->nodeCount == 1){
		return (char*) txt->left;
	}

	else{
		tmp = txt;
		while(tmp->right != NULL){
			if(index <= tmp->left->nodeCount){
				tmp = tmp->left;
 			}	
			else{
			       index = index - (tmp->left->nodeCount);
			       tmp = tmp->right;
			}
		}
		return (char *)tmp->left;
	}

}



/*
appends new line as new last line.
*/
void append_line( text_t *txt, char * new_line){
	text_t *tmp;

	if(txt->left == NULL){
		txt->left = (text_t *) new_line;
		txt->nodeCount+=1;
		txt->right = NULL;
	}

	else if(txt->nodeCount == 1){
			text_t  *leftLeaf,*rightLeaf;
			leftLeaf = getNode();
			rightLeaf = getNode();
			leftLeaf->left = txt->left;
			leftLeaf->nodeCount = txt->nodeCount;
		        rightLeaf->left = (text_t *) new_line;
			rightLeaf->nodeCount+=1;
			txt->left = leftLeaf;
			txt->right = rightLeaf;
			txt->nodeCount += 1;

	     }		
	     else{	
		       tmp = txt;

		       while(tmp->right != NULL){
			       tmp->nodeCount+=1;
			       tmp = tmp->right;
	      	       }

		       text_t *newLeaf = getNode();
		       newLeaf->left = (text_t *) new_line;
		       newLeaf->nodeCount+=1;
		       tmp->right =  newLeaf;
		       tmp->nodeCount+=1;	
	     }	
	//free(tmp);		
}



/*
sets the line of number index, if such a line exists, to new line, 
and returns a pointer to the previous line of that number. If no line of
that number exists, it does not change the structure and returns NULL.
*/
char * set_line( text_t *txt, int index, char * new_line){
	text_t *tmp = (text_t *)malloc(sizeof(text_t));
	char *obj =  (char *)malloc(sizeof(char*)*1024);
	if(txt->left == NULL){
		return NULL;
	}

	else if(txt->right == NULL){
		return (char*) txt->left;
	}

	else{
		tmp = txt;
		while(tmp->right != NULL){
			if(index <= tmp->left->nodeCount){
				tmp = tmp->left;
 			}	
			else{
			       index-=tmp->left->nodeCount;
			       tmp = tmp->right;
			}
		}
	}

	strcpy(obj,(char *)tmp->left);
	tmp->left = (text_t *) new_line;
	return obj;

} 



/*
inserts the line before the line of number index, if such a line exists, 
to new line, renumbering all lines after that line. If no such
line exists, it appends new line as new last line.
*/
void insert_line( text_t *txt, int index, char * new_line){
	text_t *tmp;  

	if(txt->left == NULL){
		txt->left = (text_t *) new_line;
		txt->nodeCount+=1;
		txt->right = NULL;
		/*Copy the contents in to line buffer*/
	}

	else if(txt->nodeCount == 1){
		    	text_t *leftLeaf,*rightLeaf;
			leftLeaf = getNode();
			rightLeaf = getNode();
			leftLeaf->left = txt->left;
			leftLeaf->nodeCount = txt->nodeCount;
		        rightLeaf->left = (text_t *) new_line;
			rightLeaf->nodeCount+=1;
			txt->left =  leftLeaf;
			txt->right = rightLeaf;
			txt->nodeCount += 1;

	     }		
	     else{	
		       tmp = txt;

		       while(tmp->right != NULL){
			       tmp->nodeCount+=1;
			       if( index <= (tmp->left->nodeCount)){
				       tmp = tmp->left;
			       }	
			       else{
				       index-=tmp->left->nodeCount;
				       tmp = tmp->right;
			       }
	      	       }

		       text_t *leftLeaf, *rightLeaf;
   		       leftLeaf = getNode();
		       rightLeaf = getNode();
		       leftLeaf->left = tmp->left;
		       leftLeaf->nodeCount = tmp->nodeCount;
		       rightLeaf->left = (text_t *) new_line;
		       rightLeaf->nodeCount+=1;
		       tmp->left =  rightLeaf;
		       tmp->right = leftLeaf;
		       tmp->nodeCount += 1; 	
	     }
	//free(tmp);	
} 



/*
deletes the line of number index, renumbering all
lines after that line, and returns a pointer to 
the deleted line.
*/

char * delete_line( text_t *txt, int index){
	char* obj = malloc(sizeof(char) * 1024); 
	text_t *tmp = getNode();
	if(txt->left == NULL){
		return NULL;
	}

	else if(txt->nodeCount == 1){
		txt->nodeCount = 0;	
		strcpy(obj,(char*) txt->left);
		txt->left = NULL;
		return obj;
	     }		
	     else{	
		       tmp = txt;
		       while(tmp->right != NULL){
			       if( index <= (tmp->left->nodeCount)){
				       tmp = tmp->left;
			       }	
			       else{
				       index-=tmp->left->nodeCount;
				       tmp = tmp->right;
			       }
			       tmp->nodeCount-=1;
	      	       }
        		strcpy(obj,(char*) txt->left);
			free(tmp);
			return obj;
	     }
}


int main(){

	
	   int i, tmp; text_t *txt1, *txt2; char *c;
	   printf("starting \n");
	   txt1 = create_text();
	   txt2 = create_text();
	   append_line(txt1, "line one" );
	   if( (tmp = length_text(txt1)) != 1)
	   {  printf("Test 1: length should be 1, is %d\n", tmp); exit(-1);
	   }
	   append_line(txt1, "line hundred" );
	   insert_line(txt1, 2, "line ninetynine" );
	   insert_line(txt1, 2, "line ninetyeight" );
	   insert_line(txt1, 2, "line ninetyseven" );
	   insert_line(txt1, 2, "line ninetysix" );
	   insert_line(txt1, 2, "line ninetyfive" );
	   for( i = 2; i <95; i++ )
	     insert_line(txt1, 2, "some filler line between 1 and 95" );
	   if( (tmp = length_text(txt1)) != 100)
	   {  printf("Test 2: length should be 100, is %d\n", tmp); //exit(-1);
	   }
	   else{
	      printf("Count Value:%d\n", tmp);
	   }		
	   printf("found at line 1:   %s\n",get_line(txt1,  1));
	   printf("found at line 2:   %s\n",get_line(txt1,  2));
	   printf("found at line 99:  %s\n",get_line(txt1, 99));
	   printf("found at line 100: %s\n",get_line(txt1,100));

	   for( i=99; i>=1; i-- )
	     delete_line(txt1, i );
	   printf("found at the last line:   %s\n",get_line(txt1,  1));

	   	
	   for(i=1; i<=10000; i++)
	   {  if( i%2==1 )
		append_line(txt2, "A");
	      else 
		append_line(txt2, "B");
	   }
	   if( (tmp = length_text(txt2)) != 10000)
	   {  printf("Test 3: length should be 10000, is %d\n", tmp); exit(-1);
	   }
	   c = get_line(txt2, 9876 );
	   if( *c != 'B')
	     {  printf("Test 4: line 9876 of txt2 should be B, found %s\n", c); exit(-1);
	   }
	   for( i= 10000; i > 1; i-=2 )
	   {  c = delete_line(txt2, i);
	      if( *c != 'B')
	      {  printf("Test 5: line %d of txt2 should be B, found %s\n", i, c); exit(-1);
	      }
	      append_line( txt2, c );
	   }
	   for( i=1; i<= 5000; i++ )
	   {  c = get_line(txt2, i);
	      if( *c != 'A')
	      {  printf("Test 6: line %d of txt2 should be A, found %s\n", i, c); exit(-1);
	      }
	   }
	   for( i=1; i<= 5000; i++ )
	     delete_line(txt2, 1 );
	   for( i=1; i<= 5000; i++ )
	   {  c = get_line(txt2, i);
	      if( *c != 'B')
	      {  printf("Test 7: line %d of txt2 should be B, found %s\n", i, c); exit(-1);
	      }
	   }
	   set_line(txt1, 100, "the last line");
		
	

	return 0;
}


