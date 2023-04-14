// Do not change the next 13 lines
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum Position {LEFT, RIGHT} Position;  // TIP: use this in your code!

int max( int x, int y ){
    return (x < y) ? y : x;
}

int abs( int x ){
    return (x < 0) ? -x : x;
}

// Fill your info here:
/****
  Student1 name: mohamad watad

  Student1 ID: 318601267
****/


// Operations
int ret_height(AVLNodePtr tnode);
int ret_size(AVLNodePtr tnode);
AVLNodePtr avl_search( AVLNodePtr tnode, int k );
AVLNodePtr avl_insert( AVLNodePtr tnode, int k );
void correcting_height_size(AVLNodePtr tnode);
void update_size(AVLNodePtr tnode);
void update_height(AVLNodePtr tnode);
AVLNodePtr avl_delete( AVLNodePtr tnode, int k );
AVLNodePtr replace_rightsub_Min (AVLNodePtr root,AVLNodePtr parent, AVLNodePtr child);
AVLNodePtr balance(AVLNodePtr tnode);
AVLNodePtr right_rotate(AVLNodePtr tnode);
AVLNodePtr left_rotate(AVLNodePtr tnode);
AVLNodePtr new_avl_node( int k );
void delete_avl_tree( AVLNodePtr root );
int avl_rank( AVLNodePtr tnode, int k );
int avl_rank_helper( AVLNodePtr temp, int k,int counter);
int avl_reversed_rank( AVLNodePtr tnode, int k );
AVLNodePtr ret_lca(AVLNodePtr root,int x ,int y);
int avl_HowMany( AVLNodePtr tnode, int x, int y );
int howmany_from_x(AVLNodePtr tnode,int x,int counter1);
int howmany_from_y(AVLNodePtr tnode,int y,int counter2);
int ret_min(AVLNodePtr root);
int ret_max(AVLNodePtr root);
int last_missing( AVLNodePtr tnode );
int last_missing_help( AVLNodePtr tnode );

//returns the height of giving node
int ret_height(AVLNodePtr tnode){
    if(tnode== NULL){
        return -1;
    }
    else {
        return tnode->height;
    }

}
//returns the size in for a giving node
int ret_size(AVLNodePtr tnode){
    if(tnode== NULL){
        return 0;
    }
    else {
        return tnode->size;
    }

}


AVLNodePtr avl_search( AVLNodePtr tnode, int k ){
    if(tnode == NULL || tnode->key == k ){//found the wanted key or there is no node with such key
        return  tnode;
    }
    else if(tnode->key < k){              //the key we in is smaller than the one we searching so we go to right child
        return avl_search(tnode->child[RIGHT],k);
    }
    else {                                 //the key we in is bigger than the one we searching so we go to left child
        return avl_search(tnode->child[LEFT],k);
    }
}


AVLNodePtr avl_insert( AVLNodePtr tnode, int k ){
    if(tnode == NULL){ //empty so we add the node we want here
        return new_avl_node(k);
    }
    else if(tnode->key == k){    //there is already a node with this key so we do nothing
       return tnode;
    }
    else if(tnode->key < k){    //we need to go right because we want to insert a bigger k than key we on
       tnode->child[RIGHT]=avl_insert(tnode->child[RIGHT],k);
    }
    else if(tnode->key > k){    //we need to go left because we want to insert a smaller k than key we on
       tnode->child[LEFT]=avl_insert(tnode->child[LEFT],k);
    }
    //we check if avl tree still balanced
    int lef =ret_height(tnode->child[LEFT]);
    int righ=ret_height(tnode->child[RIGHT]);
    if(abs(lef - righ) > 1){        //balance check
        tnode=balance(tnode);
    }
    correcting_height_size(tnode);  //update the height and size if needed for node we traveled on
    return tnode;
}

//updates the size and height
void correcting_height_size(AVLNodePtr tnode){
    if(tnode==NULL){
        return;
    }
    tnode->height=((max(ret_height(tnode->child[LEFT]), ret_height(tnode->child[RIGHT]))) + 1);
    tnode->size=ret_size(tnode->child[LEFT]) + ret_size(tnode->child[RIGHT]) + 1;
    return;
}

void update_size(AVLNodePtr tnode){
    if(tnode==NULL){
        return;
    }
    tnode->size=ret_size(tnode->child[LEFT]) + ret_size(tnode->child[RIGHT]) + 1;
    return;
}

void update_height(AVLNodePtr tnode){
    if(tnode==NULL){
        return;
    }
    tnode->height=((max(ret_height(tnode->child[LEFT]), ret_height(tnode->child[RIGHT]))) + 1);
    return;
}



AVLNodePtr avl_delete( AVLNodePtr tnode, int k ){
    AVLNodePtr temp;
    if(tnode==NULL){
        return NULL  ;   // no k to delete
    }
    else if(tnode->key < k){ // if tnode key is smaller than k search right
        tnode->child[RIGHT] = avl_delete(tnode->child[RIGHT], k);
    }
    else if(tnode->key > k){ //if tnode key is greater than k search left
        tnode->child[LEFT] = avl_delete(tnode->child[LEFT], k);
    }
    else if (tnode->key == k){//found the node to delete we see which kind of node it is
        if ((tnode->child[LEFT] != NULL)&&(tnode->child[RIGHT])!= NULL){ // node has two children
            tnode = replace_rightsub_Min (tnode, tnode, tnode->child[RIGHT]); // replace node with node with min key from right subtree
            tnode->child[RIGHT] = avl_delete(tnode->child[RIGHT],k);
        }
        else {  /* either one or no children */
            temp = tnode;
            if(tnode->child[LEFT]!= NULL){
                tnode=tnode->child[LEFT];
            }
            else{
                tnode=tnode->child[RIGHT];
            }
            free(temp);
            return tnode;
        }
    }

    //check the balance of the tree
    int lef =ret_height(tnode->child[LEFT]);
    int righ=ret_height(tnode->child[RIGHT]);
    if(abs(lef - righ) > 1){
        tnode=balance(tnode);
    }
    correcting_height_size(tnode);
    return tnode;

}


//replaces the node with the node with minimum key from right subtree
AVLNodePtr replace_rightsub_Min (AVLNodePtr root,AVLNodePtr parent, AVLNodePtr child){
    AVLNodePtr temp;
    if (child->child[LEFT]) {  /* continue until you reach the minimal node */
        return replace_rightsub_Min (root, child, child->child[LEFT]);
    } else {
        temp = (child->child[RIGHT]); /* the minimal node does not have left child, or else it wouldn't be the minimal, thus we only need to save the right child */
        if (parent == root) {    /* in case which the root of the sub-tree is the minimum */
            child->child[RIGHT] = root;
            child->child[LEFT] = root->child[LEFT];
        } else {    /* else*/
            parent->child[LEFT] = root;
            child->child[LEFT] = root->child[LEFT];
            child->child[RIGHT] = root->child[RIGHT];
        }
        root->child[LEFT] = NULL;
        root->child[RIGHT] = temp;
        child->height = root->height;
        correcting_height_size(root);
        return child;
    }
}


//tree needs balance and rotations
AVLNodePtr balance(AVLNodePtr tnode){
    if (ret_height(tnode->child[LEFT]) < ret_height(tnode->child[RIGHT])){   // the imbalanced node is right-heavy
            if ((tnode->child[RIGHT])&& (ret_height((tnode->child[RIGHT])->child[LEFT]) > ret_height((tnode->child[RIGHT])->child[RIGHT]))){//in case where the node's right son is left-heavy
                tnode->child[RIGHT] = right_rotate(tnode->child[RIGHT]);
            }
            return left_rotate(tnode);
    }
    else {  // the imbalanced node is left-heavy
            if ((tnode->child[LEFT])&&(ret_height((tnode->child[LEFT])->child[LEFT]) < ret_height((tnode->child[LEFT])->child[RIGHT]))){//in case where the node's left son is right-heavy
                tnode->child[LEFT] = left_rotate(tnode->child[LEFT]);
            }
            return right_rotate(tnode);
        }
}


AVLNodePtr right_rotate(AVLNodePtr tnode){
    AVLNodePtr temp = tnode->child[LEFT];
    tnode->child[LEFT] = temp->child[RIGHT]?temp->child[RIGHT]:NULL;
    temp->child[RIGHT]= tnode;
    update_height(temp->child[LEFT]);
    update_height(temp->child[RIGHT]);
    update_height(temp);
    update_size(temp->child[RIGHT]);
    update_size(temp);
    return temp;
}

AVLNodePtr left_rotate(AVLNodePtr tnode){
    AVLNodePtr temp = tnode->child[RIGHT];
    tnode->child[RIGHT] = temp->child[LEFT]?temp->child[LEFT]:NULL;
    temp->child[LEFT]= tnode;
    update_height(temp->child[LEFT]);
    update_height(temp->child[RIGHT]);
    update_height(temp);
    update_size(temp->child[LEFT]);
    update_size(temp);
    return temp;
}


//creaate a new node for tree
AVLNodePtr new_avl_node( int k ){
    AVLNodePtr head;
    head = (AVLNodePtr)malloc(sizeof(AVLNode));
    head->child[LEFT]=NULL;
    head->child[RIGHT]=NULL;
    head->height=0;
    head->size=1;
    head->key=k;
    return head;
}

void delete_avl_tree( AVLNodePtr root ){
    if(root == NULL){
        return;
    }
    if (root->child[LEFT]){
        delete_avl_tree(root->child[LEFT]);
    }            // postorder
    if (root->child[RIGHT]){
        delete_avl_tree(root->child[RIGHT]);
    }
    free(root);
    return; // Student code goes here. Feel free to remove this line.
}

// Queries
int last_missing(AVLNodePtr tnode){
    if(tnode == NULL){
        return 1;
    }
    return last_missing_help(tnode);
}


int last_missing_help( AVLNodePtr tnode ){
    int right_max=ret_max(tnode);//holds maximum value from tnode

    if(tnode->child[RIGHT]==NULL && tnode->child[LEFT]==NULL){//leaf
        return tnode->key -1;
    }

    else if(tnode->child[RIGHT]!=NULL && tnode->child[LEFT]!=NULL ){
        if(right_max-tnode->key > tnode->child[RIGHT]->size){//there is a missing number in right part
            return last_missing_help(tnode->child[RIGHT]);
        }
        else{//right subtree is full
            int left_max=ret_max(tnode->child[LEFT]); //holds the max key from the left subtree
            if(tnode->key - left_max !=1 ){             //checks if this node and its max from left subtree has  a missing number there
                return tnode->key-1;
            }
            else{
                return last_missing_help(tnode->child[LEFT]);
            }

        }
    }
    else if(tnode->child[RIGHT]==NULL && tnode->child[LEFT]!=NULL )
    {
        int left_max=ret_max(tnode->child[LEFT]);
        if(tnode->key - left_max !=1 ){
            return left_max+1;
        }
        else{
            return last_missing_help(tnode->child[LEFT]);
        }
    }
    else //(tnode->child[RIGHT]!=NULL && tnode->child[LEFT]==NULL )
    {
        if(right_max-tnode->key > tnode->child[RIGHT]->size){
            return last_missing_help(tnode->child[RIGHT]);
        }
        else{
           return tnode->key-1;
        }
    }


}

//returns maximus key in tree
//by keep going right
int ret_max(AVLNodePtr root){
    if(root == NULL){
        return 0;
    }
    else if(root->child[RIGHT] != NULL){
        return ret_max(root->child[RIGHT]);
    }
    else{
        return root->key;
    }

}

//returns the minimum key in tree
//by keep going left
int ret_min(AVLNodePtr root){
    if(root == NULL){
        return 0;
    }
    else if(root->child[LEFT] != NULL){
        return ret_min(root->child[LEFT]);
    }
    else{
        return root->key;
    }

}


int avl_rank( AVLNodePtr tnode, int k ){
    int counter=0;
    AVLNodePtr temp = tnode ;
    counter=avl_rank_helper(temp,k,counter);
    return counter;
}

//
int avl_rank_helper( AVLNodePtr temp, int k,int counter){
    counter=0;
     if(temp == NULL){
        return 0;
     }
     if(temp->key == k){//we add the left subtree beacuse its less than key
        if(temp->child[LEFT]!= NULL){
            counter+=temp->child[LEFT]->size+1;


        }
        else{   // no left subtree so we just add the node itself
            counter++;


        }
     }
     else if (temp->key > k){// we dont add anything and go left
        counter+=avl_rank_helper(temp->child[LEFT],k,counter);

     }
     else { // key < k
        if(temp->child[LEFT]!= NULL){
            counter+=temp->child[LEFT]->size+1;

        }
        else{   // no left subtree
            counter++;

        }
        counter+=avl_rank_helper(temp->child[RIGHT],k,counter);
     }
        return counter;

}


//it is the opposete of rank we just need to pay attention if k itself found it counts to rank we need it in reverse so we add 1
int avl_reversed_rank( AVLNodePtr tnode, int k ){
    int answer = avl_rank(tnode,k);
    answer=tnode->size-answer;
    if(avl_search(tnode,k)!=NULL){
        answer++;
    }
    return answer; // Student code goes here. Feel free to remove this line.
}


int avl_HowMany( AVLNodePtr tnode, int x, int y ){
    AVLNodePtr lca_node=ret_lca(tnode,x,y);//returns the lca node

    int answer=0;
    int counter1 =0,counter2=0;
    int lca_val=lca_node->key;
    if(lca_val >= x && lca_val <= y){       // add the lca itself
        answer++;
    }
    answer+=howmany_from_x(lca_node->child[LEFT],x,counter1); //go from left lca son node to x node
    answer+=howmany_from_y(lca_node->child[RIGHT],y,counter2);//go from right lca son node to y node
    return answer;
}

int howmany_from_x(AVLNodePtr tnode,int x,int counter1){//search from lca to x while adding relevant parts
    counter1=0;
    if(tnode == NULL){
        return 0;
    }
    if(tnode->key==x){//add the right subtree because its bigger than x and smallet than y because we searching in left from lca
        if(tnode->child[RIGHT]!=NULL){
            counter1+=tnode->child[RIGHT]->size+1;
        }
        else{
            counter1++;
        }
    }
    else if(tnode->key > x){
        if(tnode->child[RIGHT]!=NULL){
            counter1+=tnode->child[RIGHT]->size+1;
        }
        else{
            counter1++;
        }
        counter1+=howmany_from_x(tnode->child[LEFT],x,counter1);

    }
    else{ //(tnode->key < x){
        counter1+=howmany_from_x(tnode->child[RIGHT],x,counter1);
    }
    return counter1;
}

int howmany_from_y(AVLNodePtr tnode,int y,int counter2){//search from lca to y while adding relevant parts
    counter2=0;
    if(tnode == NULL){
        return 0;
    }
    if(tnode->key==y){
        if(tnode->child[LEFT]!=NULL){
            counter2+=tnode->child[LEFT]->size+1;
        }
        else{
            counter2++;
        }
    }
    else if(tnode->key < y){
        if(tnode->child[LEFT]!=NULL){
            counter2+=tnode->child[LEFT]->size+1;
        }
        else{
            counter2++;
        }
        counter2+=howmany_from_y(tnode->child[RIGHT],y,counter2);

    }
    else{ //(tnode->key > y){
        counter2+=howmany_from_y(tnode->child[LEFT],y,counter2);
    }
    return counter2;
}



AVLNodePtr ret_lca(AVLNodePtr root,int x ,int y){
    if (root == NULL){
        return NULL;
    }
    if (root->child[RIGHT] != NULL && root->key < x && root->key < y){//go right
        return ret_lca(root->child[RIGHT],x,y);
    }
    else if (root->child[LEFT]!=NULL && root->key > x && root->key > y){//go keft
        return ret_lca(root->child[LEFT],x,y);
    }
    else{
        return root;//we found the first node which is betwwen x and y
    }
}




