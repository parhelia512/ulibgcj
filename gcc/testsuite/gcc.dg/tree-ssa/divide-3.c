/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-optimized" } */

int f(int a)
{
  return -(a/10);
}

/* { dg-final { scan-tree-dump-times "a / 10" 0 "optimized"} } */
/* { dg-final { scan-tree-dump-times "a / -10" 1 "optimized"} } */

/* { dg-final { cleanup-tree-dump "optimized" } } */


