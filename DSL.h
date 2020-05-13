#define READ(what)  what
#define WRITE(what) what
#define SHOW(what)  what
#define DIFF(what)  what
#define _ ,
#define writeIt(what, where) fprintf (out, "Node%p [shape=\"octagon\",label=\""#what"\"];\n", node, where); break;


#define read_this_fucking_shit(what_to_read, code_of_this, how_much)                        \
                                                                                            \
                        if (strcmp (check, #what_to_read) == 0)                             \
                        {                                                                   \
                            node->what = code_of_this;                                      \
                            node->weight = how_much;                                        \
                            struct element* node_right =  CreateElement (NOTHING);          \
                            Merge (node_right, node, RIGHT);                                \
                            MakeElement ( node_right, buff, place, deep, head, i);          \
                            break;                                                          \
                        }

DEF_DIF (PLUS,   1, LOW_WEIGHT,
                    READ (
                          read_this_fucking_shit (+, PLUS, LOW_WEIGHT)
                         ),
                    WRITE (
                           fprintf (out, " + ");
                          ),
                    SHOW (
                          writeIt (%s, "+")
                         ),
                    DIFF (
                            diff_node->what = PLUS;
                            diff_node->weight = LOW_WEIGHT;

                            struct element* node_diff_left = CreateElement (NOTHING);
                            Merge (node_diff_left, diff_node, LEFT);
                            Differ_smth (node_diff_left, node->left, out);

                            struct element* node_diff_right = CreateElement (NOTHING);
                            Merge (node_diff_right, diff_node, RIGHT);
                            Differ_smth (node_diff_right, node->right, out);
                          ))

DEF_DIF (MINUS,  2, LOW_WEIGHT,
                    READ (
                          read_this_fucking_shit (-, MINUS, LOW_WEIGHT)
                         ),
                    WRITE (
                            fprintf (out, " - ");
                          ),
                    SHOW (
                          writeIt (%s, "-")
                         ),
                    DIFF (
                            diff_node->what = MINUS;
                            diff_node->weight = LOW_WEIGHT;

                            struct element* node_diff_left = CreateElement (NOTHING);
                            Merge (node_diff_left, diff_node, LEFT);
                            Differ_smth (node_diff_left, node->left, out);

                            struct element* node_diff_right = CreateElement (NOTHING);
                            Merge (node_diff_right, diff_node, RIGHT);
                            Differ_smth (node_diff_right, node->right, out);
                            ))

DEF_DIF (MUL,    3,   VERY_STRONG_WEIGHT,
                    READ (
                          read_this_fucking_shit (*, MUL, VERY_STRONG_WEIGHT)
                         ),
                    WRITE (
                            fprintf (out, " \\cdot ");
                          ),
                    SHOW (
                          writeIt (%s, "*")
                          ),
                    DIFF (
                            diff_node->what = PLUS;
                            diff_node->weight = LOW_WEIGHT;

                            struct element* node_diff_right = CreateElement (MUL);
                            Merge (node_diff_right, diff_node, RIGHT);

                            struct element* copy_left = CreateElement (NOTHING);
                            CopyNode (node->left, copy_left);

                            Merge (copy_left, node_diff_right, LEFT);

                            struct element* diff_right = CreateElement (NOTHING);
                            Merge (diff_right, node_diff_right, RIGHT);
                            Differ_smth (diff_right, node->right, out);

                            struct element* node_diff_left = CreateElement (MUL);
                            Merge (node_diff_left, diff_node, LEFT);

                            struct element* copy_right = CreateElement (NOTHING);
                            CopyNode (node->right, copy_right);

                            Merge (copy_right, node_diff_left, RIGHT);

                            struct element* diff_left = CreateElement (NOTHING);
                            Merge (diff_left, node_diff_left, LEFT);
                            Differ_smth (diff_left, node->left, out);
                            ))

DEF_DIF (DIV,    4,   VERY_STRONG_WEIGHT,
                    READ (
                          read_this_fucking_shit (/, DIV, VERY_STRONG_WEIGHT)
                          ),
                    WRITE (
                            fprintf (out, " ");
                          ),
                    SHOW (
                          writeIt (%s, "/")
                         ),
                    DIFF (
                            diff_node->what = DIV;
                            diff_node->weight = VERY_STRONG_WEIGHT;

                            struct element* right_pow = CreateElement (POWER);
                            Merge (right_pow, diff_node, RIGHT);

                            struct element* copy_left = CreateElement (NOTHING);
                            CopyNode (node->left, copy_left);

                            Merge (copy_left, right_pow, LEFT);
                            Merge (CreateElement (NUMBER, 2), right_pow, RIGHT);

                            struct element* node_diff_left = CreateElement (MINUS);
                            Merge (node_diff_left, diff_node, LEFT);

                            struct element* mul_left = CreateElement (MUL);
                            Merge (mul_left, node_diff_left, LEFT);

                            struct element* copy_right = CreateElement (NOTHING);
                            CopyNode (node->right, copy_right);
                            Merge (copy_right, mul_left, RIGHT);

                            struct element* diff_it_left = CreateElement (NOTHING);
                            Merge (diff_it_left, mul_left, LEFT);

                            Differ_smth (diff_it_left, node->left, out);


                            struct element* mul_right = CreateElement (MUL);
                            Merge (mul_right, node_diff_left, RIGHT);

                            struct element* copy_left_for_mul = CreateElement (NOTHING);
                            CopyNode (node->left, copy_left_for_mul);

                            Merge (copy_left_for_mul, mul_right, LEFT);

                            struct element* diff_it_right = CreateElement (NOTHING);
                            Merge (diff_it_right, mul_right, RIGHT);
                            Differ_smth (diff_it_right, node->right, out);
                            ))


#define ALPHA(alpha, numb)                                                           \
                                                                                     \
DEF_DIF (alpha, numb,   LOW_WEIGHT   ,                                               \
                    READ (                                                           \
                          if (strcmp (check, #alpha) == 0)                           \
                          {                                                          \
                              node->what   = alpha;                                  \
                              node->number = numb;                                   \
                              node->weight = LOW_WEIGHT;                             \
                              MakeElement ( node->prev, buff, place, deep, head, i); \
                              break;                                                 \
                          }                                                          \
                          ),                                                         \
                    WRITE (                                                          \
                            fprintf (out, #alpha);                                   \
                          ),                                                         \
                    SHOW (                                                           \
                          writeIt (%s, #alpha);                                      \
                         ),                                                          \
                    DIFF (                                                           \
                            diff_node->what = NUMBER;                                \
                            diff_node->number = 1;                                   \
                            diff_node->weight = LOW_WEIGHT;                          \
                            ))

ALPHA (x, 5)

ALPHA (y, 6)

ALPHA (z, 7)

#undef ALPHA

#define function(func, code, diff)                                                                  \
                                                                                                    \
DEF_DIF (func,    code,       VERY_STRONG_WEIGHT    ,                                               \
                    READ (                                                                          \
                          read_this_fucking_shit (func, func, VERY_STRONG_WEIGHT)                   \
                          ),                                                                        \
                    WRITE (                                                                         \
                            fprintf (out, #func , node->right->number);                             \
                            ),                                                                      \
                    SHOW (                                                                          \
                          writeIt (%s, #func)                                                       \
                         ),                                                                         \
                    DIFF (                                                                          \
                            if (node->right->what != NUMBER)                                        \
                            {                                                                       \
                                diff                                                                \
                            }                                                                       \
                            else                                                                    \
                            {                                                                       \
                                diff_node->what   = NUMBER;                                         \
                                diff_node->number = 0;                                              \
                                diff_node->weight = LOW_WEIGHT;                                     \
                            }                                                                       \
                            ))

#define trygonmetria(funct, numb, dif)                                                          \
                                                                                                \
function (funct, numb, DIFF (                                                                   \
                              diff_node->what = MUL;                                            \
                              diff_node->weight = VERY_STRONG_WEIGHT;                           \
                                                                                                \
                                                                                                \
                              Merge (CreateElement (dif), diff_node, LEFT);                     \
                              struct element* copy_left = CreateElement (NOTHING);              \
                              CopyNode (node->right, copy_left);                                \
                              Merge (copy_left, diff_node->left, RIGHT);                        \
                                                                                                \
                              struct element* diff_me = CreateElement (NOTHING);                \
                              Merge (diff_me, diff_node, RIGHT);                                \
                              Differ_smth (diff_me, node->right, out);                          \
                            ))

trygonmetria (sin, 8, cos)

trygonmetria (sh, 10, ch)

trygonmetria (ch, 11, sh)

#undef trygonmetria

function (cos, 12, DIFF (
                         diff_node->what = MUL;
                         diff_node->weight = VERY_STRONG_WEIGHT;

                         struct element* node_diff_minus = CreateElement (NUMBER, -1);
                         Merge (node_diff_minus, diff_node, LEFT);

                         struct element* node_diff_right = CreateElement (MUL);
                         Merge (node_diff_right, diff_node, RIGHT);

                         Merge (CreateElement (sin), node_diff_right, LEFT);
                         struct element* copy_left = CreateElement (NOTHING);
                         CopyNode (node->right, copy_left);
                         Merge (copy_left, node_diff_right->left, RIGHT);

                         struct element* diff_me = CreateElement (NOTHING);
                         Merge (diff_me, node_diff_right, RIGHT);
                         Differ_smth (diff_me, node->right, out);
                        ))

function (ln, 13, DIFF (
                        diff_node->what = MUL;
                        diff_node->weight = VERY_STRONG_WEIGHT;

                        struct element* div = CreateElement (DIV);
                        Merge (div, diff_node, LEFT);
                        Merge (CreateElement (NUMBER, 1), div, LEFT);

                        struct element* copy_func = CreateElement (NOTHING);
                        CopyNode (node->right, copy_func);
                        Merge (copy_func, div, RIGHT);

                        struct element* diff_me = CreateElement (NOTHING);
                        Merge (diff_me, diff_node, RIGHT);
                        Differ_smth (diff_me, node->right, out);
                       ))


#define difficult_trygonometria(function_, code, diff, sign)                        \
                                                                                    \
function (function_, code, DIFF (                                                   \
                        diff_node->what = MUL;                                      \
                        diff_node->weight = VERY_STRONG_WEIGHT;                     \
                                                                                    \
                        struct element* div = CreateElement (DIV);                  \
                        Merge (div, diff_node, LEFT);                               \
                        Merge (CreateElement (NUMBER, sign), div, LEFT);            \
                                                                                    \
                        struct element* pow = CreateElement (POWER);                \
                        Merge (pow, div, RIGHT);                                    \
                        Merge (CreateElement (NUMBER, 2), pow, RIGHT);              \
                                                                                    \
                        struct element* COS = CreateElement (diff);                 \
                        Merge (COS, pow, LEFT);                                     \
                        struct element* copy_func = CreateElement (NOTHING);        \
                        Merge (copy_func, COS, RIGHT);                              \
                        CopyNode (node->right, copy_func);                          \
                                                                                    \
                        struct element* diff_me = CreateElement (NOTHING);          \
                        Merge (diff_me, diff_node, RIGHT);                          \
                        Differ_smth (diff_me, node->right, out);                    \
                       ))

difficult_trygonometria (tg, 14, cos, 1)

difficult_trygonometria (ctg, 15, sin, -1)

difficult_trygonometria (th, 16, ch, 1)

difficult_trygonometria (cth, 17, sh, -1)

#undef difficult_trygonometria



#define arc(arc_function, code, sign, degree, sign_in_low_position)                     \
                                                                                        \
function (arc_function, code, DIFF (                                                    \
                            diff_node->what = DIV;                                      \
                            diff_node->weight = VERY_STRONG_WEIGHT;                     \
                                                                                        \
                            struct element* div = CreateElement (DIV);                  \
                            Merge (div, diff_node, LEFT);                               \
                            Merge (CreateElement (NUMBER, sign), div, LEFT);            \
                                                                                        \
                            struct element* pow = CreateElement (POWER);                \
                            Merge (pow, div, RIGHT);                                    \
                            Merge (CreateElement (NUMBER, degree), pow, RIGHT);         \
                                                                                        \
                            struct element* minus = CreateElement (sign_in_low_position);\
                            Merge (minus, pow, LEFT);                                   \
                            Merge (CreateElement (NUMBER, 1), minus, LEFT)   ;          \
                                                                                        \
                            struct element* pow_diff = CreateElement (POWER);           \
                            Merge (pow_diff, minus, RIGHT);                             \
                                                                                        \
                            struct element* copy_func = CreateElement (NOTHING);        \
                            Merge (CreateElement (NUMBER, 2), pow_diff, RIGHT);         \
                            Merge (copy_func, pow_diff, LEFT);                          \
                            CopyNode (node->right, copy_func);                          \
                                                                                        \
                            struct element* diff_me = CreateElement (NOTHING);          \
                            Merge (diff_me, diff_node, RIGHT);                          \
                            Differ_smth (diff_me, node->right, out);                    \
                           ))

arc (arcsin, 18, 1, 0.5, MINUS)

arc (arccos, 19, -1, 0.5, MINUS)

arc (arctg, 20, 1, 1, PLUS)

arc (arcctg, 21, -1, 1, PLUS)

#undef arc
#undef function

DEF_DIF (POWER, 30, VERY_STRONG_WEIGHT,
                    READ (
                            read_this_fucking_shit (^, POWER, VERY_STRONG_WEIGHT)
                         ),
                    WRITE (
                             fprintf (out, " ^ ");
                          ),
                    SHOW (
                             writeIt (%s, "^")
                         ),
                    DIFF (
                            diff_node->what = MUL;
                            diff_node->weight = VERY_STRONG_WEIGHT;

                            if ((node->right->what == NUMBER) && (node->left->what != NUMBER))
                            {
                                struct element* pow = CreateElement (POWER);
                                Merge (pow, diff_node, RIGHT);

                                struct element* copy_left = CreateElement (NOTHING);
                                CopyNode (node->left, copy_left);

                                Merge (copy_left, pow, LEFT);
                                Merge (CreateElement (NUMBER, node->right->number - 1), pow, RIGHT);

                                struct element* mul = CreateElement (MUL);
                                Merge (mul, diff_node, LEFT);

                                Merge (CreateElement (NUMBER, node->right->number), mul, RIGHT);

                                struct element* diff_me = CreateElement (NOTHING);
                                Merge (diff_me, mul, LEFT);
                                Differ_smth (diff_me, node->left, out);
                                return;
                            }

                            if ((node->right->what != NUMBER) && (node->left->what == NUMBER))
                            {
                                struct element* mul = CreateElement (MUL);
                                Merge (mul, diff_node, LEFT);

                                struct element* log_e = CreateElement (ln);
                                Merge (log_e, mul, RIGHT);
                                Merge (CreateElement (NUMBER, node->left->number), log_e, RIGHT);

                                struct element* pow = CreateElement (POWER);
                                Merge (pow, mul, LEFT);
                                Merge (CreateElement (NUMBER, node->left->number), pow, LEFT);

                                struct element* copy_func = CreateElement (NOTHING);
                                CopyNode (node->right, copy_func);
                                Merge (copy_func, pow, RIGHT);


                                struct element* diff_me = CreateElement (NOTHING);
                                Merge (diff_me, diff_node, RIGHT);
                                Differ_smth (diff_me, node->right, out);
                                return;
                            }

                            if (((node->right->what == NUMBER) && (node->left->what == NUMBER)))
                            {
                                diff_node->what   = NUMBER;
                                diff_node->number = 0;
                                diff_node->weight = LOW_WEIGHT;
                            }
                         ))

////    Все элементы вставляются раньше number
DEF_DIF (NUMBER, 0,   LOW_WEIGHT,
                    READ(
                            node->what = NUMBER;
                            node->number = MakeInt (check);
                            node->weight = LOW_WEIGHT;

                            MakeElement ( node->prev, buff, place, deep, head, i);
                            break;
                        ),
                    WRITE (
                            fprintf (out, "%.1lg", node->number);
                            ),
                    SHOW (
                          writeIt (%.3lf, node->number)
                         ),
                    DIFF (
                            diff_node->what  = NUMBER;
                            diff_node->number = 0;
                            diff_node->weight = LOW_WEIGHT;
                            ))

#undef read_this_fucking_shit
#undef READ
#undef WRITE
#undef SHOW
#undef writeIt
#undef DIFF
#undef _