/**
 * 实现了这篇论文:
 *   EERTREE: An Efficient Data Structure for Processing Palindromes in Strings
 *
 * Properties:
 *   1) Nodes and edges of an eertree form two weakly connected components:
 *      the tree of odd (resp., of even) nodes rooted at rte (resp., at rto).
 *   2) The tree of even (resp., odd) nodes is precisely the trie of right
 *      halves of even-length palindromes (resp., the trie of right halves,
 *      including the central symbol, of odd-length palindromes).
 *   3) Nodes and inverted suffix links of an eertree form a tree with a
 *      loop at its root rto.
 *   4) 每次add(c)之后, last节点对应当前串的最长回文后缀
 *
 * Applications:
 *  1. for a given string S find a subpalindrome P maximizing the value
 *     |P|·occ(S, P), where occ(S, P) is the number of occurrences of P in S.
 *     建好eertree后, 利用p->link->occ += p->occ, 从后往前依次枚举每个节点, 计算出
 *     occ(S, P), 之后普通遍历树计算就好了.
 *  2. for a string S, find the number of triples i, j, k such that
 *     1 ≤ i ≤ j < k ≤ |S| and the strings S[i..j], S[j+1..k] are palindromes.
 *     正反建两次eertree, 同时保存下以j开始/结尾的回文串数目, 就是last->num.
 *  3. 多串eertree: 每次build好一个串, 把last节点清零, 开始add新的字符串
 *     3.1 Find the number of subpalindromes, common to all k given strings.
 *     3.2 Find the longest subpalindrome contained in all k given strings.
 *     3.3 For strings S and T find the number of palindromes P having more
 *         occurrences in S than in T .
 *     3.4 For strings S and T find the number of equal palindromes, i.e., of
 *         triples (i, j, k) such that S[i..i+k] = T [j..j+k] is a palindrome.
 *  4. 最小回文划分: 新增一个serial link, 详细参考论文
 */
struct eertree {
  static const int SIZE = 100000 + 10, SIGMA = 26;
  struct node {
    node *ch[SIGMA];
    node *link; // suffix link
    node *slink; // serial link: used in palindromic factorization
    int len; // the length of the node
    int occ; // the occurrences of this node in the whole string
    int num; // total number of suffix palindromes in this node
    int diff, dp; // used in palindromic factorization
    node() {
      memset(ch, 0, sizeof(ch));
      len = num = occ = 0;
      link = NULL;
      // used in palindromic factorization
      slink = NULL;
      diff = 0;
    }
  } pool[SIZE], *sz, *last;
  node *rto, *rte; // two initial root nodes
  int s[SIZE], n;
  eertree() {
    rto = new node(), rte = new node();
    rto->link = rte->link = rto;
    rto->len = -1, rte->len = 0;
    s[0] = -1; n = 0;
    last = rte;
    sz = pool;
  }
  node* get_link(node *u) {
    while (s[n - u->len - 1] != s[n]) u = u->link;
    return u;
  }
  bool add(int c) { // 返回是否新增了一个节点
    s[++n] = c;
    last = get_link(last);
    bool flag = !last->ch[c];
    if (flag) {
      node *p = sz++;
      p->len = last->len + 2;
      if (p->len == 1) p->link = rte;
      else p->link = get_link(last->link)->ch[c];
      p->num = p->link->num + 1;
      last->ch[c] = p;
      // used for palindromic factorization
      p->diff = p->len - p->link->len;
      if (p->diff == p->link->diff) {
        p->slink = p->link->slink;
      } else p->slink = p->link;
    }
    last = last->ch[c];
    last->occ++;
    return flag;
  }
  // used for palindromic factorization
  // 请把ans初始化成inf, ans[0] = 0
  void update(int ans[]) {
    for (node *p = last; p->len; p = p->slink) {
      p->dp = ans[n - p->slink->len - p->diff];
      if (p->diff == p->link->diff) {
        p->dp = std::min(p->dp, p->link->dp);
      }
      ans[n] = std::min(ans[n], p->dp + 1);
    }
  }
  ~eertree() {
    delete rto;
    delete rte;
  }
};
