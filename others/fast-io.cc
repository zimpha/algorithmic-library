struct FastIO {
  static const int S = 65536;
  char buf[S];
  int pos, len;
  bool eof;
  FILE *in;
  FastIO(FILE *_in = stdin) {
    in = _in;
    pos = len = 0;
    eof = false;
  }
  inline int nextChar() {
    if (pos == len)
      pos = 0, len = fread(buf, 1, S, in);
    if (pos == len) {eof = true; return -1;}
    return buf[pos++];
  }
  inline int nextUInt() {
    int c = nextChar(), x = 0;
    while (c <= 32) c = nextChar();
    for (;'0' <= c && c <= '9'; c = nextChar()) x = x * 10 + c - '0';
    return x;
  }
  inline int nextInt() {
    int s = 1, c = nextChar(), x = 0;
    while (c <= 32) c = nextChar();
    if (c == '-') s = -1, c = nextChar();
    for (; '0' <= c && c <= '9'; c = nextChar()) x = x * 10 + c - '0';
    return x * s;
  }
  inline void nextString(char *s) {
    int c = nextChar();
    while (c <= 32) c = nextChar();
    for(; c > 32; c = nextChar()) *s++ = c;
    *s = 0;
  }
};
