#include "cog.h"


/**
 * Zig step of splaying (right rotation).
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *zig(struct cog *root, struct cog *node) {
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
  root->data.btree.rhs = node->data.btree.lhs;
  node->data.btree.lhs = root;
  return node;
}

/**
 * The splay operation moves a given node to the root.
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *splay(struct cog *root, struct cog *node) {
  if (node->data.btree.sep <= root->data.btree.sep) {
    if (root->data.btree.lhs == node) {
      zig(root, node);
    } else {
      zig(root, splay(root->data.btree.lhs, node));
    }
  } else {
    if (root->data.btree.rhs == node) {
      zag(root, node);
    } else {
      zag(root, splay(root->data.btree.rhs, node));
    }
  }
  return node;
}
