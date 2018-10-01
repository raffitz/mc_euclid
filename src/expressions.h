#ifndef MCE_HEADER_EXPRESSIONS
#define MCE_HEADER_EXPRESSIONS

enum mce_variable {
	MCE_VAR_S,
	MCE_VAR_X,
	MCE_VAR_Y,
	MCE_VAR_Z
};

enum mce_expression_type {
	MCE_EXP_VAL,
	MCE_EXP_VAR,
	MCE_EXP_BIN
};

enum mce_binary_type {
	MCE_BIN_POW,
	MCE_BIN_MUL,
	MCE_BIN_DIV,
	MCE_BIN_ADD,
	MCE_BIN_SUB
};

struct mce_expression_id {
	enum mce_expression_type type;
};

struct mce_expression_val {
	enum mce_expression_type type;
	double value;
};

struct mce_expression_var {
	enum mce_expression_type type;
	enum mce_variable var;
};

struct mce_expression_binary {
	enum mce_expression_type type;
	enum mce_binary_type operation;
	union mce_expression* left;
	union mce_expression* right;
};

union mce_expression {
	struct mce_expression_id id;
	struct mce_expression_val val;
	struct mce_expression_var var;
	struct mce_expression_binary binary;
};

struct mce_vars {
	int s;
	int x;
	int y;
	int z;
};

union mce_expression* mce_val(double value);
union mce_expression* mce_var(enum mce_variable var);
union mce_expression* mce_pow(union mce_expression* base, union mce_expression* power);
union mce_expression* mce_mul(union mce_expression* left, union mce_expression* right);
union mce_expression* mce_div(union mce_expression* dividend, union mce_expression* divisor);
union mce_expression* mce_add(union mce_expression* left, union mce_expression* right);
union mce_expression* mce_sub(union mce_expression* minuend, union mce_expression* subtrahend);

double mce_resolve_def(union mce_expression* root, struct mce_vars vars);
double mce_resolve(union mce_expression* root, struct mce_vars vars);

#endif
