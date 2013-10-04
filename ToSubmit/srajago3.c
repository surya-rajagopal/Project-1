#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define DEBUG 0
#define STACK_MAX 1000000

/*
   structure for implementing the balanced tree structure
 */
typedef struct text_t {
	int nodeCount;
	int height;
	struct text_t   *left;
	struct text_t  *right;
}text_t;

/*
   structure for implementing the stack
 */
text_t *stackNode[STACK_MAX];
int size = 0; 

//void stackPush(text_t *text)
//text_t* stackPop()
/*
   Pushes the object into the Stack
 */
void stackPush(text_t *text)
{
	if (size < STACK_MAX){
		stackNode[size] = text;
		size++;
	}	
	else
		fprintf(stderr, "Error: stack full\n");
}


/*
   returns the top of the Stack and deletes from the Stack
 */
text_t* stackPop()
{
	if (size == 0){
		fprintf(stderr, "Error: stack empty\n");
		exit(-1);
	}
	else{
		--size;
		//printf("Return elemen at index: %d\n",size);
		return stackNode[size];
	}
}

/*
   returns 1 if Stack is empty, else 0
 */
int stackEmpty(){
	if(size == 0)
		return 1;
	else 
		return 0;
}

/*
   creates an empty tree, whose length is 0.
 */
text_t * create_text(){
	text_t *tmp = (text_t *)malloc(sizeof(text_t));
	tmp->left = NULL;
	tmp->nodeCount = 0;
	tmp->height = 0;
	return (tmp);
}

/*
   creates an empty node.
 */
text_t * getNode(){
	text_t *tmp = (text_t *)malloc(sizeof(text_t));
	tmp->left = NULL;
	tmp->right = NULL;
	tmp->nodeCount = 0;
	tmp->height = 0;
	return (tmp);
}

/*
   returns the height of the tree
 */
int findHeight(text_t *txt){
	if(txt->left == NULL || txt->right == NULL)
		return 0;
	else{
		int leftHeight = findHeight(txt->left);
		int rightHeight = findHeight(txt->right);
		if(leftHeight < rightHeight){
			return rightHeight+1;		
		}
		else
			return leftHeight+1;
	}
}

/*
   Performs Left Rotation on the tree - (Referred from the Text Book)
 */
void leftRotation(text_t *txt){
	text_t *tmp = txt;
	int tmpNodeCount;
	tmp = txt->left;
	txt->left = txt->right;
	txt->right = txt->left->right;	
	txt->left->right = txt->left->left;
	txt->left->left = tmp;
	txt->left->nodeCount = txt->left->left->nodeCount + txt->left->right->nodeCount;
}

/*
   Performs Right Rotation on the tree - (Referred from the Text Book)
 */
void rightRotation(text_t *txt){
	text_t *tmp = txt;
	int tmpNodeCount;
	tmp = txt->right;
	txt->right = txt->left;
	txt->left = txt->right->left;	
	txt->right->left = txt->right->right;
	txt->right->right = tmp;
	txt->right->nodeCount = txt->right->left->nodeCount + txt->right->right->nodeCount;
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
		if(tmp->left != NULL)	
			return (char *)tmp->left;
		else
			return NULL;
	}

}

/*
   appends new line as new last line.
 */
void append_line( text_t *txt, char * new_line){
	text_t *tmp, *tmpNode;

	if(txt->left == NULL){
		txt->left = (text_t *) new_line;
		txt->nodeCount+=1;
		txt->right = NULL;
		txt->height = 0;
	}

	else{
		tmp = txt;
		while(tmp->right != NULL){
			stackPush(tmp);
			tmp->nodeCount+=1;
			tmp = tmp->right;
		}
		text_t  *leftLeaf,*rightLeaf;
		leftLeaf = getNode();
		rightLeaf = getNode();
		leftLeaf->left = tmp->left;
		leftLeaf->nodeCount = tmp->nodeCount;
		rightLeaf->left = (text_t *) new_line;
		rightLeaf->nodeCount+=1;
		tmp->left = leftLeaf;
		tmp->right = rightLeaf;
		tmp->nodeCount += 1;
		tmp->height = 1;

		//Rotation Part	- (Referred from the Text Book)
		int finished = 0;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		while( size>0 && !finished ){ 
			int tmp_height, old_height;
			tmp = stackPop();
			old_height= tmp->height;
			if( tmp->left->height - tmp->right->height == 2 ){ 
				if( tmp->left->left->height - tmp->right->height == 1 )
				{
					rightRotation( tmp );
					tmp->right->height =
						tmp->right->left->height + 1;
					tmp->height =
						tmp->right->height + 1;
				}
				else
				{
					leftRotation( tmp->left );
					rightRotation( tmp );
					tmp_height =
						tmp->left->left->height;
					tmp->left->height =
						tmp_height + 1;
					tmp->right->height =
						tmp_height + 1;
					tmp->height = tmp_height + 2;
				}
			}
			else if( tmp->left->height - tmp->right->height == -2 ){
				if( tmp->right->right->height - tmp->left->height == 1 ){
					leftRotation( tmp );
					tmp->left->height =
						tmp->left->right->height + 1;
					tmp->height =
						tmp->left->height + 1;
				}
				else{ 
					rightRotation( tmp->right );
					leftRotation( tmp );
					tmp_height =
						tmp->right->right->height;
					tmp->left->height =
						tmp_height + 1;
					tmp->right->height =
						tmp_height + 1;
					tmp->height = tmp_height + 2;
				}
			}	
			else{
				if( tmp->left->height > tmp->right->height )
					tmp->height = tmp->left->height + 1;
				else
					tmp->height = tmp->right->height + 1;
			}
			if( tmp->height == old_height )
				finished = 1;

		}

		size=0;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}	
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
	if(index == 1){
		strcpy(obj,(char *)tmp->left);
		tmp->left = (text_t *) new_line;
		return obj;
	}
	else
		return NULL;
} 

/*
   inserts the line before the line of number index, if such a line exists, 
   to new line, renumbering all lines after that line. If no such
   line exists, it appends new line as new last line.
 */
void insert_line( text_t *txt, int index, char * new_line){
	text_t *tmp, *tmpNode; 
	int finished = 0;

	if(txt->left == NULL){
		txt->left = (text_t *) new_line;
		txt->nodeCount+=1;
		txt->right = NULL;
		txt->height = 0;
		return;
	}

	if(txt->nodeCount == 1){
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
		txt->height = 1;	
	}		
	else{	
		tmp = txt;
		while(tmp->right != NULL){
			stackPush(tmp);
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
		tmp->height = 1;

		//- (Referred from the Text Book)
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		while( size>0 && !finished ){ 
			int tmp_height, old_height;
			tmp = stackPop();
			old_height= tmp->height;
			if( tmp->left->height - tmp->right->height == 2 ){ 
				if( tmp->left->left->height - tmp->right->height == 1 )
				{
					rightRotation( tmp );
					tmp->right->height =
						tmp->right->left->height + 1;
					tmp->height =
						tmp->right->height + 1;
				}
				else
				{
					leftRotation( tmp->left );
					rightRotation( tmp );
					tmp_height =
						tmp->left->left->height;
					tmp->left->height =
						tmp_height + 1;
					tmp->right->height =
						tmp_height + 1;
					tmp->height = tmp_height + 2;
				}
			}
			else if( tmp->left->height - tmp->right->height == -2 ){
				if( tmp->right->right->height - tmp->left->height == 1 ){
					leftRotation( tmp );
					tmp->left->height =
						tmp->left->right->height + 1;
					tmp->height =
						tmp->left->height + 1;
				}
				else{ 
					rightRotation( tmp->right );
					leftRotation( tmp );
					tmp_height =
						tmp->right->right->height;
					tmp->left->height =
						tmp_height + 1;
					tmp->right->height =
						tmp_height + 1;
					tmp->height = tmp_height + 2;
				}
			}	
			else{
				if( tmp->left->height > tmp->right->height )
					tmp->height = tmp->left->height + 1;
				else
					tmp->height = tmp->right->height + 1;
			}
			if( tmp->height == old_height )
				finished = 1;

		}

		size=0;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
} 

/*
   deletes the line of number index, renumbering all
   lines after that line, and returns a pointer to 
   the deleted line.
 */
char * delete_line( text_t *txt, int index){
	char* obj; 
	text_t *tmp, *upperNode, *otherNode, *tmpNode;
	int finished = 0, tmpSize ;
	if(txt->left == NULL || index > txt->nodeCount){
		printf("Left is Null\n");
		return NULL;
	}

	else{	
		tmp = txt;
		while(tmp->right != NULL){
			stackPush(tmp);	
			upperNode = tmp;
			if( index <= (tmp->left->nodeCount)){
				tmp = tmp->left;
				otherNode = upperNode->right;
			}	
			else{
				index-=tmp->left->nodeCount;
				otherNode = upperNode->left;
				tmp = tmp->right;
			}
		}
		stackPop();
		if(index == 1){
			upperNode->left  = otherNode->left;
			upperNode->right = otherNode->right;
			upperNode->height = otherNode->height;
			upperNode->nodeCount = otherNode->nodeCount;
			obj = (char*) tmp->left; 
			if(tmp->left == NULL)	printf("###################################Left NULL\n");
			//printf("Deleted str is : %s\n",obj);
			text_t *tmp1;
			tmpSize = size;
			while(size > 0){
				tmp1 = stackPop();
				tmp1->nodeCount-=1;
			}
			size = tmpSize;

			//- (Referred from the Text Book)
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			while( size>0 && !finished ){ 
				int tmp_height, old_height;
				tmp = stackPop();
				old_height= tmp->height;
				if( tmp->left->height - tmp->right->height == 2 ){ 
					if( tmp->left->left->height - tmp->right->height == 1 )
					{
						rightRotation( tmp );
						tmp->right->height =
							tmp->right->left->height + 1;
						tmp->height =
							tmp->right->height + 1;
					}
					else
					{
						leftRotation( tmp->left );
						rightRotation( tmp );
						tmp_height =
							tmp->left->left->height;
						tmp->left->height =
							tmp_height + 1;
						tmp->right->height =
							tmp_height + 1;
						tmp->height = tmp_height + 2;
					}
				}
				else if( tmp->left->height - tmp->right->height == -2 ){
					if( tmp->right->right->height - tmp->left->height == 1 ){
						leftRotation( tmp );
						tmp->left->height =
							tmp->left->right->height + 1;
						tmp->height =
							tmp->left->height + 1;
					}
					else{ 
						rightRotation( tmp->right );
						leftRotation( tmp );
						tmp_height =
							tmp->right->right->height;
						tmp->left->height =
							tmp_height + 1;
						tmp->right->height =
							tmp_height + 1;
						tmp->height = tmp_height + 2;
					}
				}	
				else{
					if( tmp->left->height > tmp->right->height )
						tmp->height = tmp->left->height + 1;
					else
						tmp->height = tmp->right->height + 1;
				}
				if( tmp->height == old_height )
					finished = 1;

			}


			size=0;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			return obj;
		}
		else{
			size =0;
			return NULL;
		}
	}
}

/*
   returns the number of lines of the current text.
 */
int length_text(text_t *txt){
	if(txt->left == NULL){
		return 0;
	}
	else{
		return (txt->nodeCount);
	}
}

int main()
{  int i, tmp; text_t *txt1, *txt2; char *c;
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
   {  printf("Test 2: length should be 100, is %d\n", tmp); exit(-1);
   }
   printf("found at line 1:   %s\n",get_line(txt1,  1));
   printf("found at line 2:   %s\n",get_line(txt1,  2));
   printf("found at line 99:  %s\n",get_line(txt1, 99));
   printf("found at line 100: %s\n",get_line(txt1,100));

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
   for( i=99; i>=1; i-- )
     delete_line(txt1, i );
   printf("found at the last line:   %s\n",get_line(txt1,  1));

   
}
