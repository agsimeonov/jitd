#include <stdio.h>

#include "cog.h"
#include "policy.h"


/**
 * Zig step of splaying (right rotation).
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *zig(struct cog *root, struct cog *node) {
#ifdef __ADVANCED
  // TODO: Aurijoy - feel free to add __ADVANCED blocks as needed in this function
  long root_count = root->data.btree.rds - (root->data.btree.rhs->data.btree.rds+root->data.btree.lhs->data.btree.rds);
  root->data.btree.rds = root->data.btree.rhs->data.btree.rds + node->data.btree.rhs->data.btree.rds+root_count;
  node->data.btree.rds = node->data.btree.lhs->data.btree.rds + root->data.btree.rds;
#endif
  root->data.btree.lhs = node->data.btree.rhs;
  node->data.btree.rhs = root;
  return node;
}

/**
 * Zag step of splaying (left rotation).
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *zag(struct cog *root, struct cog *node) {
#ifdef __ADVANCED
  // TODO: Aurijoy - feel free to add __ADVANCED blocks as needed in this function
  root->data.btree.rds = root->data.btree.lhs->data.btree.rds + node->data.btree.lhs->data.btree.rds;
  node->data.btree.rds = node->data.btree.rhs->data.btree.rds + root->data.btree.rds;
#endif
  root->data.btree.rhs = node->data.btree.lhs;
  node->data.btree.lhs = root;
  return node;
}

/**
 * Zig-Zig step of splaying.
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *zigzig(struct cog *root, struct cog *node) {
  struct cog *parent = root->data.btree.lhs;
#ifdef __ADVANCED
  // TODO: Aurijoy - feel free to add __ADVANCED blocks as needed in this function
  root->data.btree.rds = root->data.btree.rhs->data.btree.rds + parent->data.btree.rhs->data.btree.rds;
  parent->data.btree.rds = root->data.btree.rds + node->data.btree.rhs->data.btree.rds;
  node->data.btree.rds = parent->data.btree.rds + node->data.btree.lhs->data.btree.rds;
  
#endif
  root->data.btree.lhs = parent->data.btree.rhs;
  parent->data.btree.lhs = node->data.btree.rhs;
  parent->data.btree.rhs = root;
  node->data.btree.rhs = parent;
  return node;
}

/**
 * Zag-Zag step of splaying.
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *zagzag(struct cog *root, struct cog *node) {
  struct cog *parent = root->data.btree.rhs;
#ifdef __ADVANCED
  // TODO: Aurijoy - feel free to add __ADVANCED blocks as needed in this function
  root->data.btree.rds = root->data.btree.lhs->data.btree.rds + parent->data.btree.rhs->data.btree.rds;
  parent->data.btree.rds = root->data.btree.rds + node->data.btree.lhs->data.btree.rds;
  node->data.btree.rds = parent->data.btree.rds + node->data.btree.rhs->data.btree.rds;
#endif
  root->data.btree.rhs = parent->data.btree.lhs;
  parent->data.btree.rhs = node->data.btree.lhs;
  parent->data.btree.lhs = root;
  node->data.btree.lhs = parent;
  return node;
}

/**
 * Zig-Zag step of splaying.
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *zigzag(struct cog *root, struct cog *node) {
  struct cog *parent = root->data.btree.lhs;
#ifdef __ADVANCED
  // TODO: Aurijoy - feel free to add __ADVANCED blocks as needed in this function
  parent->data.btree.rds = parent->data.btree.lhs->data.btree.rds + node->data.btree.lhs->data.btree.rds;
  root->data.btree.rds = root->data.btree.rhs->data.btree.rds + node->data.btree.rhs->data.btree.rds;
  node->data.btree.rds = parent->data.btree.rds + root->data.btree.rds ;
#endif
  root->data.btree.lhs = node->data.btree.rhs;
  parent->data.btree.rhs = node->data.btree.lhs;
  node->data.btree.lhs = parent;
  node->data.btree.rhs = root;
  return node;
}

/**
 * Zag-Zig step of splaying.
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *zagzig(struct cog *root, struct cog *node) {
  struct cog *parent = root->data.btree.rhs;
#ifdef __ADVANCED
  // TODO: Aurijoy - feel free to add __ADVANCED blocks as needed in this function
  parent->data.btree.rds = parent->data.btree.rhs->data.btree.rds + node->data.btree.rhs->data.btree.rds;
  root->data.btree.rds = root->data.btree.lhs->data.btree.rds + node->data.btree.lhs->data.btree.rds;
  node->data.btree.rds = parent->data.btree.rds + root->data.btree.rds ;

#endif
  root->data.btree.rhs = node->data.btree.lhs;
  parent->data.btree.lhs = node->data.btree.rhs;
  node->data.btree.rhs = parent;
  node->data.btree.lhs = root;
  return node;
}

/**
 * Acquires the depth of a given node in a BTree.
 *
 * @param root - current root of the tree
 * @param node - we want to know the depth of this node
 * @return the depth of a given node
 */
int getDepth(struct cog *root, struct cog *node) {
  if (root == node) {
    return 0;
  } else if (node->data.btree.sep <= root->data.btree.sep) {
    return 1 + getDepth(root->data.btree.lhs, node);
  } else {
    return 1 + getDepth(root->data.btree.rhs, node);
  }
}

/**
 * The splay operation moves a given node to the root.
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @param depth - depth of node in the original tree
 * @return the new root of the rearranged tree
 */
struct cog *splayDepth(struct cog *root, struct cog *node, int depth) {
  if (root == node)  {
    return root;
  } else if (node->data.btree.sep <= root->data.btree.sep) {
    if (root->data.btree.lhs == node) {
      return zig(root, node);
    } else if (root->data.btree.lhs->data.btree.lhs == node) {
      return zigzig(root, node);
    } else if (root->data.btree.lhs->data.btree.rhs == node) {
      return zigzag(root, node);
    } else {
      if (depth % 2) {
        return zig(root, splayDepth(root->data.btree.lhs, node, depth));
      } else {
        if (node->data.btree.sep <= root->data.btree.lhs->data.btree.sep) {        
          return zigzig(root, splayDepth(root->data.btree.lhs->data.btree.lhs, node, depth));
        } else {          
          return zigzag(root, splayDepth(root->data.btree.lhs->data.btree.rhs, node, depth));
        }
      }
    }
  } else {
    if (root->data.btree.rhs == node) {
      return zag(root, node);
    } else if (root->data.btree.rhs->data.btree.rhs == node) {    
      return zagzag(root, node);
    } else if (root->data.btree.rhs->data.btree.lhs == node) {      
      return zagzig(root, node);
    } else {
      if (depth % 2) {
        return zag(root, splayDepth(root->data.btree.rhs, node, depth));
      } else {
        if (node->data.btree.sep > root->data.btree.rhs->data.btree.sep) {          
          return zagzag(root, splayDepth(root->data.btree.rhs->data.btree.rhs, node, depth));
        } else {          
          return zagzig(root, splayDepth(root->data.btree.rhs->data.btree.lhs, node, depth));
        }
      }
    }
  }
}

/**
 * The splay operation moves a given node to the root.
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *splay(struct cog *root, struct cog *node) {
  return splayDepth(root, node, getDepth(root, node));
}
