#include <stdio.h>

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
 * Zig-Zig step of splaying.
 *
 * @param root - current root of the tree
 * @param node - node to be moved to the root
 * @return the new root of the rearranged tree
 */
struct cog *zigzig(struct cog *root, struct cog *node) {
  struct cog *parent = root->data.btree.lhs;
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
  root->data.btree.rhs = node->data.btree.lhs;
  parent->data.btree.lhs = node->data.btree.rhs;
  node->data.btree.rhs = parent;
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
  if (root == node || root == NULL || node == NULL) return root;
  if (node->data.btree.sep <= root->data.btree.sep) {
    if (root->data.btree.lhs == node) {
      return zig(root, node);
    } else if (root->data.btree.lhs->data.btree.lhs == node) {
      return zigzig(root, node);
    } else if (root->data.btree.lhs->data.btree.rhs == node) {
      return zigzag(root, node);
    } else {
      return zig(root, splay(root->data.btree.lhs, node));
    }
  } else {
    if (root->data.btree.rhs == node) {
      return zag(root, node);
    } else if (root->data.btree.rhs->data.btree.rhs == node) {
      return zagzag(root, node);
    } else if (root->data.btree.rhs->data.btree.lhs == node) {
      return zagzig(root, node);
    } else {
      return zag(root, splay(root->data.btree.rhs, node));
    }
  }
}
