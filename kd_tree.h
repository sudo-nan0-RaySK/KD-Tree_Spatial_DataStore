#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include "linked_list.h"

/**
 * 
 * A generic K-D Tree implementation
 * 
 * Note: This uses `double` type as a key, hence a real number value.
 *
 * */

typedef struct KDTreeNode{
    double* key;
    struct list* data; 
    struct KDTreeNode* left;
    struct KDTreeNode* right;
} KDTreeNode;

typedef struct KDTree{
    int dimension;
    KDTreeNode* root;
} KDTree;

/**
 * 
 * Instantiate a new KDTree instance
 * 
 * */
KDTree* get_new_kd_tree(int dimension){
    KDTree *tree = (KDTree*) malloc(sizeof(KDTree));
    tree->root = NULL;
    tree->dimension = dimension;
    return tree;
}

/**
 * 
 * Instantiate a new KDTreeNode instance
 * 
 * */

KDTreeNode* _get_new_kd_tree_node(double *key, int dimension){
    KDTreeNode *node = (KDTreeNode*) malloc(sizeof(KDTreeNode));
    node->key = key;
    node->right = NULL;
    node->left=NULL;
    node->data = get_new_list();
    return node;
}

/**
 * 
 * Add data to linked list inside KDTreeNode as diffrent,
 * records with same co-ordinates might exist
 * 
 * */
void _kd_node_add_data(KDTreeNode *node, void *data){
    append(node->data,data);
}

/**
 * 
 * Check if all dimensions (x,y in this case) are same for
 * two nodes.
 * 
 * */
int _keys_are_same(double* key1, double* key2, int key_size){
    for(int i = 0; i<key_size; ++i){
        if(key1[i]!=key2[i]){
            return 0;
        }
    }
    return 1;
}

/**
 * 
 * Insert a node into KDTree
 * 
 * */
void _kd_tree_insert_helper(KDTree *tree, KDTreeNode *curr_node,
    double *key, void *data, int depth){

    if(!curr_node){
        tree->root = _get_new_kd_tree_node(key,tree->dimension);
        _kd_node_add_data(tree->root,data);
        return;
    }

    int curr_dim = depth%(tree->dimension);

    if(key[curr_dim]>=curr_node->key[curr_dim]){
        if(_keys_are_same(key,curr_node->key,tree->dimension)){
            _kd_node_add_data(curr_node,data);
        }
        else if(!curr_node->right){
            curr_node->right = _get_new_kd_tree_node(key,tree->dimension);
            _kd_node_add_data(curr_node->right,data);

        } else {
            _kd_tree_insert_helper(tree,curr_node->right,key,data,depth+1);
        }
    } else {

        if(!curr_node->left){
            curr_node->left = _get_new_kd_tree_node(key,tree->dimension);
            _kd_node_add_data(curr_node->left,data);

        } else {
            _kd_tree_insert_helper(tree,curr_node->left,key,data,depth+1);
        }
    }
}

/**
 * 
 * Wrapper to insert into KDTree
 * 
 * */
void kd_tree_insert(KDTree *tree, double *key, void *data){
    _kd_tree_insert_helper(tree,tree->root,key,data,0);
}

/**
 * 
 * Get euclidean distance of n-dimensions
 * 
 **/ 
double get_euclidean_distance(double* key1, double* key2, int key_size){
    double squared_diff = 0.0;
    for(int i  = 0; i<key_size; ++i){
        squared_diff += pow(key1[i]-key2[i],2.0);
    }
    return sqrt(squared_diff);
}
/**
 * 
 * Find a records closest to given co-ordinate
 * 
 **/
void _find_closest_helper(KDTree *tree, KDTreeNode *curr_node, 
    double *key, int depth, KDTreeNode *best_node, 
    double *best_score, int* comparisons){
    
    if(!curr_node){
        return;
    }

    // Increment the comparions count
    *comparisons+=1;
    double curr_dis = get_euclidean_distance(key,curr_node->key,tree->dimension);
    int curr_dim = depth/(tree->dimension);
    
    // Check if euclidean distanc b/w query node and curr_node is best yet
    if (curr_dis <= *best_score){
        *best_score = curr_dis;
        memcpy(best_node,curr_node,sizeof(KDTreeNode));

        // If going left is a better choice, first go left and then right
        if(curr_node->key[curr_dim]>key[curr_dim]){
            _find_closest_helper(tree,curr_node->left,key,depth+1
                ,best_node,best_score,comparisons);
            _find_closest_helper(tree,curr_node->right,key,depth+1
                ,best_node,best_score,comparisons);
        
        // If going right is a better choice, go right first and then left
        } else {
            _find_closest_helper(tree,curr_node->right,key,depth+1
                ,best_node,best_score,comparisons);
            _find_closest_helper(tree,curr_node->left,key,depth+1
                ,best_node,best_score,comparisons);
        }
    
    // If current euclidean distance is not the best yet, prune one branch
    } else{

        // Prune right branch, go left.
        if(curr_node->key[curr_dim] > key[curr_dim]){
            _find_closest_helper(tree,curr_node->left,key,depth+1
                ,best_node,best_score,comparisons);
        
        // Prune left branch, got right.
        } else {
            _find_closest_helper(tree,curr_node->right,key,depth+1
                ,best_node,best_score,comparisons);
        }
    }
}

/**
 * 
 * Wrapper to find a records closest to given co-ordinate
 * 
 * */
KDTreeNode* find_closest(KDTree *tree, double *key){

    double best_score = __DBL_MAX__;
    KDTreeNode* best_node = _get_new_kd_tree_node(NULL,tree->dimension);
    int comparisons = 0;

    _find_closest_helper(tree,tree->root,key,0,best_node,&best_score,&comparisons);

    for(int i  = 0; i<tree->dimension; i++){
        printf("%lf ",key[i]);
    }
    printf("--> %d\n",comparisons);

    return best_node;

}

/**
 * 
 * Find closest record in a given radius for given co-ordinate
 * 
 * */
void _find_closest_in_radius_helper(KDTree *tree, double *key, double radius,
    struct list *neighbors, int depth, double *best_score,
    KDTreeNode *curr_node, int *comparisons){
    
    if(!curr_node){
        return;
    }

    *comparisons+=1;
    int curr_dim  = depth%(tree->dimension);
    double curr_dis = get_euclidean_distance(curr_node->key, key, tree->dimension);

    if(curr_dis <= radius){
        append(neighbors,(void *)curr_node->data);
    }

    if(curr_dis <= *best_score){
        *best_score = curr_dis;
        
        if(curr_node->key[curr_dim] > key[curr_dim]){
            _find_closest_in_radius_helper(tree,key,radius,neighbors,depth+1,
                best_score,curr_node->left,comparisons);
            _find_closest_in_radius_helper(tree,key,radius,neighbors,depth+1,
                best_score,curr_node->right,comparisons);

        } else {
            _find_closest_in_radius_helper(tree,key,radius,neighbors,depth+1,
                best_score,curr_node->right,comparisons);
            _find_closest_in_radius_helper(tree,key,radius,neighbors,depth+1,
                best_score,curr_node->left,comparisons);
        }

    } else {

        if(curr_node->key[curr_dim] > key[curr_dim]){
            _find_closest_in_radius_helper(tree,key,radius,neighbors,depth+1,
                best_score,curr_node->left,comparisons);
        } else {
            _find_closest_in_radius_helper(tree,key,radius,neighbors,depth+1,
                best_score,curr_node->right,comparisons);
        }

    }

}

/**
 * 
 * Wrapper for finding closest record in a given radius for given co-ordinate
 * 
 * */
struct list* find_closest_in_radius(KDTree *tree, double *key, double radius){
    double best_score = __DBL_MAX__;
    int comparison_count = 0;
    struct list* neighbors = get_new_list();

    _find_closest_in_radius_helper(tree,key,radius,neighbors,0
        ,&best_score,tree->root,&comparison_count);

    for(int i  = 0; i<tree->dimension; i++){
        printf("%lf ",key[i]);
    }
    printf("--> %d\n",comparison_count);

    return neighbors;
}
