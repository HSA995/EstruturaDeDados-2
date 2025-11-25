#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

static int my_isdigit(char c)
{
     return (c >= '0' && c <= '9');
}

static char my_tolower(char c)
{
     if (c >= 'A' && c <= 'Z')
          return c + ('a' - 'A');
     return c;
}

static void toLowerBuf(const char *src, char *dst, size_t sz)
{
     size_t i = 0;
     for (; i < sz - 1 && src[i]; i++)
          dst[i] = my_tolower(src[i]);
     dst[i] = '\0';
}

static int isNumberToken(const char *t)
{
     if (!t || !t[0])
          return 0;

     int i = 0;
     int hasDigit = 0;
     int hasDot = 0;

     if (t[0] == '-' && t[1] != '\0')
          i = 1;

     for (; t[i]; i++)
     {
          if (my_isdigit(t[i]))
               hasDigit = 1;
          else if (t[i] == '.' && !hasDot)
               hasDot = 1;
          else
               return 0;
     }

     return hasDigit;
}

static int isUnary(const char *tok)
{
     char tmp[32];
     toLowerBuf(tok, tmp, sizeof(tmp));

     return (!strcmp(tmp, "sin") || !strcmp(tmp, "sen") ||
          !strcmp(tmp, "cos") || !strcmp(tmp, "tan") ||
          !strcmp(tmp, "tg") || !strcmp(tmp, "log") ||
          !strcmp(tmp, "sqrt") || !strcmp(tmp, "raiz"));
}

static double applyUnary(const char *op, double x, int *err)
{
     char tmp[32];
     toLowerBuf(op, tmp, sizeof(tmp));
     *err = 0;

     if (!strcmp(tmp, "sin") || !strcmp(tmp, "sen"))
          return sin(x * M_PI / 180);
     if (!strcmp(tmp, "cos"))
          return cos(x * M_PI / 180);
     if (!strcmp(tmp, "tan") || !strcmp(tmp, "tg"))
          return tan(x * M_PI / 180);

     if (!strcmp(tmp, "log"))
     {
          if (x <= 0)
          {
               *err = 1;
               return NAN;
          }
          return log10(x);
     }

     if (!strcmp(tmp, "sqrt") || !strcmp(tmp, "raiz"))
     {
          if (x < 0)
          {
               *err = 1;
               return NAN;
          }
          return sqrt(x);
     }

     *err = 1;
     return NAN;
}

static double applyBinary(char op, double a, double b, int *err)
{
     *err = 0;
     switch (op)
     {
     case '+':
          return a + b;
     case '-':
          return a - b;
     case '*':
          return a * b;
     case '/':
          if (b == 0)
          {
               *err = 1;
               return NAN;
          }
          return a / b;
     case '%':
          if (b == 0)
          {
               *err = 1;
               return NAN;
          }
          return fmod(a, b);
     case '^':
          return pow(a, b);
     default:
          *err = 1;
          return NAN;
     }
}

float getValorPosFixa(char *str)
{
     char buf[1024];
     strcpy(buf, str);

     double stack[256];
     int top = 0;

     char *tok = strtok(buf, " ");
     while (tok)
     {
          if (isNumberToken(tok))
          {
               stack[top++] = atof(tok);
          }
          else if (isUnary(tok))
          {
               if (top < 1)
                    return NAN;
               int err;
               double r = applyUnary(tok, stack[--top], &err);
               if (err)
                    return NAN;
               stack[top++] = r;
          }
          else if (strlen(tok) == 1)
          {
               char op = tok[0];
               if (strchr("+-*/%^", op))
               {
                    if (top < 2)
                         return NAN;
                    int err;
                    double b = stack[--top];
                    double a = stack[--top];
                    double r = applyBinary(op, a, b, &err);
                    if (err)
                         return NAN;
                    stack[top++] = r;
               }
               else
                    return NAN;
          }
          else
               return NAN;

          tok = strtok(NULL, " ");
     }

     return (top == 1 ? stack[0] : NAN);
}

typedef struct
{
     char *txt;
     int prec;
} Node;

static Node *makeNode(const char *s, int p)
{
     Node *n = malloc(sizeof(Node));
     n->txt = strdup(s);
     n->prec = p;
     return n;
}

static int precOp(char c)
{
     switch (c)
     {
     case '+':
     case '-':
          return 1;
     case '*':
     case '/':
     case '%':
          return 2;
     case '^':
          return 3;
     default:
          return 100;
     }
}

char *getFormaInFixa(char *str)
{
     char buf[1024];
     strcpy(buf, str);

     Node *stack[256];
     int top = 0;

     char *tok = strtok(buf, " ");
     while (tok)
     {

          if (isNumberToken(tok))
          {
               stack[top++] = makeNode(tok, 100);
          }
          else if (isUnary(tok))
          {
               if (top < 1)
                    return NULL;
               Node *a = stack[--top];

               char tmp[600];
               sprintf(tmp, "%s(%s)", tok, a->txt);

               stack[top++] = makeNode(tmp, 100);
               free(a->txt);
               free(a);
          }
          else if (strlen(tok) == 1)
          {
               char op = tok[0];
               if (!strchr("+-*/%^", op))
                    return NULL;

               if (top < 2)
                    return NULL;
               Node *b = stack[--top];
               Node *a = stack[--top];

               int p = precOp(op);

               char la[600], rb[600];

               if (a->prec < p)
                    sprintf(la, "(%s)", a->txt);
               else
                    strcpy(la, a->txt);

               if (b->prec < p || (op == '^' && b->prec == p))
                    sprintf(rb, "(%s)", b->txt);
               else
                    strcpy(rb, b->txt);

               char expr[1200];
               sprintf(expr, "%s%c%s", la, op, rb);

               stack[top++] = makeNode(expr, p);

               free(a->txt);
               free(a);
               free(b->txt);
               free(b);
          }
          else
               return NULL;

          tok = strtok(NULL, " ");
     }

     if (top != 1)
          return NULL;

     return stack[0]->txt;
}